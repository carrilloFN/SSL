#include <stdio.h>
#include <string.h>

#define FDT '\0'

typedef unsigned int ushort;
typedef char* str;

ushort esPalabra(str cadenaCaracteres);
ushort realizarCalculo(str operaciones, ushort posicion);
void accionarConEstado(ushort estadoActual, char caracter, ushort *decimalActual, ushort *total, ushort *posDelDigito, ushort *multiplicadorAux);
void prepararMultiplicacion(ushort decimalActual, ushort *multiplicadorAux);
ushort operar(ushort subtotal, ushort decimalActual, ushort *multiplicadorAux);
void armarDecimal(ushort *decimal, ushort digito, ushort posDelDigito);
ushort obtenerDigito(char caracter);
ushort siguienteEstado(ushort estadoActual, char caracter);
ushort columna(char caracter);


int main(int argc, char *argv[]){
/*
Si la cantidad de argumentos pasados es menor que 2,
(1er arg, ruta, 2do arg, cadena), termino el programa retornando la cantidad de argumentos.
*/

    if(argc<2){
        printf("Cantidad de parametros incorrectos.\n");
        return argc;
    }
    
    str operaciones = argv[1];
    ushort resultado;
    ushort ultPosicion = esPalabra(operaciones);
    if(ultPosicion>0){
        resultado = realizarCalculo(operaciones,ultPosicion);
        printf("%s = %d",operaciones,resultado);
    }else{
        printf("La operacion ingresada no es valida");        
    }
}

/*
------------------------------------------------------------------
Dada una cadena, analiza si es palabra, devuelve la posicion del 
último caracter o bien 0 de no ser palabra.
------------------------------------------------------------------
*/

ushort esPalabra(str cadena){
    ushort estadoActual = 0, posCadena=0;
    const ushort estadoRechazo = 5;
    char caracterLeido = cadena[posCadena];
    
    while(caracterLeido!=FDT && estadoActual!=estadoRechazo){
        estadoActual = siguienteEstado(estadoActual,caracterLeido);
        posCadena++;
        caracterLeido = cadena[posCadena];
    }
    return (estadoActual!=estadoRechazo) ? (posCadena-1) : 0;
}

/*
------------------------------------------------------------------
Recorre la palabra (desde el final hacia la principio) y realiza 
determinada accion segun el estado al cual se mueve.
------------------------------------------------------------------
*/

ushort realizarCalculo(str operaciones, ushort posicion){
    ushort estadoActual=0, decimalActual=0, total=0, posDelDigito=1, multAux=0;
    char caracter;

    while(posicion!=-1){
        caracter = operaciones[posicion];
        posicion--;
        estadoActual = siguienteEstado(estadoActual,caracter);
        accionarConEstado(estadoActual,caracter,&decimalActual,&total,&posDelDigito,&multAux);
    }
    total = operar(total,decimalActual,&multAux);
    return total;
}



void accionarConEstado(ushort estado, char caracter, ushort *decimalActual, ushort *total, ushort *posDelDigito, ushort *multAux){
    ushort digito;

    if(estado==1||estado==2){
        digito = obtenerDigito(caracter);
        armarDecimal(decimalActual,digito,(*posDelDigito));
        (*posDelDigito) = (*posDelDigito)*10;
    }
    
    if(estado==3){
        switch (caracter){
            case '+':   *total = operar( *total, *decimalActual, multAux);
                        break;

            case '-':   *total = operar( *total, -(*decimalActual), multAux);
                        break;

            case '*':   prepararMultiplicacion( *decimalActual, multAux);
                        break;
        }
        (*decimalActual)=0;
        (*posDelDigito)=1;
    }


}

void prepararMultiplicacion(ushort decimalActual, ushort *multAux){
    (*multAux) = decimalActual;
}

ushort operar(ushort subtotal, ushort decimalActual, ushort *multAux){
    if((*multAux)!=0) subtotal += (*multAux)*decimalActual;
    else subtotal += decimalActual;
    *multAux=0;
    return subtotal;
}

/*
------------------------------------------------------------------
Me permite ir armando el numero decimal a 
medida que leo sus caracteres.
------------------------------------------------------------------
*/

void armarDecimal(ushort *decimal, ushort digito, ushort posDelDigito){
    (*decimal) = (digito*posDelDigito)+(*decimal);
}

/*
------------------------------------------------------------------
Dado un caracter, devuelve su valor numérico.
------------------------------------------------------------------
*/

ushort obtenerDigito(char caracter){
    return (caracter-'0');
}

/*
------------------------------------------------------------------
Dado un caracter, devuelve su columna.
------------------------------------------------------------------
*/
ushort columna(char caracter){
    if(caracter=='0') return 0;
    if(caracter>='1' && caracter<='9') return 1;
    if(caracter=='+' || caracter=='-' || caracter=='*') return 2;
    if(caracter==FDT) return 3;
    return 4;
}

/*
------------------------------------------------------------------
Dado un estado y un caracter leído, devuelve el estado siguiente.
------------------------------------------------------------------
*/

ushort siguienteEstado(ushort eActual, char caracter){
    static ushort tablaTransiciones[6][5]={
        /*0-*/{1,2,5,5,5},
        /*1 */{5,5,3,4,5},
        /*2 */{2,2,3,4,5},
        /*3 */{1,2,5,5,5},
        /*4+*/{5,5,5,5,5},
        /*5+*/{5,5,5,5,5}
    };
    return tablaTransiciones[eActual][columna(caracter)]; 
}