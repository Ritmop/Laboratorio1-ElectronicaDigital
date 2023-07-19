/* 
 * File:   disp_7seg.h
 * Author: judah
 *
 * Created on July 19, 2023, 9:00 AM
 */

#ifndef DISP_7SEG_H
#define	DISP_7SEG_H

#include <xc.h>

uint8_t hex_to_7seg(uint8_t hex);
uint16_t split_nibbles(uint8_t hex);

#endif	/* DISP_7SEG_H */

