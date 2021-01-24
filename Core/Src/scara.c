/*
 * scara.c
 *
 *  Created on: Jan 13, 2021
 *      Author: Andres
 */

#include "scara.h"

bool lifted = false;
float actual_x = .0f;
float actual_y = .0f;
float m1_actual_angle = 90.0f;
float m2_actual_angle = 90.0f;
float step_increment = STEP_ANGLE/(REDUCTION_RATIO_DIV*MICRO_STEPPING_DIV);

bool equal_angles(float a, float b)
{
  return fabsf(a - b)/2.0f < step_increment;
}

void lift()
{
    servo_write(htim, LIFTED_ANGLE);
}

void put_down()
{
    servo_write(htim, BASE_ANGLE);
}

void draw_line(float x1, float y1, float x2, float y2, bool without_lifting)
{
  go_to(x1, y1);

  if(lifted)
    put_down();
  
  float dx = x2 - x1;
  float dy = y2 - y1;  
  float c = roundf(STEPS_PER_MM * sqrtf(dx*dx + dy*dy));
  
  for(int i = 0; i <= c; i++)
  {
    go_to(x1 + i*dx/c, y1 + i*dy/c);
  }

  if(!without_lifting)
    lift();
}

void draw_circle(float x, float y, float radius)
{
  go_to(x+cosf(0.0f)*radius, y+sinf(0.0f)*radius);
  
  if(lifted)
    put_down();
  
  for(float r = CIRCLE_PRECISION; r <= M_PI*2.0f; r += CIRCLE_PRECISION)
  {
    //draw_line(actual_x, actual_y, x+cosf(r)*radius, y+sinf(r)*radius, true);
    go_to(x+cosf(r)*radius, y+sinf(r)*radius);
  }
  
  lift();
}

void go_to(float x, float y)
{
  if (actual_x == x && actual_y == y)
    return;

  float L4 = cosine_side_rule(M_PI - M_PI/4.0f, ARM_LEN_2, ARM_LEN_3);
  float epsilon = cosine_angle_rule(L4, ARM_LEN_2, ARM_LEN_3);
  float d3 = pitagoras(M2_POS_X - x, y - M2_POS_Y);
  float theta2 = atan2f(y, (M2_POS_X - x)) + cosine_angle_rule(d3, L4, ARM_LEN_1);
  float debatan1 = atan2f(y, (M2_POS_X - x));
  float debcosin1 = cosine_angle_rule(d3, L4, ARM_LEN_1);
  
  float x4 = M2_POS_X + ARM_LEN_1*cosf(M_PI - theta2);
  float y4 = M2_POS_Y + ARM_LEN_1*sinf(M_PI - theta2);
  
  float delta = atan2f((x4-x), (y-y4));
  
  float x1 = x + ARM_LEN_3*sinf(delta-epsilon);
  float y1 = y - ARM_LEN_3*cosf(delta-epsilon);
  
  float d1 = pitagoras(x1 - M1_POS_X, y1 - M1_POS_Y);
  float theta1 = atan2f((y1 - M1_POS_Y), (x1 - M1_POS_X)) + cosine_angle_rule(d1, ARM_LEN_2, ARM_LEN_1);
  float debatan2 = atan2f((y1 - M1_POS_Y), (x1 - M1_POS_X));
  float debcosin2 = cosine_angle_rule(d1, ARM_LEN_2, ARM_LEN_1);

  float m1_angle = rad_to_deg(theta1);
  float m2_angle = rad_to_deg(M_PI - theta2);
   
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

      delay_us(1);

      HAL_GPIO_WritePin(GPIOB, M1_STEP_Pin, GPIO_PIN_SET);
      delay_us(2);
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

      delay_us(1);

      HAL_GPIO_WritePin(GPIOB, M2_STEP_Pin, GPIO_PIN_SET);
      delay_us(2);
      HAL_GPIO_WritePin(GPIOB, M2_STEP_Pin, GPIO_PIN_RESET);
    }

    //HAL_Delay(STEP_DELAY);
    delay_us(STEP_DELAY_MS);
  }
  
}