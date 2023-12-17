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

struct datos_jugador {
	int puntos;
	lista_t *pokemones;
	char ataques_utilizados[9][20];
};

struct juego {
	lista_t *lista_pokemon;
	informacion_pokemon_t *pokemones_archivo;
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
	juego->jugador_1.pokemones = NULL;
	juego->jugador_2.pokemones = NULL;
	juego->pokemones_archivo = NULL;
	juego->estado_juego = false;
	juego->turnos = 0;
	juego->jugador_1.puntos = 0;
	juego->jugador_2.puntos = 0;
	for (int i = 0; i < 9; i++) {
		strcpy(juego->jugador_1.ataques_utilizados[i], "");
	}
	return juego;
}

//FUNCION DE LAUTARO MARTIN SOTELO
//PRE: RECIBE EL POKEMON Y UN VOID QUE ES LA LISTA POKEMON.
//POST: INSERTA CADA POKEMON EN LA LISTA.
void insertar_pokemon_lista(pokemon_t *pokemon, void *lista_pokemon)
{
	if (!pokemon || !lista_pokemon) {
		return;
	}
	lista_pokemon = lista_insertar(lista_pokemon, pokemon);
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo)
{
	if (juego == NULL || archivo == NULL) {
		return ERROR_GENERAL;
	}

	// Creo una estructura para almacenar la información de los Pokémon
	juego->pokemones_archivo = pokemon_cargar_archivo(archivo);
	if (juego->pokemones_archivo == NULL) {
		return ERROR_GENERAL;
	}

	if (!juego->lista_pokemon) {
		juego->lista_pokemon = lista_crear();
		if (!juego->lista_pokemon) {
			return ERROR_GENERAL;
		}
	}
	int cantidad_pokemon = con_cada_pokemon(juego->pokemones_archivo,
						insertar_pokemon_lista,
						juego->lista_pokemon);
	if (cantidad_pokemon < 4) {
		lista_destruir(juego->lista_pokemon);
		pokemon_destruir_todo(juego->pokemones_archivo);
		juego->lista_pokemon = NULL;
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

//FUNCION LAUTARO MARTIN SOTELO
//PRE: RECIBE EL POKEMON Y UN NOMBRE
//POST: EVALUA SI SON IGUALES O NO SUS NOMBRES.
int comparador(void *pokemon, void *nombre)
{
	if (!pokemon || !nombre) {
		return -1;
	}
	pokemon_t *aux_pokemon = (pokemon_t *)pokemon;
	const char *nombre_pokemon = (const char *)nombre;

	const char *nombre_aux_pokemon = pokemon_nombre(aux_pokemon);

	if (nombre_aux_pokemon != NULL && nombre_pokemon != NULL) {
		if (strcmp(nombre_aux_pokemon, nombre_pokemon) == 0) {
			return 0;
		}
	}

	return -1;
}

//FUNCION LAUTARO MARTIN SOTELO
//PRE: RECIBE EL JUEGO Y SUS DATOS.
//POST: REACOMODA LA LISTA POKEMON DE CADA JUGADOR, ASIGNANDOLE A CADA UNO EL POKEMON QUE LE ELIGIO EL JUGADOR RIVAL.
void juego_reasignar_pokemon(juego_t *juego)
{
	if (!juego) {
		return;
	}
	pokemon_t *aux_pokemon =
		lista_elemento_en_posicion(juego->jugador_1.pokemones, 2);
	lista_quitar_de_posicion(juego->jugador_1.pokemones, 2);
	lista_insertar_en_posicion(
		juego->jugador_1.pokemones,
		lista_elemento_en_posicion(juego->jugador_2.pokemones, 2), 2);
	lista_quitar_de_posicion(juego->jugador_2.pokemones, 2);
	lista_insertar_en_posicion(juego->jugador_2.pokemones, aux_pokemon, 2);
}

//FUNCION LAUTARO MARTIN SOTELO
//PRE: RECIBE EL JUEGO , EL JUGADOR Y LA LISTA DE POKEMONES
//POST: SE LE ASIGNA LA LISTA DE POKEMONES AL JUGADOR DETERMINADO, RETORNA SI HUBO ERROR O NO.
JUEGO_ESTADO asignar_pokemones_jugador(juego_t *juego, JUGADOR jugador,
				       lista_t *pokemones)
{
	if (jugador == JUGADOR1) {
		juego->jugador_1.pokemones = pokemones;
		if (!juego->jugador_1.pokemones) {
			lista_destruir(juego->jugador_1.pokemones);
			lista_destruir(pokemones);
			return ERROR_GENERAL;
		}
		juego->jugador_1.pokemones = pokemones;
	} else if (jugador == JUGADOR2) {
		juego->jugador_2.pokemones = pokemones;
		if (!juego->jugador_1.pokemones) {
			lista_destruir(juego->jugador_2.pokemones);
			lista_destruir(pokemones);
			return ERROR_GENERAL;
		}
		juego->jugador_2.pokemones = pokemones;
		juego_reasignar_pokemon(juego);
	}
	return TODO_OK;
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador,
				       const char *nombre1, const char *nombre2,
				       const char *nombre3)
{
	JUEGO_ESTADO asignacion_pokemon;
	if (juego == NULL || nombre1 == NULL || nombre2 == NULL ||
	    nombre3 == NULL) {
		return ERROR_GENERAL;
	}
	pokemon_t *pokemon_1 = lista_buscar_elemento(
		juego->lista_pokemon, comparador, (void *)nombre1);
	pokemon_t *pokemon_2 = lista_buscar_elemento(
		juego->lista_pokemon, comparador, (void *)nombre2);
	pokemon_t *pokemon_3 = lista_buscar_elemento(
		juego->lista_pokemon, comparador, (void *)nombre3);
	if (pokemon_1 == NULL || pokemon_2 == NULL || pokemon_3 == NULL) {
		return POKEMON_INEXISTENTE; // Si alguno de los Pokémon no existe en la lista
	}
	if ((pokemon_1 == pokemon_2) || (pokemon_1 == pokemon_3) ||
	    (pokemon_2 == pokemon_3)) {
		return POKEMON_REPETIDO; // Si hay Pokémon repetidos en la selección
	}

	lista_t *pokemones;
	pokemones = lista_crear();
	if (!pokemones) {
		lista_destruir(pokemones);
		return ERROR_GENERAL;
	}
	lista_insertar(pokemones, pokemon_1);
	lista_insertar(pokemones, pokemon_2);
	lista_insertar(pokemones, pokemon_3);
	asignacion_pokemon =
		asignar_pokemones_jugador(juego, jugador, pokemones);
	return asignacion_pokemon;
}

// FUNCION LAUTARO MARITN SOTELO
//PRE: RECIBE EL POKEMON Y EL ATAQUE QUE RECIBE
//POST: EVALUA SI ES UN ATAQUE EFECTIVO, INEFECTIVO O REGULAR SEGUN LO INDICADO.
RESULTADO_ATAQUE comprobar_eficacia_ataque(pokemon_t *pokemon,
					   const struct ataque *ataque_1)
{
	if (!pokemon || !ataque_1) {
		return ATAQUE_ERROR;
	}
	RESULTADO_ATAQUE ataque_eficacia = ATAQUE_REGULAR;

	if ((pokemon_tipo(pokemon) == PLANTA && ataque_1->tipo == FUEGO) ||
	    (pokemon_tipo(pokemon) == ROCA && ataque_1->tipo == PLANTA) ||
	    (pokemon_tipo(pokemon) == ELECTRICO && ataque_1->tipo == ROCA) ||
	    (pokemon_tipo(pokemon) == AGUA && ataque_1->tipo == ELECTRICO) ||
	    (pokemon_tipo(pokemon) == FUEGO && ataque_1->tipo == AGUA)) {
		ataque_eficacia = ATAQUE_EFECTIVO; // El ataque es efectivo
	} else if ((pokemon_tipo(pokemon) == FUEGO &&
		    ataque_1->tipo == PLANTA) ||
		   (pokemon_tipo(pokemon) == PLANTA &&
		    ataque_1->tipo == ROCA) ||
		   (pokemon_tipo(pokemon) == ROCA &&
		    ataque_1->tipo == ELECTRICO) ||
		   (pokemon_tipo(pokemon) == ELECTRICO &&
		    ataque_1->tipo == AGUA) ||
		   (pokemon_tipo(pokemon) == AGUA && ataque_1->tipo == FUEGO)) {
		ataque_eficacia = ATAQUE_INEFECTIVO; // El ataque es inefectivo
	} else if (pokemon_tipo(pokemon) == ataque_1->tipo ||
		   ataque_1->tipo == NORMAL) {
		ataque_eficacia =
			ATAQUE_REGULAR; // El ataque es neutral o tipo NORMAL
	}
	return ataque_eficacia;
}

//FUNCION DE LAUTARO MARTIN SOTELO:
//PRE : RECIBE EL RESULTADO DEL ATAQUE, Y EL ATAQUE DEL JUGADOR
//POST EVALUA DE ACUERDO A LOS ENUMS SI FUE EFECTIVO Y NO, DEVUELVE LOS PUNTOS SEGUN CORRESPONDA.
int calcular_puntos(RESULTADO_ATAQUE resultado_ataque,
		    const struct ataque *ataque_jugador)
{
	if (!ataque_jugador) {
		return 0;
	}
	int puntos = (int)ataque_jugador->poder;
	if (resultado_ataque == ATAQUE_EFECTIVO) {
		puntos = (int)ataque_jugador->poder * 3;
	} else if (resultado_ataque == ATAQUE_INEFECTIVO) {
		puntos = (int)ataque_jugador->poder / 2 +
			 (int)ataque_jugador->poder % 2;
	} else if (resultado_ataque == ATAQUE_REGULAR) {
		puntos = (int)ataque_jugador->poder;
	}
	return puntos;
}

resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1,
				     jugada_t jugada_jugador2)
{
	resultado_jugada_t resultado;
	resultado.jugador1 = ATAQUE_ERROR;
	resultado.jugador2 = ATAQUE_ERROR;
	if (juego->turnos == 0) {
		juego->jugador_1.puntos = 0;
	}
	if (!juego) {
		return resultado;
	}
	pokemon_t *pokemon_jugador_1 =
		lista_buscar_elemento(juego->jugador_1.pokemones, comparador,
				      (void *)jugada_jugador1.pokemon);
	if (!pokemon_jugador_1) {
		resultado.jugador1 = ATAQUE_ERROR;
		return resultado;
	}
	pokemon_t *pokemon_jugador_2 =
		lista_buscar_elemento(juego->jugador_2.pokemones, comparador,
				      (void *)jugada_jugador2.pokemon);
	if (!pokemon_jugador_2) {
		resultado.jugador2 = ATAQUE_ERROR;
		return resultado;
	}
	char *ataque_nombre_1 = jugada_jugador1.ataque;
	const struct ataque *ataque_jugador_1 =
		pokemon_buscar_ataque(pokemon_jugador_1, ataque_nombre_1);
	if (!ataque_jugador_1) {
		resultado.jugador1 = ATAQUE_ERROR;
		return resultado;
	}
	bool ataque_ya_utilizado = false;
	for (int i = 0; i < 9; i++) {
		if (strcmp(ataque_nombre_1,
			   juego->jugador_1.ataques_utilizados[i]) == 0) {
			ataque_ya_utilizado = true;
			break;
		}
	}

	if (ataque_ya_utilizado) {
		resultado.jugador1 = ATAQUE_ERROR;
		return resultado;
	} else {
		// Guardar el ataque solo si no ha sido utilizado previamente
		strcpy(juego->jugador_1.ataques_utilizados[juego->turnos],
		       ataque_nombre_1);
	}
	char *ataque_nombre_2 = jugada_jugador2.ataque;
	const struct ataque *ataque_jugador_2 =
		pokemon_buscar_ataque(pokemon_jugador_2, ataque_nombre_2);
	if (!ataque_jugador_2) {
		resultado.jugador1 = ATAQUE_ERROR;
		return resultado;
	}
	resultado.jugador1 =
		comprobar_eficacia_ataque(pokemon_jugador_2, ataque_jugador_1);
	juego->jugador_1.puntos +=
		calcular_puntos(resultado.jugador1, ataque_jugador_1);
	resultado.jugador2 =
		comprobar_eficacia_ataque(pokemon_jugador_1, ataque_jugador_2);
	juego->jugador_2.puntos +=
		calcular_puntos(resultado.jugador2, ataque_jugador_2);
	juego->turnos += 1;
	return resultado;
}

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador)
{
	if (!juego) {
		return 0;
	}
	if (jugador == JUGADOR1) {
		return juego->jugador_1.puntos;
	} else if (jugador == JUGADOR2) {
		return juego->jugador_2.puntos;
	}
	return 0;
}

bool juego_finalizado(juego_t *juego)
{
	if (!juego) {
		return false;
	}
	if (juego->turnos == 9) {
		juego->estado_juego = true;
	}
	return juego->estado_juego;
}

void juego_destruir(juego_t *juego)
{
	if (!juego) {
		return;
	}
	lista_destruir(juego->jugador_1.pokemones);
	lista_destruir(juego->jugador_2.pokemones);
	lista_destruir(juego->lista_pokemon);
	pokemon_destruir_todo(juego->pokemones_archivo);
	free(juego);
}