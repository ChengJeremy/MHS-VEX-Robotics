const int MAX_SPEED = 11; //this isn't used it's just for reference
const int adjustment = 1;
const int WAIT_TIME = 250;
float realRightSpeed = 0;
float realLeftSpeed = 0;
int leftShaftVal = 0;
int rightShaftVal = 0;
string string1 = "";
string string2 = "";


float getLeftShooterSpeed()//this gets the speed of the shooter in revolutions of whatever shaft the encoder is on per second
{
	leftShaftVal = SensorValue(ENCODER_LEFT); //Gets shaft reading
	wait1Msec(100);
	leftShaftVal = SensorValue(ENCODER_LEFT) - leftShaftVal; //Current shaft reading minus eariler shaft reading
	return (leftShaftVal / 100.0) * (1000.0 / 360.0); //1000 ms/s and 1 rev/360 degrees
}

float getRightShooterSpeed()//this gets the speed of the shooter in revolutions of whatever shaft the encoder is on per second
{
	rightShaftVal = SensorValue(ENCODER_RIGHT);
	wait1Msec(100);
	rightShaftVal = SensorValue(ENCODER_RIGHT) - rightShaftVal;
	return (rightShaftVal / 100.0) * (1000.0 / 360.0);
}

task setLeftShooterSpeed() //0-11
{
	while(true)
	{
		realLeftSpeed = getLeftShooterSpeed();
		if(abs(realLeftSpeed - desiredSpeed) > 0.2)
		{
			if(realLeftSpeed < desiredSpeed)
			{
				motor[leftShooter] += adjustment;
				wait1Msec(WAIT_TIME);
			}
			if(realLeftSpeed > desiredSpeed)
			{
				motor[leftShooter] -= adjustment;
				wait1Msec(WAIT_TIME);
			}
		}
	}
}

task setRightShooterSpeed() //0-11
{
	while(true)
	{
		realRightSpeed = getRightShooterSpeed();
		if(abs(realRightSpeed - desiredSpeed) > 0.2)
		{
			if(realRightSpeed < desiredSpeed)
			{
				motor[rightShooter] += adjustment;
				wait1Msec(WAIT_TIME);
			}
			if(realRightSpeed > desiredSpeed)
			{
				motor[rightShooter] -= adjustment;
				wait1Msec(WAIT_TIME);
			}
		}
	}
}

void setShooterSpeed()
{
	startTask(setLeftShooterSpeed);
	startTask(setRightShooterSpeed);
}
