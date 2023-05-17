/*
 * matrixLed.h
 *
 *  Created on: 6 thg 4, 2023
 *      Author: ADMIN
 */

#ifndef MATRIXLED_H_
#define MATRIXLED_H_

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"

#define PORT_ROW    GPIO_PORTA_BASE
#define SCK_YEL     GPIO_PIN_4
#define RCK_YEL     GPIO_PIN_3
#define SER_YEL     GPIO_PIN_2
#define SCK_RED     GPIO_PIN_5
#define RCK_RED     GPIO_PIN_6
#define SER_RED     GPIO_PIN_7

void scanRowYellow(uint16_t);
void outByteYellow(uint8_t);
void scanRowRed(uint16_t);
void outByteRed(uint8_t);
void outColumn(uint8_t);

#endif /* MATRIXLED_H_ */
