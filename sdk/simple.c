#include <stdint.h>

#define SVC(code) __asm__ __volatile__ ("svc %0" : : "I" (code) )

void entry() {
	int a = 0;
	while(1) {
		a += func(a, a-1) * func(a, a*2);
		SVC('a');
	}
};


int func(int a, int b) {
	return (a*a)+b;
}