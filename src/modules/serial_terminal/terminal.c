#include <kernel.h>
#include "arch.h"

static char buffer[128]; // Command buffer
static int buffer_size;
static bool welcome_message;

void terminal_handle_command(const char* command);

bool terminal_init(void) {
    welcome_message = false;
    buffer_size = 0;

    // Printing welcome message here is not good because kernel still prints initialization messages

    return true;
}

void terminal_loop(void) {
    if (!welcome_message) {
        hal_uart_puts_const(uart0, "\033[36;1mWelcome to CalcOS terminal!\033[0m\r\nType \033[33;1mhelp\033[0m for list of available commands.\033[0m\r\n> ");
        welcome_message = true;
    }

    if (hal_uart_rx_available(uart0)) {
        char c = hal_uart_getc(uart0);

        if (c == '\r' || c == '\n') {
            hal_uart_puts_const(uart0, "\r\n");
            terminal_handle_command(buffer);
            memset(buffer, '\0', sizeof(buffer));        
            buffer_size = 0;

            hal_uart_puts_const(uart0, "\r\n> ");
            return;
        }

        if (c == '\b') {
            hal_uart_putc(uart0, c);
            buffer_size--;
            return;
        }

        hal_uart_putc(uart0, c);
        buffer[buffer_size++] = c;
    }
}

MODULE_REGISTER(serial_terminal, MODULE_TYPE_SERVICE, MODULE_FLAG_NONE, terminal_init, terminal_loop, NULL);

void terminal_handle_command(const char* command) {
    if (strncmp(command, "help", 4) == 0) {
        hal_uart_puts_const(uart0, 
            "List of available commands:\r\n"
            " help - prints this\r\n"
            " version - prints OS version\r\n"
            " echo <message> - prints message\r\n");
        return;
    }

    if (strncmp(command, "version", 7) == 0) {
        hal_uart_puts_const(uart0, "CalcOS v1.0.0 "__ARCH__" (GCC "__VERSION__"; "__RUNTIME_INFO__")");
        return;
    }

    if (strncmp(command, "echo ", 5) == 0) {
        hal_uart_puts(uart0, command + 5);
        return;
    }

    hal_uart_puts_const(uart0, "Unknown command: ");
    hal_uart_puts(uart0, command);
    hal_uart_puts_const(uart0, "\r\n");
}