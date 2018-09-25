#include <stdint.h>
#include <stddef.h>
extern char _heap_start;

#define BREAKPOINT(code) __asm__ __volatile__ ("bkpt %0" : : "I" (code) )
#define SVC(code) __asm__ __volatile__ ("svc %0" : : "I" (code) )
#define ISR __attribute__((weak, interrupt("IRQ")))
extern "C" {
void _isr_empty() {};
void ISR _isr_nmi() {};
void ISR _isr_hardfault() {
	// 1) peek the return addres from the stack into r0
	register uint32_t* stack asm("sp");
	BREAKPOINT(0x42);
};
void ISR _isr_svcall() {};
void ISR _isr_pendsv() {};
void ISR _isr_systick() {};
void ISR _isr_external_interruput() {};

class blah {
public:
	blah()
		: _address(0) {

	}
	uint32_t plop() const {
		return _address * 2;
	}
	void mutate() {
		_address = 42;
	}
private:
	uint32_t _address;
};

void entry() {
	volatile uint32_t* heap = (uint32_t*)&_heap_start;
	for(uint32_t i = 0; i < 1024; i++) {
		heap[i % 16] = i;
	}
	blah* blas = reinterpret_cast<blah*>(&_heap_start);
	for(uint32_t i = 0; i < 16; i++) {
		blas[i].mutate();
	}
	BREAKPOINT(0);
}


void _isr_reset() {
	// zero bss
	extern uint32_t* __bss_start__;
	extern uint32_t* __bss_end__;
	for (uint8_t* dst = (uint8_t*)__bss_start__; dst < (uint8_t*)__bss_end__; dst++) {
		*dst = 0;
	}
	entry();
}
}
