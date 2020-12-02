#include "Vector.hpp"
#include <stdio.h>

void print_vector(Vector<int>& vec)
{
	for (Rank i = 0; i < vec.size(); i++)
	{
		printf("a[%d] = %d\n", i, vec[i]);
	}
	printf("\n");
}

int main()
{
    const int size = 10;
    const int init_elem = 1;
    Vector<int> a(size, init_elem);

	printf("origin:\n");
	print_vector(a);

	printf("push_back:\n");
	a.push_back(10);
	print_vector(a);

	printf("insert:\n");
	a.insert(0, 11);
	print_vector(a);

	getchar();
    
    return 0;
}