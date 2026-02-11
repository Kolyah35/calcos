#include "uart.h"

void kernel_main(void) {
	uart_init(115200);
	uart_puts("Hello UART!");

	while(1);
}
