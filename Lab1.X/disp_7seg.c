#include "disp_7seg.h"
//Valores para display de cátodo común
#define display0 0b00111111
#define display1 0b00000110
#define display2 0b01011011
#define display3 0b01001111
#define display4 0b01100110
#define display5 0b01101101
#define display6 0b01111101
#define display7 0b00000111
#define display8 0b01111111
#define display9 0b01101111
#define displayA 0b01110111
#define displayB 0b01111100
#define displayC 0b00111001
#define displayD 0b01011110
#define displayE 0b01111001
#define displayF 0b01110001

//Función para convertir hexadecimal a display 7 seg
uint8_t hex_to_7seg(uint8_t hex){
    switch(hex){
        case 0x00:
            return display0;
        case 0x01:
            return display1;
        case 0x02:
            return display2;
        case 0x03:
            return display3;
        case 0x04:
            return display4;
        case 0x05:
            return display5;
        case 0x06:
            return display6;
        case 0x07:
            return display7;
        case 0x08:
            return display8;
        case 0x09:
            return display9;
        case 0x0A:
            return displayA;
        case 0x0B:
            return displayB;
        case 0x0C:
            return displayC;
        case 0x0D:
            return displayD;
        case 0x0E:
            return displayE;
        case 0x0F:
            return displayF;        
    }
}

//Función para separar los nibbles de un registro
uint16_t split_nibbles(uint8_t hex){
    uint8_t high_nib;
    uint8_t low_nib;    
    low_nib = hex & 0x0F;   //Get low nibble
    high_nib = hex>>4 & 0x0F;//Get high nibble    
    return high_nib<<8 | low_nib;
}