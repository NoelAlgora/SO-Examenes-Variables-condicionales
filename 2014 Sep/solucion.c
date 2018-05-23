#define N 	2

int num_libres 			= N;

pthread_mutex_t mutex;
pthread_cond_t cond;

sem_t surtidores;

int main() {
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&cond, NULL);
	/**/
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);


	//Para semaforos
	sem_init(&surtidores, 0, N);
	/**/
	sem_destroy(&surtidores);
}

void cliente(int dinero){
	pthread_mutex_lock(&mutex); 		//Bloqueamos el mutex
	
	while (num_libres == 0)				//Esperamos a un surtidor
		pthread_cond_wait(&cond, &mutex); /*bloqueo*/

	num_libres--;

	pthread_mutex_unlock(&mutex);		//Desbloqueamos el mutex

	ServirCombustible(surtidor,dinero); //Nos servimos

	pthread_mutex_lock(&mutex); 		//Bloqueamos el mutex
	num_libres++;						//Liberamos el surtidor
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);		//Desbloqueamos el mutex
}

//Semaforos
void cliente(int dinero){
	sem_wait(&surtidores);				//Esperamos a un surtidor

	ServirCombustible(surtidor,dinero); //Nos servimos

	sem_post(&surtidores);
}