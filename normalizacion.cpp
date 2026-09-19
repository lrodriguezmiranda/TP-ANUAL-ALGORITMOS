#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

const int K = 3; // K=3 !!!
const float TASA_COMISION = 0.10f;
const int MAX_MOZOS = 50;          // tamaño del array de mozos, margen grande por si acaso
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
    char contrasena[20];      // se guarda encriptada
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
        clave[i] = clave[i] + K; //k es 3
        i++;
    }
}

void desencriptar(char clave[]) {
    int i = 0;
    while (clave[i] != '\0') {
        clave[i] = clave[i] - K;
        i++;
    }
}

int cargarComandasHistoricas(ComandaHistorica historicas[]) { //ARCHIVO DEL LA LISTA VIEJA
    FILE* f = fopen("comandas_historicas.dat", "rb");

    if (f == NULL) {
        cout << "No se pudo abrir comandas_historicas.dat" << endl;
        return 0;
    }

    int cantidadRegistros = 0;
    while (fread(&historicas[cantidadRegistros], sizeof(ComandaHistorica), 1, f) == 1) {
        cantidadRegistros++;
    }

    fclose(f);
    return cantidadRegistros;
}

int extraerMozos(ComandaHistorica historicas[], int cantidadHistoricas, Mozo mozos[]) { //RECORRE COMANDAS HISTORICAS Y ARMA UN  ARRAY DE MOZOS SIN REPETIR
    int cantidadMozos = 0;

    for (int i = 0; i < cantidadHistoricas; i++) {
        // BUSQUEDA SECUENCIAL DE MOZOS
        int posicion = -1;
        for (int j = 0; j < cantidadMozos; j++) {
            if (strcmp(mozos[j].nombre, historicas[i].nombreMozo) == 0) {
                posicion = j;
            }
        }

        if (posicion == -1) { //MOZO NUEVO
            if (cantidadMozos < MAX_MOZOS) {
                int nuevoId = cantidadMozos + 1;
                mozos[cantidadMozos].idMozo = nuevoId;
                strcpy(mozos[cantidadMozos].nombre, historicas[i].nombreMozo);
  
                char claveIn[20];
                if (nuevoId < 10) {
                    claveIn[0] = '0' + nuevoId;
                    claveIn[1] = '\0';
                }
                else {
                    claveIn[0] = '0' + (nuevoId / 10);
                    claveIn[1] = '0' + (nuevoId % 10);
                    claveIn[2] = '\0';
                }
                encriptar(claveIn);
                strcpy(mozos[cantidadMozos].contrasena, claveIn);

                mozos[cantidadMozos].totalComision = historicas[i].comision;
                cantidadMozos++;
            }
            else {
                cout << "Array de mozos lleno" << endl;
            }
        }
        else {
			// SI EL MOZO YA EXISTE, SUMAR LA COMISION
            mozos[posicion].totalComision = mozos[posicion].totalComision + historicas[i].comision;
        }
    }

    return cantidadMozos;
}

int main() {


	return 0;
}