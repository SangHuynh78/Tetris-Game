
/*
 * matrixLed.c
 *
 *  Created on: 6 thg 4, 2023
 *      Author: ADMIN
 */
#include "matrixLed.h"

int GPIO[8] = { GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7};

void scanRowYellow(uint16_t dataRow)
{
    outByteYellow(dataRow>>8);
    outByteYellow(dataRow);
    GPIOPinWrite(PORT_ROW, RCK_YEL, 0);
    GPIOPinWrite(PORT_ROW, RCK_YEL, RCK_YEL);
}

void outByteYellow(uint8_t byte)
{
    int8_t i;
    for (i = 7; i>= 0; i--)
    {
        int8_t bitSER_YEL = byte&(1<<i);
        if (bitSER_YEL) GPIOPinWrite(PORT_ROW, SER_YEL, SER_YEL);
        else GPIOPinWrite(PORT_ROW, SER_YEL, 0);
        GPIOPinWrite(PORT_ROW, SCK_YEL, 0);
        GPIOPinWrite(PORT_ROW, SCK_YEL, SCK_YEL);
    }
}

void scanRowRed(uint16_t dataRow)
{
    outByteRed(dataRow>>8);
    outByteRed(dataRow);
    GPIOPinWrite(PORT_ROW, RCK_RED, 0);
    GPIOPinWrite(PORT_ROW, RCK_RED, RCK_RED);
}

void outByteRed(uint8_t byte)
{
    int8_t i;
    for (i = 7; i>= 0; i--)
    {
        int8_t bitSER_RED = byte&(1<<i);
        if (bitSER_RED) GPIOPinWrite(PORT_ROW, SER_RED, SER_RED);
        else GPIOPinWrite(PORT_ROW, SER_RED, 0);
        GPIOPinWrite(PORT_ROW, SCK_RED, 0);
        GPIOPinWrite(PORT_ROW, SCK_RED, SCK_RED);
    }
}

void outColumn(uint8_t data)
{
    int8_t j;
    for (j = 7; j>=0; j--)
    {
        int8_t bit = data&(1<<j);
        if (bit) GPIOPinWrite(GPIO_PORTB_BASE, GPIO[j], GPIO[j]);
        else GPIOPinWrite(GPIO_PORTB_BASE, GPIO[j], 0);
    }
}

