#include<iostream>
#include<dlfcn.h>
#include"lib.h"
#include<string>

using namespace std;

int main()
{
	string lib1 = "./liblib1.so";
	string lib2 = "./liblib2.so";
	
	void *curlib = dlopen(lib1.c_str(), RTLD_LAZY);

	int (*PrimeCount)(int A, int B);
	float (*Square)(float A, float B);

	PrimeCount = (int (*)(int, int))dlsym(curlib, "PrimeCount");
    Square = (float (*)(float, float))dlsym(curlib, "Square");
	
	int k;
	int number_lib = 1;
	cout << "Enter 1 or 2 to select a function or -1 to exit or 0 switch to library" << endl;

	while(cin >> k) {

		if (k == 0) {
			dlclose(curlib);
			if (number_lib == 1) {
				cout << "You switched to library number 2" << endl;
				curlib = dlopen(lib2.c_str(), RTLD_LAZY);
				number_lib = 2;

			} else {
				cout << "You switched to library number 1" << endl;
				curlib = dlopen(lib1.c_str(), RTLD_LAZY);
				number_lib= 1;
			}
			PrimeCount = (int (*)(int, int))dlsym(curlib, "PrimeCount");
    		Square = (float (*)(float, float))dlsym(curlib, "Square");
		}

		if (k == 1) {
			int A, B, Ans1;
			cout << "Enter section [A,B]" << endl;
			cin >> A >> B;
			Ans1 = PrimeCount(A, B);
			cout<<"Count of prime numbers:" << Ans1 << endl;
		}

		if (k == 2) {
			float A, B, Ans2;
			cout << "Enter side lengths A and B" << endl;
			cin >> A >> B;
			Ans2 = Square(A, B);
			cout << "Square:" << Ans2 << endl;
		}

		if (k == -1) {
			cout << "Exit" << endl;
			return 0;
		}

		cout << "Enter 1 or 2 to select a function or 0 or -1 to exit" << endl;
		
	}

}