#include <uart.h>
#include "esp32s3_mem.h"

#define __IM  volatile const
#define __OM  volatile
#define __IOM volatile

// https://documentation.espressif.com/api/resource/doc/file/wyBAprze/FILE/esp32-s3_technical_reference_manual_en.pdf#Regfloat.17.4
struct system_reg_t {
    union {
        uint32_t system_core_1_control_0_reg;   // 0x0000

        struct {
            __IOM int system_control_core_1_runstall : 1;
            __IOM int system_control_core_1_clkgate_en : 1;
            __IOM int system_control_core_1_reseting : 1;
        } system_core_1_control_0_flags;
    };

    uint32_t system_core_1_control_1_reg;       // 0x0004

    char _pad_0010[0x02];
    union {
        uint32_t system_cpu_per_conf_reg;       // 0x0010

        struct {
            int system_cpuperiod_sel : 1;           // Set this field to select the CPU clock frequency. (R/W)
            
            int system_pll_freq_sel : 1;            // Set this bit to select the PLL clock frequency. (R/W)

            int system_cpu_wait_mode_force_on : 1;  // Set this bit to force on the clock gate of CPU wait mode.
                                                    // Usually, after executing the WAITI instruction, CPU enters the wait mode, during which the clock
                                                    // gate of CPU is turned off until any interrupts occur. In this way, power consumption is reduced.
                                                    // However, if this bit is set, the clock gate of CPU is always on and will not be turned off by the
                                                    // WAITI instruction. (R/W)

            int system_cpu_waiti_delay_num : 4;     // Sets the number of delay cycles to turn off the CPU clock gate
                                                    // after the CPU enters the wait mode because of a WAITI instruction. (R/W)
        } system_cpu_per_conf_flags;
    };

    char _pad_0018[0x04];
    union {
        uint32_t system_perip_clc_eno_reg;  // 0x0018

        struct {
            int                         : 1;
            int system_spi01_clk_en     : 1;
            int system_uart_clk_en      : 1;
            int                         : 1;
            int system_i2so_clk_en      : 1;
            int system_uart1_clk_en     : 1;
            int system_spi2_clk_en      : 1;
            int system_i2c_exto_clk_en  : 1;
            int system_uhci0_clk_en     : 1;
            int system_rmt_clk_en       : 1;
            int system_pcnt_clk_en      : 1;
            int system_ledc_clk_en      : 1;
            int                         : 1;
            int system_timergroup_clk_en : 1;
            int                         : 1;
            int system_timergroup1_clk_en : 1;
            int system_spi3_clk_en      : 1;
            int system_pwm0_clk_en      : 1;
            int system_i2c_ext1_clk_en  : 1;
            int system_can_clk_en       : 1;
            int system_pwm1_clk_en      : 1;
            int system_i2s1_clk_en      : 1;
            int                         : 1;
            int system_usb_clk_en       : 1;
            int system_uart_mem_clk_en  : 1;
            int                         : 3;
            int system_apb_saradc_clk_en : 1;
            int system_systimer_clk_en  : 1;
            int system_adc2_arb_clk_en  : 1;
        } system_perip_clc_eno_flags;
    };
    
    char _pad_0020[0x04];
    union {
        uint32_t system_perip_rst_eno_reg;  // 0x0020

        struct {
            int _reserved0              : 1;
            int system_spi01_rst        : 1;
            int system_uart_rst         : 1;
            int                         : 1;
            int system_i2so_rst         : 1;
            int system_uart1_rst        : 1;
            int system_spi2_rst         : 1;
            int system_i2c_exto_rst     : 1;
            int system_uhci0_rst        : 1;
            int system_rmt_rst          : 1;
            int system_pcnt_rst         : 1;
            int system_ledc_rst         : 1;
            int                         : 1;
            int system_timergroup_rst   : 1;
            int                         : 1;
            int system_timergroup1_rst  : 1;
            int system_spi3_rst         : 1;
            int system_pwm0_rst         : 1;
            int system_i2c_ext1_rst     : 1;
            int system_can_rst          : 1;
            int system_pwm1_rst         : 1;
            int system_i2s1_rst         : 1;
            int _reserved22             : 1;
            int system_usb_rst          : 1;
            int system_uart_mem_rst     : 1;
            int                         : 3;
            int system_apb_saradc_rst   : 1;
            int system_systimer_rst     : 1;
            int system_adc2_arb_rst     : 1;
        } system_perip_rst_eno_flags;
    };
};

volatile struct system_reg_t* system_reg = (struct system_reg_t*)SYS_LOW;

// Chapter 26.7.1 UART Registers
struct uart_reg_t {
    union {     // 0x0000
        uint32_t uart_fifo_reg;         
        uint8_t  uart_rxfifo_rd_byte;   // UARTn accesses FIFO via this field. (RO)
    };

    char _pad001C[0x18];
    union {     // 0x0018
        uint32_t uart_status_reg;
        
        struct {
            int uart_rxfifo_cnt : 10;
            int _reserved10     : 3;
            int uart_dsrn       : 1;
            int uart_ctsn       : 1;
            int uart_txfifo_cnt : 10;
            int _reserved26     : 3;
            int uart_dtrn       : 1;
            int uart_rtsn       : 1;
            int uart_txd        : 1;
        } uart_status_flags;
    };

    char _pad0078[0x56];
    union {     // 0x0078
        uint32_t uart_clk_conf_reg;
        struct {
            int uart_sclk_div_b     : 6;    // The denominator of the frequency divisor. (R/W)
            int uart_sclk_div_a     : 6;    // The numerator of the frequency divisor. (R/W)
            int uart_sclk_div_num   : 8;    // The integral part of the frequency divisor. (R/W)
            int uart_sclk_sel       : 2;    // Selects UART clock source. 1: APB_CLK; 2: RC_FAST_CLK; 3: XTAL_CLK. (R/W)
            int uart_sclk_en        : 1;    // Set this bit to enable UART TX/RX clock. (R/W)
            int uart_rst_core       : 1;    // Write 1 and then write 0 to this bit, to reset UART TX/RX. (R/W)
            int uart_tx_sclk_en     : 1;    // Set this bit to enable UART TX clock. (R/W)
            int uart_rx_sclk_en     : 1;    // Set this bit to enable UART RX clock. (R/W)
            int uart_tx_rst_core    : 1;    // Write 1 and then write 0 to this bit, to reset UART TX. (R/W)
            int uart_rx_rst_core    : 1;    // Write 1 and then write 0 to this bit, to reset UART RX. (R/W)
        } uart_clk_conf_flags;
    };

    union {     // 0x0080
        uint32_t uart_id_reg;

        struct {
            int uart_id : 30;
            int uart_update_ctrl : 1;
            int uart_reg_update  : 1;
        } uart_id_flags;
    };
};

struct uart_reg_t* uart0_reg = (struct uart_reg_t*)UART0_LOW;

// #ifdef ARCH_ESP
#define APB_CLK 80000000

static inline int uart_tx_ready(void) {
    return uart0_reg->uart_status_flags.uart_txfifo_cnt < 128;
}

static inline int uart_rx_available(void) {
    return uart0_reg->uart_status_flags.uart_rxfifo_cnt;
}

int uart_init(uint32_t baudrate) {
    // ESP32-S3 Technical Reference 26.5.2.1

    // Initializing UART0
    system_reg->system_perip_clc_eno_flags.system_uart_mem_clk_en = 1;
    system_reg->system_perip_clc_eno_flags.system_uart_clk_en = 1;
    system_reg->system_perip_rst_eno_flags.system_uart_rst = 0;

    uart0_reg->uart_clk_conf_flags.uart_rst_core = 1;
    system_reg->system_perip_rst_eno_flags.system_uart_rst = 1;
    system_reg->system_perip_rst_eno_flags.system_uart_rst = 0;
    uart0_reg->uart_clk_conf_flags.uart_rst_core = 0;

    uart0_reg->uart_id_flags.uart_update_ctrl = 0;

    // Configuring UART0 Communication
    while (uart0_reg->uart_id_flags.uart_reg_update);
    // TODO: finish it

    return 0;
}

void uart_putc(char data) {
    while (!uart_tx_ready());
    // TODO: write data to TX buffer
}

int uart_getc(void) {
    if (!uart_rx_available())
        return -1;

    return (int)uart0_reg->uart_rxfifo_rd_byte;
}

int uart_available(void) {
    return uart_rx_available();
}

void uart_flush(void) {
    while (uart0_reg->uart_status_flags.uart_txfifo_cnt);
}

// #endif