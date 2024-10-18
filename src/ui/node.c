#include <node.h>
#include <menu.h>
#include <image.h>

#include <stdlib.h>

void unload_node(ui_node_t* node) {
    switch(node->type) {
        case UI_MENU: 
            unload_ui_menu((ui_menu_t*)node); 
            break;
        
        case UI_IMAGE:
            unload_ui_image((ui_image_t*)node);
            break;

        default: 
            free(node);
            node = NULL;
            break;
    }
}