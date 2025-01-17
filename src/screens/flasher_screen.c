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

#define SELECTED_FIRMWARE_SIZE 64
#define RESET_PIN 8

PGM_P template_selstr = "calcos.bin";
PGM_P g_methods[] = {"Serial", "SPI"};
PGM_P g_firmwares[] = {"blink500.hex", "blink1000.hex"};

flasher_screen_t* load_flasher_screen(void) {
    flasher_screen_t* flasher_screen = (flasher_screen_t*)malloc(sizeof(flasher_screen_t));

    init_screen((screen_t*)flasher_screen, SCREEN_FLASHER, NULL, "Select", "Flash!");

    flasher_screen->sel_str = (char*)malloc(SELECTED_FIRMWARE_SIZE);
    flasher_screen->selected_firm = 0;
    flasher_screen->state = -1;
    strcpy_P(flasher_screen->sel_str, template_selstr);

    ui_text_min_t* selected_firmware = load_ui_text_min(COLOR_BLACK, NULL);
    selected_firmware->text = flasher_screen->sel_str;

    add_node_to_screen((screen_t*)flasher_screen, (ui_node_t*)selected_firmware);

    ui_text_min_t* txt = load_ui_text_min(COLOR_BLACK, "Flash method:");
    add_node_to_screen((screen_t*)flasher_screen, (ui_node_t*)txt);

    const char** methods = malloc(sizeof(g_methods));
    memcpy_P(methods, g_methods, sizeof(g_methods));
    ui_popup_menu_t* method_popup = load_ui_popup_menu(methods, 2);
    add_node_to_screen((screen_t*)flasher_screen, (ui_node_t*)method_popup);

    ui_button_t* btn = load_ui_button("Flash!");
    add_node_to_screen((screen_t*)flasher_screen, (ui_node_t*)btn);

    return flasher_screen;
}

void unload_flasher_screen(flasher_screen_t* flasher_screen) {
    free(flasher_screen->sel_str);
}

void update_flasher_screen(flasher_screen_t* flasher_screen) {
    key_t key = get_pressed_key();

    switch(key_to_option(key)) {
        case OPTION_CENTER:
            const char** firmwares = malloc(sizeof(g_firmwares));
            memcpy_P(firmwares, g_firmwares, sizeof(g_firmwares));

            ui_menu_t* menu = load_ui_menu("Firmwares", firmwares, 2, NULL);
            add_node_to_screen((screen_t*)flasher_screen, (ui_node_t*)menu);
            break;

        case OPTION_BOTTOM:
            flasher_screen->state = FLASHER_START;
            break;

        default: break;
    }
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

void flasher_write(uint8_t byte) {
    uart_write(byte);
}

uint8_t flasher_read(void) {
    while(!uart_available());
    return uart_read();
}

void update_flasher_state(flasher_screen_t* flasher_screen) {
    switch(flasher_screen->state) {
        case FLASHER_START: {
            uart_begin(115200);
#ifdef PLATFORM_AVR
            pin_mode(RESET_PIN, OUTPUT);
            digital_write(RESET_PIN, true);
#endif

            flasher_screen->state = FLASHER_PROGRAMMING_MODE;
            break;
        }
        
        case FLASHER_PROGRAMMING_MODE: {
#ifdef PLATFORM_AVR
            digital_write(RESET_PIN, false);
#endif

            flasher_write(0x50);
            flasher_write(0x20);

            flasher_screen->state = FLASHER_BOARD_ID_VERIFY;
            break;
        }

        case FLASHER_BOARD_ID_VERIFY: {
            flasher_write(0x75);
            flasher_write(0x20);

            delay_ms(10);

            uint8_t tries = 0;
            uint32_t boardid = 0;
            
            while(tries < 10) {
                if(!uart_available() || uart_read() != 0x14) {
                    dbg_err("First byte isn't 0x14");
                    tries++;
                    delay_ms(10);
                    continue;
                }

                uint8_t byte;
                while((byte = uart_read()) != 0x10 && uart_available()) {
                    boardid = (boardid << 8) | byte;
                }

                if(byte == 0x10) {
                    dbg_info("Boardid received successfully!");
                    break;
                }

                tries++;
                delay_ms(10);
            }

            flasher_screen->state = FLASHER_WRITING_BLOCK;
            break;
        }

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

        case FLASHER_FINISH: {
            flasher_write(0x51);
            flasher_write(0x20);
            break;
        }
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