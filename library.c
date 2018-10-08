#include "library.h"

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