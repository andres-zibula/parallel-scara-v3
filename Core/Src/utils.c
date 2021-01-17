/*
 * utils.c
 *
 *  Created on: Jan 13, 2021
 *      Author: Andres
 */

#include "utils.h"

uint16_t map(uint16_t st1, uint16_t fn1, uint16_t st2, uint16_t fn2, uint16_t value)
{
    return (1.0f*(value-st1))/((fn1-st1)*1.0f) * (fn2-st2)+st2;
}

//3000 ticks 1ms
void servo_write(TIM_HandleTypeDef *htim, uint8_t angle)
{
	htim->Instance->CCR1 = map(0, 180, 1700, 7000, angle);
}

float cosine_angle_rule(float a, float b, float c)
{
  // cosine rule for angle between c and a
  return acosf((a * a + c * c - b * b) / (2.0f * a * c));
}

float cosine_side_rule(float A, float b, float c)
{
  return sqrtf(b*b+c*c-2*b*c*cosf(A));
}

float rad_to_deg(float rad)
{
  return rad * (180.0f / M_PI);
}

float pitagoras(float b, float c)
{
  return sqrtf(b*b + c*c);
}

