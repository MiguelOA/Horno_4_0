//***********************************************************
//* Encrtiptado de cadenas de caracteres (contraseñas)
//* Software de prueba
//* Version 1.0 alpha
//* Desarrollador: Miguel Alonso
//***********************************************************

//************************************************* LIBRERIAS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//***********************************************************
using namespace std;

//********************************* DESCRIPCION DEL ALGORITMO
//* 1.- Fijar el tamaño de la cadena a 16
//* 2.- Llenar los espacios con '+'
//* 3.- Intercambiar la posicion de los caracteres
//* 4.- restar en # de caractere al valor ASCII
//* 5.- sumar la posicion en la cadena en ASCII
//* 6.- Sumarle 1 en ASCII
//***********************************************************

//****************************************** FUNCION PRINCIPAL
int main(int argc, char *argv[])
{
	char Cad[17];
	int i;
	int j;
	for (i=0; i<16; i++)
	{
		Cad[i]='+';
	}
	if (argc > 1)
	{
		for (j=0; argv[1][j]!='\0'; j++);
		for (i=0; i<j; i++)
		{
			Cad[15-i] = argv[1][i];
		}
		for (i=0; i<16; i++)
		{
			Cad[i]=Cad[i]-i+j+1;
		}
		cout << Cad << endl;
		return 0;
	}
	else
	{
		cout << "Sin texto a encriptar" << endl;
		return 0;
	}
}
//************************************************************
