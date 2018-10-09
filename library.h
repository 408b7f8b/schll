/*
 * Schluesselliste
 * Version 0.1
 */

#ifndef MAP_LIBRARY_H
#define MAP_LIBRARY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Typen
 */
typedef enum schll_rueckgabe {
	IO, SCHL_VORHANDEN, MAP_NULL, EINTRAG_NIO, ALLOK_FEHLG, NAME_FEHLT, KEIN_EINTRAG, KEINE_EINTRAEGE
} schll_rueckgabe;

typedef struct schll_eintrag {
	char* name;
	unsigned int name_l;
	void* wert;

	struct schll_eintrag* n_eintrag;
} schll_eintrag;

typedef struct schll_liste {
	struct schll_eintrag* starteintrag;
	unsigned int eintraege_l;
} schll_liste;

/*
 * Deklarationen
 */

/*
 * Konstruieren mit direkter Rückgabe, alloziert eine schll_liste
 * Muss aufgelöst werden
 */
schll_liste* schll_ListeErstellenZeiger();

/*
 * Konstruieren mit Rückgabe über extern gegebenen Zeiger, alloziert eine schll_liste
 * Muss aufgelöst werden
 * schll_liste** karte: Zeiger auf schll_liste-Zeiger
 */
schll_rueckgabe schll_ListeErstellen(schll_liste** karte);

/*
 * Auflösen einer schll_liste
 * schll_liste* karte: Zeiger auf azuflösende schll_liste
 * const int free_name: wenn 1, Schlüssel der Einträge deallozieren
 * const int free_wert: wenn 1, Werte der Einträge deallozieren
 */
schll_rueckgabe schll_ListeAufloesen(const schll_liste* karte, const int free_name, const int free_wert);

/*
 * Eintrag zu einer schll_liste hinzufügen
 * schll_liste* karte: Zeiger auf schll_liste, zu der der Eintrag hinzugefügt werden soll
 * const char* name: Schlüssel des Eintrags
 * const int name_kopieren: Schlüssel kopieren? Muss aufgelöst werden.
 * const void* wert: Wert für den Eintrag
 * const int wert_kopieren: Wert kopieren?
 * const unsigned int wert_laenge: Falls der Wert kopiert werden soll, ist hier die Länge des zu allozierenden Speicherplatzes einzutragen
 */
schll_rueckgabe schll_ListeEintragHinzufuegen(schll_liste* karte, const char* name, const int name_kopieren,
											  const void* wert, const int wert_kopieren,
											  const unsigned int wert_laenge);

/*
 * Eintrag aus einer schll_liste entfernen
 * const schll_liste* karte: Zeiger auf schll_liste, aus der der Eintrag entfernt werden soll
 * const char* name: Schlüssel des Eintrags
 * const int free_name: wenn 1, Schlüssel des Eintrags deallozieren
 * const int free_wert: wenn 1, Wert des Eintrags deallozieren
 */
schll_rueckgabe schll_ListeEintragEntfernen(const schll_liste* karte, const char* name, const int free_name,
											const int free_wert);

/*
 * Zeiger auf Wert holen
 * const schll_liste* karte: Zeiger auf schll_liste, aus der der Wert geholt werden soll
 * void** v: Zeiger auf den Zeiger, der den Wert angeben soll
 * const char* name: Schlüssel des Eintrags
 */
schll_rueckgabe schll_ListeWertHolen(const schll_liste* karte, void** v, const char* name);

/*
 * Zeiger auf Wert direkt ausgeben
 * const schll_liste* karte: Zeiger auf schll_liste, aus der der Wert geholt werden soll
 * const char* name: Schlüssel des Eintrags
 */
void* schll_ListeWertHolenZeiger(const schll_liste* karte, const char* name);

/*
 * Definitionen
 */

schll_liste* schll_ListeErstellenZeiger(){

	schll_liste* ret = (schll_liste*)calloc(1, sizeof(schll_liste));
	return ret;

}

schll_rueckgabe schll_ListeAufloesen(const schll_liste* karte, const int free_name, const int free_wert){

	if(karte == NULL)
		return MAP_NULL;

	schll_eintrag* p = karte->starteintrag;

	while(p != NULL) {
		schll_eintrag* t= p->n_eintrag;

		if(free_name){
			free(p->name);
		}

		if(free_wert){
			free(p->wert);
		}

		p = t;
	}

	return IO;

}

schll_rueckgabe schll_ListeEintragHinzufuegen(schll_liste* karte, const char* name, const int name_kopieren,
											  const void* wert, const int wert_kopieren, const unsigned int wert_laenge){

	if(karte == NULL)
		return MAP_NULL;


	schll_eintrag neuer_eintrag;
	neuer_eintrag.name = (char*)name;
	neuer_eintrag.name_l = strlen(name)+1;
	neuer_eintrag.wert = (void*)wert;
	neuer_eintrag.n_eintrag = NULL;

	schll_eintrag* p = karte->starteintrag;

	while(p != NULL) {
		if (p->name_l == neuer_eintrag.name_l) {
			if (strncmp(p->name, neuer_eintrag.name, neuer_eintrag.name_l) == 0) {
				return SCHL_VORHANDEN;
			}
		}

		p = p->n_eintrag;
	}

	p->n_eintrag = (schll_eintrag*)calloc(1, sizeof(schll_eintrag));

	if(p->n_eintrag == NULL){
		return ALLOK_FEHLG;
	}

	p->n_eintrag->name_l = neuer_eintrag.name_l;

	if(name_kopieren){
		p->n_eintrag->name = (char*)calloc(neuer_eintrag.name_l, 1);

		if(p->n_eintrag->name == NULL){
			free(p->n_eintrag);
			return ALLOK_FEHLG;
		}

		strcat(p->n_eintrag->name, neuer_eintrag.name);
	}else{
		p->n_eintrag->name = neuer_eintrag.name;
	}

	if(wert_kopieren){
		p->n_eintrag->wert = (char*)calloc(wert_laenge, 1);

		if(p->n_eintrag->wert == NULL){
			if(name_kopieren){
				free(p->n_eintrag->name);
			}
			free(p->n_eintrag);
			return ALLOK_FEHLG;
		}

		memcpy(p->n_eintrag->wert, wert, wert_laenge);
	}else{
		p->n_eintrag->name = neuer_eintrag.name;
	}

	karte->eintraege_l++;

	return IO;
}

schll_rueckgabe schll_ListeEintragEntfernen(const schll_liste* karte, const char* name, const int free_name,
											const int free_wert){

	if(karte == NULL)
		return MAP_NULL;

	if(name == NULL)
		return NAME_FEHLT;

	if(karte->starteintrag == NULL)
		return KEINE_EINTRAEGE;

	schll_eintrag* p = karte->starteintrag;

	unsigned int l;
	l = strlen(name)+1;

	while(p != NULL) {
		if (p->n_eintrag->name_l == l) {
			if (strncmp(p->n_eintrag->name, name, l) == 0) {
				schll_eintrag* t = p->n_eintrag->n_eintrag;

				if(free_name){
					free(p->n_eintrag->name);
				}

				if(free_wert){
					free(p->n_eintrag->wert);
				}

				p->n_eintrag = t;

				return IO;
			}
		}

		p = p->n_eintrag;
	}

	return KEIN_EINTRAG;

}

schll_rueckgabe schll_ListeWertHolen(const schll_liste* karte, void** v, const char* name){

	schll_eintrag* e;
	e = NULL;

	unsigned int l;
	l = strlen(name)+1;

	schll_eintrag* p = karte->starteintrag;

	while(p != NULL) {
		if (p->name_l == l) {
			if (strncmp(p->name, name, l) == 0) {
				*v = p->wert;
				return IO;
			}
		}

		p = p->n_eintrag;
	}

	return KEIN_EINTRAG;
}

void* schll_ListeWertHolenZeiger(const schll_liste* karte, const char* name){

	void* v;
	v = NULL;

	if(schll_ListeWertHolen(karte, &v, name) == IO){
		return v;
	}else{
		return NULL;
	}

}

#ifdef __cplusplus
}
#endif

#endif