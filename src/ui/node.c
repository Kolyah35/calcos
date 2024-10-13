#include <node.h>
#include <menu.h>

#include <stdlib.h>

void unload_node(ui_node_t* node) {
    switch(node->type) {
        case UI_MENU: 
            unload_ui_menu((ui_menu_t*)node); 
            break;
        default: 
            free(node); 
            break;
    }
}