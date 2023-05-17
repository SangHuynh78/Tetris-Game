#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "matrixLed.h"
#include "random.h"

typedef enum {PLAYING=0, GAMEOVER}  gameState_t;
gameState_t status = PLAYING;
int lowest_row = 14;
int dichTrai = 2;
int MD[18] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
int OVER[69] = {0X1E, 0X22, 0X22, 0X26, 0X20, 0X20, 0X1E, 0X00, 0X22, 0X22, 0X22, 0X3E, 0X22, 0X22, 0X1C, 0X00,
                0X22, 0X22, 0X22, 0X2A, 0X2A, 0X36, 0X22, 0X00, 0X3E, 0X20, 0X20, 0X3C, 0X20, 0X20, 0X3E, 0X00, 0X00, 0X00,
                0X1C, 0X22, 0X22, 0X22, 0X22, 0X22, 0X1C, 0X00, 0X08, 0X14, 0X22, 0X22, 0X22, 0X22, 0X22, 0X00,
                0X3E, 0X20, 0X20, 0X3C, 0X20, 0X20, 0X3E, 0X00, 0X22, 0X24, 0X28, 0X3C, 0X22, 0X22, 0X3C, 0X00, 0xFF, 0x00, 0x00};
int TURN[3] ={7, 7, 7};

int K_chu_O[3] = {6, 6, 0};
int K_chu_O_right[3] = {3, 3, 0};

int K_chu_Z_0[3] = {3, 6, 0};
int K_chu_Z_1[3] = {4, 6, 2};
int K_chu_Z_1_right[3] = {2, 3, 1};

int K_chu_S_0[3] = {6, 3, 0};
int K_chu_S_1[3] = {2, 6, 4};
int K_chu_S_1_right[3] = {1, 3, 2};

int K_chu_I_0[3] = {7, 0, 0};
int K_chu_I_1[3] = {2, 2, 2};
int K_chu_I_1_left[3] = {4, 4, 4};
int K_chu_I_1_right[3] = {1, 1, 1};

int K_chu_J_0[3] = {7, 4, 0};
int K_chu_J_1[3] = {4, 4, 6};
int K_chu_J_1_right[3] = {2, 2, 3};
int K_chu_J_2[3] = {1, 7, 0};
int K_chu_J_3[3] = {3, 1, 1};
int K_chu_J_3_left[3] = {6, 2, 2};

int K_chu_L_0[3] = {7, 1, 0};
int K_chu_L_1[3] = {6, 4, 4};
int K_chu_L_1_right[3] = {3, 2, 2};
int K_chu_L_2[3] = {4, 7, 0};
int K_chu_L_3[3] = {1, 1, 3};
int K_chu_L_3_left[3] = {2, 2, 6};

int K_chu_T_0[3] = {7, 2, 0};
int K_chu_T_1[3] = {4, 6, 4};
int K_chu_T_1_right[3] = {2, 3, 2};
int K_chu_T_2[3] = {2, 7,0 };
int K_chu_T_3[3] = {1, 3, 1};
int K_chu_T_3_left[3] = {2, 6, 2};

int *current;

void SysTickIntHandler(void)
{
    if (status == PLAYING)
    {
        if (!((*current)<<dichTrai & MD[lowest_row-1]) && !((*(current+1))<<dichTrai & MD[lowest_row])
                 && !((*(current+2))<<dichTrai & MD[lowest_row+1]) && lowest_row )
            lowest_row--;
        else
        {
            SysTickPeriodSet(SysCtlClockGet()); //set up systick 1s
            int j;
            for (j = 0; j <3; j++)
            {
                MD[lowest_row+j] = MD[lowest_row+j] | *(current + j)<<dichTrai;
            }
            lowest_row = 14;
            dichTrai = 2;
            int r = Getrandom(1, 7);
            switch (r) {
            case 1:
                current = &K_chu_O[0];
                break;
            case 2:
                current = &K_chu_Z_0[0];
                break;
            case 3:
                current = &K_chu_S_0[0];
                break;
            case 4:
                current = &K_chu_I_0[0];
                break;
            case 5:
                current = &K_chu_J_0[0];
                break;
            case 6:
                current = &K_chu_L_0[0];
                break;
            case 7:
                current = &K_chu_T_0[0];
                break;
            default:
                break;
            }
        }
    }
    else
    {
        uint8_t i;
        uint16_t temp = OVER[0];
        for (i=0; i<68; i++)
        {
            OVER[i] = OVER[i+1];
        }
        OVER[68] = temp;
    }
}

void GPIO_ISR (void)
{
    static uint16_t int_status;
    int_status = GPIOIntStatus(GPIO_PORTE_BASE, GPIO_INT_PIN_5 | GPIO_INT_PIN_2 | GPIO_INT_PIN_3 | GPIO_INT_PIN_4);
    GPIOIntClear(GPIO_PORTE_BASE, int_status);
    //increat speed systick interrupt
    if ((int_status & GPIO_INT_PIN_4)==GPIO_INT_PIN_4)
    {
        SysTickPeriodSet(SysCtlClockGet()/100); //set up systick 10ms
    }
    //dich trai
    if ((int_status & GPIO_INT_PIN_3)==GPIO_INT_PIN_3 && !((*current)<<(dichTrai+1) & MD[lowest_row])
         && !((*(current+1))<<(dichTrai+1) & MD[lowest_row+1]) && !((*(current+2))<<(dichTrai+1) & MD[lowest_row+2]))
    {
        if (dichTrai < 5) dichTrai ++;
        //nhung trung hop dich trai dac biet
        //KHOI CHU O
        else if (current == &K_chu_O_right[0])      current = &K_chu_O[0];
        //KHOI CHU Z
        else if (current == &K_chu_Z_1_right[0])    current = &K_chu_Z_1[0];
        //KHOI CHU S
        else if (current == &K_chu_S_1_right[0])    current = &K_chu_S_1[0];
        //KHOI CHU I
        else if (current == &K_chu_I_1[0])          current = &K_chu_I_1_left[0];
        else if (current == &K_chu_I_1_right[0])    current = &K_chu_I_1[0];
        //KHOI CHU J
        else if (current == &K_chu_J_3[0])          current = &K_chu_J_3_left[0];
        else if (current == &K_chu_J_1_right[0])    current = &K_chu_J_1[0];
        //KHOI CHU L
        else if (current == &K_chu_L_3[0])          current = &K_chu_L_3_left[0];
        else if (current == &K_chu_L_1_right[0])    current = &K_chu_L_1[0];
        //KHOI CHU T
        else if (current == &K_chu_T_3[0])          current = &K_chu_T_3_left[0];
        else if (current == &K_chu_T_1_right[0])    current = &K_chu_T_1[0];
    }

    //dich phai
    if ((int_status & GPIO_INT_PIN_5)==GPIO_INT_PIN_5 && !((*current)<<(dichTrai-1) & MD[lowest_row])
        && !((*(current+1))<<(dichTrai-1) & MD[lowest_row+1]) && !((*(current+2))<<(dichTrai-1) & MD[lowest_row+2]))
    {
        if (dichTrai > 0) dichTrai--;
        //nhung trung hop dich phai dac biet
        //KHOI CHU O
        else if (current == &K_chu_O[0])        current = &K_chu_O_right[0];
        //KHOI CHU Z
        else if (current == &K_chu_Z_1[0])      current = &K_chu_Z_1_right[0];
        //KHOI CHU S
        else if (current == &K_chu_S_1[0])      current = &K_chu_S_1_right[0];
        //KHOI CHU I
        else if (current == &K_chu_I_1[0])      current = &K_chu_I_1_right[0];
        else if (current == &K_chu_I_1_left[0]) current = &K_chu_I_1[0];
        //KHOI CHU J
        else if (current == &K_chu_J_1[0])      current = &K_chu_J_1_right[0];
        else if (current == &K_chu_J_3_left[0]) current = &K_chu_J_3[0];
        //KHOI CHU L
        else if (current == &K_chu_L_1[0])      current = &K_chu_L_1_right[0];
        else if (current == &K_chu_L_3_left[0]) current = &K_chu_L_3[0];
        //KHOI CHU T
        else if (current == &K_chu_T_1[0])      current = &K_chu_T_1_right[0];
        else if (current == &K_chu_T_3_left[0]) current = &K_chu_T_3[0];
    }

    //quay 90*
    if ((int_status & GPIO_INT_PIN_2)==GPIO_INT_PIN_2 && !(TURN[0]<<dichTrai & MD[lowest_row])
        && !(TURN[1]<<dichTrai & MD[lowest_row+1]) && !(TURN[2]<<dichTrai & MD[lowest_row+2]))
    {
        //quay KHOI CHU Z
        if (current == &K_chu_Z_0[0])                                   {current = &K_chu_Z_1[0]; return;}
        if (current == &K_chu_Z_1[0] || current == &K_chu_Z_1_right[0]) {current = &K_chu_Z_0[0]; return;}
        //quay KHOI CHU S
        if (current == &K_chu_S_0[0])                                   {current = &K_chu_S_1[0]; return;}
        if (current == &K_chu_S_1[0] || current == &K_chu_S_1_right[0]) {current = &K_chu_S_0[0]; return;}
        //quay KHOI CHU I
        if (current == &K_chu_I_0[0])                                   {current = &K_chu_I_1[0]; return;}
        if (current == &K_chu_I_1[0] || current == &K_chu_I_1_right[0]
                                     || current == &K_chu_I_1_right[0]) {current = &K_chu_I_0[0]; return;}
        //quay KHOI CHU J
        if (current == &K_chu_J_0[0])                                   {current = &K_chu_J_1[0]; return;}
        if (current == &K_chu_J_1[0] || current == &K_chu_J_1_right[0]) {current = &K_chu_J_2[0]; return;}
        if (current == &K_chu_J_2[0])                                   {current = &K_chu_J_3[0]; return;}
        if (current == &K_chu_J_3[0] || current == &K_chu_J_3_left[0])  {current = &K_chu_J_0[0]; return;}
        //quay KHOI CHU L
        if (current == &K_chu_L_0[0])                                   {current = &K_chu_L_1[0]; return;}
        if (current == &K_chu_L_1[0] || current == &K_chu_L_1_right[0]) {current = &K_chu_L_2[0]; return;}
        if (current == &K_chu_L_2[0])                                   {current = &K_chu_L_3[0]; return;}
        if (current == &K_chu_L_3[0] || current == &K_chu_L_3_left[0])  {current = &K_chu_L_0[0]; return;}
        //quay KHOI CHU L
        if (current == &K_chu_T_0[0])                                   {current = &K_chu_T_1[0]; return;}
        if (current == &K_chu_T_1[0] || current == &K_chu_T_1_right[0]) {current = &K_chu_T_2[0]; return;}
        if (current == &K_chu_T_2[0])                                   {current = &K_chu_T_3[0]; return;}
        if (current == &K_chu_T_3[0] || current == &K_chu_T_3_left[0])  {current = &K_chu_T_0[0]; return;}
    }
}

int main(void)
{
//    uint16_t score = 0;
    current = &K_chu_T_0[0];
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_18MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, SCK_YEL | RCK_YEL | SER_YEL | SCK_RED | RCK_RED | SER_RED);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    GPIOPinWrite(GPIO_PORTA_BASE, SCK_YEL | SCK_RED , 0);
    //systick interrupt
    SysTickPeriodSet(SysCtlClockGet()); //set up systick 1s
    SysTickIntEnable();
    SysTickEnable();
    SysTickIntRegister(&SysTickIntHandler);
    //GPIO interrupt
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);
    GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_5 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntRegister(GPIO_PORTE_BASE, &GPIO_ISR);
    GPIOIntEnable(GPIO_PORTE_BASE, GPIO_INT_PIN_5 | GPIO_INT_PIN_2 | GPIO_INT_PIN_3 | GPIO_INT_PIN_4);
    IntEnable(INT_GPIOE);
    IntMasterEnable();

    while(1)
    {
        uint8_t k;
        for (k = 0; k <30; k++)
        {
            uint8_t row;
            uint16_t dataRow;
            for (row = lowest_row; row < lowest_row + 18; row ++)
            {
                dataRow = 1<<row;
                scanRowYellow(dataRow);
                if (row < lowest_row + 3) outColumn(*(current + row - lowest_row) << dichTrai);
                else outColumn(0);
                SysCtlDelay(SysCtlClockGet()/16000);
            }
            for (row = 0; row < 18; row ++)
            {
                dataRow = 1<<row;
                scanRowRed(dataRow);
                outColumn(MD[row]);
                SysCtlDelay(SysCtlClockGet()/16000);
            }
            for (row = 0; row < 18; row ++)
            {
                if (MD[row] == 0xFF)
                {
                    int i;
                    for(i= row; i < 18; i++)
                    {
                        MD[i] = MD[i+1];
                    }
                    MD[17] = 0x00;
//                    score ++;
                }
            }
            if (MD[15]) gameOver(OVER);
        }
    }
}

void gameOver(int* over)
{
    status = GAMEOVER;
    SysTickPeriodSet(SysCtlClockGet()/10);
    while (1)
    {
        uint8_t row;
        uint16_t dataRow;
        for (row = 0; row < 18; row ++)
        {
            dataRow = 1<<row;
            scanRowRed(dataRow);
            outColumn(OVER[row]);
            SysCtlDelay(SysCtlClockGet()/16000);
        }
    }
}

