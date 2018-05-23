// 8 EQUIPOS DE 4 -> 32 NADADORES


int team_turn[8] = {0};
int count = 0;
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

void nadador(int id_nadador){
	begin_race(id_nadador);
	swim(id_nadador);
	notify_arrival(id_nadador);
}

void begin_race(int id_nadador){

	pthread_mutex_lock(&mutex); 	//Bloqueamos el mutex
	
	count++;						//Sumamos 1 a la cola
	
	//EQUIPO Nº id_nadador / 4
	//Turno 	id_nadadir % 5

	//Esoeramos a todos y despues al turno
	while (count != 32 || team_turn[id_nadador / 4] != id_nadador % 4)
		pthread_cond_wait(&cond, &mutex); /*bloqueo*/

	if(count == 32)					//Si estan todos despertamos a los demas
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