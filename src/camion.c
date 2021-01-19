#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "camion.h"

struct camion* build_camion_list (struct camion* head_camion)
{	//inizializzo 4 liste di ordini (una per ogni camion), che determineranno gli ordini eseguiti da un certo camion
	struct node* head1,* head2 ,* head3,* head4;
	head1 = NULL; head2 = NULL; head3 = NULL; head4 = NULL;

	//inizializzo i 4 nodi della lista dei camion
	head_camion = set_new_camion(head1, 1);
	struct camion* camion2 = set_new_camion(head2, 2);
	struct camion* camion3 = set_new_camion(head3, 3);
	struct camion* camion4 = set_new_camion(head4, 4);

	//lego i 4 nodi in modo che si formi una lista a struttura "circolare", ovvero l'ultimo nodo è collegato al primo
	head_camion->next = camion2;
	camion2->next = camion3;
	camion3->next = camion4;
	camion4->next = head_camion;
	
	//ritorna la "head" della lista, ovvero il primo camion
	return head_camion;
}

struct camion* set_new_camion(struct node *head_list, int numero)
{	//creo il nuovo nodo
	struct camion* new_node = malloc(sizeof(struct camion));
	
	//inizializzo i valori
	new_node->numero=numero;
	new_node->head_list=head_list;
	new_node->bancali_trasportati = 0;
	new_node->ore = 0;
	new_node->orario = 240;		//suppongo che tutti i camion inizino le consegne alle 04:00
	new_node->codice_flag = malloc(4*sizeof(char));
	strcpy(new_node->codice_flag, "000");	//inizialmente il codice flag avrà un valore non valido per un possibile codice di un ordine

	//ritorno il nuovo nodo
	return new_node;
}

float time_needed (char* dest)
{	FILE *fp;
	fp = fopen("dest.txt", "r");
	char* destinazione = malloc(2*sizeof(char));
	float tempo;
	while(fscanf(fp, "%s %f", destinazione, &tempo) != -1)	//finchè fscanf non ha più nulla da leggere
	{	if(strcmp(dest, destinazione) == 0)
		{	free(destinazione);
			fclose(fp);
			return tempo;						//ritorno il tempo (in ore) corrispondente alla destinazione inserita come parametro
		}
	}
	free(destinazione);
	fclose(fp);
	return 0;					//ritono 0 se la destinazione non è valida
}

struct camion* add_new_order (struct camion* n, struct day* head_day)
{	//se il codice dell'ordine è uguale al codice flag del camion vuol dire che ho già provato ad evadere l'ordine corrispondente senza successo
	if(strcmp(head_day->head_list->codice, n->codice_flag) == 0)
	{	if(head_day->next != NULL)		//caso in cui non sia domenica, ovvero ci sia la possibilità di evadere l'ordine il giorno successivo
		{	attach(head_day, head_day->next);		//sposto i rimanenti ordini per il giorno corrispondente nella lista degli ordini per il giorno successivo (con priorità maggiore): esco dal ciclo interno del main (quello che scorre i camion)
			return n;
		}
		else						//caso in cui è domenica: l'ordine slitta per la settimana successiva
		{	struct camion* head = set_new_camion(head_day->head_list, 0);	//camion "virtuale": mi serve per indicare gli ordini rimanenti non evasi
			head_day->head_list = NULL;		//svuoto la lista di domenica: esco dal ciclo interno nel main (quello che scorre i camion)
			return head;			//ritono il camion "virtuale"
		}
	}

	//inizializzo i vari parametri
	int bancali = atoi(head_day->head_list->bancali);			//converto il numero di bancali da char* a int
	float tempo = time_needed(head_day->head_list->destinazione);	//tempo per percorrere la strada per arrivare a destinazione
	if(tempo == 0)					//destinazione non valida, ignoro ordine
	{	head_day->head_list = delete_element(head_day->head_list, head_day->head_list->codice);
		return n;
	}
	int tempo_minuti = hmin_conversion (tempo);				//converto in minuti il tempo per percorrere la strada
	float ore = n->ore + 2*tempo;							//aggiorno il numero di ore in viaggio del camion (in ore)
	int orario = n->orario + tempo_minuti*2 + 40;				//aggiorno l'orario (in minuti) segnato dall'orologio sul camion (conto anche il tempo di carico e scarico)
	int orario_partenza = n->orario;						//parto all'orario corrente (prima di eseguire l'ordine)
	int orario_arrivo = orario_partenza + tempo_minuti;			//arrivo all'orario di partenza più il tempo per il tragitto di andata

	//Primo caso: camion disponibile per la consegna
	if(bancali <= 6 && ore <= 18 && orario <= 1440)		//1440 sono 24 ore in minuti
	{	//aggiorno lista di ordini evasi dal camion
		n->head_list = add_last_orario(n->head_list, head_day->head_list->codice, head_day->head_list->giorno, head_day->head_list->destinazione, head_day->head_list->bancali, orario_partenza, orario_arrivo);
		head_day->head_list = delete_element(head_day->head_list, head_day->head_list->codice);		//elimino l'ordine dalla lista di ordini da evadere nel giorno corrispondente
		n->bancali_trasportati = n->bancali_trasportati + bancali;			//aggiorno bancali
		n->ore = ore;			//aggiorno ore di viaggio
		n->orario = orario; 		//aggiorno orario
		head_day->ordini_evasi = head_day->ordini_evasi + 1;		//aggiorno numero di ordini evasi nel giorno corrispondente
		if(bancali == 6)
		{	n->ore_bancali_pieni = n->ore_bancali_pieni + tempo;		//se il camion trasporta 6 bancali, aggiorno ore di viaggio coi bancali pieni
		}
	}

	//Secondo caso: camion disponibile solo in parte
	else if(bancali > 6 && ore <= 18 && orario <= 1440)
	{	n->head_list = add_last_orario(n->head_list, head_day->head_list->codice, head_day->head_list->giorno, head_day->head_list->destinazione, "06", orario_partenza, orario_arrivo);	//effettuo ordine solo in parte
		int bancali_rimanenti = bancali-6;			//trovo il numero di bancali rimanenti in modo da delegare al prossimo camion il trasporto dei bancali rimanent
		sprintf(head_day->head_list->bancali, "%.2d", bancali_rimanenti);		//aggiorno valore di bancali da consegnare per l'ordine che verrà evaso dal prossimo camion
		//aggiorno valori camion
		n->bancali_trasportati = n->bancali_trasportati + 6;
		n->ore = ore;
		n->orario = orario; 
		n->ore_bancali_pieni = n->ore_bancali_pieni + tempo;
	}

	//Terzo caso: camion non disponibile
	else
	{	n->codice_flag = head_day->head_list->codice;		//setto il codice flag del camion al valore del codice dell'ordine che non è riuscito ad evadere
		return n->next;		//provo con il prossimo camion
	}
	return n->next;		//passo all'ordine successivo
}

void print_camion(struct camion* n)
{	if(n->bancali_trasportati != 0)
	{	char* ore_form = hhmm_format(hmin_conversion(n->ore),0);
		char* orario_form = hhmm_format(n->orario,1);
		char* ore_bancali_pieni_form =  hhmm_format(hmin_conversion(n->ore_bancali_pieni),0);
		float ore_autista = n->ore/2;		//gli autisti si danno il cambio a metà del tempo
		char* ore_autista_form = hhmm_format(hmin_conversion(ore_autista),0);
		printf("CAMION %d: %d bancali trasportati e %s ore di viaggio. Orario rientro: %s\n", n->numero, n->bancali_trasportati, ore_form, orario_form);
		printf("Ore di viaggio con i bancali pieni: %s\n", ore_bancali_pieni_form);
		printf("Ore di viaggio Autista1: %s. Ore di viaggio Autista2: %s\n\n", ore_autista_form, ore_autista_form);
		print_list(n->head_list);
		free(ore_form);
		free(orario_form);
		free(ore_bancali_pieni_form);
		free(ore_autista_form);
	}
}

void print_camion_list(struct camion* head_camion)
{	struct camion* s = head_camion;
	int i = 0;
	while(i<4)
	{	print_camion(s);
		s=s->next;
		printf("\n");
		i++;
	}
}

struct camion* clear_camion_list(struct camion* head)
{	struct camion* s = head;
	int i = 0;
	//scorro la lista e elimino tutti gli elementi
	while(i < 4)
	{	free_camion(s);
		s = s->next;
		i++;
	}
	return head;
}

void free_camion(struct camion* n)
{	clear_list(n->head_list);
	free(n->codice_flag);
	free(n);
}

int hmin_conversion (float ore)
{	int minuti = (int) (ore*60);
	return minuti;
}

char* hhmm_format(int minuti, int formato)
{	float ore = (float) minuti/60;
	int hh = (int) ore;
	int minutes = minuti%60;
	char* hhmm;
	if(formato == 0)		//selezione del formato desiderato
	{	hhmm = malloc(16*sizeof(char));
		sprintf(hhmm, "%d ore e %d min", hh, minutes);
	}
	else
	{	hhmm = malloc(7*sizeof(char));	
		sprintf(hhmm, "%.2d:%.2d", hh, minutes);
	}
	return hhmm;
}