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
	void (*funcion)(juego_t *);
};

menu_t* menu_crear(){
	menu_t* m = calloc(1,sizeof(menu_t));
	m->comandos = hash_crear(3);
	return m;
}
void menu_agregar_comando(menu_t* m , char* comando, char* descripcion , void (*f)(juego_t*)){
	struct informacion_comando *i = malloc(sizeof(struct informacion_comando));
	i->descripcion = descripcion;
	i->funcion = f;
	hash_insertar(m->comandos,comando,i,NULL);
}

bool menu_ejecutar_comando(menu_t* m , char* comando , juego_t * contexto){
	struct informacion_comando *i = hash_obtener(m->comandos,comando);
	if (i){
		i->funcion(contexto);
		return true;
	}
	return false;
}

void menu_destruir(menu_t* m){
	free(m);
}

bool mostrar_pokemon(void *p,void * contexto){
	printf("Nombre: %s\n",pokemon_nombre(p));
	return true;
}

void cargar_archivo(juego_t *j){

}

void listar_pokemones(juego_t *j){
	lista_con_cada_elemento(juego_listar_pokemon(j),mostrar_pokemon,NULL);
}

int main(int argc, char *argv[])
{
	juego_t *j = juego_crear();
	juego_cargar_pokemon(j,"ejemplos/correcto.txt");
	menu_t *menu = menu_crear();
	menu_agregar_comando(menu,"c","cargar un archivo",cargar_archivo);
	menu_agregar_comando(menu,"l","Listar pokemones",listar_pokemones);
	printf("Ingresa un comando a continuacion\n");
	printf("TP2 > \n");
	char linea[200];
	fgets(linea,200,stdin);
	linea[1] = 0;
	JUGADOR jugador = JUGADOR1;
	const char *nombre1 = "Pikachu";
	const char *nombre2 = "Cacnea";
	const char *nombre3 = "Charmander";
	JUGADOR jugador2 = JUGADOR2;
	juego_seleccionar_pokemon(j,jugador,nombre1,nombre2,nombre3);
	juego_seleccionar_pokemon(j,jugador2,nombre1,nombre2,nombre3);
	jugada_t jugada_jugador1 = {"Charmander", "Lanzallamas"};
	jugada_t jugada_jugador2 = {"Cacnea", "Absorber"};
	juego_jugar_turno(j,jugada_jugador1,jugada_jugador2);
	menu_ejecutar_comando(menu,linea,j);
	menu_destruir(menu);
}
