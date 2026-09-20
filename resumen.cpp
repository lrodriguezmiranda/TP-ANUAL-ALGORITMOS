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

  FILE * archivo;
  Comanda reg;

  int mozoActual;
  int productosMozo;
  int totalBuffet = 0;

  float comisionMozo;

  archivo = fopen("comandas_semana_s1-06.dat","rb");
  
  if(archivo == NULL){
    cout << "No existe el archivo";
    return 1;
  }

  if(fread(&reg,sizeof(Comanda),1,archivo)==1){
    while (true) {
      mozoActual = reg.idMozo;

      productosMozo = 0;
      comisionMozo = 0;

      while(reg.idMozo == mozoActual){
        productosMozo += reg.cantidad;
        comisionMozo += reg.comision;
        totalBuffet += reg.cantidad;

        if(fread(&reg,sizeof(Comanda),1,archivo)!=1) {
                    reg.idMozo = -1;
                    break;
                }
            }
      cout << "Mozo: " << mozoActual << endl;
      cout << "Productos vendidos: " << productosMozo << endl;
      cout << "Comision: $" << comisionMozo << endl;

      if (reg.idMozo == -1) {
        break;
      }
    }

    cout << "TOTAL DEL BUFFET: " << totalBuffet << " productos" << endl;

    fclose(archivo);

    return 0;
  }
  
}
