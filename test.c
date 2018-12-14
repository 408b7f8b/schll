#include <zconf.h>
#include "schll.h"

void pruefeWert(schll_list* l, const char* str, int vorh){

	char* c = (char*) schll_getValuePointer(l, str);

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

	schll_list* l = schll_MakeListPointer();

	const char* wert1 = "test1";
	const char* wert2 = "test2";
	const char* wert3 = "test3";
	schll_addEntry(l, wert1, 0, wert1, 0, 0);
	schll_addEntry(l, wert2, 0, wert2, 0, 0);
	schll_addEntry(l, wert3, 0, wert3, 0, 0);

	pruefeWert(l, wert1, 1);
	pruefeWert(l, wert2, 1);
	pruefeWert(l, wert3, 1);

	schll_removeEntry(l, wert1, 0, 0);

	pruefeWert(l, wert1, 0);
	pruefeWert(l, wert2, 1);
	pruefeWert(l, wert3, 1);

	schll_addEntry(l, wert1, 0, wert1, 0, 0);

	pruefeWert(l, wert1, 1);
	pruefeWert(l, wert2, 1);
	pruefeWert(l, wert3, 1);

	schll_DeleteList(&l, 0, 0);

	return 0;
}