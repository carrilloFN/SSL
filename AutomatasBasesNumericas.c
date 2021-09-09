#include <stdio.h>
#include <string.h>

#define FDT '\0'

typedef unsigned int ushort;
typedef char* str;

ushort obtenerSiguienteEstado(ushort eActual, char caracter);
void esPalabra(str cad, ushort *octales, ushort *decimales, ushort *hexadecimales, ushort *cantNoRec);
ushort columna(char car);

int main(int argc, char *argv[]){
  
    /*
        Si la cantidad de argumentos pasados es menor que 2,
        (1er arg, ruta, 2do arg, cadena), termino el programa retornando la cantidad de argumentos.
    */

    if(argc<2){
        printf("Cantidad de parametros incorrectos.\n");
        return argc;
    }

    str cadenaAEvaluar = argv[1];
    str subCadenaActual = strtok(cadenaAEvaluar,"&");
    ushort cantOctales=0, cantDecimales=0, cantHexadecimales=0, cantNoRec=0, cantCadenas=0;

    while(subCadenaActual!=NULL){    
        esPalabra(subCadenaActual,&cantOctales,&cantDecimales,&cantHexadecimales,&cantNoRec);
        cantCadenas++;
        subCadenaActual = strtok(NULL,"&");
    }

    printf("\nSe encontraron %d:\n%d Numeros Octales\n%d Numeros Decimales\n%d Numeros Hexadecimales\n%d Errores lexicos\n",cantCadenas,cantOctales,cantDecimales,cantHexadecimales,cantNoRec);

    return 0;
    
}

void esPalabra(str cadena, ushort *cOct, ushort *cDec, ushort *cHex, ushort *cNoReconocidos){
    ushort estadoActual = 0, posicionCadena = 0;
    char caracter = cadena[posicionCadena];

    while(caracter!=FDT){
        estadoActual = obtenerSiguienteEstado(estadoActual, caracter);
        posicionCadena++;
        caracter = cadena[posicionCadena];
    }

    switch(estadoActual){
        case 2: case 3: 
            (*cOct)++;
            break;
        case 1: case 7:
            (*cDec)++;
            break;
        case 5: case 6:
            (*cHex)++;
            break;
        case 9:
            (*cNoReconocidos)++;
            break;
    }
}

ushort obtenerSiguienteEstado(ushort eActual, char caracter){
    static ushort tablaTransiciones[10][7]={
        /*0-*/{1,7,7,9,9,8,9},
        /*1+*/{3,2,9,9,4,9,9},
        /*2+*/{2,2,9,9,9,9,9},
        /*3+*/{9,9,9,9,9,9,9},
        /*4*/{5,6,6,6,9,9,9},
        /*5+*/{9,9,9,9,9,9,9},
        /*6*/{6,6,6,6,9,9,9},
        /*7+*/{7,7,7,9,9,9,9},
        /*8*/{9,7,7,9,9,9,9},
        /*9*/{9,9,9,9,9,9,9}
    };
    return tablaTransiciones[eActual][columna(caracter)];
}

ushort columna(char caracter){
    if(caracter=='0') return 0;
    if(caracter>='1' && caracter<='7') return 1;
    if(caracter>='8' && caracter<='9') return 2;
    if((caracter>='a' && caracter<='f')||(caracter>='A' && caracter<='F')) return 3;
    if(caracter=='x' || caracter=='X') return 4;
    if(caracter=='+' || caracter=='-') return 5;
    if(caracter=='&') return 6;
    if(caracter==FDT) return 7;
    return 8;
}