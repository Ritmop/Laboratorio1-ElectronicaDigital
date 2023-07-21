/* 
 * File:   Lab1.c
 * Device: PIC16F887
 * Author: Judah Pérez - 21536
 *Compiler: XC8 (v2.41)
 * 
 * Program: Contador en PORTA con botones e interrupciones
 * Hardware: 8 Leds Puerto A, 2 display 7 seg multiplexados Puerto D
 *           4 botones Puerto B (0-3), Potenciometro AN13 y led RE2.
 * 
 * Created: July 18, 2023
 * Last updated:
 */

/*--------------------------------- LIBRARIES --------------------------------*/
#include <xc.h>
#include <PIC16F887.h>
#include "iocb_init.h"
#include "ADC_lib.h"
#include "disp_7seg.h"

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
#define _TMR0n 50   //TMR0 load value
uint8_t adc_val;    //ADC read value
uint16_t nibbles;   //High & Low nibbles in a single reg
uint8_t high_nib;   //High nibble
uint8_t low_nib;    //Low nibble
uint8_t disp1;      //7 seg display 1 value
uint8_t disp2;      //7 seg display 2 value
uint8_t dispSel;    //Display select
        
        
/*-------------------------------- PROTOTYPES --------------------------------*/
void ioc_portB(void);
void setup(void);
void multiplex(uint8_t selector);
void alarm(uint8_t ref,uint8_t var);
/*------------------------------- RESET VECTOR -------------------------------*/

/*----------------------------- INTERRUPT VECTOR -----------------------------*/
void __interrupt() isr(void){
    if(T0IF){
        dispSel++;  //Change selected display
        //Reset timer0
        TMR0 = _TMR0n;
        T0IF = 0;
    }
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
        //Display pot value
        adc_val = adc_read()>>8;            //Read pot value
        nibbles = split_nibbles(adc_val);   //Separate pot value into nibbles
        low_nib = nibbles;                  //Capture low nibble
        high_nib = nibbles>>8;              //Capture high nibble
        disp2 = hex_to_7seg(low_nib);       //Convert low nibble into disp2 val
        disp1 = hex_to_7seg(high_nib);      //Convert high nibble into disp1 val  
        multiplex(dispSel);                 //Switch selected display & output
        //Check alarm
        alarm(PORTA,adc_val); //Check if pot val > PORTA counter
    }
}
/*-------------------------------- SUBROUTINES -------------------------------*/
void setup(void){
   //I/O CONFIG
    ANSEL = 0;  //PORTA as Digital
    ANSELH= 0b00101000; //AN11 & AN13 enabled
    TRISA = 0;  //Counter Output
    PORTA = 0x7F;  //Start counter at 0x7F
    TRISC = 0;  //ADC Output
    PORTC = 0;  //Clear PORTC
    TRISD = 0;  //Selected channel Output
    PORTD = 0;  //Clear PORTD 
    TRISE = 0;  //Selected channel Output
    PORTE = 0;  //Clear PORTD 
    
    //OSCILLATOR CONFIG
    OSCCONbits.IRCF = 0b111;  //Internal clock frequency 8MHz
    SCS = 1;
    
    iocb_init(0x0F); //Initialize IOCB
    adc_init(0, 0, 8, 0b1101); //Initialize ADC. Left, Vdd/Vss, 8MHz, AN13.
    
    //TMR0 config
    GIE = 1;    //Enable global interrupts
    T0IE = 1;   //Enable TMR0 interrupt
    OPTION_REGbits.PS = 0b000;  //Prescaler = 2
    T0CS = 0;   //Use internal cycle clock
    TMR0 = _TMR0n;
    T0IF = 0;   //Clear flag
}

void multiplex(uint8_t selector){
    if(selector & 0x01 == 1){        //If disp 1 is enabled
            RE0 = 0;    //Disable disp 1
            PORTD = disp2;
            RE1 = 1;    //Enable disp 2            
        }
        else{           //If disp 1 is disabled
            RE1 = 0;    //Disable disp 2
            PORTD = disp1;
            RE0 = 1;    //Enable disp 1           
        }        
}

void alarm(uint8_t ref,uint8_t var){
    if(var >= ref) RE2 = 1;
    else RE2 = 0;
}