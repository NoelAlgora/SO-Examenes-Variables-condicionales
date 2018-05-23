#define N 	32

int count = 0;
int turno_salida_glob = 0; 	//Podriamos usar el mismo count y reiniciarlo en 
							//el if(count == N) a 0 la pongo para que se vea mas claro

pthread_mutex_t mutex;
pthread_cond_t cond;

int main() {
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&cond, NULL);

}

void monja​(){
	int turno_salida;
	while​(1){
		//labor_en_cuestion
		turno_salida=entrar_comedor_y_sentarse();
		//comer
		salir_comedor(turno_salida);
	}
}

int entrar_comedor_y_sentarse(){


	pthread_mutex_lock(&mutex); 	//Bloqueamos el mutex
	
	int turno_propio = count;		//Guardamos nuestro puesto
	count++;						//Sumamos 1 a la cola
	
	while (count != N)				//Esperamos a todos
		pthread_cond_wait(&cond, &mutex); /*bloqueo*/

	if(count == N)					//Si estan todos despertamos a los demas
		pthread_cond_broadcast(&cond);

	pthread_mutex_unlock(&mutex);	//Desbloqueamos el mutex

	return turno_propio; 			//Devolvemos nuestro puesto de llegada (empezando desde 0)
}

void salir_comedor(int turno_salida){
	
	pthread_mutex_lock(&mutex); 	//Bloqueamos el mutex

	while(turno_salida != turno_salida_glob)
		pthread_cond_wait(&cond, &mutex); /*bloqueo*/

	turno_salida_glob++;			//Sumamos 1 a la cola
	
	pthread_cond_broadcast(&cond);	//Es nuestro turno, despertamos a los demas

	pthread_mutex_unlock(&mutex);	//Desbloqueamos el mutex
}