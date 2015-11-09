// these are functions relevant to both the autonomous and manual control
// it assumes that the main.c file already has the motor config done in #params

const int defaultSpeedSpeed = 80;
const int blockLength = 61; // centimeters
const float topSpeed = (3 * blockLength) / 2000.0; // robot top speed in cm/ms

bool isAlmost(int value, int constant, const float minimumAccuracy = 95.0) {
	if (constant == 0 && value != 0) {
		int aux = constant;
		constant = value;
		value = aux;
	} else if (constant == 0 && value == 0) {
		return true;
	} else {
		return abs((value - constant) / constant) <= (1 - minimumAccuracy / 100);
	}
}

void setShooterSpeed(int speed) {
	motor[launchTopLeft] = speed;
	motor[launchTopRight] = speed;
	motor[launchBottomLeft] = speed;
	motor[launchBottomRight] = speed;
}

void setRightWheelSpeed(int speed) {
	motor[wheelFrontRight] = speed;
	motor[wheelBackRight] = speed;
}

void setLeftWheelSpeed(int speed) {
	motor[wheelFrontLeft] = speed;
	motor[wheelBackLeft] = speed;
}

void spinClockwise(int speed = defaultSpeedSpeed) {
	setRightWheelSpeed(-speed);
	setLeftWheelSpeed(speed);
}

void spinCounterClockwise(int speed = defaultSpeedSpeed) {
	spinClockwise(-speed);
}

void spin(int speed = defaultSpeedSpeed) {
	spinClockwise(speed);
}

void setWheelSpeed(int speed) {
	setLeftWheelSpeed(speed);
	setRightWheelSpeed(speed);
}

// convert from the raw speed (like 127) to centimeters per millisecond (cm/ms)
float rawSpeedToCMMS(int raw) {
	// right now it is a very rough conversion process
	// I saw the robot move at 127 speed and it went about 3 blocks in 2 seconds
	// that gives us a raw calculation for the top speed right now
	// we assume that the raw speed to actual speed ratio is linear
	// moving at 67 is half of the topSpeed if we assume that the ratio is linear
	const float percentage = raw / 127; // dimensionless
	return percentage * topSpeed; // cm/ms
}

int rawSpeedFromCMMS(float cmms) {
	// just reverse the formula from rawSpeedToCMMS
	// cmms is percentage * topSpeed
	// percentage is raw / 127
	// we want to find raw, so we're given the formula:
	// cmms = raw / 127 * topSpeed
	// we already know cmms and topSpeed, so solving for raw gives us:
	// cmms / topSpeed * 127 = raw
	return cmms / topSpeed * 127;
}

// based on rawSpeedToCMMS
// converts the raw speed used for spin() into degrees per millisecond
float rawSpeedToDMS(int raw) {
	return raw * ratio; // TODO: find the linear ratio
}

// based on rawSpeedFromCMMS
// converts from degrees per millisecond to raw speed used for spin()
int rawSpeedFromDMS(float raw) {
	return raw / ratio;
}

void freeze() {
	setLeftWheelSpeed(0);
	setRightWheelSpeed(0);
}

// turns right in duration milliseconds
void faceRight(int duration = 1000) {
	const int distance = 90; // degrees
	const int speed = rawSpeedFromDMS(distance / duration);
	spin(speed);
	wait1Msec(duration);
	freeze();
}

// turns right in duration milliseconds
void faceRight(int duration = 1000) {
	const int distance = 90; // degrees
	const int speed = rawSpeedFromDMS(distance / duration);
	spin(-speed);
	wait1Msec(duration);
	freeze();
}

// distance is in centimeters
// speed here is in centimeters per millisecond, unlike in setWheelSpeed
void go(int distance, float speed = topSpeed) {
	const int rawSpeed = rawSpeedFromCMMS(speed);
	setWheelSpeed(rawSpeed);
	wait1Msec(distance / speed);
	freeze();
}

// finds the distance by taking the average of a bunch of different measurements
int distance(tSensors sensor = senseDistanceBack) {
	const int totalChecks = 5;
	int total = 0;
	for (int i; i < totalChecks; i++)
		total += SensorValue[sensor];
	return total / totalChecks;
}
