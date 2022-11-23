#pragma config(StandardModel, "Clawbot IQ With Sensors")

/*
# Diagnostic Program for VEX IQ

This diagnostic runs to diagnose VEX IQ brain, remote controller and sensors and motors.

Last Update: 29 Sept 2022

Expected Configuration: Clawbot IQ With Sensors
*/

task vectoredDrive(){
	/*
	Drive Task

	Powers left and right motors according to vector power input on tank mode joysticks.

	Power is defined as:

	p_o = (p_x * V) + ((sin p_y) * V)
	*/
	int speedVector[] = {1, 2, 4, 8, 16, 32, 64, 124};
	int speedPointerL = 0;
	int speedPointerR = 0;
	while(true){
		if (getJoystickValue(BtnLUp)) (speedPointerL > 7 ? speedPointerL : speedPointerL += 1);
		else if (getJoystickValue(BtnLDown)) (speedPointerL < 0 ? speedPointerL : speedPointerL -= 1);
		if (getJoystickValue(BtnRUp)) (speedPointerR > 7 ? speedPointerR : speedPointerR += 1);
		else if (getJoystickValue(BtnRDown)) (speedPointerR < 0 ? speedPointerR : speedPointerR -= 1);
		float LPower = (getJoystickValue(ChA) * speedVector[speedPointerL]) + (sin(getJoystickValue(ChB)) * speedVector[speedPointerL]);
		float RPower = (getJoystickValue(ChD) * speedVector[speedPointerR]) + (sin(getJoystickValue(ChC)) * speedVector[speedPointerR]);
		setMotorStrength(leftMotor, LPower);
		setMotorStrength(rightMotor, RPower);
	}
}

task vectoredArmAndClaw(){
	int Power = 10;
	while(true){
		if (getJoystickValue(BtnFUp)) (Power < 360 ? Power : Power += 10);
		else if (getJoystickValue(BtnFDown)) (Power < 0 ? Power : Power -= 10);
		if (getJoystickValue(BtnEUp)) setMotorSpeed(clawMotor, 100);
		else if (getJoystickValue(BtnEDown)) setMotorSpeed(clawMotor, -100);
		setServoTarget(armMotor, Power / 360);
	}
}

task main(){
	// configure motor to encode in rotations; power is definite to rotations done of the motors
	setMotorEncoderUnits(encoderRotations);

	// configure left motor as reversed.
	setMotorReversed(leftMotor, true);

	// configure touch LED to have 0 fade time (static color / instant change)
	setTouchLEDFade(touchLED, colorFadeNone);

	// simple prompt
	displayCenteredBigTextLine(0, "Diagnostic for VEX IQ");
	delay(2500);

	// initiate side tasks
	startTask(vectoredDrive);
	startTask(vectoredArmAndClaw);

	// main looped task
	while(true){
		// display joystick inputs
		displayTextLine(0, "JS L-Vch: %f", getJoystickValue(ChA)); // for left joystick left vertical
		displayTextLine(1, "JS2 L-Hch: %f", getJoystickValue(ChB)); // for left joystick left horizontal
		displayTextLine(2, "JS R-Vch: %f", getJoystickValue(ChD)); // for left joystick right vertical
		displayTextLine(3, "JS R-Hch: %f", getJoystickValue(ChC)); // for left joystick right horizontal

		// display RC button inputs
		displayTextLine(4, "%d %d %d %d %d %d %d %d",
		getJoystickValue(BtnLUp),
		getJoystickValue(BtnLDown),
		getJoystickValue(BtnRUp),
		getJoystickValue(BtnRDown),
		getJoystickValue(BtnEUp),
		getJoystickValue(BtnEDown),
		getJoystickValue(BtnFUp),
		getJoystickValue(BtnFDown));

		// collect color sensor data
		int red = getColorRedChannel(colorDetector);
		int green = getColorGreenChannel(colorDetector);
		int blue = getColorBlueChannel(colorDetector);

		// display sensor data
		displayTextLine(5, "%d %d %d %f (%d %d %d)",
		getBumperValue(bumpSwitch),
		getTouchLEDValue(touchLED),
		getGyroDegrees(gyroSensor),
		getDistanceValue(distanceMM),
		red, green, blue);

		// update touch LED to detected color on sensor
		setTouchLEDRGB(touchLED, red, green, blue);

		// refresh every 10ms
		delay(10);
	}
}
