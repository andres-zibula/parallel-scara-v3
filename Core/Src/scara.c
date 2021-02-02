/*
 * scara.c
 *
 *  Created on: Jan 13, 2021
 *      Author: Andres
 */

#include "scara.h"

bool lifted = true;
double actual_x = .0;
double actual_y = .0;
double m1_actual_angle = 90.0;
double m2_actual_angle = 90.0;
double step_increment = STEP_ANGLE/(REDUCTION_RATIO_DIV*MICRO_STEPPING_DIV);

bool equal_angles(double a, double b)
{
  return fabs(a - b) <= step_increment/2.0;
}

void lift()
{
  lifted = true;
  servo_write(htim, LIFTED_ANGLE);
  HAL_Delay(LIFT_DELAY);
}

void put_down()
{
  lifted = false;
  servo_write(htim, BASE_ANGLE);
  HAL_Delay(LIFT_DELAY);
}

void draw_line(double x1, double y1, double x2, double y2, bool without_lifting)
{
  go_to(x1, y1);

  if(lifted)
    put_down();
  
  double dx = x2 - x1;
  double dy = y2 - y1;  
  uint16_t c = round(STEPS_PER_MM * sqrt(dx*dx + dy*dy));
  
  for(int i = 0; i <= c; i++)
  {
    go_to(x1 + i*dx/c, y1 + i*dy/c);
  }

  if(!without_lifting)
    lift();
}

void draw_circle(double x, double y, double radius)
{
  go_to(x+cos(0.0)*radius, y+sin(0.0)*radius);
  
  if(lifted)
    put_down();
  
  for(double r = CIRCLE_PRECISION; r <= M_PI*2.0; r += CIRCLE_PRECISION)
  {
    //draw_line(actual_x, actual_y, x+cosf(r)*radius, y+sinf(r)*radius, true);
    go_to(x+cos(r)*radius, y+sin(r)*radius);
  }
  
  lift();
}

void go_to(double x, double y)
{
  if (actual_x == x && actual_y == y)
    return;

  double L4 = cosine_side_rule(M_PI - M_PI/4.0, ARM_LEN_2, ARM_LEN_3);
  double epsilon = cosine_angle_rule(L4, ARM_LEN_2, ARM_LEN_3);
  double d3 = pitagoras(M2_POS_X - x, y - M2_POS_Y);
  double theta2 = atan2(y, (M2_POS_X - x)) + cosine_angle_rule(d3, L4, ARM_LEN_1);
  //double debatan1 = atan2(y, (M2_POS_X - x));
  //double debcosin1 = cosine_angle_rule(d3, L4, ARM_LEN_1);
  
  double x4 = M2_POS_X + ARM_LEN_1*cos(M_PI - theta2);
  double y4 = M2_POS_Y + ARM_LEN_1*sin(M_PI - theta2);
  
  double delta = atan2((x4-x), (y-y4));
  
  double x1 = x + ARM_LEN_3*sin(delta-epsilon);
  double y1 = y - ARM_LEN_3*cos(delta-epsilon);
  
  double d1 = pitagoras(x1 - M1_POS_X, y1 - M1_POS_Y);
  double theta1 = atan2((y1 - M1_POS_Y), (x1 - M1_POS_X)) + cosine_angle_rule(d1, ARM_LEN_2, ARM_LEN_1);
  //double debatan2 = atan2((y1 - M1_POS_Y), (x1 - M1_POS_X));
  //double debcosin2 = cosine_angle_rule(d1, ARM_LEN_2, ARM_LEN_1);

  double m1_angle = rad_to_deg(theta1);
  double m2_angle = rad_to_deg(M_PI - theta2);
   
  while (!equal_angles(m1_angle, m1_actual_angle) || !equal_angles(m2_angle, m2_actual_angle))
  {
    if (!equal_angles(m1_angle, m1_actual_angle))
    {
      if (m1_angle > m1_actual_angle)
      {
        HAL_GPIO_WritePin(GPIOB, M1_DIR_Pin, GPIO_PIN_SET);
        m1_actual_angle += step_increment;
      }
      else
      {
        HAL_GPIO_WritePin(GPIOB, M1_DIR_Pin, GPIO_PIN_RESET);
        m1_actual_angle -= step_increment;
      }

      delay_us(DIR_TOGGLE_DELAY_MS);

      HAL_GPIO_WritePin(GPIOB, M1_STEP_Pin, GPIO_PIN_SET);
      delay_us(STEP_TOGGLE_DELAY_MS);
      HAL_GPIO_WritePin(GPIOB, M1_STEP_Pin, GPIO_PIN_RESET);
    }
  
    if (!equal_angles(m2_angle, m2_actual_angle))
    {
      if (m2_angle > m2_actual_angle)
      {
        HAL_GPIO_WritePin(GPIOB, M2_DIR_Pin, GPIO_PIN_SET);
        m2_actual_angle += step_increment;
      }
      else
      {
        HAL_GPIO_WritePin(GPIOB, M2_DIR_Pin, GPIO_PIN_RESET);
        m2_actual_angle -= step_increment;
      }

      delay_us(DIR_TOGGLE_DELAY_MS);

      HAL_GPIO_WritePin(GPIOB, M2_STEP_Pin, GPIO_PIN_SET);
      delay_us(STEP_TOGGLE_DELAY_MS);
      HAL_GPIO_WritePin(GPIOB, M2_STEP_Pin, GPIO_PIN_RESET);
    }

    //HAL_Delay(STEP_DELAY);
    delay_us(STEP_DELAY_MS);
  }
  
}