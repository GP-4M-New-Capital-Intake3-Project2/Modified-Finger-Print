/*
 * Finger-print.h
 *
 *  Created on: Oct 22, 2023
 *      Author: sabry
 */

#ifndef HAL_FINGER_PRINT_FINGER_PRINT_H_
#define HAL_FINGER_PRINT_FINGER_PRINT_H_
#include "../../LIB/STD_TYPES.h"

u8 read_finger_1(void) ;
u8 read_finger_2(void);
u8 check_finger(void);
void make_template(void);
void store(unsigned char charBuf ,unsigned int ID);



#endif /* HAL_FINGER_PRINT_FINGER_PRINT_H_ */
