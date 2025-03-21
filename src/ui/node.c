#include <node.h>
#include <menu.h>
#include <ui.h>
#include <stdlib.h>

void load_node(ui_node_t* node, node_type_t type) {
    node->type = type;
    node->x = -1;
    node->y = -1;
    node->width = -1; 
}

void unload_node(ui_node_t* node) {
    switch(node->type) {
        case UI_IMAGE:
            unload_ui_image((ui_image_t*)node);
            break;
        
        case UI_BUTTON:
            unload_ui_button((ui_button_t*)node);
            break;

        default: 
            free(node);
            node = NULL;
            break;
    }
}