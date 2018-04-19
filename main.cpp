#define _CRTDGB_MAP_ALLOC
#include "main.h"
#include <stdlib.h>
#include <crtdbg.h>
#include "Perzistencia.h"
#include "Objednavka.h"

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	SetConsoleOutputCP(65001);

	DatabazaBiofarmarov dbBiofarmarov;
	DatabazaNakupov dbNakupov;
	DatabazaVozidiel dbVozidiel;
	DatabazaZakaznikov dbZakaznikov;
	DatabazaObjednavok dbObjednavok;

	Menu menu(dbBiofarmarov, dbNakupov, dbVozidiel, dbZakaznikov, dbObjednavok);
	
	//moj

	while (true) {
		// vypis hlavne menu
		menu.mHlavne();

		// zisti pozadovanu akciu a vykonaj ju
		ulong volba = menu.ziskajVolbu();
		if (volba != 0) {
			menu.volbaHlavne(volba);
			system("pause");
		}
		else {
			break;
		}
	}

	system("pause");
	return 0;
}