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

void transformarComandas(ComandaHistorica historicas[], int cantidadHistoricas, Mozo mozos[], int cantidadMozos, Comanda comandas[], int& cantidadComandas){
    for(int i=0; i<cantidadHistoricas; i++){  
        for(int j=0; j<cantidadMozos; j++){
            //Busqueda secuencial de nombreMozo para obtener su idMozo
            if(strcmp(historicas[i].nombreMozo, mozos[j].nombre)==0){
                comandas[cantidadComandas].idMozo=mozos[j].idMozo;
                break;
            }
        }

        //Guardar las comandas transformadas en el arreglo comandas
        comandas[cantidadComandas].codigoProducto=historicas[i].codigoProducto;
        comandas[cantidadComandas].cantidad=historicas[i].cantidad;
        comandas[cantidadComandas].comision=historicas[i].comision;
        cantidadComandas++;
    }
}

void crearArchivosDiarios(ComandaHistorica historicas[], int cantidadHistoricas, Comanda comandas[]){
    for(int i=0; i<cantidadHistoricas; i++){
        bool fechaRepetida = false;
        for(int j=0; j<i; j++){
            if(strcmp(historicas[i].fecha, historicas[j].fecha)==0){
                fechaRepetida = true;
            }
        }
        if(!fechaRepetida){
            //Nombre y creacion de archivos
            char nombreArchivo[50] = "comandas_";
            strcat(nombreArchivo, historicas[i].fecha);
            strcat(nombreArchivo, ".dat");
            
            FILE* f = fopen(nombreArchivo, "wb");
            if(f == NULL){
                cout<<"No se pudo crear el archivo."<<endl;
                return;
            }
            
            Comanda comandasPorDia[MAX_COMANDAS];
            int cantidadComandasPorDia=0;

            for(int k=0; k<cantidadHistoricas; k++){
                
                //Si las fechas son iguales, corresponden al mismo archivo
                if(strcmp(historicas[i].fecha, historicas[k].fecha)==0){
                    comandasPorDia[cantidadComandasPorDia]=comandas[k];
                    cantidadComandasPorDia++;
                }
            }
            
            ordenarComandasPorMozo(comandasPorDia, cantidadComandasPorDia);

            //Grabar las comandas ordenadas por mozo
            for(int m=0; m<cantidadComandasPorDia; m++){
                fwrite(&comandasPorDia[m], sizeof(Comanda),1, f);
            }
            fclose(f);
            cout<<"Archivo creado."<<endl;
        }
    }
}

void ordenarComandasPorMozo(Comanda comandasPorDia[], int cantidadComandasPorDia){
    int posicionActual;
    Comanda valorPosicion;
    for(int i=0; i<cantidadComandasPorDia; i++){
        posicionActual=i;
        valorPosicion=comandasPorDia[i];
    }               
}


void grabarMozos(Mozo mozos[], int cantidadMozos) {
    FILE* f = fopen("mozos.dat", "wb");

    if (f == NULL) {
        cout << "Error:No se pudo crear mozos.dat" << endl;
        return;
    }

    for (int i = 0; i < cantidadMozos; i++) {
        fwrite(&mozos[i], sizeof(Mozo), 1, f);
    }

    fclose(f);
}

int main() {

    ComandaHistorica historicas[MAX_COMANDAS]; //DECLARACIONES
    Mozo mozos[MAX_MOZOS];
    Comanda comandas[MAX_COMANDAS];

    cout << "Inicio del proceso de normalizacion" << endl;

    int cantidadHistoricas = cargarComandasHistoricas(historicas);
    cout << "Se cargaron " << cantidadHistoricas << " comandas historicas" << endl;

    int cantidadMozos = extraerMozos(historicas, cantidadHistoricas, mozos);
    cout << "Se identificaron " << cantidadMozos << " mozos unicos" << endl;

    int cantidadComandas = 0;

    transformarComandas(historicas, cantidadHistoricas, mozos, cantidadMozos, comandas, cantidadComandas);

    grabarMozos(mozos, cantidadMozos);
    cout << "Archivo mozos.dat creado" << endl;

    cout << "Finalizacion del proceso de normalizacion" << endl;

    return 0;
}