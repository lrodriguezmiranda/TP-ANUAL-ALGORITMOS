#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

const int k = 3;
const float Tasa_Comision = 0.10f;
const int Max_Mozos = 50;
const int Max_Productos = 100;
const int Max_Comandas = 300;

struct Producto {
    int codigo;
    char descripcion[50];
    float precio;
    int stockActual;
};

struct Mozo {
    int idMozo;
    char nombre[50];
    char contrasena[20];
    float totalComision;
};

struct Comanda{
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

// Encriptar / Desencriptar

void encriptar(char clave[]) {
    int i = 0;
    while (clave [i] != '\0') {
        clave [i] = clave [i] + k;
        i++;
    }
}

// Carga de mozos

int cargaMozos(Mozo mozos[]) {
    FILE * f = fopen ("mozos.dat", "rb");
    if (f == NULL){
        cout << "No se pudo abrir mozos.dat" << endl;
        return 0;
    }

    int cantidadMozos = 0;
    while (cantidadMozos < Max_Mozos && fread(&mozos[cantidadMozos], sizeof(Mozo), 1, f) == 1) {
        cantidadMozos++;
    }
    
    fclose(f);
    return cantidadMozos;
}

// Buscar Mozos

int buscarMozo (Mozo mozos[], int cantidadMozos, int idMozo){

    for(int i =0; i< cantidadMozos; i++) {
        
        if (mozos[i].idMozo == idMozo) {
            return i;
        }
    }
    return -1;
}

// Validar contraseña

bool validarContrasena(Mozo mozo, char ClaveIngresada[]) {
    encriptar(ClaveIngresada);

    if (strcmp(mozo.contrasena, ClaveIngresada) == 0) {
        return true;
    }
    return false;
}

// Cargar el inventario

int cargarInventario (Producto productos[]) {

    FILE * f = fopen("inventario.dat", "rb");

    if (f == NULL) {
        cout << "No se pudo abrir inventario.dat" << endl;
        return 0; 
    }
    
    int cantidadProductos = 0;

    while (cantidadProductos < Max_Productos && fread (&productos[cantidadProductos], sizeof (Producto), 1, f) == 1) {
        cantidadProductos++;
    }
    fclose(f);
    return cantidadProductos;
}

// Busque de Producto

int buscarProducto(Producto productos[], int cantidadProductos, int codigoProducto) {

    for(int i = 0; i < cantidadProductos; i++) {
        if (productos[i].codigo == codigoProducto) {
            return i;
        }
    }
    return -1;
}

// Actualización del inventario

void guardarInventario(Producto productos[], int cantidadProductos) {

    FILE* f = fopen ("inventario.dat", "r+b");
    
    if (f == NULL) {
        cout << "No se pudo abrir inventario.dat" << endl;
        return;
    }

    for (int i = 0; i < cantidadProductos; i++) {
        fseek(f, i * sizeof(Producto), SEEK_SET);
        fwrite(&productos[i], sizeof(Producto), 1, f);
    }

    fclose(f);
}

// Agregar comanda

void agregarComanda(FILE* f, Comanda comanda) {
    fseek (f, 0, SEEK_END);
    fwrite (&comanda, sizeof(Comanda), 1, f);
}

// Ordenar comandas por mozos

void ordenarComandas(Comanda comandas[], int cantidadComandas) {

    for (int i = 0; i < cantidadComandas - 1; i++) {
        for(int j = 0; j < cantidadComandas -1 - i; j++) {
            if(comandas[j].idMozo > comandas[j + 1].idMozo) {
                Comanda aux = comandas [j];
                comandas[j] = comandas [j + 1];
                comandas[j + 1] = aux;
            }
        }
    }
}

// Leer comandas del dia

int cargaComandas(FILE* f, Comanda comandas[]) {
    
    rewind(f);

    int cantidadComandas = 0;

    while(cantidadComandas < Max_Comandas && fread(&comandas[cantidadComandas], sizeof(Comanda), 1, f) == 1) {
        cantidadComandas++;
    }

    return cantidadComandas;
}

// Comandas ordenadas

int main() {   
    Mozo mozos[Max_Mozos];
    int cantidadMozos = cargaMozos(mozos);
    
    if (cantidadMozos == 0) {
        cout << "No hay mozos cargados" << endl;
        return 1;
    }

    Producto productos[Max_Productos];
    int cantidadProductos = cargarInventario(productos);

    if (cantidadProductos == 0){
        cout << "No hay productos cargados" << endl;
        return 1;
    }
    
    // Fecha, Nombre del archivo y creacion de la planilla del dia

    char fecha[11];
    cout << "Ingrese la fecha (DD-MM-AAAA):";
    cin >> fecha;

    char nombreArchivo[40];
    strcpy (nombreArchivo, "comandas_");
    strcat(nombreArchivo, fecha);
    strcat(nombreArchivo, ".dat");

    FILE* f = fopen(nombreArchivo, "a+b");
    if(f == NULL) {
        cout << "No se pudo abrir la planilla del dia" << endl;
        return 1;
    }

    char continuar = 's';
    while (continuar == 's' || continuar == 'S') {
        
        int idMozo;
        cout << "Ingrese numero de mozo:";
        cin >> idMozo;

        int posicionMozo = buscarMozo(mozos, cantidadMozos, idMozo);
        if (posicionMozo == -1) {
            cout << "El numero de mozo no existe";
            continue;
        }

        char clave[20];
        cout << "Ingrese su clave:";
        cin >> clave;
        
        if(!validarContrasena(mozos[posicionMozo], clave)) {
            cout << "Clave incorrecta";
            continue;
        }

        int codigoProducto;
        cout << "Ingrese codigo del producto:";
        cin >> codigoProducto;

        int posicionProducto = buscarProducto(productos, cantidadProductos, codigoProducto);
        if (posicionProducto == -1) {
            cout << "El producto no existe";
            continue;
        }

        int cantidad;
        cout << "Ingrese cantidad:";
        cin >> cantidad;
        if (cantidad <= 0) {
            cout << "La cantidad debe ser mayor a cero";
            continue;
        }

        if(cantidad > productos[posicionProducto].stockActual) {
            cout << "No hay stock suficiente";
            continue;
        }

        float importe = productos[posicionProducto].precio * cantidad;
        float comision = importe * Tasa_Comision;
        Comanda nuevaComanda;
        nuevaComanda.idMozo = idMozo;
        nuevaComanda.codigoProducto = codigoProducto;
        nuevaComanda.cantidad = cantidad;
        nuevaComanda.comision = comision;
       
        agregarComanda(f, nuevaComanda);
        productos[posicionProducto].stockActual -= cantidad;
        mozos[posicionMozo].totalComision += comision;

        cout << "La venta se cargo correctamente";
        cout << "¿Queres cargar otra venta?:";
        cin >> continuar;
    }
    
    guardarInventario(productos, cantidadProductos);
    fclose(f);
    f = fopen(nombreArchivo, "rb");

if (f != NULL) {
    Comanda comandas[Max_Comandas];
    int cantidadComandas = cargaComandas(f, comandas);
    fclose(f);

    ordenarComandas(comandas, cantidadComandas);
    f = fopen(nombreArchivo, "wb");

    if (f != NULL) {
        for (int i = 0; i < cantidadComandas; i++) {
            fwrite(&comandas[i], sizeof(Comanda), 1, f);
        }
        fclose(f);
    }
}
cout << "La planilla del dia se actualizo correctamente";
return 0;
}
