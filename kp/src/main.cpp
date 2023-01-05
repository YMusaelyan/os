#include <iostream>
#include <ctime>
#include <vector>
#include "binary_allocator.h"
#include "freeblock_allocator.h"
 	
using namespace std;

int main() {
	int start, end, size;
	auto allocator1 = BinaryAllocator(1024000);
	vector<void *> pointer(10000);
	start = clock();
	for(int i = 0; i < 10000; i++) {
		pointer[i] = allocator1.allocate(i + 1);
	}
	end = clock();
	cout << "Time allocate BinaryAllocator:" << end - start <<"\n"<< endl;

	start = clock();
	for(int i = 0; i < 10000; i++) {
		allocator1.deallocate(pointer[i]);
	}
	end = clock();
	cout << "Time deallocate BinaryAllocator:" << end - start <<"\n"<< endl;

	auto allocator2 = FreeBlocksAllocator(1024000);
	start = clock();
	for(int i = 0; i < 10000; i++) {
		pointer[i] = allocator2.allocate(i + 1);
	}
	end = clock();
	cout << "Time allocate FreeBlocksAllocator:" << end - start <<"\n"<< endl;

	start = clock();
	for(int i = 0; i < 10000; i++) {
		allocator2.deallocate(pointer[i]);
	}
	end = clock();
	cout << "Time deallocate FreeBlocksAllocator:" << end - start <<"\n"<< endl;

	return 0;
}