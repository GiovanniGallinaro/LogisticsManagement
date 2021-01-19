/**
 *	@brief Struttura dati camion
 */
struct camion
{	struct node* head_list;	 /**< Camion#head_list Riferimento alla lista degli ordini evasi dal camion */
	int bancali_trasportati; /**< Camion#bancali_trasportati bancali trasportati dal camion */
	float ore;			/**< Camion#ore Ore di viaggio */
	int orario;			/**< Camion#orario Orologio nel camion */
	int numero;			/**< Camion#numero Numero identificativo del camion */
	char* codice_flag;	 /**< Camion#codice_flag Memorizza il codice di un ordine che non è riuscito ad evadere */
	float ore_bancali_pieni; /**< Camion#ore_bancali_pieni Ore di viaggio coi bancali pieni */

	struct camion *next;	/**< Camion#next Riferimento al camion successivo */
};

/**
 * @brief Costruisce una lista di 4 nodi di tipo camion
 *
 * @details L'ultimo nodo non avrà un rifermento a NULL come next, bensì sarà collegato al primo nodo
 *
 * @param head_camion head
 * @return puntatore all' head
 *
 */
struct camion* build_camion_list (struct camion* head_camion);

/**
 * @brief Inizializza un nuovo nodo camion con i parametri di default
 *
 * @param head_list head della lista degli ordini da collegare
 * @param numero numero del camion assegnato
 * @return puntatore al nodo camion
 */
struct camion* set_new_camion(struct node *head_list, int numero);

/**
 * @brief Calcola il tempo necessario per arrivare a una certa destinazione
 *
 * @param dest destinazione indicata
 * @return tempo in ore per raggiungere la destinazione
 */
float time_needed (char* dest);

/**
 * @brief Aggiunge (se possibile) un nuovo ordine al camion indicato
 *
 * @param n nodo camion a cui si vuole delegare l'ordine
 * @param head_day puntatore alla head della lista degli ordini della settimana
 * @return puntatore al prossimo nodo camion
 */
struct camion* add_new_order (struct camion* n, struct day* head_day);

/**
 * @brief Stampa un nodo camion con i suoi parametri
 *
 * @param n camion da stampare
 */
void print_camion(struct camion* n);

/**
 * @brief Stampa l'intera lista di camion
 *
 * @param head_camion head della lista camion
 */
void print_camion_list(struct camion* head_camion);

/**
 * @brief Elimina la lista di camion
 *
 * @param head head della lista
 * @return puntatore alla nuova head
 */
struct camion* clear_camion_list(struct camion* head);

/**
 * @brief Libera la memoria occupata da un camion
 *
 * @param n camion da liberare
 */
void free_camion(struct camion* n);

/**
 * @brief Converte ore in minuti
 *
 * @param ore ore da convertire
 * @return ore in minuti
 */
int hmin_conversion (float ore);

/**
 * @brief Formatta minuti
 *
 * @param minuti minuti da formattare
 * @param formato 0 o 1 a seconda del formato desiderato
 * @return stringa formattata
 */
char* hhmm_format(int minuti, int formato);