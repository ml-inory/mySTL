/*
 * Date: 2020/11/30
 * Description: 向量 
 */
#ifndef __VECTOR_HPP__
#define __VECTOR_HPP__

// 默认容量
#define DEFAULT_CAPACITY    8

// 秩
typedef int     Rank;

template <typename T>
class Vector
{
public:
    // 用容量初始化
    Vector(int capacity = DEFAULT_CAPACITY):
        _capacity(capacity),
        _size(0),
        _elem(nullptr),
        _elem_size(sizeof(T))
    {
        _elem = new T[_capacity];
    }

    // 用初始元素初始化
    Vector(int size, T init_elem):
        _capacity(2*size),
        _size(size),
        _elem(nullptr),
        _elem_size(sizeof(T))
    {
        _elem = new T[_capacity];
        for (Rank i = 0; i < _size; i++)
            _elem[i] = init_elem;
    }

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

    // 获取元素
    T& operator [] (Rank rank)
    {
        return _elem[rank];
    }

private:
    // 容量
    int _capacity;
    // 实际规模
    int _size;
    // 元素数组
    T* _elem;
    // 元素大小
    int _elem_size;
};

#endif