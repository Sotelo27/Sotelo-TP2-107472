#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "src/adversario.h"
#include "stdlib.h"
#include "src/hash.h"
#include <stdio.h>

/**
* Este main debe ser modificado para que el usuario pueda jugar el juego. Las
* instrucciones existentes son solamente a modo ilustrativo del funcionamiento
* muy alto nivel del juego.
*
* Las interacciones deben realizarse por entrada/salida estandar y estar validadas.
*
* Se aconseja en todo momento mostrar información relevante para el jugador (por
* ejemplo, mostrar puntaje actual y movimientos disponibles) para hacer que el
* juego sea facil de utilizar.
*/

typedef struct 
{
	hash_t* comandos;
}menu_t;

struct informacion_comando{
	char* descripcion;
	bool (*funcion)(juego_t *);
};

menu_t* menu_crear(){
	menu_t* m = calloc(1,sizeof(menu_t));
	m->comandos = hash_crear(3);
	return m;
}
void menu_agregar_comando(menu_t* m , char* comando, char* descripcion , bool (*f)(juego_t*)){
	struct informacion_comando *i = malloc(sizeof(struct informacion_comando));
	i->descripcion = descripcion;
	i->funcion = f;
	hash_insertar(m->comandos,comando,i,NULL);
}

bool menu_ejecutar_comando(menu_t* m , char* comando , juego_t * contexto){
	struct informacion_comando *i = hash_obtener(m->comandos,comando);
	if (!i){
		return false;
	}
	return i->funcion(contexto);
}

void menu_destruir(menu_t* m){
	hash_destruir(m->comandos);
	free(m);
}

bool mostrar_pokemon(void *p,void * contexto){
	printf("Nombre: %s\n",pokemon_nombre(p));
	return true;
}

bool cargar_archivo(juego_t *j){
	return false;
}

bool listar_pokemones(juego_t *j){
	lista_con_cada_elemento(juego_listar_pokemon(j),mostrar_pokemon,NULL);
	return true;
}

void mostrar_comandos(){
    printf("\n");
    printf("\t||~~~~~~~~~~~~~~~~~~LISTA DE COMANDOS~~~~~~~~~~~~~~~~||\n");
    printf("\t|| [i] Iniciar juego                                 ||\n");
    printf("\t|| [c] Cargar un archivo                             ||\n");
    printf("\t|| [l] Listar pokemones Disponible                   ||\n");
    printf("\t|| [s] Seleccionar pokemones                         ||\n");
    printf("\t|| [j] Jugar                                         ||\n");
    printf("\t|| [m] Mostrar ataques de pokemon                    ||\n");
    printf("\t|| [t] Mostrar tipo de pokemon                       ||\n");
    printf("\t|| [d] Seleccionar Dificultad                        ||\n");
    printf("\t|| [q] Finalizar juego                               ||\n");
    printf("\t||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n");
    printf("\n");
}

bool verificar_ataque(char *ataques_jugador[], char *ataque_nombre,int limite) {
    for (int i = 0; ataques_jugador[i] != NULL; i++) {
       if(ataque_nombre == ataques_jugador[i]){
	return true;
       }
    }
    return false; 
}


int main(int argc, char *argv[])
{
	juego_t *j = juego_crear();
	juego_cargar_pokemon(j,"ejemplos/correcto.txt");
	menu_t *menu = menu_crear();
	adversario_t *adversario =adversario_crear(juego_listar_pokemon(j));
	menu_agregar_comando(menu,"c","cargar un archivo",cargar_archivo);
	menu_agregar_comando(menu,"l","Listar pokemones",listar_pokemones);
	printf("TP2 > \n");
	while(!juego_finalizado(j)){
		printf("Ingresa un comando a continuacion\n");
		char linea[200];
		fgets(linea,200,stdin);
		linea[1] = 0;
		if(menu_ejecutar_comando(menu,linea,j) == false){
			printf("\nEse comando no existe\n");
		}
		JUGADOR jugador = JUGADOR1;
		resultado_jugada_t resultado;
		resultado.jugador1 = ATAQUE_ERROR;
		resultado.jugador2 = ATAQUE_ERROR;
		char *nombre1 = "Pikachu";
		char *nombre2 = "Charmander";
		char *nombre3 = "Floatzel";
		JUGADOR jugador2 = JUGADOR2;
		juego_seleccionar_pokemon(j,jugador,nombre1,nombre2,nombre3);
		jugada_t jugada_jugador2 = {"Cacnea", "Absorber"};
		jugada_t jugada_jugador1 = {"Charmander", "Ascuas"};
		char *eleccionAdversario1, *eleccionAdversario2, *eleccionAdversario3;
		adversario_pokemon_seleccionado(adversario,nombre1,nombre2,nombre3);
		adversario_seleccionar_pokemon(adversario, &eleccionAdversario1,
				       &eleccionAdversario2,
				       &eleccionAdversario3);
		juego_seleccionar_pokemon(j,jugador2,eleccionAdversario1,eleccionAdversario2,eleccionAdversario3);
		resultado = juego_jugar_turno(j,jugada_jugador1,jugada_jugador2);
		if(resultado.jugador1 == ATAQUE_EFECTIVO){
			printf("\nNo se cual es el error\n");
			}
		jugada_t jugada_jugador3 = {"Pikachu", "Rayo"};
		jugada_t jugada_jugador4 = {"Floatzel", "Cascada"};
		resultado = juego_jugar_turno(j,jugada_jugador3,jugada_jugador4);
		if(resultado.jugador1 == ATAQUE_EFECTIVO){
			printf("\nNo se cual es el error\n");
			}
	}
	
	/*
	printf("Ingresa un comando a continuacion\n");
	char linea[200];
	fgets(linea,200,stdin);
	linea[1] = 0;
	if(menu_ejecutar_comando(menu,linea,j) == false){
		printf("\nEse comando no existe\n");
	}
	JUGADOR jugador = JUGADOR1;
	char *nombre1 = "Pikachu";
	char *nombre2 = "Cacnea";
	char *nombre3 = "Charmander";
	JUGADOR jugador2 = JUGADOR2;
	juego_seleccionar_pokemon(j,jugador,nombre1,nombre2,nombre3);
	jugada_t jugada_jugador1 = {"Pikachu", "Rayo"};
	jugada_t jugada_jugador2 = {"Charmander", "Ascuas"};
	char *eleccionAdversario1, *eleccionAdversario2, *eleccionAdversario3;
	adversario_pokemon_seleccionado(adversario,nombre1,nombre2,nombre3);
	adversario_seleccionar_pokemon(adversario, &eleccionAdversario1,
			       &eleccionAdversario2,
			       &eleccionAdversario3);
	juego_seleccionar_pokemon(j,jugador2,eleccionAdversario1,eleccionAdversario2,eleccionAdversario3);
	juego_jugar_turno(j,jugada_jugador1,jugada_jugador2);
	jugada_t jugada_jugador3 = {"Pikachu", "Chispa"};
	jugada_t jugada_jugador4 = {"Charmander", "Furia"};
	juego_jugar_turno(j,jugada_jugador3,jugada_jugador4);
	printf("\nMarca antes de jugada de adversario\n");
	int contador = 0;
	while(contador < 9){
		adversario_proxima_jugada(adversario);
		contador ++;
	}
	*/
	menu_destruir(menu);
}
