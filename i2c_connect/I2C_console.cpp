//*******************************************************************
//* Intercomunicacion I2C via wiriningPi
//* Version: 0.40 alph
//* Fecha: Mayo 2020
//* Desarrollador: Miguel Orozco
//* gitHub: "por definir"
//*******************************************************************

//********************************************************* LIBRERIAS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>
#include <sstream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
//*******************************************************************

//******************************************* PROTOTIPOS DE FUNCIONES
int lecturaAnaloga(int, int);
int lecturaDigital(int, int);
int escrituraDigital(int, bool, int);
//*******************************************************************

//******************************************** DECLARACIONES GLOBALES
char dir_I2C[64];	//Direccion en texto del esclavo I2C
int  dir_I2C_N;		//Direccion en decimal del esclavo I2C
char pswd[17];		//Password desencriptado
int foo;		//Atrapa salidas tipo null
using namespace std;
//*******************************************************************


//*******************************************************FUNCION MAIN
int main(int argc, char *argv[])
{
	int numCh;
	dir_I2C_N = 5; //Direccion del I2C del esclavo

	//**************************** SELECCION DE ACCION A EJECUTAR
	char cmd_char;
	int  cmd_int;
	int  ExSal;
	if (argc > 1)
	{
		cmd_char = argv[1][0];
		cmd_int = (int) cmd_char;
	}
	else
	{
		cmd_int = 255;
	}
	switch (cmd_int)
	{
		case 48:
			ExSal = lecturaAnaloga(0, dir_I2C_N);
			return ExSal; //Accion ejecutada
			break;
		case 49:
			ExSal = lecturaAnaloga(1, dir_I2C_N);
			return ExSal;
			break;
		case 50:
			ExSal = lecturaAnaloga(2, dir_I2C_N);
			return ExSal;
			break;
		case 51:
			ExSal = lecturaAnaloga(3, dir_I2C_N);
			return ExSal;
			break;
		case 67:
			ExSal = lecturaDigital(0, dir_I2C_N);
			return ExSal;
			break;
		case 68:
			ExSal = lecturaDigital(1, dir_I2C_N);
			return ExSal;
			break;
		case 69:
			ExSal = escrituraDigital(0, true, dir_I2C_N);
			return ExSal;
			break;
		case 101:
			ExSal = escrituraDigital(0, false, dir_I2C_N);
			return ExSal;
			break;
		case 70:
			ExSal = escrituraDigital(1, true, dir_I2C_N);
			return ExSal;
			break;
		case 102:
			ExSal = escrituraDigital(1, false, dir_I2C_N);
			return ExSal;
			break;
		case 255:
			cout << "Sin accion asignada" << endl;
			return 4; //Sin comanmdo asignado
			break;
		default:
			cout << "ERROR: Comando no valido" << endl;
			return 0; //Error de ninguna accion ejecutada
			break;
	}
}
//********************************************************************

//************************************************** FUNCIONES PROPIAS
int lecturaDigital(int Ch, int Dir)
{
	int Ent1;
	int Ent2;
	int cmdSal;
	string cmdDB;
	switch (Ch)
	{
		case 0:
			cmdSal = 0xD0;
			break;
		case 1:
			cmdSal = 0xD1;
			break;
		default:
			return 5;
			break;
	}
	int fd = wiringPiI2CSetup(Dir);
	delay(100);//optimizar los tiempos de espera de respuesta
	int Esc = wiringPiI2CWrite(fd, cmdSal);
	delay(100);
	Ent1 = wiringPiI2CRead(fd);
	delay(100);
	Ent2 = wiringPiI2CRead(fd);
	if (Ent1 == 68)
	{
		if (Ent2 == 0)
		{
			cmdDB = "APG";
		}
		else
		{
			cmdDB = "ENC";
		}
		cout << cmdDB << endl;
		return 1;
	}
	else
	{
		return 5;
	}
}


int lecturaAnaloga(int Ch, int Dir)
{
	int Ent1;
        int Ent2;
        int cmdSal;
	int Val;
	string cmdDB;
	switch (Ch)
        {
                case 0:
                        cmdSal = 0xA0;
                        break;
                case 1:
                        cmdSal = 0xA1;
                        break;
		case 2:
			cmdSal = 0xA2;
			break;
		case 3:
			cmdSal = 0xA3;
			break;
              default:
                        return 5;
                        break;
        }
        //cout << "Conectando por I2C" << endl;
        int fd = wiringPiI2CSetup(Dir);
        delay(100);//optimizar los tiempos de espera de respuesta
        int Esc = wiringPiI2CWrite(fd, cmdSal);
        delay(100);
        Ent1 = wiringPiI2CRead(fd);
        delay(100);
        Ent2 = wiringPiI2CRead(fd);
	Val = 256 * Ent1 + Ent2;
	cout << Val << endl;
	return 1;
}

int escrituraDigital(int Ch, bool Acn, int Dir)
{
        int Ent1;
        int Ent2;
        int cmdSal;
        int Val;
        string cmdDB;
        switch (Ch)
        {
                case 0:
			if (Acn == false){cmdSal = 0xE0;}
			if (Acn == true){cmdSal = 0xE1;}
                        break;
                case 1:
                        if (Acn == false){cmdSal = 0xF0;}
			if (Acn == true){cmdSal = 0xF1;}
                        break;
              default:
                        return 5;
                        break;
        }
        //cout << "Conectando por I2C" << endl;
        int fd = wiringPiI2CSetup(Dir);
        delay(100);//optimizar los tiempos de espera de respuesta
        int Esc = wiringPiI2CWrite(fd, cmdSal);
        delay(100);
        Ent1 = wiringPiI2CRead(fd);
        delay(100);
        Ent2 = wiringPiI2CRead(fd);
        cout << Ent1 << Ent2 << endl;
	return 1;
}

//********************************************************************
