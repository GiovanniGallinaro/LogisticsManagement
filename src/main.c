#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "camion.h"

int main(int argc, char* argv[])
{	FILE *fp;

	if(argc != 2)
	{	printf("usage: main file_name\n");
		exit(EXIT_FAILURE);
	}

	fp=fopen(argv[1], "r");

	if(fp == NULL)
	{	printf("Can't open %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	struct node* head;	//inizializzo la lista degli ordini
	head=NULL;

	struct day* head_day;	//inizializzo la lista dei giorni
	head_day=NULL;

	head=file_reader(head, fp);					//leggo il file e creo la lista degli ordini

	fclose(fp);

	head_day = build_week(head_day, head);			//costruisco la lista degli ordini organizzata 
	print_week(head_day);						//per giorni della settimana

	struct camion* head_camion;					//inizializzo la head per la lista di camion (non avrò nessun riferimento a NULL alla fine)
	struct camion* s1, *s2, *s3, *s4, *s5, *s6, *s7;
	int i = 1;
	struct day* c = head_day;

	while(c != NULL)									//scorro i vari giorni
	{	printf("*************GIORNO %d*************\n\n", c->giorno);
		head_camion = build_camion_list(head_camion);			//costruisco una nuova lista di 4 camion per il giorno corrente
		struct camion* s = head_camion;
		while((is_empty(c->head_list)) != 0)				//finchè la lista degli ordini da evadere per il giorno corrente non è vuota...
		{	s = add_new_order (s, c);					//evadi nuovo ordine
		}
		print_camion_list(head_camion);					//stampo lista ordini evasi dai camion corrispondenti

		if(i == 1)		s1 = head_camion;					//assegno dei puntatori alle varie liste di camion create per liberare la memoria
		else if(i == 2)	s2 = head_camion;
		else if(i == 3)	s3 = head_camion;
		else if(i == 4)	s4 = head_camion;
		else if(i == 5)	s5 = head_camion;
		else if(i == 6)	s6 = head_camion;
		else if(i == 7)	s7 = head_camion;

		int count = c->ordini_evasi;						//ordini evasi oggi
		printf("Numero di ordini evasi oggi: %d\n\n", count);

		if(s->numero == 0)							//se s è il camion "virtuale", stampo gli ordini non evasi
			{	printf("-----------------------------------\n\nOrdini non evasi:\n");
				print_list(s->head_list);
				free_camion(s);
			}

		c = c->next;
		i++;
	}

	s1 = clear_camion_list(s1);
	s2 = clear_camion_list(s2);
	s3 = clear_camion_list(s3);
	s4 = clear_camion_list(s4);
	s5 = clear_camion_list(s5);
	s6 = clear_camion_list(s6);
	s7 = clear_camion_list(s7);
	head_day = clear_week(head_day);

	return 0;
}