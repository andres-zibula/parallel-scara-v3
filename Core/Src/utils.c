/*
 * utils.c
 *
 *  Created on: Jan 13, 2021
 *      Author: Andres
 */

#include "utils.h"

void delay_us (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(htim_ms, 0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(htim_ms) < us);  // wait for the counter to reach the us input in the parameter
}

uint16_t map(uint16_t st1, uint16_t fn1, uint16_t st2, uint16_t fn2, uint16_t value)
{
    return (1.0f*(value-st1))/((fn1-st1)*1.0f) * (fn2-st2)+st2;
}

//3000 ticks 1ms
void servo_write(TIM_HandleTypeDef *htim, uint8_t angle)
{
	htim->Instance->CCR1 = map(0, 180, 1700, 7000, angle);
}

double cosine_angle_rule(double a, double b, double c)
{
  // cosine rule for angle between c and a
  double x = (a * a + c * c - b * b) / (2.0 * a * c);
  if (x > 1.0) {
    x = 1.0;
  }
  else if (x < -1.0) {
    x = -1.0;
  }
  return acos(x);
}

double cosine_side_rule(double A, double b, double c)
{
  return sqrt(b*b+c*c-2*b*c*cos(A));
}

double rad_to_deg(double rad)
{
  return rad * (180.0 / M_PI);
}

double pitagoras(double b, double c)
{
  return sqrt(b*b + c*c);
}

