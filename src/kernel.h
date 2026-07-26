#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <hal/uart.h>

extern hal_uart_handle_t* uart0;

typedef enum {
    MODULE_TYPE_LIBRARY  = 0,
    MODULE_TYPE_SERVICE  = 1,
    MODULE_TYPE_APP      = 2,
    MODULE_TYPE_UI       = 3
} module_type_t;

#define MODULE_FLAG_NONE      0x00
#define MODULE_FLAG_DONT_INIT 0x01

typedef struct module_t {
    const char* name;
    uint8_t type;
    uint8_t flags;

    bool (*init)(void);
    void (*loop)(void);
    void (*deinit)(void);
} module_t;

#define MODULE_REGISTER(_name, _type, _flags, _init, _loop, _deinit) \
    static const module_t _module_##_name \
    __attribute__((section(".modules"), used, aligned(sizeof(void*)))) = { \
        .name   = #_name, \
        .type   = _type, \
        .flags  = _flags, \
        .init   = _init, \
        .loop   = _loop, \
        .deinit = _deinit  \
    }

void kernel_init(void);
void kernel_loop(void);


#endif /* _KERNEL_H_ */