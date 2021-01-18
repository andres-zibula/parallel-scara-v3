/*
 * scara.h
 *
 *  Created on: Jan 13, 2021
 *      Author: Andres
 */

#ifndef INC_SCARA_H_
#define INC_SCARA_H_

#include "utils.h"

#define M1_POS_X .0f
#define M1_POS_Y .0f

#define M2_POS_X 50.0f
#define M2_POS_Y .0f

#define ARM_LEN_1 75.0f
#define ARM_LEN_2 100.0f
#define ARM_LEN_3 35.0f

#define LIFT_HEIGHT_ANGLE 90
#define BASE_HEIGHT_ANGLE 40

#define STEPS_PER_MM 1.0f
#define CIRCLE_PRECISION 0.10f
#define MS_PER_DEG 3.0f //1.7

#define REDUCTION_RATIO 1.4f
#define MICRO_STEPPING_DIV 32

extern TIM_HandleTypeDef *htim;
extern bool lifted;
extern float actual_x;
extern float actual_y;
extern float m1_angle;
extern float m2_angle;

void lift();

void put_down();

void draw_line(float, float, float, float, bool);

void draw_circle(float, float, float);

void go_to(float, float);

#endif /* INC_SCARA_H_ */
