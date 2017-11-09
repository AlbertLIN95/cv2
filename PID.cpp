#include "PID.h"

PID::PID(double dt, double max, double min, double Kp, double Kd, double Ki)
{
	this->dt = dt;
	this->max = max;
	this->min = min;
	this->Kp = Kp;
	this->Kd = Kd;
	this->Ki = Ki;
	pre_error = 0;
	integral = 0;
}
PID::~PID() {}
double PID::calculate(double setpoint, double pv)
{

	double error = setpoint - pv;
	double Pout = Kp * error;
	integral += error * dt;
	double Iout = Ki * integral;
	double derivative = (error - pre_error) / dt;
	double Dout = Kd * derivative;
	double output = Pout + Iout + Dout;
	if (output > max)
		output = max;
	else if (output < min)
		output = min;
	pre_error = error;
	return output;
}