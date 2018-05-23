// 4 EQUIPOS DE 4 -> 16 CORREDORES
#define N 			16

//Equipo nº 	id_corredor / 4
//Turno 		id_corredor % 4

int team_turn[8] = {0};		//Relevo de los equipos
int count = 0;				//Contador para esperar a todos
int turno_llegada = 0; ; 	//Podriamos usar el mismo count y reiniciarlo en 
							//el if(count == N) a 0 la pongo para que se vea mas claro

pthread_mutex_t mutex;
pthread_cond_t cond;

int main() {
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&cond, NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}

void corredor(int id_corredor){
	start_race(id_corredor);
	run(id_corredor);
	notify_arrival(id_corredor);
}

void start_race(int id_corredor){

	pthread_mutex_lock(&mutex); 	//Bloqueamos el mutex
	
	count++;						//Sumamos 1 a la cola
	
	//Equipo nº 	id_corredor / 4
	//Turno 		id_corredor % 4

	//Esperamos a todos y despues al turno
	while (count != N || team_turn[id_corredor / 4] != (id_corredor % 4))
		pthread_cond_wait(&cond, &mutex); /*bloqueo*/

	if(count == N)					//Si estan todos despertamos a los demas
		pthread_cond_broadcast(&cond);

	pthread_mutex_unlock(&mutex);	//Desbloqueamos el mutex
}

void notify_arrival(int id_nadador){
	
	pthread_mutex_lock(&mutex);		//Bloqueamos el mutex

	if((id_nadador % 4) == 3)		//Si somos el ultimo hemos ganado
	{
		turno_llegada++;
		printf ("El equipo %i ha llegado en posicion %i\n", id_nadador / 4, turno_llegada); //++turno_llegada?
	}
	else							//Sino pasamos el relevo y despertamos a los demas
	{
		team_turn[id_nadador / 4]++;
		pthread_cond_broadcast(&cond);
	}

	pthread_mutex_unlock(&mutex);
}