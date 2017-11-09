
class PID
{
public:
	PID(double dt, double max, double min, double Kp, double Kd, double Ki);
	~PID();
	double calculate(double setpoint, double pv);
private:
	double dt;
	double max;
	double min;
	double Kp;
	double Kd;
	double Ki;
	double pre_error;
	double integral;
};