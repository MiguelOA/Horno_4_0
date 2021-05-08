#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

using namespace std;

int main ()
{
	int Ent;
	int Sal;

	cout << "Coneccion I2C" << endl;
	int fd = wiringPiI2CSetup(0x05);
	delay(200);
	int Esc = wiringPiI2CWrite(fd, 0xA0);
	delay(200);
	Ent = wiringPiI2CRead(fd);
	cout << Ent << endl;
	delay(200);
	Ent = wiringPiI2CRead(fd);
	cout << Ent << endl;
	system("ls");
	//close(fd);
	return 0;
}
