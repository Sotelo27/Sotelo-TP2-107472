#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "src/adversario.h"
#include "stdlib.h"
#include "src/hash.h"
#include "src/menu.h"
#include <stdio.h>
#include <string.h>

struct estado_juego {
	juego_t *juego;
	menu_t *menu;
	lista_t *pokemon_jugador;
	lista_t *pokemon_adversario;
	bool continuar;
	bool juego_iniciado;
};

bool mostrar_pokemon(void *p, void *contexto)
{
	printf("Nombre: %s\n", pokemon_nombre(p));
	return true;
}

bool cargar_archivo(void *e)
{
	if (!e) {
		return false;
	}
	struct estado_juego *estado = e;
	if (!estado) {
		return false;
	}
	JUEGO_ESTADO archivo_estado;
	printf("Ingresa el nombre del archivo a continuacion:\n");
	char linea[200];
	if (fgets(linea, sizeof(linea), stdin) != NULL) {
		size_t longitud = strlen(linea);

		if (longitud > 0 && linea[longitud - 1] == '\n') {
			linea[longitud - 1] = '\0';
		}
	}
	archivo_estado = juego_cargar_pokemon(estado->juego, linea);
	if (archivo_estado == TODO_OK) {
		printf("\nEl archivo se cargo correctamente\n");
		return true;
	} else if (archivo_estado == POKEMON_INSUFICIENTES) {
		printf("\nEl archivo tiene pokemones insuficientes para el juego \n");
	} else if (archivo_estado == ERROR_GENERAL) {
		printf("\nEL archivo no se cargo correctamente \n");
	}
	return true;
}

bool listar_pokemones(void *e)
{
	if (!e) {
		return false;
	}
	struct estado_juego *estado = e;
	if (!estado) {
		return false;
	}
	if (lista_tamanio(juego_listar_pokemon(estado->juego)) == 0) {
		printf("\nAun no se a cargado ningun archivo, no hay pokemones por mostrar.\n");
		return true;
	}
	lista_con_cada_elemento(juego_listar_pokemon(estado->juego),
				mostrar_pokemon, NULL);
	return true;
}

void ingresar_nombres(char nombre_1[], char nombre_2[], char nombre_3[])
{
	printf("Jugador ingresa el primer pokemon:\n");
	fgets(nombre_1, 20, stdin);
	nombre_1[strlen(nombre_1) - 1] = '\0';

	printf("Jugador ingresa el segundo pokemon:\n");
	fgets(nombre_2, 20, stdin);
	nombre_2[strlen(nombre_2) - 1] = '\0';

	printf("Jugador ingresa el tercer pokemon:\n");
	fgets(nombre_3, 20, stdin);
	nombre_3[strlen(nombre_3) - 1] = '\0';
}

void mostrar_error_seleccion(JUEGO_ESTADO estado_seleccion)
{
	if (estado_seleccion == POKEMON_INEXISTENTE) {
		printf("\nHas ingresado un pokemon que no se encuentra en la lista cargada, reintenta nuevamente.\n");
	} else if (estado_seleccion == POKEMON_REPETIDO) {
		printf("\nUno de los pokemon que has ingresado se encuentra repetido\n");
	} else if (estado_seleccion == ERROR_GENERAL) {
		printf("\nHa habido un error al ingresarse,reintenta nuevamente\n");
	} else if (estado_seleccion == TODO_OK) {
		printf("\nSe a cargado correctamente los pokemon\n");
	}
}

jugada_t seleccionar_jugada(void *e, int turno)
{
	jugada_t jugada_jugador = { .pokemon = "nada", .ataque = "nada" };
	char nombre_1[20], nombre_2[20];
	printf("Jugador ingresa el nombre del pokemon:\n");
	fgets(nombre_1, 20, stdin);
	nombre_1[strlen(nombre_1) - 1] = '\0';
	printf("Jugador ingresa el nombre del ataque:\n");
	fgets(nombre_2, 20, stdin);
	nombre_2[strlen(nombre_2) - 1] = '\0';
	strcpy(jugada_jugador.pokemon, nombre_1);
	strcpy(jugada_jugador.ataque, nombre_2);
	return jugada_jugador;
}

void mostrar_ganador(int puntos_jugador, int puntos_adversario)
{
	printf("\nEL juego a finalizado!\n");
	printf("\nEstos son los puntos de cada uno de los jugadores :\n");
	printf("\nEl Jugador numero 1 ha conseguido : %d puntos!\n",
	       puntos_jugador);
	printf("\nEl Jugador numero 2 ha conseguido : %d puntos!\n",
	       puntos_adversario);
	if (puntos_jugador > puntos_adversario) {
		printf("\nEl ganador es el Jugador numero 1 .Felicidades!\n");
	} else if (puntos_adversario > puntos_jugador) {
		printf("\nEl ganador es el Jugador numero 2 .Felicidades!\n");
	} else {
		printf("\nLa partida a finalizado en un empate!!!\n");
	}
}

void mostrar_efectividad_ataque(resultado_jugada_t resultado)
{
	if (resultado.jugador1 == ATAQUE_EFECTIVO) {
		printf("\nEL ataque del Jugador 1 a sido Efectivo!!\n");
	} else if (resultado.jugador1 == ATAQUE_INEFECTIVO) {
		printf("\nEL ataque del Jugador 1 a sido Inefectivo!!\n");
	} else if (resultado.jugador1 == ATAQUE_REGULAR) {
		printf("\nEL ataque del Jugador 1 a sido Normal!!\n");
	}
	if (resultado.jugador2 == ATAQUE_EFECTIVO) {
		printf("\nEL ataque del Jugador 2 a sido Efectivo!!\n");
	} else if (resultado.jugador2 == ATAQUE_INEFECTIVO) {
		printf("\nEL ataque del Jugador 2 a sido Inefectivo!!\n");
	} else if (resultado.jugador2 == ATAQUE_REGULAR) {
		printf("\nEL ataque del Jugador 2 a sido Normal!!\n");
	}
}

int comparar_nombre_pokemon(void *pokemon, void *nombre)
{
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

lista_t *guardar_pokemones_jugador(void *e, lista_t *lista, char nombre_1[],
				char nombre_2[], const char *nombre_3)
{
	if (!e) {
		return NULL;
	}
	struct estado_juego *estado = e;
	if (!estado) {
		return NULL;
	}
	pokemon_t *pokemon_1 = lista_buscar_elemento(
		juego_listar_pokemon(estado->juego), comparar_nombre_pokemon,
		(void *)nombre_1);
	pokemon_t *pokemon_2 = lista_buscar_elemento(
		juego_listar_pokemon(estado->juego), comparar_nombre_pokemon,
		(void *)nombre_2);
	pokemon_t *pokemon_3 = lista_buscar_elemento(
		juego_listar_pokemon(estado->juego), comparar_nombre_pokemon,
		(void *)nombre_3);
	lista_t * lista_con_pokemones = lista_crear();
	lista_insertar(lista_con_pokemones,pokemon_1);
	lista_insertar(lista_con_pokemones,pokemon_2);
	lista_insertar(lista_con_pokemones,pokemon_3);
	return lista_con_pokemones;
}

char *conseguir_tipo(enum TIPO tipo_pokemon)
{
	switch (tipo_pokemon) {
	case ELECTRICO:
		return "E";

	case FUEGO:
		return "F";

	case NORMAL:
		return "N";

	case AGUA:
		return "A";

	case ROCA:
		return "R";

	case PLANTA:
		return "P";
	}
	return NULL;
}

void mostrar_datos_ataque(const struct ataque *a, void *aux)
{
	printf("┌─────────────────────┬───────────┬─────────┐\n");
	printf("│ Nombre del Ataque   │ Poder     │ Tipo    │\n");
	printf("├─────────────────────┼───────────┼─────────┤\n");
	printf("│ %-20s│ %-9d │ %-8s│\n", a->nombre, a->poder,
	       conseguir_tipo(a->tipo));
	printf("└─────────────────────┴───────────┴─────────┘\n");
}

bool mostrar_datos_equipo(void *p, void *contexto)
{
	printf("\nEl pokemon numero 1 es %s de tipo %s y sus ataques son : \n",pokemon_nombre(p),conseguir_tipo(pokemon_tipo(p)));
	con_cada_ataque(p,mostrar_datos_ataque,NULL);
	return true;
}

bool jugar(void *e)
{
	if (!e) {
		return false;
	}
	struct estado_juego *estado = e;
	if (!estado) {
		return false;
	}
	if (lista_tamanio(juego_listar_pokemon(estado->juego)) == 0) {
		printf("\nAun no se a cargado ningun archivo, no se puede jugar.\n");
		return true;
	}
	printf("\n---------!Juego iniciado--------\n");
	printf("\n----------Selecciona los pokemon para continuar, a continuacion ingresa el comando------------\n");
	char nombre_1[20], nombre_2[20], nombre_3[20];
	JUEGO_ESTADO estado_seleccion = ERROR_GENERAL;
	adversario_t *adversario =
		adversario_crear(juego_listar_pokemon(estado->juego));
	while (estado_seleccion != TODO_OK) {
		ingresar_nombres(nombre_1, nombre_2, nombre_3);
		estado_seleccion = juego_seleccionar_pokemon(
			estado->juego, JUGADOR1, nombre_1, nombre_2, nombre_3);
		mostrar_error_seleccion(estado_seleccion);
	}
	adversario_pokemon_seleccionado(adversario, nombre_1, nombre_2,
					nombre_3);
	char *eleccionAdversario1, *eleccionAdversario2, *eleccionAdversario3;
	adversario_seleccionar_pokemon(adversario, &eleccionAdversario1,
				       &eleccionAdversario2,
				       &eleccionAdversario3);
	juego_seleccionar_pokemon(estado->juego, JUGADOR2, eleccionAdversario1,
				  eleccionAdversario2, eleccionAdversario3);
	int turno = 0;
	resultado_jugada_t resultado = { .jugador1 = ATAQUE_ERROR };
	estado->pokemon_jugador = guardar_pokemones_jugador(estado,estado->pokemon_jugador, nombre_1, nombre_2,
				  eleccionAdversario3);
	estado->pokemon_adversario = guardar_pokemones_jugador(estado,estado->pokemon_adversario, eleccionAdversario1,
				  eleccionAdversario2, nombre_3);
	printf("\n Asi se a quedado conformado tu equipo pokemon : \n");
	lista_con_cada_elemento(estado->pokemon_jugador,mostrar_datos_equipo,NULL);
	printf("\n Asi se a quedado conformado el equipo pokemon  adversario: \n");
	lista_con_cada_elemento(estado->pokemon_adversario,mostrar_datos_equipo,NULL);
	printf("\nPerfecto que comienze el juego!\n");
	while (!juego_finalizado(estado->juego)) {
		jugada_t jugada_jugador_1;
		jugada_t jugada_jugador_2;
		jugada_jugador_1 = seleccionar_jugada(estado->juego, turno);
		jugada_jugador_2 = adversario_proxima_jugada(adversario);
		resultado = juego_jugar_turno(estado->juego, jugada_jugador_1,
					      jugada_jugador_2);
		if (resultado.jugador1 == ATAQUE_ERROR) {
			printf("\nHas escrito incorrectamente o el ataque es invalido, reintenta nuevamente\n");
		} else {
			turno++;
			printf("\nEl adversario a realizado el movimiento Pokemon: %s ataque: %s\n",
			       jugada_jugador_2.pokemon,
			       jugada_jugador_2.ataque);
			mostrar_efectividad_ataque(resultado);
			printf("\nEstos son tus ataques: \n");
			lista_con_cada_elemento(estado->pokemon_jugador,mostrar_datos_equipo,NULL);
		}
		juego_finalizado(estado->juego);
	}
	int puntos_jugador = juego_obtener_puntaje(estado->juego, JUGADOR1);
	int puntos_adversario = juego_obtener_puntaje(estado->juego, JUGADOR2);
	mostrar_ganador(puntos_jugador, puntos_adversario);
	adversario_destruir(adversario);
	lista_destruir(estado->pokemon_jugador);
	lista_destruir(estado->pokemon_adversario);
	juego_destruir(estado->juego);
	return true;
}

bool finalizar_juego(void *e)
{
	struct estado_juego *estado = e;
	estado->continuar = false;
	return true;
}

bool mostrar_comandos()
{
	printf("\n");
	printf("\t||~~~~~~~~~~~~~~~~~~LISTA DE COMANDOS~~~~~~~~~~~~~~~~||\n");
	printf("\t|| [v] Ver comandos                                  ||\n");
	printf("\t|| [c] Cargar un archivo                             ||\n");
	printf("\t|| [m] Mostrar tabla de Tipos                        ||\n");
	printf("\t|| [l] Listar pokemones Disponibles                  ||\n");
	printf("\t|| [j] Jugar                                         ||\n");
	printf("\t|| [q] Finalizar juego                               ||\n");
	printf("\t||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n");
	printf("\n");
	return true;
}

bool mostrar_tabla_tipos()
{
	printf("╔══════════════════════╗\n");
	printf("║ Tipos de Pokémon:    ║\n");
	printf("╠══════════════════════╣\n");
	printf("║ ELECTRICO: E         ║\n");
	printf("║ FUEGO:     F         ║\n");
	printf("║ NORMAL:    N         ║\n");
	printf("║ AGUA:      A         ║\n");
	printf("║ ROCA:      R         ║\n");
	printf("║ PLANTA:    P         ║\n");
	printf("╚══════════════════════╝\n");
	return true;
}

void motrar_reglas()
{
	printf("\n------------ Bienvenido al juego de pokemon! -------------------\n");
	printf("\n");
	printf("\nReglas del juego:\n");
	printf("----------------------");
	printf("\n||- Se necesitan 2 jugadores.                                                               ||");
	printf("\n||- Cada jugador elige 3 Pokémon de una lista.                                              ||");
	printf("\n||- Los primeros 2 Pokémon seleccionados son propios, el tercero se asigna del otro jugador.||");
	printf("\n||- El juego consta de 9 rondas.                                                            ||");
	printf("\n||- Cada ronda: un Pokémon y un ataque por jugador.                                         ||");
	printf("\n||- Los puntajes se basan en el tipo de ataque y el Pokémon atacado.                        ||");
	printf("\n||- Al final, gana el jugador con más puntos.                                               ||");
	printf("\n");
	printf("\n¡A disfrutar el juego!\n");
	printf("\n");
}

int main(int argc, char *argv[])
{
	juego_t *j = juego_crear();
	struct estado_juego estado = { .juego = j,
				       .continuar = true,
				       .juego_iniciado = false };
	menu_t *menu = menu_crear();
	menu_agregar_comando(menu, "v", "Ver comandos", mostrar_comandos);
	menu_agregar_comando(menu, "l", "Listar pokemones", listar_pokemones);
	menu_agregar_comando(menu, "m", "Tabla de tipos", mostrar_tabla_tipos);
	menu_agregar_comando(menu, "c", "Cargar un archivo", cargar_archivo);
	menu_agregar_comando(menu, "j", "Iniciar la partida", jugar);
	menu_agregar_comando(menu, "q", "Finalizar juego", finalizar_juego);
	mostrar_comandos();
	
	while (estado.continuar) {
		printf("Ingresa un comando a continuacion\n");
		char linea[200];
		char *leido;
		leido = fgets(linea, 200, stdin);
		if (leido == NULL)
			continue;
		linea[strlen(linea) - 1] = 0;
		MENU_RESULTADO resultado =
			menu_ejecutar_comando(menu, linea, &estado);
		if (resultado == MENU_INEXISTENTE) {
			printf("\nEse comando no existe.\n");
		} else if (resultado == MENU_ERROR) {
			printf("Hubo un error al ejecutar el menu, reintente nuevamente.\n");
		}
	}
	menu_destruir(menu);
}
