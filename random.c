/*
 * random.c
 *
 *  Created on: 7 thg 4, 2023
 *      Author: ADMIN
 */

#include "random.h"

int Getrandom(int min, int max)
{
    return min + (int)rand()*(max - min + 1.0)/(1.0 + RAND_MAX);
}
