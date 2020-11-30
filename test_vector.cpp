#include "Vector.hpp"
#include <stdio.h>

int main()
{
    const int size = 10;
    const int init_elem = 1;
    Vector<int> a(size, init_elem);
    for (Rank i = 0; i < size; i++)
    {
        printf("a[%d] = %d\n", i, a[i]);
    }
    
    return 0;
}