#include <hal/uart.h>
#include "kernel.h"
#include <stdio.h>
#include <stdlib.h>
#include <hal/memory.h>

hal_uart_handle_t* uart0;

extern const module_t __modules_start[];
extern const module_t __modules_end[];

static bool* initialized_modules = NULL;

void kernel_init(void) {
	{
		hal_uart_config_t uart_config;
		hal_uart_get_default_config(&uart_config);
		uart0 = hal_uart_init(HAL_UART_PORT_0, &uart_config);
	}
		
	if (uart0 != NULL) {
		hal_uart_puts_const(uart0, "UART0 initialized successfully.\r\n");
	}

	initialized_modules = calloc((__modules_end - __modules_start) / sizeof(module_t), 1);

	if (initialized_modules == NULL) {
		hal_uart_puts_const(uart0, "Out of memory!\r\n");
		exit(1);
	}

	module_t* m;
	for (m = &__modules_start; m != &__modules_end; m++) {
		module_t mod;
		hal_memory_read_module(&mod, m);
	
		int i = m - __modules_start;

		if (mod.flags & MODULE_FLAG_DONT_INIT) {
			initialized_modules[i] = false;
			continue;
		}

		initialized_modules[i] = mod.init();

		if (initialized_modules[i]) {
			hal_uart_puts_const(uart0, "Successfully initialized module ");
		} else {
			hal_uart_puts_const(uart0, "Failed to initialize module ");
		}

		hal_uart_puts(uart0, mod.name);
		hal_uart_puts_const(uart0, "\r\n");
	}
}

void kernel_loop(void) {
	while (1) {
		module_t* m;

		for (m = &__modules_start; m != &__modules_end; m++) {
			int i = m - __modules_start;

			if (initialized_modules[i]) {
				module_t mod;
				hal_memory_read_module(&mod, m);
				
				mod.loop();
			}
		}
	}
}