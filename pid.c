/*
 * pid.c
 *
 * Created: 24.03.2017 13:52:07
 *  Author: Jorgen Jackwitz
 */ 

#define TIMECONSTANT 1000000
#define GAINCONSTANT 1000

static int16_t lastError = 0;
static int16_t totError = 0;
static int16_t intError = 0;
static int32_t derError = 0;

static int16_t propGain = 0;
static int16_t intGain = 0;
static int16_t derGain = 0;

static int32_t timeError = 0;
static int32_t timeStep	= 0;
static int16_t Kp = 0;
static int16_t Kd = 0;
static int16_t Ki = 0;


/*
pid_init receives Kp, Kd and Ki as positive floats. TimeStep, t, is a positive float in seconds. 

As long as the setpoint and currentValue are in the same range the pid can calculate for any values between +- 2^15  
*/


void pid_init(float t, float p, float d, float i){
	timeStep = t * TIMECONSTANT;
	Kp = p * GAINCONSTANT;
	Kd = d * GAINCONSTANT;
	Ki = i * GAINCONSTANT;
}

int32_t pid(int16_t currentValue, int16_t setPoint){
		
		totError = setPoint - currentValue;
		
		propGain = (totError * Kp)/GAINCONSTANT;
		
		timeError = (totError*TIMECONSTANT)/timeStep;
		
		intError = intError + timeError;
		
		intGain = (intError * Ki)/GAINCONSTANT;
		
		derError = ((totError - lastError)*TIMECONSTANT)/timeStep;
		
		derGain = (derError * Kd)/GAINCONSTANT;
		
		return(propGain + intGain + derGain);
}