/*
 * Schluesselliste
 * Version 0.1
 */

#ifndef SCHLL_H
#define SCHLL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Typen
 */
typedef enum schll_return_value {
	OK, SCHLL_EXISTS, MAP_NULL, ALLOC_FAIL, NAME_MISSING, NO_ENTRY, NO_ENTRIES
} schll_return_value;

typedef struct schll_entry {
	char* name;
	size_t name_l;
	void* value;

	struct schll_entry* next_entry;
} schll_entry;

typedef struct schll_list {
	struct schll_entry* first_entry;
	unsigned int number_entries;
} schll_list;

/*
 * Deklarationen
 */

/*
 * Konstruieren mit direkter Rückgabe, alloziert eine schll_list
 * Muss aufgelöst werden
 */
schll_list* schll_MakeListPointer();

/*
 * Konstruieren mit Rückgabe über extern gegebenen Zeiger, alloziert eine schll_list
 * Muss aufgelöst werden
 * schll_list** karte: Zeiger auf schll_list-Zeiger
 */
schll_return_value schll_MakeList(schll_list** list);

/*
 * Auflösen einer schll_list
 * schll_list* karte: Zeiger auf azuflösende schll_list
 * const int free_name: wenn 1, Schlüssel der Einträge deallozieren
 * const int free_wert: wenn 1, Werte der Einträge deallozieren
 */
schll_return_value schll_DeleteList(schll_list** list, int free_names, int free_values);

/*
 * Eintrag zu einer schll_list hinzufügen
 * schll_list* karte: Zeiger auf schll_list, zu der der Eintrag hinzugefügt werden soll
 * const char* name: Schlüssel des Eintrags
 * const int name_kopieren: Schlüssel kopieren? Muss aufgelöst werden.
 * const void* value: Wert für den Eintrag
 * const int wert_kopieren: Wert kopieren?
 * const unsigned int wert_laenge: Falls der Wert kopiert werden soll, ist hier die Länge des zu allozierenden Speicherplatzes einzutragen
 */
schll_return_value schll_addEntry(schll_list* list, const char* name, int copy_name,
								  const void* value, int copy_value,
								  size_t value_size);

/*
 * Eintrag aus einer schll_list entfernen
 * const schll_list* karte: Zeiger auf schll_list, aus der der Eintrag entfernt werden soll
 * const char* name: Schlüssel des Eintrags
 * const int free_name: wenn 1, Schlüssel des Eintrags deallozieren
 * const int free_wert: wenn 1, Wert des Eintrags deallozieren
 */
schll_return_value schll_removeEntry(schll_list* list, const char* name, int free_name,
									 int free_value);

/*
 * Zeiger auf Wert holen
 * const schll_list* karte: Zeiger auf schll_list, aus der der Wert geholt werden soll
 * void** v: Zeiger auf den Zeiger, der den Wert angeben soll
 * const char* name: Schlüssel des Eintrags
 */
schll_return_value schll_getValue(const schll_list* list, void** value, const char* name);

/*
 * Zeiger auf Wert direkt ausgeben
 * const schll_list* karte: Zeiger auf schll_list, aus der der Wert geholt werden soll
 * const char* name: Schlüssel des Eintrags
 */
void* schll_getValuePointer(const schll_list* list, const char* name);

/*
 * Definitionen
 */

schll_list* schll_MakeListPointer(){

	schll_list* ret = (schll_list*)calloc(1, sizeof(schll_list));
	return ret;

}

schll_return_value schll_DeleteList(schll_list** list, const int free_names, const int free_values){

	if(list == NULL){
		return MAP_NULL;
	}

	if(*list == NULL) {
		return MAP_NULL;
	}

	schll_entry* p = (*list)->first_entry;

	while(p != NULL) {
		schll_entry* t = p->next_entry;

		if(free_names){
			free(p->name);
		}

		if(free_values){
			free(p->value);
		}

		free(p);

		p = t;
	}

	free(*list);
	*list = NULL;

	return OK;

}

schll_return_value schll_addEntry(schll_list* list, const char* name, const int copy_name,
								  const void* value, const int copy_value, size_t value_size){

	if(list == NULL)
		return MAP_NULL;


	schll_entry neuer_eintrag;
	neuer_eintrag.name = (char*)name;
	neuer_eintrag.name_l = strlen(name)+1;
	neuer_eintrag.value = (void*)value;
	neuer_eintrag.next_entry = NULL;

	schll_entry** p = &(list->first_entry);

	while(*p != NULL) {
		if ((*p)->name_l == neuer_eintrag.name_l) {
			if (strncmp((*p)->name, neuer_eintrag.name, neuer_eintrag.name_l) == 0) {
				return SCHLL_EXISTS;
			}
		}

		p = &((*p)->next_entry);
	}

	(*p) = (schll_entry*)calloc(1, sizeof(schll_entry));

	if((*p) == NULL){
		return ALLOC_FAIL;
	}

	(*p)->name_l = neuer_eintrag.name_l;

	if(copy_name){
		(*p)->name = (char*)calloc(neuer_eintrag.name_l, 1);

		if((*p)->name == NULL){
			free(p);
			return ALLOC_FAIL;
		}

		strcat((*p)->name, neuer_eintrag.name);
	}else{
		(*p)->name = neuer_eintrag.name;
	}

	if(copy_value){
		(*p)->value = (char*)calloc(value_size, 1);

		if((*p)->value == NULL){
			if(copy_name){
				free((*p)->name);
			}
			free(p);
			return ALLOC_FAIL;
		}

		memcpy((*p)->value, value, value_size);
	}else{
		(*p)->value = neuer_eintrag.value;
	}

	list->number_entries++;

	return OK;
}

schll_return_value schll_removeEntry(schll_list* list, const char* name, const int free_name,
									 const int free_value){

	if(list == NULL)
		return MAP_NULL;

	if(name == NULL)
		return NAME_MISSING;

	if(list->first_entry == NULL)
		return NO_ENTRIES;

	schll_entry* p_ = NULL;
	schll_entry* p = list->first_entry;

	size_t l;
	l = strlen(name)+1;

	while(p != NULL) {
		if (p->name_l == l) {
			if (strncmp(p->name, name, l) == 0) {

				if(p_ == NULL){
					list->first_entry = list->first_entry->next_entry;
				}else{
					p_->next_entry = p->next_entry;
				}

				if(free_name){
					free(p->name);
				}

				if(free_value){
					free(p->value);
				}

				free(p);
				list->number_entries--;

				return OK;
			}
		}

		p_ = p;
		p = p->next_entry;

	}

	return NO_ENTRY;

}

schll_return_value schll_getValue(const schll_list* list, void** value, const char* name){

	schll_entry* e;
	e = NULL;

	size_t l;
	l = strlen(name)+1;

	schll_entry* p = list->first_entry;

	while(p != NULL) {
		if (p->name_l == l) {
			if (strncmp(p->name, name, l) == 0) {
				*value = p->value;
				return OK;
			}
		}

		p = p->next_entry;
	}

	return NO_ENTRY;
}

void* schll_getValuePointer(const schll_list* list, const char* name){

	void* v;
	v = NULL;

	if(schll_getValue(list, &v, name) == OK){
		return v;
	}else{
		return NULL;
	}

}

#ifdef __cplusplus
}
#endif

#endif