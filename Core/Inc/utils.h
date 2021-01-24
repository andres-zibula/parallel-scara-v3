/*
 * utils.h
 *
 *  Created on: Jan 13, 2021
 *      Author: Andres
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include "main.h"
#include "math.h"
#include "stdbool.h"

extern TIM_HandleTypeDef *htim;
extern TIM_HandleTypeDef *htim_ms;

void delay_us (uint16_t);

uint16_t map(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);

void servo_write(TIM_HandleTypeDef *, uint8_t);

double cosine_angle_rule(double, double, double);

double cosine_side_rule(double, double, double);

double rad_to_deg(double);

double pitagoras(double, double);




#endif /* INC_UTILS_H_ */
