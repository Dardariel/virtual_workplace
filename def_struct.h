#ifndef DEF_STRUCT_H
#define DEF_STRUCT_H
#include <stdint.h>

#define KEY_RIGHT_ALT 108
#define KEY_RIGHT_SHIFT 62
#define KEY_RIGHT_CTRL 105
#define KEY_LEFT_ALT 64
#define KEY_LEFT_SHIFT 50
#define KEY_LEFT_CTRL 37


typedef struct
{
    uint8_t MODIFIER;
    /* MODIFIER
    bit 7 - Right GUI
    bit 6 - Right ALT
    bit 5 - Right SHIFT
    bit 4 - Right CTRL
    bit 3 - Left GUI
    bit 2 - Left ALT
    bit 1 - Left SHIFT
    bit 0 - Left CTRL
    */
    uint8_t RESERVED;
    uint8_t KEYCODE1;
    uint8_t KEYCODE2;
    uint8_t KEYCODE3;
    uint8_t KEYCODE4;
    uint8_t KEYCODE5;
    uint8_t KEYCODE6;
} keyboardHID;






#endif // DEF_STRUCT_H
