#include<iostream>

using namespace std;

extern "C" int PrimeCount(int A, int B) {
	int cnt = 0, flag = 1;
	for(int i = A; i <= B; i++) {
		flag = 1;
		if (i <= 1) {
			continue;
		}
		for(int j = 2; j*j <= i; j++) {
			if(i % j == 0) {
				flag = 0;
			}
		}
		if (flag == 1) {
			cnt++;
		}
	}
	return cnt;
}

extern "C" float Square(float A, float B) {
	return A * B;
}