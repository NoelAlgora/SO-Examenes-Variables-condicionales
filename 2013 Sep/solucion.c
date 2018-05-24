pthread_mutex_t mutex;
pthread_cond_t cond;

int count_C = 0;
int count_O = 0;

int main() {
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&cond, NULL);

	while(1) {
		n=random();
		if(n%2 == 0)	//Si n es par
			crear hilo Carbono
		else			//Si n impar
			crear hilo Oxígeno
	}

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}

/*
	N.B No es necesario garantizar que únicamente dos oxígenos y un carbono ejecutan
	concurrentemente la función FormarMolecula(). Sólo hay que resolver el ejercicio de
	señalización cruzada para que esperen si no hay recursos suficientes.
*/

void Carbono() {
	pthread_mutex_lock(&mutex); 	//Bloqueamos el mutex

	count_C++;

	while(count_O < 2)
		pthread_cond_wait(&cond, &mutex); /*bloqueo*/

	pthread_cond_broadcast(&cond);	//Despertar hilos… Hay que despertar solo 2 Oxigenos? Habria que usar 2 variables condicionales
	pthread_mutex_unlock(&mutex);	//Desbloqueamos el mutex los tres tienen que ejecutar FormarMolecula a la vez

	FormarMolecula();
}


void Oxígeno() {
	pthread_mutex_lock(&mutex); 	//Bloqueamos el mutex

	count_O++;

	while(count_O < 2 && count_C < 1)
		pthread_cond_wait(&cond, &mutex); /*bloqueo*/

	pthread_cond_broadcast(&cond);	//Despertar hilos…
	pthread_mutex_unlock(&mutex);	//Desbloqueamos el mutex los tres tienen que ejecutar FormarMolecula a la vez

	FormarMolecula();
}