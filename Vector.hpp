/*
 * Date: 2020/11/30
 * Description: 向量 
 */
#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

#include <assert.h>
#include <string.h>

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
	// 用容量初始化
	Vector(int capacity = DEFAULT_CAPACITY) :
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
		_capacity *= 2;
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
		copyFrom(V._elem, 0, V._size);
	}

	// 析构函数
	~Vector()
	{
		if (nullptr != _elem)
			delete _elem;
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
			delete _elem;
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
		for (Rank i = _size - index - 1; i >= index + 1; i--)
		{
			// printf("copy %d - %d : %d\n", i - 1, i, _elem[i - 1]);
			_elem[i] = _elem[i - 1];
		}

		_elem[index] = new_elem;
		return index;
	}

protected:

	// 复制数组区间A[lo, hi)
	void copyFrom(T const* A, Rank lo, Rank hi)
	{
		if (lo > hi)	return;

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
		if (_size >= (int)(_expand_ratio * _capacity))
		{
			int invert_ratio = (int)(1.0 / _expand_ratio);
			_capacity = invert_ratio * _capacity;
			T* new_elem = new T[_capacity];
			memset(new_elem, 0, _elem_size * _capacity);
			for (Rank i = 0; i < _size; i++)
			{
				new_elem[i] = _elem[i];
			}
			delete _elem;
			_elem = new_elem;
		}
	}

	// 缩容
	void shrink(void)
	{
		_size--;
		if (_size <= (int)(_shrink_ratio * _capacity))
		{
			_capacity = (int)(_shrink_ratio * _capacity);
			T* new_elem = new T[_capacity];
			for (Rank i = 0; i < _size; i++)
			{
				new_elem[i] = _elem[i];
			}
			delete _elem;
			_elem = new_elem;
		}
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