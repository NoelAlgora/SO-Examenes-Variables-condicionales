/***************

Solucion con doble variable condicional para separar bebidas y bocadillos

****************/

#define REPONER_P		5
#define REPONER_B		5

pthread_mutex_t mutex;
pthread_cond_t cond1;
pthread_cond_t cond2;

int count_B = 0;
int count_P = 0;

int main() {
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&cond1, NULL);
	pthread_cond_init (&cond2, NULL);

	//SE CREAN LOS HILOS Y EJECUCION DEL PROGRAMA

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond1);
	pthread_cond_destroy(&cond2);
}

// Separamos los bocadillos en cond1 y las bebidas en cond2 de esta forma podemos invocar
// a los reponedores de forma independiente, pero seguimos usando el mismo mutex con las
// dos variables condicionales para asegurar la exclusion

Bocadillos() {
	pthread_mutex_lock(&mutex); 		//Bloqueamos el mutex

	while (1) {
		do 								//Esta vez vamos a esperar que nos avise sino no reponemos
		{
			pthread_cond_wait(&cond1, &mutex);
		} while(count_B != 0)

		count_B += REPONER_B;				//Repone P bocadillos
		
		pthread_cond_broadcast(&cond1);		//Despertamos a todos
	}

	pthread_mutex_unlock(&mutex);			//Desbloqueamos el mutex
}

Bebidas() {
	pthread_mutex_lock(&mutex); 		//Bloqueamos el mutex

	while (1) {
		do 								//Esta vez vamos a esperar que nos avise sino no reponemos
		{
			pthread_cond_wait(&cond2, &mutex);
		} while(count_P != 0)

		count_P += REPONER_P;				//Repone B bebidas
		
		pthread_cond_broadcast(&cond2);	//Despertamos a todos
	}

	pthread_mutex_unlock(&mutex);		//Desbloqueamos el mutex
}

void Alumno() {
	while (1) {
		pthread_mutex_lock(&mutex); 		//Bloqueamos el mutex

		while(count_P == 0)					// Si no hay bocadillos, avisa a cocinero (y espera)
		{
			pthread_cond_broadcast(&cond1);	//Despertamos a los reponedores de bocadillos
			pthread_cond_wait(&cond1, &mutex); /*bloqueo*/
		}
		count_P--; 							// Coge un bocadillo
		
		
		while(count_B == 0)					// Si no hay bebidas, avisa a cocinero (y espera)
		{
			pthread_cond_broadcast(&cond2);	//Despertamos a los reponedores de bebidas
			pthread_cond_wait(&cond2, &mutex); /*bloqueo*/
		}
		count_B--;							// Coge una bebida

		pthread_mutex_unlock(&mutex);		//Desbloqueamos el mutex

		comer();
	}
}