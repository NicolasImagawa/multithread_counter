/* Data de Submissao:
 * Nome: Nicolas de Sousa Imagawa
 * RA: 204147
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>


//Variaveis globais para as threads

//Contador de threads ativas no programa.
int thread_counter = 0;

//Contador para o loop for da funcao main, que permite acessar tambem o vetor de numeros a ser analisados.
int main_counter = 0;

//Contador de numeros primos.
int n_primes = 0;

//Trava para a sincronizacao de threads.
pthread_mutex_t lock;

//Vetor para escrever os numeros de entrada apos a conversao em long int.
long int input_num[1000];


//Chamada da thread, nela verificamos se um dado numero do vetor input_num é primo.
void* thread_function(void* arg)
{	
    //Counter eh o contador do loop
    int counter;

    //mult serve para detectar quantos multiplos a entrada tem
    long int mult = 0;

    //Travamento da thread criada.
    pthread_mutex_lock(&lock);

    //Calcula-se o resto da entrada por todos os numeros ate ele.
    for(counter = 1; counter <= input_num[main_counter]; counter++)
    {	 
	//Em caso do resto ser 0, mult eh incrementado.	
            if((input_num[main_counter] % counter) == 0)
            {
            	 mult = mult + 1;
            }
    }

    //Se o resto foi 0 mais de duas vezes, o numero nao eh primo.
    if(mult == 2)
    {
	n_primes = n_primes + 1;
    }
    
    //Para continuar a criacao de threads, decrementamos o contador. 
    if(thread_counter >= 4)
    {
	thread_counter = thread_counter - 1;
    }
	
    //Entao, permite-se a execucao de outra thread.
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(void)
{

	//Variaveis para o vetor de ints usado na conversao da string para int,tamanho da string e cursor que percorre a string, respectivamente.
	long int length = 0, cursor;
	
	//Character se refere ao valor a ser escrito como long int no vetor input_num, ou seja, para ser processado como numero.
	long int character;
	//Input eh a string de entrada inicial
	char input[1000];

	//Le-se a string de entrada ate um fim de linha.
	fgets(input, sizeof(input), stdin);

	//Como não podemos incrementar a string de entrada em si, craimos um ponteiro para a string.
	char* char_input = input;

	//int counter = 0;
	//Error retorna se a thread foi criada com sucesso, 0 neste caso.
	int error;

	//int random = 0;
	//int mult1 = 0;

	// Vetor que guarda os IDs das threads geradas. O tamanho do vetor eh 4 por causa do numero maximo de threads que podem estar ativas ao mesmo tempo.
	pthread_t tid[4];

	//Loop responsavel por ler a string de entrada e converte-la em long ints. cada posicao do vetor de long ints recebe um valor percorrido pelo incremento de char_input, sendo esse carregado pelo %ln vinculado a cursor.
	while (sscanf(char_input, "%ld%ln", &character, &cursor) > 0)
	{
    		input_num[length++] = character;
    		char_input += cursor;
	}

	//Loop para percorrer o vetor de long ints.
	for(main_counter = 0; main_counter < length; main_counter++)
	{		
		//Se ainda nao ha 4 threads em execucao, criamos uma nova.
		if(thread_counter <= 4)
		{
			error = pthread_create(&(tid[thread_counter]), NULL, &thread_function, NULL);
		}

		//Esperamos o processo criado terminar.

		if(thread_counter <= 4)
		{
			pthread_join((tid[thread_counter]), NULL);
		}
		
		thread_counter = thread_counter + 1;
	}
	//Mostramos o numero de primos na string de entrada.
	printf("%d\n", n_primes);
	return 0;
}
