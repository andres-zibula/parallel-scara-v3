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

#define M1_POS_X .0
#define M1_POS_Y .0

#define M2_POS_X 100.0
#define M2_POS_Y .0

#define ARM_LEN_1 160.0
#define ARM_LEN_2 200.0
#define ARM_LEN_3 80.0

#define LIFTED_ANGLE 60
#define BASE_ANGLE 100
#define LIFT_DELAY 500

#define STEPS_PER_MM 4.0
#define CIRCLE_PRECISION 0.001
#define MS_PER_DEG 3.0 //1.7

#define STEP_ANGLE 1.8
#define REDUCTION_RATIO_DIV 4.0
#define MICRO_STEPPING_DIV 32.0

#define STEP_DELAY 1
#define STEP_DELAY_MS 300
#define STEP_TOGGLE_DELAY_MS 4
#define DIR_TOGGLE_DELAY_MS 2

extern bool lifted;
extern double actual_x;
extern double actual_y;
extern double m1_actual_angle;
extern double m2_actual_angle;
double step_increment;

void lift();

void put_down();

void draw_line(double x1, double y1, double x2, double y2, bool without_lifting);

void draw_circle(double x, double y, double radius);

void go_to(double x, double y);

#endif /* INC_SCARA_H_ */
