/*
 * Date: 2020/11/30
 * Description: 向量 
 */
#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

#include <assert.h>
#include <string.h>
#include <stdlib.h>

// 默认容量
#define DEFAULT_CAPACITY    8
// 默认扩容率/缩容率
#define DEFAULT_RATIO		0.5f
// 最大、最小
#define max(a, b)			((a) > (b) ? (a) : (b))
#define min(a, b)			((a) < (b) ? (a) : (b))

// 秩
typedef int     Rank;

template <typename T>
class Vector
{
public:
	// 默认构造
	Vector() :
		Vector(DEFAULT_CAPACITY)
	{

	}

	// 用容量初始化
	Vector(int capacity) :
		_capacity(capacity),
		_size(0),
		_elem(nullptr),
		_elem_size(sizeof(T)),
		_expand_ratio(DEFAULT_RATIO),
		_shrink_ratio(DEFAULT_RATIO)
	{
		assert(_capacity > 0);
		_elem = new T[_capacity];
		memset(_elem, 0, _capacity * sizeof(T));
	}

	// 用初始元素初始化
	Vector(int size, T init_elem) :
		_capacity(2 * size),
		_size(size),
		_elem(nullptr),
		_elem_size(sizeof(T)),
		_expand_ratio(DEFAULT_RATIO),
		_shrink_ratio(DEFAULT_RATIO)
	{
		assert(_capacity > 0);
		assert(_size > 0);
		_elem = new T[_capacity];
		for (Rank i = 0; i < _size; i++)
			_elem[i] = init_elem;
	}

	// 数组区间复制
	Vector(T const* A, Rank lo, Rank hi)
	{
		copyFrom(A, lo, hi);
	}

	// 数组整体复制
	Vector(T const* A, Rank n)
	{
		copyFrom(A, 0, n);
	}

	// 向量区间复制
	Vector(Vector<T> const& V, Rank lo, Rank hi)
	{
		copyFrom(V._elem, lo, hi);
	}

	// 向量整体复制
	Vector(Vector<T> const& V)
	{
		_capacity = V._capacity;
		_size = V._size;
		_elem_size = V._elem_size;
		_expand_ratio = V._expand_ratio;
		_shrink_ratio = V._shrink_ratio;
		_elem = new T[_capacity * sizeof(T)];
		copyFrom(V._elem, 0, _size);
	}

	// 析构函数
	~Vector()
	{
		if (nullptr != _elem)
			delete [] _elem;
	}

	// 只读访问接口
		// 获取容量
	inline int capacity() const
	{
		return _capacity;
	}

	// 获取实际规模
	inline int size() const
	{
		return _size;
	}

	// 判空
	inline bool empty() const
	{
		return !_size;
	}

// 可写访问元素
	// 重载下标操作符
	T& operator [] (Rank rank) const
	{
		assert(rank >= 0 && rank < _size);
		return _elem[rank];
	}

	// 重载赋值操作符
	Vector<T>& operator=(Vector<T> const& other)
	{
		_capacity = other._capacity;
		_size = other._size;
		_elem_size = other._elem_size;
		_expand_ratio = other._expand_ratio;
		_shrink_ratio = other._shrink_ratio;
		if (nullptr != _elem)
			delete [] _elem;
		_elem = new T[_capacity * sizeof(T)];
		copyFrom(other._elem, 0, _size);
		return *this;
	}

	// 删除秩为r的元素
	T remove(Rank r)
	{
		assert(r >= 0 && r < _size);

		T ret = _elem[r];

		// 拷贝元素
		for (Rank i = r; i < _size; i++)
		{
			// printf("copy %d - %d : %d\n", i - 1, i, _elem[i - 1]);
			_elem[i] = _elem[i + 1];
		}
		shrink();
		return ret;
	}

	// 删除秩在区间[lo, hi)之内的元素
	int remove(Rank lo, Rank hi)
	{
		int ret = 0;

		if (lo > hi)	return ret;

		Rank min_index = max(lo, 0);
		Rank max_index = min(hi, _size);

		if (min_index > _size - 1)	return ret;

		// 拷贝元素
		Rank offset = max_index - min_index;
		for (Rank i = 0; i < _size - max_index; i++)
		{
			_elem[i + min_index] = _elem[i + max_index];
		}

		// 缩小
		for (Rank i = 0; i < max_index - min_index; i++, ret++)
		{
			shrink();
		}
		return ret;
	}

	// 添加元素
	void push_back(const T& new_elem)
	{
		expand();
		_elem[_size - 1] = new_elem;
	}

	// 插入元素
	Rank insert(int index, T const& new_elem)
	{
		// 退化为push_back
		if (index == _size)
			push_back(new_elem);

		if (index < 0 || index >= _size)
			return -1;

		expand();

		// 拷贝元素
		for (Rank i = _size - 1; i >= index + 1; i--)
		{
			// printf("copy %d - %d : %d\n", i - 1, i, _elem[i - 1]);
			_elem[i] = _elem[i - 1];
		}

		_elem[index] = new_elem;
		return index;
	}

	// 随机置乱算法
	void unsort(void)
	{
		for (Rank i = _size; i > 0; i--)
		{
			swap(i - 1, rand() % i);
		}
	}

// 算法接口
	// 顺序查找，区间在[lo, hi)
	Rank find(const T& e, Rank lo, Rank hi) const
	{
		assert(lo >= 0 && hi <= _size);
		for (Rank i = hi - 1; i >= lo; i--)
		{
			if (_elem[i] == e)
				return i;
		}
		return -1;
	}

	// 顺序查找，全局
	Rank find(const T& e) const
	{
		return find(e, 0, _size);
	}

	// 唯一化，返回被删除元素的总数
	int deduplicate(void)
	{
		int old_size = _size;
		Rank i = 1;
		while (i < _size)
		{
			(find(_elem[i], 0, i) < 0) ? i++ : remove(i);
		}
		return _size - old_size;
	}

	// 利用函数指针遍历操作
	void traverse(void (*visit)(T&))
	{
		for (Rank i = 0; i < _size; i++)
			visit(_elem[i]);
	}

	// 利用函数对象遍历操作
	template <typename VST>
	void traverse(VST& visit)
	{
		for (Rank i = 0; i < _size; i++)
			visit(_elem[i]);
	}

	// 判断数组是否有序，返回0表示有序
	int disordered(void) const
	{
		int ret = 0;
		for (Rank i = 1; i < _size; i++)
		{
			if (_elem[i - 1] > _elem[i])
				ret++;
		}
		return ret;
	}

	// 数组有序时的唯一化操作
	int uniquify(void)
	{
		if (disordered() == 0)
		{
			Rank i = 0, j = 0;
			int old_size = _size;
			while (++j < _size)
			{
				if (_elem[i] != _elem[j])
				{
					_elem[++i] = _elem[j];
				}
			}
			_size = ++i;
			return old_size - _size;
		}
		else
		{
			return deduplicate();
		}
	}

	// 查找接口，针对有序向量
	Rank search(T const& e, Rank lo, Rank hi) const
	{
		if (disordered() == 0)
		{
			// 随机选择二分查找还是斐波那契查找
			// rand() % 2 ? return biSearch(e, lo, hi) : return fibSearch(e, lo, hi);
			biSearch(e, lo, hi);
		}
		else
		{
			return find(e, lo, hi);
		}
	}

	Rank search(T const& e) const
	{
		return search(e, 0, _size);
	}

	// 二分查找
	Rank biSearch(T const& e, Rank lo, Rank hi) const
	{
		while (hi - lo > 1)
		{
			Rank mid = (lo + hi) >> 1;
			(e < _elem[mid]) ? hi = mid : lo = mid;
		}
		return (e == _elem[lo]) ? lo : -1;
	}

	// 冒泡排序
	void bubbleSort(Rank lo, Rank hi)
	{
		for (Rank i = lo; i < hi - 1; i++)
		{
			bool sorted = true;
			for (Rank j = i + 1; j < hi; j++)
			{
				if (_elem[i] > _elem[j])
				{
					sorted = false;
					swap(i, j);
				}
			}
			if (sorted)	break;
			
		}
	}

	void bubbleSort(void)
	{
		bubbleSort(0, _size);
	}

	// 归并排序
	void mergeSort(Rank lo, Rank hi)
	{
		if (hi - lo < 2)	return;
		Rank mid = (hi + lo) / 2;
		mergeSort(lo, mid);
		mergeSort(mid, hi);
		merge(lo, mid, hi);
	}

	void mergeSort(void)
	{
		mergeSort(0, _size);
	}

	void merge(Rank lo, Rank mid, Rank hi)
	{
		int lb = mid - lo;
		int lc = hi - mid;

		T* A = _elem + lo;
		T* B = new T[lb];
		T* C = _elem + mid;

		for (Rank i = 0; i < lb; i++)
			B[i] = A[i];

		Rank i = 0, j = 0, k = 0;
		for (; i < lb || j < lc;)
		{
			if ( i < lb && (j == lc || B[i] <= C[j]) )	A[k++] = B[i++];
			if ( j < lc && (i == lb || C[j] <  B[i]) )	A[k++] = C[j++];
		}		
		delete []B;
	}

protected:
	// 复制数组区间A[lo, hi)
	void copyFrom(T const* A, Rank lo, Rank hi)
	{
		if (lo > hi)	return;
		while (_size < hi - lo)
			expand();
		while (_size > hi - lo)
			shrink();

		Rank min_index = max(lo, 0);
		Rank max_index = min(hi, _size);

		if (min_index > _size - 1)	return;

		for (Rank i = min_index; i < max_index; i++)
		{
			_elem[i] = A[i];
		}
	}

	// 扩容
	void expand(void)
	{
		_size++;
		if (_size > _capacity)
		{
#ifdef DEBUG
			printf("expand size: %d  old_capacity: %d  new_capacity: %d\n", _size, _capacity, (int)(1.0 / _expand_ratio) * _capacity);
#endif

			int invert_ratio = (int)(1.0 / _expand_ratio);
			_capacity = invert_ratio * _capacity;
			T* new_elem = new T[_capacity];
			memset(new_elem, 0, _elem_size * _capacity);
			for (Rank i = 0; i < _size; i++)
			{
				new_elem[i] = _elem[i];
			}
			delete [] _elem;
			_elem = new_elem;
		}
	}

	// 缩容
	void shrink(void)
	{
		_size--;
		if (_size <= (int)(_shrink_ratio * _capacity) && _capacity >= DEFAULT_CAPACITY)
		{
#ifdef DEBUG
			printf("shrink size: %d  old_capacity: %d  new_capacity: %d\n", _size, _capacity, max((int)(_shrink_ratio * _capacity), _size));
#endif
			_capacity = max((int)(_shrink_ratio * _capacity), _size);
			T* new_elem = new T[_capacity];
			for (Rank i = 0; i < _size; i++)
			{
				new_elem[i] = _elem[i];
			}
			delete [] _elem;
			_elem = new_elem;
		}
	}

	// 交换元素
	void swap(Rank lhs, Rank rhs)
	{
		T tmp = _elem[lhs];
		_elem[lhs] = _elem[rhs];
		_elem[rhs] = tmp;
	}

	

protected:
    // 容量
    int _capacity;
    // 实际规模
    int _size;
    // 元素数组
    T* _elem;
    // 元素大小
    int _elem_size;
	// 扩容率
	float _expand_ratio;
	// 缩容率
	float _shrink_ratio;
};

#endif