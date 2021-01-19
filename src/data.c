#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "camion.h"

struct node* set_new_node(char* codice, char* giorno, char* destinazione, char* bancali)
{	int codice_length, giorno_length, destinazione_length, bancali_length;
	
	//valuto la lunghezza delle stringhe
	codice_length = strlen(codice);
	giorno_length = strlen(giorno);
	destinazione_length = strlen(destinazione);
	bancali_length = strlen(bancali);
	
	//creo un nuovo nodo
	struct node* new_node = malloc(sizeof(struct node));
	
	//alloco memoria per i nuovi paramentri
	new_node->codice = malloc(codice_length + 1);
	new_node->giorno = malloc(giorno_length + 1);
	new_node->destinazione = malloc(destinazione_length + 1);
	new_node->bancali = malloc(bancali_length + 1);
	//inizializzo gli orari di partenza di default alle 04:00 (240 minuti...)
	new_node->orario_partenza = 240;
	new_node->orario_arrivo = 240;
	
	//inserisco i paramentri della funzione nei dati della struttura
	strcpy(new_node->codice, codice);
	strcpy(new_node->giorno, giorno);
	strcpy(new_node->destinazione, destinazione);
	strcpy(new_node->bancali, bancali);

	return new_node;
}

struct node* add_last(struct node* head, char* codice, char* giorno, char* destinazione, char* bancali)
{	//se la lista è vuota, aggiungo il primo elemento
	if(head==NULL)
	{	//inizializzo il nodo da aggiungere
		struct node* new_node = set_new_node (codice, giorno, destinazione, bancali);

		//connetto il nuovo elemento con la vecchia head della lista
		new_node->next = head;
		
		//ritorno il nuovo nodo, che sarà la nuova head
		return new_node;
	}

	//inizializzo il nodo da aggiungere
	struct node* new_node = set_new_node (codice, giorno, destinazione, bancali);

	//scorro la lista fino ad arrivare all'ultimo elemento
	struct node* s = head;
	while(s->next!=NULL)
	{	s=s->next;
	}

	//lego l'ultimo elemento all'elemento da aggiungere
	s->next=new_node;
	new_node->next=NULL;

	return head;
}

struct node* add_last_orario(struct node* head, char* codice, char* giorno, char* destinazione, char* bancali, int orario_partenza, int orario_arrivo)
{	//se la lista è vuota, aggiungo il primo elemento
	if(head==NULL)
	{	//inizializzo il nodo da aggiungere
		struct node* new_node = set_new_node (codice, giorno, destinazione, bancali);
		//connetto il nuovo elemento con la vecchia head della lista
		new_node->next = head;		
		//ritorno il nuovo nodo, che sarà la nuova head
		new_node->orario_partenza = orario_partenza;					//aggiorno orario partenza
		new_node->orario_arrivo = orario_arrivo;					//aggiorno orario arrivo
		return new_node;
	}

	//inizializzo il nodo da aggiungere
	struct node* new_node = set_new_node (codice, giorno, destinazione, bancali);
	new_node->orario_partenza = orario_partenza;
	new_node->orario_arrivo = orario_arrivo;

	//scorro la lista fino ad arrivare all'ultimo elemento
	struct node* s = head;
	while(s->next!=NULL)
	{	s=s->next;
	}

	//lego l'ultimo elemento all'elemento da aggiungere
	s->next=new_node;
	new_node->next=NULL;

	return head;
}

void print_node(struct node* n)
{	//se l'ordine non è stato evaso non stampo gli orari di partenza e arrivo (non hanno significato)
	if(n->orario_partenza == n->orario_arrivo)
	{	printf("%s%s%s%s", n->codice, n->giorno, n->destinazione, n->bancali);
	}
	//se l'ordine è stato evaso gli orari di partenza e arrivo vengono aggiornati dalla funzione add_new_order
	else
	{	printf("%s%s%s%s	Orario partenza: %s. Orario arrivo: %s", n->codice, n->giorno, n->destinazione, n->bancali, hhmm_format(n->orario_partenza,1), hhmm_format(n->orario_arrivo,1));
	}
}

void print_list(struct node* head)
{	struct node* s = head;
	while(s!=NULL)
	{	print_node(s);
		printf("\n");
		s=s->next;
	}
}

int list_length(struct node* head)
{	struct node* s = head;
	int count=0;
	//scorro la lista e incremento il contatore
	while(s!=NULL)
	{	count++;
		s=s->next;
	}

	return count;
}

struct node* delete_element(struct node* head, char* codice)
{	//puntaore per scorrere la lista
	struct node* s = head;
	
	if(head == NULL)
	{	return head;
	}

	//se il primo elemento è quello da eliminare, aggiorno la head della lista
	if(s != NULL && strcmp(s->codice, codice)==0)
	{	//inizializzo un puntatore a head per poter liberare la memoria allocata in precedenza
		struct node* p = head;
		head = s->next;
		free_node(p);
		return head;
	}

	//se l'elemento da eliminare non è la head
	struct node* n = s->next;
	while(n != NULL)
	{	if(strcmp(s->codice, codice)==0)
		{	struct node* p;
			p = n;
			//lego l'elemento precedente a quello da eliminare con il suo successivo
			s->next = n->next;
			free_node(p);
			return head;
		}
		s=n;
		n=n->next;
	}

	//se l'elemento da eliminare è l'ultimo elemento della lista
	if(strcmp(s->codice, codice)==0)
	{	struct node* p = s;
		//libero la memoria precedentemente allocata
		free_node(p);
		return head;
	}
	
	return head;
}

struct node* clear_list(struct node* head)
{	if(head == NULL)
	{	return head;
	}
	struct node* s = head;
	struct node* n = s->next;
	//scorro la lista e elimino tutti gli elementi
	while(n != NULL)
	{	free_node(s);
		s=n;
		n=n->next;
	}
	//elimino l'ultimo elemento
	free_node(s);

	return head;
}

void free_node(struct node* n)
{	free(n->codice);
	free(n->giorno);
	free(n->destinazione);
	free(n->bancali);
	free(n);
}

struct node* file_reader(struct node* head, FILE *fp)
{	//alloco memoria per i parametri da leggere dal file di input
	char* codice = malloc(3*sizeof(char)),* giorno = malloc(2*sizeof(char)),* destinazione = malloc(2*sizeof(char)),* bancali = malloc(3*sizeof(char));
	while(fscanf(fp, "%3s%2s%2s%3s", codice, giorno, destinazione, bancali) != -1)		//finchè fscanf trova una riga vuota
	{	head = add_last (head, codice, giorno, destinazione, bancali);				//aggiungi per ultimo l'ordine (priorità minore)
	}
	free(codice);
	free(giorno);
	free(destinazione);
	free(bancali);
	return head;				//restituisce la testa alla lista degli ordini
}

struct day* build_week(struct day* head_day, struct node* head)
{	//creo 7 liste di ordini (una per ogni giorno della settimana)
	struct node* head1,* head2,* head3,* head4,* head5,* head6,* head7;
	head1 = NULL; head2 = NULL; head3 = NULL; head4 = NULL; head5 = NULL; head6 = NULL; head7 = NULL;

	//scorro tutta la lista degli ordini letta con file_reader: ad ogni iterazione trasferisco l'ordine nella lista degli ordini del giorno corrispondente
	struct node* s = head;
	while(s != NULL)
	{	if(strcmp(s->giorno, "Lu") == 0)
		{	head1 = add_last (head1, s->codice, s->giorno, s->destinazione, s->bancali);
			s = delete_element (s, s->codice);		//elimino l'ordine sulla prima lista
		}
		else if(strcmp(s->giorno, "Ma") == 0)
		{	head2 = add_last (head2, s->codice, s->giorno, s->destinazione, s->bancali);
			s = delete_element (s, s->codice);
		}
		else if(strcmp(s->giorno, "Me") == 0)
		{	head3 = add_last (head3, s->codice, s->giorno, s->destinazione, s->bancali);
			s = delete_element (s, s->codice);
		}
		else if(strcmp(s->giorno, "Gi") == 0)
		{	head4 = add_last (head4, s->codice, s->giorno, s->destinazione, s->bancali);
			s = delete_element (s, s->codice);
		}
		else if(strcmp(s->giorno, "Ve") == 0)
		{	head5 = add_last (head5, s->codice, s->giorno, s->destinazione, s->bancali);
			s = delete_element (s, s->codice);
		}
		else if(strcmp(s->giorno, "Sa") == 0)
		{	head6 = add_last (head6, s->codice, s->giorno, s->destinazione, s->bancali);
			s = delete_element (s, s->codice);
		}
		else if(strcmp(s->giorno, "Do") == 0)
		{	head7 = add_last (head7, s->codice, s->giorno, s->destinazione, s->bancali);
			s = delete_element (s, s->codice);
		}
		else
		{	s = delete_element (s, s->codice);		//ordine non valido
		}
	}
	
	//creo la lista della settimana la quale avrà, per ogni nodo, un riferimento alla lista degli ordini da evadere del giorno corrispondente
	head_day = add_last_day(head_day, head1, 1);
	head_day = add_last_day(head_day, head2, 2);
	head_day = add_last_day(head_day, head3, 3);
	head_day = add_last_day(head_day, head4, 4);
	head_day = add_last_day(head_day, head5, 5);
	head_day = add_last_day(head_day, head6, 6);
	head_day = add_last_day(head_day, head7, 7);
	return head_day;
}

struct day* set_new_day(struct node *head_list, int giorno)
{	//creo il nuovo nodo
	struct day* new_node = malloc(sizeof(struct day));
	
	//aggiorno i suoi parametri
	new_node->giorno=giorno;
	new_node->head_list=head_list;		//riferimento alla lista degli ordini del giorno corrispondente
	new_node->ordini_evasi = 0;			//inizialmente 0 ordini evasi al giorno

	return new_node;
}

struct day* add_last_day(struct day* head, struct node *head_list, int giorno)
{	//se la lista è vuota, aggiungo il primo elemento
	if(head==NULL)
	{	//inizializzo il nodo da aggiungere
		struct day* new_node = set_new_day (head_list, giorno);

		//connetto il nuovo elemento con la vecchia head della lista
		new_node->next = head;
		
		//ritorno il nuovo nodo, che sarà la nuova head
		return new_node;
	}

	//inizializzo il nodo da aggiungere
	struct day* new_node = set_new_day (head_list, giorno);

	//scorro la lista fino ad arrivare all'ultimo elemento
	struct day* s = head;
	while(s->next!=NULL)
	{	s=s->next;
	}

	//lego l'ultimo elemento all'elemento da aggiungere
	s->next=new_node;	
	new_node->next=NULL;

	return head;
}

void print_day(struct day* n)
{	if(n->head_list != NULL)
	{	printf("GIORNO %d:\n", n->giorno);
		print_list(n->head_list);			//stampo la lista degli ordini per quel giorno
	}
}

void print_week(struct day* head)
{	struct day* s = head;
	while(s != NULL)
	{	print_day(s);					//stampo tutti i giorni
		s=s->next;
		printf("\n");
	}
}

struct day* clear_week(struct day* head)
{	struct day* s = head;
	struct day* n = s->next;
	//scorro la lista e elimino tutti gli elementi
	while(n != NULL)
	{	free_day(s);
		s=n;
		n=n->next;
	}
	//elimino l'ultimo elemento
	free_day(s);

	return head;
}

void free_day(struct day* n)
{	clear_list(n->head_list);
	free(n);
}

int is_empty (struct node* head_list)
{	if(head_list == NULL)				//la lista degli ordini è vuota se la head è NULL
		return 0;					
	return 1;						//ritorna un valore positivo se non è vuota
}

void attach (struct day* head_day1,struct day* head_day2)
{	struct node* s = head_day1->head_list;		
	while(s->next != NULL)
	{	s = s->next;				//scorro i nodi della prima lista di ordini per arrivare all'ultimo nodo valido
	}
	s->next = head_day2->head_list;		//lego l'ultimo nodo della prima lista di ordini al primo nodo della seconda lista di ordini
	head_day2->head_list = head_day1->head_list;		//la head della lista di ordini per il secondo giorno sarà ora la head della precedente lista di ordini del primo giorno
	head_day1->head_list = NULL;			//la lista di ordini per il primo giorno sarà ora vuota, in quanto non si riescono ad evadere altri ordini in quel giorno
}