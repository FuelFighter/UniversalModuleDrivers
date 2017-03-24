/*
 * pid.h
 *
 * Created: 24.03.2017 15:01:25
 *  Author: Ultrawack
 */ 


#ifndef PID_H_
#define PID_H_

void pid_init(float t, float p, float d, float i);

int32_t pid(int16_t currentValue, int16_t setPoint);

#endif /* PID_H_ */