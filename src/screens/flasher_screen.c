#include <flasher_screen.h>
#include <ui/ui.h>
#include <keyboard.h>
#include <stdlib.h>
#include <string.h>
#include <platform.h>
#include <uart.h>
#include <digital.h>
#include <firmwares.h>
#include <debug.h>
#include <utils.h>
#include <drv/spi.h>

#define SELECTED_FIRMWARE_SIZE 64
#define RESET_TARGET_PIN 8
#define SS_TARGET_PIN 9

uint8_t method = 0;
PGM_P g_methods[] = {"Serial", "SPI"};

flasher_screen_t* load_flasher_screen(void) {
    flasher_screen_t* flasher_screen = (flasher_screen_t*)malloc(sizeof(flasher_screen_t));

    init_screen((screen_t*)flasher_screen, SCREEN_FLASHER, NULL, "Firms", "OK");

    flasher_screen->sel_str = (char*)malloc(SELECTED_FIRMWARE_SIZE);
    flasher_screen->selected_firm = 0;
    flasher_screen->state = -1;

    memcpy_P(flasher_screen->sel_str, "None");

    ui_text_min_t* selected_firmware = load_ui_text_min(COLOR_BLACK, NULL);
    selected_firmware->text = flasher_screen->sel_str;

    add_node_to_screen((screen_t*)flasher_screen, (ui_node_t*)load_ui_text_min(COLOR_BLACK, "Selected firm:"));
    add_node_to_screen((screen_t*)flasher_screen, (ui_node_t*)selected_firmware);
    add_node_to_screen((screen_t*)flasher_screen, (ui_node_t*)load_ui_text_min(COLOR_BLACK, "Flash method:"));
    // add_node_to_screen((screen_t*)flasher_screen, (ui_node_t*)load_ui_popup_menu(g_methods, 2));
    // add_node_to_screen((screen_t*)flasher_screen, (ui_node_t*)load_ui_button("Flash!"));

    return flasher_screen;
}

void unload_flasher_screen(flasher_screen_t* flasher_screen) {
    free(flasher_screen->sel_str);
}

void update_flasher_screen(flasher_screen_t* flasher_screen) {
    calc_key_t key = get_pressed_key();

    switch(key_to_option(key)) {
        case OPTION_CENTER:
            ui_menu_t* menu = load_ui_menu("Firmwares", g_firmwares_names, 2, NULL);
            add_node_to_screen((screen_t*)flasher_screen, (ui_node_t*)menu);
            break;

        case OPTION_BOTTOM:
            flasher_screen->state = FLASHER_START;
            break;

        default: break;
    }
}

void on_select_flasher_firmware(void* sender, int index) {
    flasher_screen_t* scr = (flasher_screen_t*)get_current_screen();
    int strend = strlen_P(template_selstr);

    if(scr->selected_firm != index) {
        memset((void*)((uintptr_t)scr->selected_firm + strend), 0, SELECTED_FIRMWARE_SIZE - strend);
        strcpy((char*)((uintptr_t)scr->selected_firm + strend), ((ui_menu_t*)sender)->elements[index]);
    }

    scr->selected_firm = index;
    delete_node_from_screen_ptr((ui_node_t*)sender, true);
}

// I put it here temproraly
uint8_t parse_byte(const char* str) {
    uint8_t ret = 0;
    ret = ((*str++) - '0') & 0xF;
    ret = (ret << 4) | (((*str++) - '0') & 0xF);

    return ret;
}

uint16_t parse_word(const char* str) {
    uint16_t ret = parse_byte(str);
    ret = (ret << 8) | parse_byte(str);

    return ret;
}

uint8_t flasher_spi_transfer(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    spi_transfer(a);
    spi_transfer(b);
    spi_transfer(c);
    return spi_transfer(d);
}

void reset_target() {
#ifdef PLATFORM_AVR
    pin_mode(RESET_PIN, OUTPUT);
    digital_write(RESET_PIN, true);
    delay_ms(100);
    digital_write(RESET_PIN, false);
#else
    if(method == FLASHER_SERIAL) {
        uart_set_dtr_flag(true);
        delay_ms(100);
        uart_set_dtr_flag(false);
    }
#endif
}

bool start_programming() {
    reset_target();

    if(method == FLASHER_SERIAL) {
        uart_begin(115200);

        if(uart_read() != 0x14 && uart_read() != 0x10) {
            dbg_err("Failed to start programming mode!");
            return false;
        }

        flasher_write(0x50);
        flasher_write(0x20);
    } else if(method == FLASHER_SPI) {
        spi_begin();
        spi_settings_t settings;
        spi_settings_init(&settings, 100000, MSBFIRST, SPI_MODE0);
        spi_begin_transaction(settings);
        delay_ms(10);

        flasher_spi_transfer(0xAC, 0x53, 0x00, 0x00);

        delay_ms(10);
    }

    return true;
}

void end_programming() {
    if(method == FLASHER_SERIAL) {
        flasher_write(0x51);
        flasher_write(0x20);
    } else if(method == FLASHER_SPI) {
        reset_target();
    }
}

uint32_t get_boardid() {
    uint32_t boardid = 0;

    if(method == FLASHER_SERIAL) {
        flasher_write(0x75);
        flasher_write(0x20);

        delay_ms(10);

        if(!uart_available() || uart_read() != 0x14) {
            dbg_err("First byte isn't 0x14");
            return 0;
        }

        while((uint8_t byte = uart_read()) != 0x10 && uart_available()) {
            boardid = (boardid << 8) | byte;
        }

        return boardid;
    }else if(method == FLASHER_SPI) {
        for(uint8_t i = 0; i < 3; i++) {
            boardid = (boardid << 8) | flasher_spi_transfer(0x30, 0x00, i, 0x00);
        }
    }
}

bool write_block(uint16_t addr, uint8_t* bytes, uint16_t length) {
    bool is_pgm = IS_PGM(bytes);

    if(method == FLASHER_SERIAL) {
        uart_write(0x55); // ADDR_BEGIN
        uart_write(LOW_BYTE(addr));
        uart_write(HIGH_BYTE(addr));
        uart_write(0x20); // ADDR_END

        delay_ms(10);

        uart_write(0x64); // LENGTH_BEGIN
        uart_write(HIGH_BYTE(length));
        uart_write(LOW_BYTE(length));
        uart_write(0x46); // LENGTH_END

        for(int i = 0; i < length; i++) {
            uart_write((is_pgm ? pgm_read_byte(bytes + i) : bytes[i]));
        }
        
        uart_write(0x20);

        if(uart_read() != 0x14 && uart_read() != 0x10) {
            dbg_err("Failed to write data to address %04x!", addr);
            return false;
        }

        return true;
    }
}

void update_flasher_state(flasher_screen_t* flasher_screen) {
    switch(flasher_screen->state) {

        case FLASHER_WRITING_BLOCK: {
            // char* p = (char*)firmwares[flasher_screen->selected_firm] + flasher_screen->hex_file_pos;
            
            // if(pgm_read_byte(p++) != ':') {
            //     dbg_warn("First char isn't ':'!");
            //     break;
            // }

            // char len[2] = {pgm_read_byte(p++), pgm_read_byte(p++)};
            // uint8_t block_len = parse_byte(len);

            // int block_size = 8 + block_len * 2; // addr (4 bytes) + type (2 bytes) + checksum (2 bytes)
            
            // char* block = malloc(block_size);
            // memcpy_P(block, p, block_size);

            // char* blockp = block;
            // uint16_t addr = parse_word(blockp);
            // uint8_t type = parse_byte(blockp);
            // // uint8_t checksum = block_len + addr + type;

            // flasher_write(0x55);
            // flasher_write(addr >> 8);
            // flasher_write(addr & 0xFF);
            // flasher_write(0x20);

            // delay_ms(10);

            // if(!uart_available() || uart_read() != 0x14 || uart_read() != 0x10) {
            //     dbg_err("Failed to write addr!");
            //     break;
            // }

            // flasher_write(0x64);
            // flasher_write(0x00);
            // flasher_write(block_len);
            // flasher_write(0x46);

            // for(int i = 0; i < block_len; i++) {
            //     uint8_t byte = parse_byte(blockp);
            //     flasher_write(byte);
            //     // checksum += byte;
            // }

            // flasher_write(0x20);
            // free(block);

            // if(!uart_available() || uart_read() != 0x14 || uart_read() != 0x10) {
            //     dbg_err("Failed to write data to address %04x!", addr);
            //     break;
            // }

            // while(pgm_read_byte(p) != ':') p++; // seeking next block

            // if(type == 2) {
            //     flasher_screen->state = FLASHER_FINISH;
            //     break;
            // }

            break;
        }
    }
}