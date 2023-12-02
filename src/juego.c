#include "juego.h"
#include "lista.h"
#include "tipo.h"
#include <stdbool.h>
#include "pokemon.h"
#include "ataque.h"
#include "stdlib.h"
#include "stdio.h"
#include "adversario.h"
#include "hash.h"
#include <string.h>


struct datos_jugador{
	int puntos;
	lista_t *pokemones;
	lista_t * ataques_jugador;
};

struct juego {
	lista_t *lista_pokemon;
	bool estado_juego;
	struct datos_jugador jugador_1;
	struct datos_jugador jugador_2;
	int turnos;
};

juego_t *juego_crear()
{
	juego_t *juego = malloc(sizeof(juego_t));
	if (!juego) {
		return NULL;
	}
	juego->lista_pokemon = NULL;
	juego->estado_juego = false;
	return juego;
}

//FUNCION DE LAUTARO MARTIN SOTELO
//PRE: RECIBE EL POKEMON Y UN VOID QUE ES LA LISTA POKEMON.
//POST: INSERTA CADA POKEMON EN LA LISTA.
void insertar_pokemon_lista(pokemon_t *pokemon, void *lista_pokemon)
{
	lista_pokemon = lista_insertar(lista_pokemon, pokemon);
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo)
{
	if (juego == NULL || archivo == NULL) {
		return ERROR_GENERAL;
	}

	// Creo una estructura para almacenar la información de los Pokémon
	informacion_pokemon_t *pokemon_info = pokemon_cargar_archivo(archivo);
	if (pokemon_info == NULL) {
		return POKEMON_INSUFICIENTES;
	}

	if (!juego->lista_pokemon) {
		juego->lista_pokemon = lista_crear();
		if (!juego->lista_pokemon) {
			return ERROR_GENERAL;
		}
	}
	int cantidad_pokemon = con_cada_pokemon(
		pokemon_info, insertar_pokemon_lista, juego->lista_pokemon);
	if (cantidad_pokemon < 4) {
		juego_destruir(juego);
		return POKEMON_INSUFICIENTES;
	}
	return TODO_OK;
}

lista_t *juego_listar_pokemon(juego_t *juego)
{
	if (!juego) {
		return NULL;
	}
	return juego->lista_pokemon;
}


//comparador de nombres
int comparador(void *pokemon, void *nombre) {
    pokemon_t* aux_pokemon = (pokemon_t*)pokemon;
    const char *nombre_pokemon = (const char *)nombre;
    
    const char *nombre_aux_pokemon = pokemon_nombre(aux_pokemon);
    
    if (nombre_aux_pokemon != NULL && nombre_pokemon != NULL) {
        if (strcmp(nombre_aux_pokemon, nombre_pokemon) == 0) {
            return 0;
        }
    }
    
    return -1;
}

//solo de prueba boorar despues
bool mostra_pokemon(void *p,void * contexto){
	printf("Nombre: %s\n",pokemon_nombre(p));
	return true;
}

void guardar_ataques(informacion_pokemon_t* pokemons,juego_t *juego){
	con_cada_pokemon(juego->jugador_1.pokemones,mostra_pokemon,NULL);
}

void juego_reasignar_pokemon(juego_t *juego){
	pokemon_t * aux_pokemon = lista_elemento_en_posicion(juego->jugador_1.pokemones,2);
	lista_quitar_de_posicion(juego->jugador_1.pokemones,2);
	lista_insertar_en_posicion(juego->jugador_1.pokemones,lista_elemento_en_posicion(juego->jugador_2.pokemones,2),2);
	lista_quitar_de_posicion(juego->jugador_2.pokemones,2);
	lista_insertar_en_posicion(juego->jugador_2.pokemones,aux_pokemon,2);
	for (int i = 0; i++ ; i < 3){
		con_cada_ataque(juego->jugador_1.pokemones,mostra_pokemon,NULL);
	}
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
				       const char *nombre1, const char *nombre2,
				       const char *nombre3)
{
	if (juego == NULL || nombre1 == NULL || nombre2 == NULL ||
	    nombre3 == NULL) {
		return ERROR_GENERAL;
	}
	pokemon_t* pokemon_1 = lista_buscar_elemento(juego->lista_pokemon,comparador,(void *)nombre1);
	pokemon_t* pokemon_2 = lista_buscar_elemento(juego->lista_pokemon,comparador,(void *)nombre2);
	pokemon_t* pokemon_3 = lista_buscar_elemento(juego->lista_pokemon,comparador,(void *)nombre3);
	if (pokemon_1 == NULL || pokemon_2 == NULL || pokemon_3 == NULL) {
		return POKEMON_INEXISTENTE; // Si alguno de los Pokémon no existe en la lista
	}
	if ((pokemon_1 == pokemon_2) ||
	    (pokemon_1 == pokemon_3) ||
	    (pokemon_2 == pokemon_3)) {
		return POKEMON_REPETIDO; // Si hay Pokémon repetidos en la selección
	}
	lista_t * pokemones ;
	pokemones = lista_crear();
	if (!pokemones){
		return ERROR_GENERAL;
	}
	pokemones = lista_insertar(pokemones,pokemon_1);
	pokemones = lista_insertar(pokemones,pokemon_2);
	pokemones = lista_insertar(pokemones,pokemon_3);
	if(jugador == JUGADOR1){
		juego->jugador_1.pokemones = lista_crear();
		juego->jugador_1.pokemones = pokemones;
	}else if(jugador == JUGADOR2){
		juego->jugador_2.pokemones = lista_crear();
		juego->jugador_2.pokemones = pokemones;
	}
	return TODO_OK;
}

// solo de prueba borrar despues
void mostrar_ataque(const struct ataque *a, void *aux)
{
	printf("%s: %i\n", a->nombre, a->poder);
}


// FUNCION LAUTARO MARITN SOTELO
int comprobar_eficacia_ataque(pokemon_t *pokemon, const struct ataque *ataque_1) {
    int ataque_eficacia = 2; // Inicializo en 2, reprensenta que el ataque es regular, 1 representa que fue efectivo y 0 representa que es inefectivo.

    if ((pokemon_tipo(pokemon)== PLANTA && ataque_1->tipo == FUEGO) ||
        (pokemon_tipo(pokemon)== ROCA && ataque_1->tipo == PLANTA) ||
        (pokemon_tipo(pokemon)== ELECTRICO && ataque_1->tipo == ROCA) ||
        (pokemon_tipo(pokemon)== AGUA && ataque_1->tipo == ELECTRICO) ||
        (pokemon_tipo(pokemon)== FUEGO && ataque_1->tipo == AGUA)) {
        ataque_eficacia = 1; // El ataque es efectivo
    } else if ((pokemon_tipo(pokemon)== FUEGO && ataque_1->tipo == PLANTA) ||
               (pokemon_tipo(pokemon)== PLANTA && ataque_1->tipo == ROCA) ||
               (pokemon_tipo(pokemon)== ROCA && ataque_1->tipo == ELECTRICO) ||
               (pokemon_tipo(pokemon)== ELECTRICO && ataque_1->tipo == AGUA) ||
               (pokemon_tipo(pokemon)== AGUA && ataque_1->tipo == FUEGO)) {
        ataque_eficacia = 0; // El ataque es inefectivo
    } else if (pokemon_tipo(pokemon)== ataque_1->tipo || ataque_1->tipo == NORMAL) {
        ataque_eficacia = 2; // El ataque es neutral o tipo NORMAL
    }
    return ataque_eficacia;
}

RESULTADO_ATAQUE asignar_resultado(int eficacia_ataque){
	RESULTADO_ATAQUE resultado_ataque = ATAQUE_INEFECTIVO;
	if(eficacia_ataque == 1){
		resultado_ataque = ATAQUE_EFECTIVO;
	}else if(eficacia_ataque == 2){
		resultado_ataque = ATAQUE_REGULAR;
	}
	return resultado_ataque;
}


int calcular_puntos(RESULTADO_ATAQUE resultado_ataque,const struct ataque * ataque_jugador){
	int puntos = 1;
	if (resultado_ataque == ATAQUE_EFECTIVO){
		puntos = (int)ataque_jugador->poder * 3;
	}else if(resultado_ataque == ATAQUE_INEFECTIVO){
		puntos = (int)ataque_jugador->poder / 2 ;
	}else if(resultado_ataque == ATAQUE_REGULAR){
		puntos = (int)ataque_jugador->poder ;
	}
	return puntos;
}

resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
				     jugada_t jugada_jugador2)
{
	if (juego->turnos == 0){
		juego_reasignar_pokemon(juego);
	}
	printf("\nEstos son los pokemon del usuario 1\n");
	lista_con_cada_elemento(juego->jugador_1.pokemones,mostra_pokemon,NULL);
	printf("\nesos eran\n");
	resultado_jugada_t resultado;
	resultado.jugador1 = ATAQUE_ERROR;
	resultado.jugador2 = ATAQUE_ERROR;
	pokemon_t * pokemon_jugador_1 = lista_buscar_elemento(juego->jugador_1.pokemones,comparador,(void *)jugada_jugador1.pokemon) ;
	if(!pokemon_jugador_1){
		resultado.jugador1 = ATAQUE_ERROR;
		return resultado;
	}
	pokemon_t * pokemon_jugador_2 = lista_buscar_elemento(juego->jugador_2.pokemones,comparador,(void *) jugada_jugador2.pokemon); 
	if(!pokemon_jugador_2){
		resultado.jugador2 = ATAQUE_ERROR;
		return resultado;
	}
	const char * ataque_nombre_1 = jugada_jugador1.ataque;
	const char * ataque_nombre_2 = jugada_jugador2.ataque;
	const struct ataque * ataque_jugador_1 = pokemon_buscar_ataque(pokemon_jugador_1,ataque_nombre_1);
	if (!ataque_jugador_1){
		resultado.jugador1 = ATAQUE_ERROR;
		return resultado;
	}
	const struct ataque * ataque_jugador_2 = pokemon_buscar_ataque(pokemon_jugador_2,ataque_nombre_2);
	if (!ataque_jugador_2){
		resultado.jugador2 = ATAQUE_ERROR;
		return resultado;
	}
	int eficacia_ataque = comprobar_eficacia_ataque(pokemon_jugador_2,ataque_jugador_1);
	resultado.jugador1 = asignar_resultado(eficacia_ataque);
	juego->jugador_1.puntos = calcular_puntos(resultado.jugador1,ataque_jugador_1);
	eficacia_ataque = comprobar_eficacia_ataque(pokemon_jugador_1,ataque_jugador_2);
	resultado.jugador2 = asignar_resultado(eficacia_ataque);
	juego->jugador_2.puntos = calcular_puntos(resultado.jugador2,ataque_jugador_2);
	juego->turnos +=1;
	return resultado;
}

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador)
{
	if (jugador == JUGADOR1){
		return juego->jugador_1.puntos;
	}else if(jugador ==JUGADOR2){
		return juego->jugador_2.puntos;
	}
	return 0;
}

bool juego_finalizado(juego_t *juego)
{
	if(juego->turnos == 9){
		juego->estado_juego = false;
	}
	return juego->estado_juego;
}

void juego_destruir(juego_t *juego)
{	
	lista_destruir(juego->lista_pokemon);
	free(juego);
}