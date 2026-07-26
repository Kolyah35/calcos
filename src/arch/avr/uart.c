#ifdef __AVR__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <hal/uart.h>

struct hal_uart_instance {
    volatile uint8_t *ubrrh_reg;
    volatile uint8_t *ubrrl_reg;
    volatile uint8_t *ucsra_reg;
    volatile uint8_t *ucsrb_reg;
    volatile uint8_t *ucsrc_reg;
    volatile uint8_t *udr_reg;
    uint8_t           initialized;
};

#if defined(UBRRH) || defined(UBRR0H)
#   define USART_MAX_PORT 0

static struct hal_uart_instance uart0_instance = {
    .ubrrh_reg = &UBRR0H,
    .ubrrl_reg = &UBRR0L,
    .ucsra_reg = &UCSR0A,
    .ucsrb_reg = &UCSR0B,
    .ucsrc_reg = &UCSR0C,
    .udr_reg   = &UDR0,
    .initialized = 0
};

#endif
#if defined(UBRR1H)
#   define USART_MAX_PORT 1

static struct hal_uart_instance uart1_instance = {
    .ubrrh_reg = &UBRR1H,
    .ubrrl_reg = &UBRR1L,
    .ucsra_reg = &UCSR1A,
    .ucsrb_reg = &UCSR1B,
    .ucsrc_reg = &UCSR1C,
    .udr_reg   = &UDR1,
    .initialized = 0
};

#endif
#if defined(UBRR2H)
#   define USART_MAX_PORT 2

static struct hal_uart_instance uart2_instance = {
    .ubrrh_reg = &UBRR2H,
    .ubrrl_reg = &UBRR2L,
    .ucsra_reg = &UCSR2A,
    .ucsrb_reg = &UCSR2B,
    .ucsrc_reg = &UCSR2C,
    .udr_reg   = &UDR2,
    .initialized = 0
};

#endif
#if defined(UBRR3H)
#   define USART_MAX_PORT 3

static struct hal_uart_instance uart3_instance = {
    .ubrrh_reg = &UBRR3H,
    .ubrrl_reg = &UBRR3L,
    .ucsra_reg = &UCSR3A,
    .ucsrb_reg = &UCSR3B,
    .ucsrc_reg = &UCSR3C,
    .udr_reg   = &UDR3,
    .initialized = 0
};

#endif

#define USART_PORT_COUNT (USART_MAX_PORT + 1)

static struct hal_uart_instance* uart_ports[USART_PORT_COUNT] = {
#if USART_PORT_COUNT > 0
    &uart0_instance,
#endif

#if USART_PORT_COUNT > 1
    &uart1_instance,
#endif

#if USART_PORT_COUNT > 2
    &uart2_instance,
#endif

#if USART_PORT_COUNT > 3
    &uart3_instance
#endif
};

void hal_uart_get_default_config(hal_uart_config_t* dest) {
    dest->enable_tx = 1;
    dest->enable_rx = 1;
    dest->data_bits = 8;
    dest->parity    = HAL_UART_PARITY_NONE;
    dest->stop_bits = 1;
    dest->baudrate  = 115200;
}

hal_uart_handle_t* hal_uart_init(hal_uart_port_t port, const hal_uart_config_t* config) {
    if (port > USART_MAX_PORT || config == NULL) {
        return NULL;
    }
    
    struct hal_uart_instance* uart = uart_ports[port];

    if (uart->initialized) {
        return uart;
    }
    
    uint16_t ubrr = (F_CPU / 16UL / config->baudrate) - 1;
    *uart->ubrrl_reg = (uint8_t)(ubrr & 0xFF);
    *uart->ubrrh_reg = (uint8_t)(ubrr >> 8);
    
    uint8_t ucsrc_value = ((config->data_bits - 5) & 0b0000011) << UCSZ00;
    
    if (config->stop_bits == 2) {
        ucsrc_value |= (1 << USBS0);
    }

    if (config->parity != HAL_UART_PARITY_NONE) {
        // 00 - disabled
        // 01 - reserved
        // 10 - even
        // 11 - odd
        ucsrc_value |= ((config->parity + 1) << UPM00);
    }
    
    *uart->ucsrc_reg = ucsrc_value;
    *uart->ucsrb_reg = (config->enable_rx << RXEN0) | (config->enable_tx << TXEN0);
    uart->initialized = 1;

    return uart;
}

void hal_uart_deinit(hal_uart_handle_t* handle) {
    hal_uart_flush(handle);

    struct hal_uart_instance *uart = (struct hal_uart_instance *)handle;

    if (!uart) return;
    
    *uart->ucsrb_reg = 0;
    uart->initialized = 0;
}

void hal_uart_putc(hal_uart_handle_t* handle, uint8_t c) {
    struct hal_uart_instance *uart = (struct hal_uart_instance *)handle;

    while (!(*uart->ucsra_reg & (1 << UDRE0)));
    *uart->udr_reg = c;
}

void hal_uart_write(hal_uart_handle_t* handle, uint8_t* data, size_t len) {
    struct hal_uart_instance *uart = (struct hal_uart_instance *)handle;

    for (size_t i = 0; i < len; i++) {
        while (!(*uart->ucsra_reg & (1 << UDRE0)));
        *uart->udr_reg = data[i];
    }
}

uint8_t hal_uart_getc(hal_uart_handle_t* handle) {
    struct hal_uart_instance *uart = (struct hal_uart_instance *)handle;
    
    while (!(*uart->ucsra_reg & (1 << RXC0)));
    return *uart->udr_reg;
}

bool hal_uart_rx_available(hal_uart_handle_t* handle) {
    struct hal_uart_instance *uart = (struct hal_uart_instance *)handle;
    return (*uart->ucsra_reg & (1 << RXC0));
}

bool hal_uart_tx_ready(hal_uart_handle_t* handle) {
    struct hal_uart_instance *uart = (struct hal_uart_instance *)handle;
    return (*uart->ucsra_reg & (1 << UDRE0));
}

void hal_uart_flush(hal_uart_handle_t* handle) {
    struct hal_uart_instance *uart = (struct hal_uart_instance *)handle;
    unsigned char dummy;
    while (*uart->ucsra_reg & (1 << RXC0)) dummy = *uart->udr_reg;
}

#endif /* __AVR__ */