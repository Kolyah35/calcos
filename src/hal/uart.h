#ifndef HAL_UART_H
#define HAL_UART_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define hal_uart_puts(handle, str) hal_uart_write(handle, (uint8_t*)(str), strlen(str))
#define hal_uart_puts_const(handle, str) hal_uart_write(handle, (uint8_t*)(str), sizeof(str)-1)

typedef struct hal_uart_instance hal_uart_handle_t;

typedef enum {
    HAL_UART_PORT_0 = 0,
    HAL_UART_PORT_1 = 1,
    HAL_UART_PORT_2 = 2,
    HAL_UART_PORT_3 = 3,
    HAL_UART_PORT_MAX
} hal_uart_port_t;

typedef struct {
    uint8_t  enable_tx : 1;
    uint8_t  enable_rx : 1; 
    uint8_t  stop_bits : 1; // stop bits count [1-2]
    uint8_t  parity    : 2; // parity verification (NONE; ODD; EVEN)
    uint8_t  data_bits : 3; // data bits count [5-8]
    uint32_t baudrate;      // bandwidth (bits per sec)
} hal_uart_config_t;

#define HAL_UART_PARITY_NONE  0
#define HAL_UART_PARITY_ODD   1
#define HAL_UART_PARITY_EVEN  2

void hal_uart_get_default_config(hal_uart_config_t* dest);

hal_uart_handle_t* hal_uart_init(hal_uart_port_t port, const hal_uart_config_t* config);

void hal_uart_deinit(hal_uart_handle_t* handle);

void hal_uart_putc(hal_uart_handle_t* handle, uint8_t c);

void hal_uart_write(hal_uart_handle_t* handle, uint8_t* data, size_t len);

uint8_t hal_uart_getc(hal_uart_handle_t* handle);

bool hal_uart_rx_available(hal_uart_handle_t* handle);

bool hal_uart_tx_ready(hal_uart_handle_t* handle);

void hal_uart_flush(hal_uart_handle_t* handle);

#endif /* HAL_UART_H */