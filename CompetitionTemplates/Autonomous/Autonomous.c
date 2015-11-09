// this file is only supposed to be used in the main.c file
// that is why it assumes that it has the sensors all configured via pragmas

#include "lib.c"

// the gyro sensor cannot be set up via a pragma
// thus, I have to define the port it is connected to
// then, the setUpGyro() function will set it up
#define gyroPort in8

void setUpGyro() {
	// the gyro sensor only needs to be set up once, and it takes a while to do it
	// thus, we want to avoid setting it up 2+ times because we have limited time
	// have a static variable that remembers if the gyro has been set up
	// it is static, so it gets initialised when the function first gets run
	// after the first run, it is set to true and doesn't get destroyed
	// the second time the function is called, alreadySetUp is true
	// thus, we know that there is no good reason to run the function and quit out
	static bool alreadySetUp = false;

	if (alreadySetUp)
		return;

	const int waitDurations[2] = {2000, 4000}; // these seem to work the best

	// you have to give the sensor some time to let it set everything up
	SensorType[gyroPort] = sensorNone;
	wait1Msec(waitDurations[0]);
	SensorType[gyroPort] = sensorGyro;
	wait1Msec(waitDurations[1]);

	alreadySetUp = true;
}

// the gyro sensor can register postive and negative values
// spinning it one way will cause it to detect negative and the other positive
// the problem is that the sensor might be accidentally put upside down
// to make sure that we know what direction the sensor is using, use this
// returns true if the sensor goes up when the robot moves clockwise
// you can call this function multiple times and it will not remeasure
// it remembers what it detected last time and will return the value from before
bool gyroIsClockwise() {
	setUpGyro();
	static bool isClockwise, alreadyRun;

	if (alreadyRun)
		return isClockwise;
	const int spinSpeed = 40;
	const int spinDuration = 200;

	// see if the measurement goes up when the robot goes clockwise
	const int originalMeasurement = SensorValue[gyroPort];
	spinClockwise(spinSpeed);
	wait1Msec(spinDuration);
	isClockwise = SensorValue[gyroPort] > originalMeasurement;

	// go back to the original position and stop
	spinCounterClockwise(spinSpeed);
	wait1Msec(spinDuration);
	freeze();

	alreadyRun = true;

	return isClockwise;
}

// use this function instead of using the SensorValue directly
// it will take the raw measurement from the SensorValue and normalise it
float getGyroDegrees() {
	// we need to return the measurement in degrees turned clockwise
	return (gyroIsClockwise() ? 1 : -1) * SensorValue[gyroPort] / 10.0;
}

// @param spinDirection is positive to move clockwise and negative for counter
void faceGoal(int spinDirection = 1) {
	// we must face the goal of the correct colour, and we know that it's diagonal

	spinDirection = sgn(spinDirection);
	const int finalTurnAngle = spinDirection * 45; // degrees
	const int spinSpeed = 40; // 127 is the maximum

	while (getGyroDegrees() < finalTurnAngle) {
		spin(spinDirection * spinSpeed);
	}

	freeze();
}

// this task does not manage time because the main task will do it
task autonomous() {
	// determine if its on the block facing the other color goal or the other team
	faceLeft();
	int turnDirection = distance() > blockLength * 1.5 ? 

	go(blockLength);
	faceGoal();
}

task testAutonomous() {
	// as the tests pass, remove them and uncomment the next one

	go(blockLength);
	// while (true) int x = gyroIsClockwise();
	// while (true) int x = getGyroDegrees();
	// spin(30);
	// spin(-30);
	// while (true) int x = distance();
	// faceGoal();
	// startTask(autonomous);
}
