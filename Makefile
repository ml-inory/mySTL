test_vector : test_vector.o
	g++ -o test_vector test_vector.o

test_vector.o : test_vector.cpp
	g++ -c test_vector.cpp

clean:
	rm *.o