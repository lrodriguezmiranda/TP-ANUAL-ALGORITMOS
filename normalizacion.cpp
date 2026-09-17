#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

const int K = 3; // K=3 !!!
const float TASA_COMISION = 0.10f;
const int MAX_MOZOS = 50;          // tamaño del array de mozoz, margen grande por si acaso
const int MAX_COMANDAS = 300;	  // tamaño del array de comandas, margen grande por si acaso

struct ComandaHistorica {        //enunciado 
    char fecha[11];         // "DD-MM-AAAA" + '\0'
    char nombreMozo[50];
    int codigoProducto;
    int cantidad;
    float comision;
};

struct Producto {
    int codigo;
    char descripcion[50];
    float precio;
    int stockActual;
};
struct Mozo {
    int idMozo;
    char nombre[50];
    char password[20];      // se guarda encriptada
    float totalComision;
};

// Formato de comandas_dd-mm-aaaa.dat y comandas_semana_sX-mm.dat
struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

//     Funciones 

void encriptar(char clave[]) { //void sin return 
    int i = 0;
    while (clave[i] != '\0') {
        clave[i] = clave[i]+K //k es 3
        i++;
    }
}

void desencriptar(char clave[]) {
    int i = 0;
    while (clave[i] != '\0') {
        clave[i] = clave[i]-K;
    }
}

int main() {


	return 0;
}