#include <iostream>
#include <stdio.h>

using namespace std;

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

int main() {

  FILE * archivo;    // Puntero al archivo semanal
  Comanda reg;       // Acá se guarda el registro que voy leyendo

  int mozoActual;     // Guarda el mozo del grupo actual
  int productosMozo;
  int totalBuffet = 0;

  float comisionMozo;
  int numeroSemana;
  int mes;

  char nombreArchivo[50];

//Pido qué semana quiero consultar
  cout << "Numero de semana: ";
  cin >> numeroSemana;
  cout << "Mes: ";
  cin >> mes;

// Armo el nombre del archivo semanal
  sprintf(nombreArchivo, "comandas_semana_s%d-%02d.dat", numeroSemana, mes);

// Abro el archivo semanal
  archivo = fopen(nombreArchivo, "rb");
    
// Si el archivo no existe, termina el programa
  if(archivo == NULL){
    cout << "No existe el archivo";
    return 1;
  }

// Leo el 1er registro
    
  if(fread(&reg,sizeof(Comanda), 1, archivo) == 1 ){
    while (true) {
      mozoActual = reg.idMozo;   // Empieza un nuevo mozo

      productosMozo = 0;
      comisionMozo = 0;

// Recorro todas las ventas de ese mozo
      while(reg.idMozo == mozoActual){
        productosMozo += reg.cantidad;
        comisionMozo += reg.comision;
        totalBuffet += reg.cantidad;

// Intento leer el siguiente registro
        if(fread(&reg, sizeof(Comanda), 1, archivo) != 1) {
                    reg.idMozo = -1;    // Si no lo puedo leer, llegué al final
                    break;
                }
            }
        
// Terminó el mozo, muestro el resúmen
      cout << "Mozo: " << mozoActual << endl;
      cout << "Productos vendidos: " << productosMozo << endl;
      cout << "Comision: $" << comisionMozo << endl;

      if (reg.idMozo == -1) {
        break;
      }
    }

    cout << "Total del buffet: " << totalBuffet << " productos" << endl;

    fclose(archivo);

    return 0;
  }
  
}
