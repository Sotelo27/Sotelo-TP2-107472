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
	bool archivo_cargado;
};

//FUNCION LAUTARO MARTIN SOTELO
//FUNCION AUXILIAR QUE SE ENCARGA DE MOSTRAR EL NOMBRE DE POKEMON.
bool mostrar_pokemon(void *p, void *contexto)
{
	printf("Nombre: %s\n", pokemon_nombre(p));
	return true;
}

//FUNCION LAUTARO MARTIN SOTELO
//PRE: RECIBE EL STRUCT ESTADO DE PARTIDA EN VOID
//POST : CARGA EL ARCHIVO DE POKEMONES,Y OBTIENE LOS DATOS DEL MISMO.INFORMA SI EL ARCHIVO SELECCIONADO DA ERROR.
bool cargar_archivo(void *e)
{
	if (!e) {
		return false;
	}
	struct estado_juego *estado = e;
	if (!estado) {
		return false;
	}
	if (estado->archivo_cargado == true) {
		printf("\nYa se a cargado el archivo\n");
		return true;
	}
	juego_t *j = juego_crear();
	estado->juego = j;
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
		estado->archivo_cargado = true;
		printf("\nEl archivo se cargo correctamente\n");
		return true;
	} else if (archivo_estado == POKEMON_INSUFICIENTES) {
		estado->archivo_cargado = false;
		printf("\nEl archivo tiene pokemones insuficientes para el juego \n");
		return false;
	} else if (archivo_estado == ERROR_GENERAL) {
		estado->archivo_cargado = false;
		printf("\nEL archivo no se cargo correctamente \n");
		return false;
	}
	return true;
}

//FUNCION LAUTARO MARITN SOTELO
//PRE: RECIBE EL ESTADO DE PARTIDA.
//POST: MUESTRA POR PANTALLA CADA UNO DE LOS POKEMONES.
bool listar_pokemones(void *e)
{
	if (!e) {
		return false;
	}
	struct estado_juego *estado = e;
	if (!estado) {
		return false;
	}
	if (estado->archivo_cargado == false) {
		printf("\nNo se a cargado ningun archivo, cargue un archivo para listar pokemones\n");
		return true;
	}
	lista_con_cada_elemento(juego_listar_pokemon(estado->juego),
				mostrar_pokemon, NULL);
	return true;
}

//FUNCION LAUTARO MARTIN SOTELO
//RECIBE LOS CHAR NOMBRE Y SE ENCARGA DE DARLE EL VALOR QUE EL USUARIO LE DESEE INGRESAR.
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

//FUNCINO LAUTARO MARTIN SOTELO
//RECIBE EL ESTADO DE JUEGO Y MUESTRA POR PANTALLA SEGUN CORRESPONDA SI FUE CORRECTO O NO LA SELECCION.
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

//FUNCION LAUTARO MARTIN SOTELO
//PRE: RECIBE EL ESTADO DE PARTIDA Y EL TURNO
//POST: RETORNA UNA JUGADA T, PIDIENDOLE AL USUARIO QUE INGRESE LOS DATOS DEL ATAQUE QUE DESEA HACER.
jugada_t seleccionar_jugada()
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

//FUNCION LAUTARO MARTIN SOTELO
//PRE: RECIBE UN INT DE PUNTOS USUARIO Y UN INT PUNTOS DE ADVERSARIO
//POST: PRINTEA LOS PUNTOS DE CADA JUGADOR.
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

//FUNCION LAUTARO MARTIN SOTELO
//RECIBE EL RESULTADO, Y EVALUANDO EL RESULTADO DE CADA JUGADOR MOSTRARA UN PRINTF .
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

//FUNCION LAUTARO MARTIN SOTELO
//PRE: RECIBE EL POKEMON Y UN NOMBRE
//POST: EVALUA SI SON IGUALES O NO SUS NOMBRES.
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

//FUNCION LAUTARO MARTIN SOTELO.
//PRE : RECIBE LA LISTA DE POKEMON, Y LOS NOMBRES QUE ELIGIO EL USUARIO MAS EL ESTADO DE PARTIDA.
//POST: BUSCA SI LOS NOMBRES EXISTEN EN LA LISTA DE POKEMON DE LA PARTIDA, Y LO INSERTA EN UNA LISTA DE POKEMONES.
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
	lista_t *lista_con_pokemones = lista_crear();
	lista_insertar(lista_con_pokemones, pokemon_1);
	lista_insertar(lista_con_pokemones, pokemon_2);
	lista_insertar(lista_con_pokemones, pokemon_3);
	return lista_con_pokemones;
}

//FUNCION LAUTARO MARTIN SOTELO
//PRE: RECIBE UN TIPO DE POKEMON.
//POST: RETORNA UN CHAR QUE REPRESENTA SU TIPO.
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

//FUNCION LAUTARO MARTIN SOTELO
//PRE: RECIBE UN ATAQUE Y UN AUX.
//POST: PRINTEA LOS DATOS DEL ATAQUE.
void mostrar_datos_ataque(const struct ataque *a, void *aux)
{
	printf("┌─────────────────────┬───────────┬─────────┐\n");
	printf("│ Nombre del Ataque   │ Poder     │ Tipo    │\n");
	printf("├─────────────────────┼───────────┼─────────┤\n");
	printf("│ %-20s│ %-9d │ %-8s│\n", a->nombre, a->poder,
	       conseguir_tipo(a->tipo));
	printf("└─────────────────────┴───────────┴─────────┘\n");
}

//FUNCION LAUTARO MARTIN SOTELO
//PRE : RECIBE UN VOID *P Y UN CONTEXTO.
//POST : MUESTRA EL NOMBRE DEL POKEMON Y SU TIPO.
bool mostrar_datos_equipo(void *p, void *contexto)
{
	printf("\nEl pokemon numero 1 es %s de tipo %s y sus ataques son : \n",
	       pokemon_nombre(p), conseguir_tipo(pokemon_tipo(p)));
	con_cada_ataque(p, mostrar_datos_ataque, NULL);
	return true;
}

//FUNCION LAUTARO MARTIN SOTELO
//PRE:RECIBE UN VOID *E QUE REPRESENTA EL ESTADO DE PARTIDA.
//POST: CON LA FUNCION LISTA_CON_CADA_ELEMENTO CONSIGUIE LOS ELEMENTOS DE CADA LISTA DE POKEMON DE CADA JUGADOR, SIENDO POKEMON_T PARA MOSTRARLOS
void mostrar_equipos_pokemon(void *e)
{
	if (!e) {
		return;
	}
	struct estado_juego *estado = e;
	if (!estado) {
		return;
	}
	printf("\n Asi se a quedado conformado tu equipo pokemon : \n");
	lista_con_cada_elemento(estado->pokemon_jugador, mostrar_datos_equipo,
				NULL);
	printf("\n Asi se a quedado conformado el equipo pokemon  adversario: \n");
	lista_con_cada_elemento(estado->pokemon_adversario,
				mostrar_datos_equipo, NULL);
}

//FUNCION DE LAUTARO MARTIN SOTELO
//SE ENCARGA DE RELLENAR LOS DATOS DE LOS JUGADORES EN LA PARTIDA.
void datos_jugadores(adversario_t *adversario, void *e)
{
	struct estado_juego *estado = e;
	printf("\n---------!Juego iniciado--------\n");
	printf("\n----------Selecciona los pokemon para continuar, a continuacion ingresa el comando------------\n");
	char nombre_1[20], nombre_2[20], nombre_3[20];
	JUEGO_ESTADO estado_seleccion = ERROR_GENERAL;
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
	estado->pokemon_jugador = guardar_pokemones_jugador(
		estado, estado->pokemon_jugador, nombre_1, nombre_2,
		eleccionAdversario3);
	estado->pokemon_adversario = guardar_pokemones_jugador(
		estado, estado->pokemon_adversario, eleccionAdversario1,
		eleccionAdversario2, nombre_3);
	mostrar_equipos_pokemon(estado);
}

//FUNCION LAUTARO MARTIN SOTELO
//PRE : RECIBE UN VOID * E QUE REPRESENTA EL ESTADO.
//POST: EJECUTA EL JUEGO , PIDE AL USUARIO SUS POKEMON Y A LO LARGO DE 9 TURNOS PIDE LAS JUGADAS QUE DECIDE REALIZAR.
bool jugar(void *e)
{
	if (!e) {
		return false;
	}
	struct estado_juego *estado = e;
	if (!estado) {
		return false;
	}
	if (estado->archivo_cargado == false) {
		printf("\nNo se a cargado ningun archivo, cargue un archivo para jugar\n");
		return true;
	}
	adversario_t *adversario =
		adversario_crear(juego_listar_pokemon(estado->juego));
	resultado_jugada_t resultado = { .jugador1 = ATAQUE_ERROR };
	int turno = 0;
	datos_jugadores(adversario, estado);
	printf("\nPerfecto que comienze el juego!\n");
	while (!juego_finalizado(estado->juego)) {
		jugada_t jugada_jugador_1;
		jugada_t jugada_jugador_2;
		jugada_jugador_1 = seleccionar_jugada();
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
			lista_con_cada_elemento(estado->pokemon_jugador,
						mostrar_datos_equipo, NULL);
		}
		juego_finalizado(estado->juego);
	}
	mostrar_ganador(juego_obtener_puntaje(estado->juego, JUGADOR1),
			juego_obtener_puntaje(estado->juego, JUGADOR2));
	adversario_destruir(adversario);
	lista_destruir(estado->pokemon_jugador);
	lista_destruir(estado->pokemon_adversario);
	juego_destruir(estado->juego);
	estado->archivo_cargado = false;
	return true;
}

//FUNCION LAUTARO MARTIN SOTELO
//PRE: RECIBE UN VOID QUE REFERENCIA AL ESTADO
//POST : CAMBIA EL VALOR BOLEANO DE CONTINUAR PARA FINALIZAR EL JUEGO.
bool finalizar_juego(void *e)
{
	struct estado_juego *estado = e;
	estado->continuar = false;
	return true;
}

//FUNCION LAUTARO MARTIN SOTELO
//SE ENCARGA DE PRINTEAR LOS COMANDOS QUE EL USUARIO PUEDE REALIZAR
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

//FUNCION LAUTARO MARTIN SOTELO
//FUNCION QUE SE ENCARGA DE MOSTRAR AL USUARIO LOS TIPOS Y LA TABLA DE EFECTIVIDADES.
bool mostrar_tabla_tipos(void *e)
{
	if (!e) {
		return false;
	}
	struct estado_juego *estado = e;
	if (!estado) {
		return false;
	}

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

	printf("╔════════════════════════════════════════╗\n");
	printf("║     ATAQUE       |     EFECTIVIDAD     ║\n");
	printf("╠══════════════════╪════════════════════╣\n");
	printf("║  FUEGO           |  Efectivo vs PLANTA ║\n");
	printf("║                  |  Inefectivo vs AGUA ║\n");
	printf("╠══════════════════╪════════════════════╣\n");
	printf("║  PLANTA          |  Efectivo vs ROCA   ║\n");
	printf("║                  |  Inefectivo vs FUEGO║\n");
	printf("╠══════════════════╪════════════════════╣\n");
	printf("║  ROCA            |  Efectivo vs ELECT. ║\n");
	printf("║                  |  Inefectivo vs PLANTA║\n");
	printf("╠══════════════════╪════════════════════╣\n");
	printf("║  ELECTRICO       |  Efectivo vs AGUA  ║\n");
	printf("║                  |  Inefectivo vs ROCA║\n");
	printf("╠══════════════════╪════════════════════╣\n");
	printf("╠══════════════════╪════════════════════╣\n");
	printf("║  AGUA            |  Efectivo vs FUEGO. ║\n");
	printf("║                  |  Inefectivo vs ELECT.║\n");
	printf("╠══════════════════╪════════════════════╣\n");
	return true;
}

//FUNCION LAUTARO MARTIN SOTELO
//FUNCION QUE  SE ENCARGA CON PRINTF DE MOSTRAR LAS REGLAS DEL JUEGO AL JUGADOR.
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

//MAIN
int main(int argc, char *argv[])
{
	struct estado_juego estado = { .continuar = true,
				       .juego_iniciado = false,
				       .archivo_cargado = false };
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
	if (estado.archivo_cargado == true) {
		juego_destruir(estado.juego);
	}
	menu_destruir(menu);
}
