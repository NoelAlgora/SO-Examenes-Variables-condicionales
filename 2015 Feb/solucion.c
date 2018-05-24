#define TIPOS_PIZZA		2
#define REPONER_PIZZAS	5

int pizza[TIPOS_PIZZA] = {0};

pthread_mutex_t mutex;
pthread_cond_t cond;

int main() {
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&cond, NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}

/* Tipo pizza = (0) peperoni, (1) margarita */

void Cliente(int tipoPizza){
	while (true) {
		conseguirPizza(tipoPizza);
		comer();
	}
}

void Camarero(){
	while (true) {
		servirPizzas(); 
	}
}

void conseguirPizza(int tipoPizza){
	pthread_mutex_lock(&mutex); 	//Bloqueamos el mutex

	while(pizza[tipoPizza] == 0)	//Si no queda de nuestra pizza despertamos a todos para buscar al camarero y esperamos
	{
		pthread_cond_broadcast(&cond);
		pthread_cond_wait(&cond, &mutex); /*bloqueo*/
	}

	retirarPizzaBuffet(tipoPizza); 	// Suponemos que hace pizza[tipoPizza]--;

	pthread_mutex_unlock(&mutex);	//Desbloqueamos el mutex
}

void servirPizzas(){
	pthread_mutex_lock(&mutex); 	//Bloqueamos el mutex

	while (pizza[0] != 0 && pizza[1] != 0)	//Mientras hay pizzas esperamos
		pthread_cond_wait(&cond, &mutex); /*bloqueo*/

	if(pizza[0] == 0)				//Reponemos
		reponerPizzas(REPONER_PIZZAS, 0);
	if(pizza[1] == 0)
		reponerPizzas(REPONER_PIZZAS, 1);

	pthread_cond_broadcast(&cond);	//Despertamos a todos los clientes
	pthread_mutex_unlock(&mutex);	//Desbloqueamos el mutex
}