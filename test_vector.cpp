#define DEBUG	1

#include "Vector.hpp"
#include <stdio.h>

const int size = 5;

void print_vector(Vector<int>& vec)
{
	for (Rank i = 0; i < vec.size(); i++)
	{
		printf("a[%d] = %d\n", i, vec[i]);
	}
	printf("\n");
}

Vector<int> get_vector()
{
	Vector<int> ret(size, 0);
	for (Rank i = 0; i < size; i++)
	{
		ret[i] = i;
	}
	return ret;
}

void test_push_back()
{
	printf("push_back:\n");
	Vector<int> a = get_vector();
	a.push_back(10);
	print_vector(a);
}

void test_insert()
{
	printf("insert:\n");
	Vector<int> a = get_vector();
	a.insert(0, 0);
	print_vector(a);
}

void test_deduplicate()
{
	printf("deduplicate:\n");
	Vector<int> a = get_vector();
	a.push_back(1);
	a.push_back(2);
	a.deduplicate();
	print_vector(a);
}

void test_expand()
{
	printf("expand:\n");
	Vector<int> a = get_vector();
	int vec_size = a.size();
	for (int i = 0; i < vec_size; i++)
		a.push_back(1);
}

void test_shrink()
{
	printf("shrink:\n");
	Vector<int> a = get_vector();
	while (!a.empty())
		a.remove(0);
}

void test_uniquify()
{
	printf("uniquify:\n");
	Vector<int> a = get_vector();
	a.insert(1, 1);
	a.insert(1, 1);
	a.uniquify();
	print_vector(a);
}

void test_search()
{
	printf("search:\n");
	Vector<int> a = get_vector();
	//a.insert(1, 1);
	//a.insert(1, 1);
	//print_vector(a);
	Rank i = a.search(3);
	print_vector(a);
	printf("found 3 at %d\n", i);
}

int main()
{
	printf("origin:\n");
	print_vector(get_vector());

	test_push_back();
	test_insert();
	test_expand();
	test_shrink();
	test_deduplicate();
	test_uniquify();
	test_search();

	getchar();
    
    return 0;
}