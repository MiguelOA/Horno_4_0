//**********************************************************************
//* Desencriptador de cadena de caracteres (contraseñas)
//* Software de prueba
//* Version 1.0 alpha
//* Desarrollador: Miguel Orozco
//**********************************************************************

//************************************************************ LIBRERIAS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//*********************************************************************
using namespace std;

//******************************************* DESCRIPCION DEL ALGORITMO
//* 1.- Obtener el argumento de main entre comillas
//* 2.- Por posibilidad de caracteres especiales
//* 3.- Restar 1 al valor ASCII de cada posicion
//* 4.- Sumar la posicion al valor ASCII
//* 5.- Adquirir el # de caracteres de original primer != /
//*********************************************************************

//*************************************************** FINCION PRINCIPAL
int main(int argc, char *argv[])
{
	char Cad[17];
	char nd;
	int i;
	int j;
	if (argc > 1)
	{
		for (j=0; j<16; j++)
		{
			argv[1][j]=argv[1][j]-1+j;
		}
		nd = argv[1][0];
		for (j=0; argv[1][j]==nd; j++);
		j=16-j;
		for (i=0; i<16; i++)
		{
			argv[1][i]=argv[1][i]-j;
		}
		for (i=0; i<j+1; i++)
		{
			Cad[i]=argv[1][16-i];
			cout << Cad[i];
		}
		cout << endl;
		return 1;
	}
	else
	{
		cout << "Sin texto a desencriptar" << endl;
		return 0;
	}
}
//*********************************************************************
