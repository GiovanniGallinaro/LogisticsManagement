#ifndef LIST_H
#define LIST_H

/**
 * @brief Struttura nodo della lista degli ordini
 */ 
struct node
{	char* codice;
	char* giorno;
	char* destinazione;
	char* bancali;
	
	int orario_partenza;
	int orario_arrivo;
	struct node *next;
};

/**
 * @brief Struttura dati nodo della lista dei giorni della settimana
 */
struct day
{	struct node *head_list;	/**< Camion#head_list Riferimento alla lista degli ordini del giorno corrispondente */
	int giorno;			/**< Camion#giorno Giorno della settimana */
	int ordini_evasi;		/**< Camion#ordini_evasi Ordini evasi in giornata da tutti camion */
	
	struct day *next;
};

/**
 * @brief Inizializza un nuovo nodo ordine con i parametri desiderati
 *
 * @details L'orario di partenza e di arrivo saranno settati di default alle 4 del mattino
 *
 * @param codice codice dell'ordine
 * @param giorno giorno in cui evadere l'ordine
 * @param destinazione destinazione dell'ordine
 * @param bancali bancali richiesti
 * @return puntatore al nuovo nodo creato
 */
struct node* set_new_node(char* codice, char* giorno, char* destinazione, char* bancali);

/**
 * @brief Aggiunge un nuovo nodo alla fine della lista degli ordini (non considera gli orari di partenza e arrivo)
 *
 * @param codice codice dell'ordine
 * @param giorno giorno in cui evadere l'ordine
 * @param destinazione destinazione dell'ordine
 * @param bancali bancali richiesti
 * @return puntatore alla head della lista
 */
struct node* add_last(struct node* head, char* codice, char* giorno, char* destinazione, char* bancali);

/**
 * @brief Aggiunge un nuovo nodo alla fine della lista degli ordini (considera gli orari di partenza e arrivo)
 *
 * @param codice codice dell'ordine
 * @param giorno giorno in cui evadere l'ordine
 * @param destinazione destinazione dell'ordine
 * @param bancali bancali richiesti
 * @param orario_partenza orario di partenza del camion
 * @param orario_arrivo orario arrivo a destinazione
 * @return puntatore alla head della lista
 */
struct node* add_last_orario(struct node* head, char* codice, char* giorno, char* destinazione, char* bancali, int orario_partenza, int orario_arrivo);

/**
 * @brief Stampa un nodo ordine
 *
 * @param n nodo ordine da stampare
 */
void print_node(struct node* n);

/**
 * @brief Stampa la lista degli ordini
 *
 * @param head head della lista da stampare
 */
void print_list(struct node* head);

/**
 * @brief Conta gli ordini presenti nella lista
 *
 * @param head head della lista
 * @return numero di ordini
 */
int list_length(struct node* head);

/**
 * @brief Elimina un ordine dalla lista degli ordini
 *
 * @param head head della lista
 * @param codice codice dell'ordine da eliminare
 * @return nuova head della lista
 */
struct node* delete_element(struct node* head, char* codice);

/**
 * @brief Elimina la lista di ordini
 *
 * @param head head della lista
 * @return puntatore alla nuova head della lista
 */
struct node* clear_list(struct node* head);

/**
 * @brief Libera la memoria di un singolo nodo
 *
 * @param n nodo da eliminare
 */
void free_node(struct node* n);

/**
 * @brief Inserisce in una lista di ordini tutti gli ordini letti da file
 *
 * @param head head della lista
 * @param fp file da cui leggere gli ordini
 * @return nuova head della lista
 */
struct node* file_reader(struct node* head, FILE *fp);

/**
 * @brief Organizza gli ordini per giorni
 *
 * @param head_day head della lista dei giorni
 * @param head della lista degli ordini
 * @return nuova head della lista dei giorni
 */
struct day* build_week(struct day* head_day, struct node* head);

/**
 * @brief Inizializza un nodo giorno
 *
 * @param head_list lista degli ordini da collegare al giorno
 * @param giorno giorno da assegnare al nodo
 * @return nuovo nodo giorno
 */
struct day* set_new_day(struct node *head_list, int giorno);

/**
 * @brief Aggiunge un nuovo giorno alla lista dei giorni
 *
 * @param head head della lista dei giorni
 * @param head_list head della lista degli ordini da collegare al giorno
 * @param giorno giorno della settimana da assegnare
 * @return nuova head della lista dei giorni
 */
struct day* add_last_day(struct day* head, struct node *head_list, int giorno);

/**
 * @brief Stampa un singolo giorno
 *
 * @param n nodo giorno da stampare
 */
void print_day(struct day* n);

/**
 * @brief Stampa la lista dei giorni
 *
 * @param head head della lista da stampare
 */
void print_week(struct day* head);

/**
 * @brief Elimina la lista dei giorni
 *
 * @param head head della lista da eliminare
 * @return puntatore alla nuova head
 */
struct day* clear_week(struct day* head);

/**
 * @brief Libera la memoria per un singolo nodo giorno
 *
 * @param n nodo giorno da eliminare
 */
void free_day(struct day* n);

/**
 * @brief Controlla se una lista di ordini è vuota
 *
 * @param head_list head della lista
 * @return 0 se la lista è vuota, 1 se la lista non è vuota
 */
int is_empty (struct node* head_list);

/**
 * @brief Fonde due liste di ordini in un'unica lista
 *
 * @details La prima lista viene trasferita nella seconda con priorità maggiore
 *
 * @param head_day1 nodo giorno 1
 * @param head_day2 nodo giorno 2
 */
void attach (struct day* head_day1,struct day* head_day2);

#endif  // LIST_H