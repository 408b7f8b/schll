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

schll_liste* schll_ListeErstellenZeiger();
schll_rueckgabe schll_ListeErstellen(schll_liste** karte);
schll_rueckgabe schll_ListeAufloesen(const schll_liste* karte, const int free_name, const int free_wert);

schll_rueckgabe schll_ListeEintragHinzufuegen(schll_liste* karte, const char* name, const int name_kopieren,
											  const void* wert, const int wert_kopieren, const unsigned int wert_laenge);
schll_rueckgabe schll_ListeEintragEntfernen(const schll_liste* karte, const char* name, const int free_name,
											const int free_wert);

schll_rueckgabe schll_ListeWertHolen(const schll_liste* karte, void** v, const char* name);
void* schll_ListeWertHolenZeiger(const schll_liste* karte, const char* name);

#ifdef __cplusplus
}
#endif

#endif