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

void lift()
{
    servo_write(htim, LIFT_HEIGHT_ANGLE);
}

void lift()
{
    servo_write(htim, BASE_HEIGHT_ANGLE);
}

void draw_line(float x1, float y1, float x2, float y2, bool without_lifting)
{
  go_to(x1, y1);

  if(lifted)
    put_down();
  
  float dx = x2 - x1;
  float dy = y2 - y1;  
  float c = round(STEPS_PER_MM * sqrt(dx*dx + dy*dy));
  
  for(int i = 0; i <= c; i++)
  {
    go_to(x1 + i*dx/c, y1 + i*dy/c);
  }
  

  if(!without_lifting)
    lift();
}

void draw_circle(float x, float y, float radius)
{
  go_to(x+cos(0)*radius, y+sin(0)*radius);
  
  if(lifted)
    put_down();
  
  for(float r = CIRCLE_PRECISION; r <= M_PI*2; r += CIRCLE_PRECISION)
  {
    //draw_line(actual_x, actual_y, x+cos(r)*radius, y+sin(r)*radius, true);
    go_to(x+cos(r)*radius, y+sin(r)*radius);
  }
  
  lift();
}

void go_to(float x, float y)
{
  if (actual_x == x && actual_y == y)
    return;


  float L4 = cosine_side_rule(M_PI - M_PI/4.0, ARM_LEN_2, ARM_LEN_3);
  float epsilon = cosine_angle_rule(L4, ARM_LEN_2, ARM_LEN_3);
  float d3 = pitagoras(S2_POS_X - x, y - S2_POS_Y);
  float theta2 = atan2(y, (S2_POS_X - x)) + cosine_angle_rule(d3, L4, ARM_LEN_1);
  
  float x4 = S2_POS_X + ARM_LEN_1*cos(M_PI - theta2);
  float y4 = S2_POS_Y + ARM_LEN_1*sin(M_PI - theta2);
  
  float delta = atan2((x4-x), (y-y4));
  
  float x1 = x + ARM_LEN_3*sin(delta-epsilon);
  float y1 = y - ARM_LEN_3*cos(delta-epsilon);
  
  float d1 = pitagoras(x1 - S1_POS_X, y1 - S1_POS_Y);
  float theta1 = atan2((y1 - S1_POS_Y), (x1 - S1_POS_X)) + cosine_angle_rule(d1, ARM_LEN_2, ARM_LEN_1);

  float s1deg = rad_to_deg(theta1);
  float s2deg = rad_to_deg(M_PI - theta2);
   
  servo1.write(s1deg + S1_OFFSET);
  servo2.write(s2deg + S2_OFFSET);
  
  float max_dist = max(abs(actual_s1_deg - s1deg), abs(actual_s2_deg - s2deg));
  int delay_servo = ceil(MS_PER_DEG*ceil(max_dist));

  if (delay_servo < 10)
    delay_servo = 10;
  
  actual_x = x;
  actual_y = y;
  actual_s1_deg = s1deg;
  actual_s2_deg = s2deg;
  
  delay(delay_servo);
}