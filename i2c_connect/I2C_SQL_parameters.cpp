//*******************************************************************
//* Intercomunicacion I2C via wiringPi y base de datos mySQL
//* Version: 0.42 alpha (Configuracion de salida por argumentos)
//* Fecha: Mayo 2020
//* Desarrollador: Miguel Orozco
//* gitHub: "por definir"
//*******************************************************************

// NOTAS:
// 1.- Incluir en los parametros el nombre de la tabla, de la
//     columna y el id para hacer el programa mas versatil

//********************************************************* LIBRERIAS
#include <iostream>
#include <fstream>
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
int Desencriptador();
int AcknowledgeDB();
//******************************************************************

//******************************************* DECLARACIONES GLOBALES
char name_USR[64];	//Nombre del usuario de la DB
char name_PSW[17];	//Password del usuario de la DB
char name_DB[64];	//Nombre de la base de datos (R1)
char name_TBO[64];	//Nombre de la tabla de salida (R2)
char name_DTO[64];	//Nombre del rango de salida (R3)
char name_TBI[64];	//Nombre de la tabla de entrada (R4)
char name_DTI[64];	//Nombre del rango de entrada (R5)
char name_id[64];	//Nombre del id de renglon (R6)
char dir_I2C[64];	//Direccion en texto del esclavo I2C
int  dir_I2C_N;		//Direccion en decimal del esclavo I2C
char pswd[17];		//Password desencriptado
int foo;		//Atrapa salidas tipo null
using namespace std;
//******************************************************************


//******************************************************FUNCION MAIN
int main(int argc, char *argv[])
{
	//*********************** LECTURA DE ARCHIVO DE CONFIGURACION
	ifstream fe("I2C_SQL.cnf"); //OJO PARA PRUEBAS SOLAMENTE
	// Archivo de texto de configuracion con los datos de la DB
	fe.getline(name_USR, 64);
	fe.getline(name_PSW, 17);
	fe.getline(name_DB, 64);
	//fe.getline(name_TBO, 64);
	//fe.getline(name_DTO, 64);
	fe.getline(name_TBI, 64);
	fe.getline(name_DTI, 64);
	//fe.getline(name_id, 64);
	fe.getline(dir_I2C, 64);
	int numCh;
	int Pot;
	numCh = strlen(dir_I2C);
	dir_I2C_N = 0;
	for (int i =0; i<numCh; i++)
	{
		Pot = pow(10, ((numCh-1)-i));
		dir_I2C_N = dir_I2C_N + (dir_I2C[i]-48)*Pot;
	}
	//cout << dir_I2C_N << endl;
	foo = Desencriptador();
	//cout << "--Configuracion cargada--" << endl;
	//************************************************************

	//***************************** SELECCION DE ACCION A EJECUTAR
	char cmd_char;
	int  cmd_int;
	int  ExSal;
	if (argc > 1)
	{
		cmd_char = argv[1][0];
		cmd_int = (int) cmd_char;
		string s_TBO = argv[2];
		string s_DTO = argv[3];
		string s_id = argv[4];
		//Pruebas de captura de multi argumento
		strcpy(name_TBO, s_TBO.c_str());
		//cout << name_TBO << endl;
		strcpy(name_DTO, s_DTO.c_str());
		//cout << name_DTO << endl;
		strcpy(name_id, s_id.c_str());
		//cout << name_id << endl;
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
	//cout << "Conectando por I2C" << endl;
	int fd = wiringPiI2CSetup(Dir);
	delay(100);//optimizar los tiempos de espera de respuesta
	int Esc = wiringPiI2CWrite(fd, cmdSal);
	delay(100);
	Ent1 = wiringPiI2CRead(fd);
	delay(100);
	Ent2 = wiringPiI2CRead(fd);
	string usr(name_USR);
	string pass(pswd);
	string Db(name_DB);
	string TblO(name_TBO);
	string RngO(name_DTO);
	string idN(name_id);
	cmdDB = "mysql -u " + usr +" -p" + pass + " -e \"USE ";
	cmdDB = cmdDB + Db + "; UPDATE " + TblO + " SET ";
	cmdDB = cmdDB + RngO + " = \'";
	if (Ent1 == 68)
	{
		if (Ent2 == 0)
		{
			cmdDB = cmdDB + "APG\' ";
		}
		else
		{
			cmdDB = cmdDB + "ENC\' ";
		}
		cmdDB = cmdDB + "WHERE id=" + idN + ";\"";
		int nC = cmdDB.length();
		char cmd_Sal[nC+1];
		strcpy(cmd_Sal, cmdDB.c_str());
		//cout << cmd_Sal << endl;
		system(cmd_Sal);
		AcknowledgeDB();
		return 1;
	}
	else
	{
		return 5;
	}
}

int Desencriptador()
{
	char nd;
	int i;
	int j;
	for (j=0; j<16; j++)
	{
		name_PSW[j]=name_PSW[j]-1+j;
	}
	nd = name_PSW[0];
	for (j=0; name_PSW[j]==nd; j++);
	j=16-j;
	for (i=0; i<16; i++)
	{
		name_PSW[i]=name_PSW[i]-j;
	}
	for (i=0; i<j; i++)
	{
		pswd[i]=name_PSW[15-i];
	}
	return 1;
}

int lecturaAnaloga(int Ch, int Dir)
{
	int Ent1;
        int Ent2;
        int cmdSal;
	int Val;
	string cmdDB;
	ostringstream Val_ost;
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
	//cout << Val << endl;
	string usr(name_USR);
	string pass(pswd);
	string Db(name_DB);
	string TblO(name_TBO);
	string RngO(name_DTO);
	string idN(name_id);
	Val_ost << Val;
	string Val_str = Val_ost.str();
	cmdDB = "mysql -u " + usr +" -p" + pass + " -e \"USE ";
	cmdDB = cmdDB + Db + "; UPDATE " + TblO + " SET ";
	cmdDB = cmdDB + RngO + " = ";
	cmdDB = cmdDB + Val_str + " ";
	cmdDB = cmdDB + "WHERE id=" + idN + ";\"";
	//cout << cmdDB << endl;
	int nC = cmdDB.length();
	char cmd_Sal[nC+1];
	strcpy(cmd_Sal, cmdDB.c_str());
	system(cmd_Sal);
	AcknowledgeDB();
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
        //cout << Ent1 << Ent2 << endl;
	AcknowledgeDB();
	return 1;
}

//funcion para el AKN en la base de datos
int AcknowledgeDB()
{
	string cmdDB;
	string usr(name_USR);
        string pass(pswd);
        string Db(name_DB);
        string TblI(name_TBI);
        string RngI(name_DTI);
        string idN(name_id);
        cmdDB = "mysql -u " + usr +" -p" + pass + " -e \"USE ";
        cmdDB = cmdDB + Db + "; UPDATE " + TblI + " SET ";
        cmdDB = cmdDB + RngI + " = \'N\' WHERE id=" + idN + ";\"";;
	//cout << cmdDB << endl;
	int nC = cmdDB.length();
        char cmd_Sal[nC+1];
        strcpy(cmd_Sal, cmdDB.c_str());
        system(cmd_Sal);
	return 1;
}
//********************************************************************
