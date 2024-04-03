#include "client.h"
#include <readline/readline.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Soy un Log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	 
	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	
	valor = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	// Loggeamos el valor de config
	log_info(logger, "IP: %s\nCLAVE: %s\nPUERTO: %s", ip, valor, puerto);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);
	log_info(logger, "Conexion creada hacia el servidor");
	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	log_info(logger, "Comenzando a armar el paquete:");
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "Hola!, Soy un log", true, LOG_LEVEL_INFO);
	if ( nuevo_logger == NULL ) {
		exit(-1);
	}
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");
	if ( nuevo_config == NULL ) {
		exit(-1);
	}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	int i = 1;
	
	// La primera te la dejo de yapa
	leido = readline("> ");
	log_info(logger, "String leido: %s", leido);

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	leido = readline("> ");
	while (strcmp(leido, "")){
		log_info(logger, "Linea %d: %s", i, leido);
		leido = readline("> ");		
		i++;
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> String: "); // Leo de consola
	while (strcmp(leido, "")){ // Mientras no sea cadena vacia
		agregar_a_paquete(paquete, leido, strlen(leido)+1); // Agregamos al paquete el stream
		leido = readline("> String: "); // Leo nueva linea
	}
	enviar_paquete(paquete, conexion); // Enviamos el paquete

	/*
	paquete = malloc(sizeof(t_paquete)); // almaceno el tamaño de memoria con el tamaño de paquete
	paquete->codigo_operacion = PAQUETE; // Declaro que el paquete es tipo paquete

	// Leemos y esta vez agregamos las lineas al paquete

	leido = readline("> "); // Leo de consola
	int offset = 0; // Declaro offset
	while (strcmp(leido, "")){ // Mientras no sea cadena vacia
		memcpy(paquete->buffer->stream + offset, leido, sizeof(leido)); // Copia lo que esta en leido a el stream del buffer sumandole el off set y con el tamaño de leido
		offset += sizeof(leido); // Aumento el offset en el tamaño de leido
		leido = readline("> "); // Leo nueva linea
	}
	paquete->buffer->size = sizeof(paquete->buffer->stream); // Declaro el tamaño del buffer con el stream del buffer
	send(conexion, paquete->buffer->stream, paquete->buffer->size, 0); // Envio a conexion el stream del buffer con el tamaño del buffer y de flag mando un 0
	*/
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!

	free(leido);
	eliminar_paquete(paquete);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	  log_destroy(logger);
	  config_destroy(config);
	  liberar_conexion(conexion);
	  
}
