/*
 * scara.h
 *
 *  Created on: Jan 13, 2021
 *      Author: Andres
 */

#ifndef INC_SCARA_H_
#define INC_SCARA_H_

#include "utils.h"
#include "stdbool.h"

#define M1_POS_X .0f
#define M1_POS_Y .0f

#define M2_POS_X 100.0f
#define M2_POS_Y .0f

#define ARM_LEN_1 160.0f
#define ARM_LEN_2 200.0f
#define ARM_LEN_3 80.0f

#define LIFTED_ANGLE 90
#define BASE_ANGLE 40

#define STEPS_PER_MM 1.0f
#define CIRCLE_PRECISION 0.1f
#define MS_PER_DEG 3.0f //1.7

#define STEP_ANGLE 1.8f
#define REDUCTION_RATIO_DIV 4.0f
#define MICRO_STEPPING_DIV 32.0f

#define STEP_DELAY 1
#define STEP_DELAY_MS 500

extern bool lifted;
extern float actual_x;
extern float actual_y;
extern float m1_actual_angle;
extern float m2_actual_angle;
float step_increment;

void lift();

void put_down();

void draw_line(float x1, float y1, float x2, float y2, bool without_lifting);

void draw_circle(float x, float y, float radius);

void go_to(float x, float y);

#endif /* INC_SCARA_H_ */
