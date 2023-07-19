/* 
 * File:   Lab1.c
 * Device: PIC16F887
 * Author: Judah Pérez - 21536
 *Compiler: XC8 (v2.41)
 * 
 * Program: Contador en PORTA con botones e interrupciones
 * Hardware:
 * 
 * Created: July 18, 2023
 * Last updated:
 */

/*--------------------------------- LIBRARIES --------------------------------*/
#include <xc.h>
#include <PIC16F887.h>
#include "iocb_init.h"
#include "ADC_lib.h"

/*---------------------------- CONFIGURATION BITS ----------------------------*/
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

/*----------------------- GLOBAL VARIABLES & CONSTANTS -----------------------*/
#define _XTAL_FREQ 8000000  //Constante para delay
        
/*-------------------------------- PROTOTYPES --------------------------------*/
void ioc_portB(void);
void setup(void);
/*------------------------------- RESET VECTOR -------------------------------*/

/*----------------------------- INTERRUPT VECTOR -----------------------------*/
void __interrupt() isr(void){
    if(RBIF){
        ioc_portB();
        RBIF = 0;
    }
}

/*--------------------------- INTERRUPT SUBROUTINES --------------------------*/
void ioc_portB(void){
    if(!RB0) PORTA++;
    if(!RB1) PORTA--;
    if(!RB2) adc_sel_channel(13);
    if(!RB3) adc_sel_channel(11);
}
/*---------------------------------- TABLES ----------------------------------*/

/*----------------------------------- SETUP ----------------------------------*/

/*----------------------------------- MAIN -----------------------------------*/
int main(void) {
    setup();
    
    while(1){
        //Loop
        PORTC = adc_read()>>8; //PORTC = ADRESH
        __delay_us(24);
        PORTD = adc_get_channel();
    }
}
/*-------------------------------- SUBROUTINES -------------------------------*/
void setup(void){
   //I/O CONFIG
    ANSEL = 0;  //PORTA as Digital
    ANSELH= 0b00101000; //AN11 & AN13 enabled
    TRISA = 0;  //Counter Output
    PORTA = 0;  //Clear PORTA
    TRISC = 0;  //ADC Output
    PORTC = 0;  //Clear PORTC
    TRISD = 0;  //Selected channel Output
    PORTD = 0;  //Clear PORTD        
    
    //OSCILLATOR CONFIG
    OSCCONbits.IRCF = 0b111;  //Internal clock frequency 8MHz
    SCS = 1;
    
    iocb_init(0x0F); //Initialize IOCB
    adc_init(0, 0, 8, 0b1101); //Initialize ADC. Left, Vdd/Vss, 8MHz, AN13.
}