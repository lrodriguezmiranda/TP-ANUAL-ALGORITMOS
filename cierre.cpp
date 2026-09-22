#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

const int MAX_DIAS = 7;
const int MAX_NOMBRE = 50;

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};


int abrirArchivosSemana(FILE* archivos[], int anio, int mes, int semana)
{
    int primerDia = (semana - 1) * 7 + 1;
    int cantidadArchivos = 0;

    for (int i = 0; i < MAX_DIAS; i++)
    {
        int dia = primerDia + i;

        if (dia > 31)
            break;

        char nombreArchivo[50];

        sprintf_s(
            nombreArchivo,
            "comandas_%02d-%02d-%04d.dat",
            dia,
            mes,
            anio
        );

        FILE* archivo = nullptr;
        fopen_s(&archivo, nombreArchivo, "rb");

        archivos[cantidadArchivos] = archivo;

        if (archivos[cantidadArchivos] != NULL)
        {
            cout << "Se encontro: " << nombreArchivo << endl;
            cantidadArchivos++;
        }
    }

    return cantidadArchivos;
}

void combinarArchivos(
    FILE* archivos[],
    int cantidadArchivos,
    FILE* archivoSemanal
)
{
    Comanda actuales[MAX_DIAS];
    bool hayComanda[MAX_DIAS];

    for (int i = 0; i < cantidadArchivos; i++)
    {
        if (fread(&actuales[i], sizeof(Comanda), 1, archivos[i]) == 1)
        {
            hayComanda[i] = true;
        }
        else
        {
            hayComanda[i] = false;
        }
    }

    bool quedanComandas = true;

    while (quedanComandas)
    {
        int posicionMenor = -1;

        for (int i = 0; i < cantidadArchivos; i++)
        {
            if (hayComanda[i])
            {
                if (posicionMenor == -1 ||
                    actuales[i].idMozo < actuales[posicionMenor].idMozo)
                {
                    posicionMenor = i;
                }
            }
        }

        if (posicionMenor == -1)
        {
            quedanComandas = false;
        }
        else
        {
            fwrite(
                &actuales[posicionMenor],
                sizeof(Comanda),
                1,
                archivoSemanal
            );

            if (fread(
                &actuales[posicionMenor],
                sizeof(Comanda),
                1,
                archivos[posicionMenor]
            ) == 1)
            {
                hayComanda[posicionMenor] = true;
            }
            else
            {
                hayComanda[posicionMenor] = false;
            }
        }
    }
}

void cerrarArchivos(FILE* archivos[], int cantidadArchivos)
{
    for (int i = 0; i < cantidadArchivos; i++)
    {
        fclose(archivos[i]);
    }
}

int main()
{
    int anio;
    int mes;
    int semana;

    cout << "===== CIERRE SEMANAL =====" << endl;

    cout << "Ingrese el anio: ";
    cin >> anio;

    cout << "Ingrese el mes: ";
    cin >> mes;

    cout << "Ingrese el numero de semana: ";
    cin >> semana;

    if (mes < 1 || mes > 12)
    {
        cout << "Mes invalido." << endl;
        return 1;
    }

    if (semana < 1 || semana > 5)
    {
        cout << "Numero de semana invalido." << endl;
        return 1;
    }

    FILE* archivos[MAX_DIAS];

    int cantidadArchivos = abrirArchivosSemana(
        archivos,
        anio,
        mes,
        semana
    );

    if (cantidadArchivos == 0)
    {
        cout << "No se encontraron planillas para esa semana." << endl;
        return 1;
    }

    char nombreSemanal[50];

    sprintf_s(
        nombreSemanal,
        "comandas_semana_s%d-%02d.dat",
        semana,
        mes
    );

    FILE* archivoSemanal = nullptr;
    fopen_s(&archivoSemanal, nombreSemanal, "wb");

    if (archivoSemanal == NULL)
    {
        cout << "No se pudo crear el archivo semanal." << endl;

        cerrarArchivos(archivos, cantidadArchivos);

        return 1;
    }

    combinarArchivos(
        archivos,
        cantidadArchivos,
        archivoSemanal
    );

    fclose(archivoSemanal);

    cerrarArchivos(archivos, cantidadArchivos);

    cout << endl;
    cout << "El cierre semanal se realizo correctamente." << endl;
    cout << "Archivo generado: " << nombreSemanal << endl;

    return 0;
}
