/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/
#define SVC(code) __asm__ __volatile__ ("svc %0" : : "I" (code) )
#define ISR __attribute__((weak, interrupt("IRQ")))
void _isr_empty() {};
void ISR _isr_nmi() {};
void ISR _isr_hardfault() {};
void ISR _isr_svcall() {};
void ISR _isr_pendsv() {};
void ISR _isr_systick() {};
void ISR _isr_external_interruput() {};


int fib(int n){
	if ( n == 0 ) {
		return 0;
	} else if ( n == 1 ) {
		return 1;
	} else {
		return (fib(n-1) + fib(n-2));
	}
}

void _isr_reset() {
	SVC(42);
	fib(25);
}
