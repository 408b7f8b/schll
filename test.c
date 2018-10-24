#include <zconf.h>
#include "schll.h"

void pruefeWert(schll_liste* l, char* str, int vorh){

	char* c = (char*)schll_ListeWertHolenZeiger(l, str);

	if(c == NULL && !vorh){
		printf("%s in Ordnung\n", str);
	}else{
		if( strncmp(str, c, strlen(str)) == 0 && vorh){
			printf("%s in Ordnung\n", str);
		}else{
			printf("%s nicht in Ordnung\n", str);
		}
	}
}

int main(){

	schll_liste* l = schll_ListeErstellenZeiger();

	const char* wert1 = "test1";
	const char* wert2 = "test2";
	const char* wert3 = "test3";
	schll_ListeEintragHinzufuegen(l, wert1, 0, wert1, 0, 0);
	schll_ListeEintragHinzufuegen(l, wert2, 0, wert2, 0, 0);
	schll_ListeEintragHinzufuegen(l, wert3, 0, wert3, 0, 0);

	pruefeWert(l, wert1, 1);
	pruefeWert(l, wert2, 1);
	pruefeWert(l, wert3, 1);

	schll_ListeEintragEntfernen(l, wert1, 0, 0);

	pruefeWert(l, wert1, 0);
	pruefeWert(l, wert2, 1);
	pruefeWert(l, wert3, 1);

	schll_ListeEintragHinzufuegen(l, wert1, 0, wert1, 0, 0);

	pruefeWert(l, wert1, 1);
	pruefeWert(l, wert2, 1);
	pruefeWert(l, wert3, 1);

	schll_ListeAufloesen(&l, 0, 0);

	return 0;
}