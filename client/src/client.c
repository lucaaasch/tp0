#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;
	char* valor = malloc(sizeof(char));

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- Hecho*/ 

	logger = iniciar_logger();
	log_info(logger,"Soy un Log\n");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	valor = config_get_string_value (config,"CLAVE");
	log_info(logger,valor);
	ip = config_get_string_value (config,"IP");
	puerto  = config_get_string_value (config,"PUERTO");

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/
	conexion = crear_conexion(ip, puerto);
	enviar_mensaje(valor,conexion);
	
	log_info(logger,"Las lineas que le pasaremos al server  son: ");
	paquete(conexion,logger);
	
	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create("tp0.log","TP0",1,LOG_LEVEL_INFO);
	if(nuevo_logger==NULL){
		printf("No se pudo crear el logger.\n");
		exit(1);
	}

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config=config_create("cliente.config");
	if(nuevo_config==NULL){
		printf("No se pudo crear la config.");
		exit(2);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido=malloc(sizeof(char));

	// La primera te la dejo de yapa
	leido = readline("> ");
	while(*leido!='\0'){
		log_info(logger,leido);
		leido=readline("> ");
	}

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	free(leido);

	// ¡No te olvides de liberar las lineas antes de regresar!
}

void paquete(int conexion,t_log *logger)
{
	// Ahora toca lo divertido!
	
	char* leido=malloc(sizeof(char));
	t_paquete* paquete = crear_paquete();
	if(paquete==NULL){
		printf("No se pudo crear el paquete.\n");
		exit(3);
	}
	leido = readline("> ");
	while(*leido!='\0'){
		agregar_a_paquete(paquete,leido,strlen(leido)+1);
		log_info(logger,leido);
		leido=readline("> ");
	}
	// Leemos y esta vez agregamos las lineas al paquete
	enviar_paquete(paquete,conexion);

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
