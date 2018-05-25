#define REPONER_P		5
#define REPONER_B		5

pthread_mutex_t mutex;
pthread_cond_t cond;

int count_B = 0;
int count_P = 0;

int main() {
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&cond, NULL);

	//SE CREAN LOS HILOS Y EJECUCION DEL PROGRAMA

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}

Bocadillos() {
	pthread_mutex_lock(&mutex); 		//Bloqueamos el mutex

	while (1) {
		while(count_B != 0)					//Espera a que un alumno le avise
			pthread_cond_wait(&cond, &mutex); /*bloqueo*/
			
		count_B = REPONER_B;				//Repone P bocadillos
		
		pthread_cond_broadcast(&cond);	//Despertamos a todos
	}

	pthread_mutex_unlock(&mutex);		//Desbloqueamos el mutex
}

Bebidas() {
	pthread_mutex_lock(&mutex); 		//Bloqueamos el mutex

	while (1) {
		while(count_P != 0)					//Espera a que un alumno le avise
			pthread_cond_wait(&cond, &mutex); /*bloqueo*/

		/* Espera a que un alumno le avise para reponer? Podemos bloquearnos a la espera 
		de una llamada simplemente usamos un do while

		do
		{
			pthread_cond_wait(&cond, &mutex);
		} while(count_P != 0)
		*/

		count_P = REPONER_P;				//Repone B bebidas
		
		pthread_cond_broadcast(&cond);	//Despertamos a todos
	}

	pthread_mutex_unlock(&mutex);		//Desbloqueamos el mutex
}

void Alumno() {
	while (1) {
		pthread_mutex_lock(&mutex); 		//Bloqueamos el mutex

		while(count_P == 0)					// Si no hay bocadillos, avisa a cocinero (y espera)
		{
			pthread_cond_broadcast(&cond);	//Despertamos a todos para buscar al reponedor... Despertar solo al de los bocadillos? Dos variables condicionales
			pthread_cond_wait(&cond, &mutex); /*bloqueo*/
		}
		count_P--; 							// Coge un bocadillo
		
		
		while(count_B == 0)					// Si no hay bebidas, avisa a cocinero (y espera)
		{
			pthread_cond_broadcast(&cond);	//Despertamos a todos para buscar al reponedor... 
			pthread_cond_wait(&cond, &mutex); /*bloqueo*/
		}
		count_B--;							// Coge una bebida

		pthread_mutex_unlock(&mutex);		//Desbloqueamos el mutex

		comer();
	}
}