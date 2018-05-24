// N EQUIPOS DE 1 COCHE
#define N 			4

int depositos[N] = {0};		//Litros en los depositos

pthread_mutex_t mutex;
pthread_cond_t cond;

int main() {
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&cond, NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}

void mecanico1(int equipo){
	while(true){
		esperar_llegada_coche(equipo);

		pthread_mutex_lock(&mutex); 	//Bloqueamos el mutex
		while(depositos[equipo] == 0)
		{
			pthread_cond_broadcast(&cond);	//Desbloqueamos la variable condicional
			pthread_cond_wait(&cond, &mutex); /*bloqueo*/
		}
		pthread_mutex_unlock(&mutex);	//Desbloqueamos el mutex

		llenar_depósito_coche(L,equipo);	//Podemos desbloquear? Operaciones no protegidas

		pthread_mutex_lock(&mutex); 	//Bloqueamos el mutex
		if(depositos[equipo] == 0)
			pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mutex);	//Desbloqueamos el mutex
	}
}

void mecánico2(int equipo){
	while(true){
		pthread_mutex_lock(&mutex); 	//Bloqueamos el mutex

		while (depositos[equipo] != 0)
			pthread_cond_wait(&cond, &mutex); /*bloqueo*/

		llenar_depósito_surtidor(M,equipo);

		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mutex);	//Desbloqueamos el mutex
	}
}