#include "iocb_init.h"

//Función para inicializar IOCB
void iocb_init(uint8_t pinesB){
    nRBPU = 0;  //Enable PORTB pull-up's 
    TRISB |= pinesB;  //RB0 & RB1 as Input    
    WPUB  |= pinesB;  //on RB0 & RB1
}
