#include "menu.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

const string ODDELOVAC = "- - - - - - - - - - - - - - - - - - - -";
const string NEPLATNA_VOLBA = "Vaša predošlá vo¾ba bola neplatná. Prosím zadajte èíslo z požadovaného intervalu a potvrïte stlaèením klávesu ENTER.\n";

ulong Menu::ziskajVolbu() {
	ulong volba = 0;
	cout << "Èo si želáte spravi?\n";
	cout << "Vaša vo¾ba: ";
	cin >> volba;
	cout << endl;

	return volba;
}

void Menu::mHlavne() {
	cout << ODDELOVAC << endl << endl;
	cout << "Hlavné menu:\n";
	cout << "(0) Ukonèi program\n";
	cout << "(1) Správa biofarmárov\n";
	cout << "(2) Správa vozidiel\n";
	cout << "(3) Správa zákazníkov\n";
	cout << "(4) Správa objednávok\n";
	cout << "(5) Kontrola požiadaviek na najbližších 7 dní\n";
	cout << "(6) Odovzdanie tovaru zákazníkom\n";
	cout << "(7) Vypísanie zisku za èasové obdobie\n";
	cout << "(8) Vypísanie aktuálneho dátumu\n";
	cout << "(9) Prechod na ïaåší deñ\n";
	cout << "(10) Uloženie aktuálneho stavu evidencie do súboru\n";
	cout << "(11) Naèítanie aktuálneho stavu evidencie zo súboru\n";
	cout << "[12] Prechod na ïaåší týždeñ\n";
	cout << "(13) Celkový zisk spoloènosti\n";
	cout << "(14) Výpis aktuálnych skladových zásob\n";
	cout << "(15) Nachystaj objednávky pre výrobu na zajtrajší deò(odpoèítaj množstvo skladových zásob pre výrobu objednávky), " << "ak je nedostatok polotvarov zruš objednávku.\n";
	cout << endl;
}

void Menu::volbaHlavne(ulong volba) {
	cout << ODDELOVAC << endl << endl;

	switch (volba) {
	case 0: return;
	case 1: mBiofarmar();
		volbaBiofarmar(ziskajVolbu());
		break;
	case 2: mVozidlo();
		volbaVozidlo(ziskajVolbu());
		break;
	case 3: mZakaznik();
		volbaZakaznik(ziskajVolbu());
		break;
	case 4: mObjednavka();
		volbaObjednavka(ziskajVolbu());
		break;
	case 5: dbNakupov.zabezpecDostatokPolotovarov(dbObjednavok, dbBiofarmarov);
		break;
	case 6: dbVozidiel.odovzdajTovar(dbZakaznikov);
		break;
	case 7: break;
	case 8: cout << "Aktuálny dátum/epocha: " << ctime(&dnesnyDatum) << "/" << epochaNaPocetDni(dnesnyDatum) << endl;
		break;
	case 9: dnesnyDatum += pocetSekundVdni;
		cout << "Dátum posunutý o deñ neskôr." << endl;
		break;
	case 10: persist.uloz(dbObjednavok, dbZakaznikov, dbVozidiel, dbBiofarmarov, dbNakupov);
		break;
	case 11: persist.vratSaNaZaciatok();
		persist.nacitaj(dbObjednavok, dbZakaznikov, dbVozidiel, dbBiofarmarov, dbNakupov);
		break;
	case 12: dnesnyDatum += 7 * pocetSekundVdni;
		cout << "Dátum posunutý o týždeñ neskôr." << endl;
		break;
	case 13: cout << "Celkový zisk spoloènosti: " << dbObjednavok.zisk() - dbNakupov.cenaPolotovarov() - dbVozidiel.prevadzkoveNaklady() << endl;
		break;
	case 14: cout << "Aktuálne skladové zásoby: " << endl << "Zemiaky: " << dbNakupov.getMnozstvoZemiakov() << " |Olej: " << dbNakupov.getMnozstvOleja() << " |Ochucovadlo: " << dbNakupov.getMnozstvoOchucovadla() << endl;
		break;
	case 15: dbObjednavok.zrusObjednavkyPreNedostatokPolotovarov(dbNakupov);
		break;
	default: mHlavne();
		cout << NEPLATNA_VOLBA << endl;
		volbaHlavne(ziskajVolbu());
	}
}

void Menu::mBiofarmar() {
	cout << "Správa biofarmárov:\n";
	cout << "(0) Návrat do hlavného menu\n";
	cout << "(1) Pridaj biofarmára\n";
	cout << "(2) Vypíš biofarmárov\n";
	cout << "(3) Nájdi biofarmára od ktorého spoloènos nakúpila najviac\n";
	cout << endl;
}

void Menu::volbaBiofarmar(ulong volba) {
	switch (volba) {
	case 0: break;
	case 1: dbBiofarmarov.pridaj();
		break;
	case 2: dbBiofarmarov.vypis(dbNakupov);
		break;
	case 3: dbNakupov.vypisNajBiofarmara();
		break;
	default: mBiofarmar();
		cout << NEPLATNA_VOLBA << endl;
		volbaBiofarmar(ziskajVolbu());
	}
}

void Menu::mVozidlo() {
	cout << "Správa vozidiel:\n";
	cout << "(0) Návrat do hlavného menu\n";
	cout << "(1) Pridaj vozidlo\n";
	cout << "(2) Vypíš vozidlá\n";
	cout << "(3) Naplò vozidlá\n";
	cout << endl;
}

void Menu::volbaVozidlo(ulong volba) {
	switch (volba) {
	case 0: break;
	case 1: dbVozidiel.pridaj();
		break;
	case 2: dbVozidiel.vypis();
		break;
	case 3: dbVozidiel.naplnVozidla(dbObjednavok);
		break;
	default: mVozidlo();
		cout << NEPLATNA_VOLBA << endl;
		volbaVozidlo(ziskajVolbu());
	}
}

void Menu::mZakaznik() {
	cout << "Správa zákazníkov:\n";
	cout << "(0) Návrat do hlavného menu\n";
	cout << "(1) Registruj zákazníka\n";
	cout << "(2) Vypíš zákazníkov\n";
	cout << "(3) Vypíš zákazníkov z daného regiónu\n";
	cout << endl;
}

void Menu::volbaZakaznik(ulong volba) {

	ulong od, po;

	switch (volba) {
	case 0: break;
	case 1: dbZakaznikov.Registruj();
		break;
	case 2: dbZakaznikov.vypis();
		break;
	case 3: ulong region;
		cout << "Vložte región: ";
		cin >> region;
		cout << "Dnes je deò " << epochaNaPocetDni(dnesnyDatum) << endl;
		cout << "Den od ktorého mám vypísa zákazníkov: " << endl;
		cin >> od;
		cout << "Den po ktorý mám vypísa zákazníkov: " << endl;
		cin >> po;
		dbObjednavok.vypisPocetHmotnostTrzbyZrusenychAzamietnutych(region, od, po, dbZakaznikov);
		break;
	default: mZakaznik();
		cout << NEPLATNA_VOLBA << endl;
		volbaZakaznik(ziskajVolbu());
	}
}

void Menu::mObjednavka() {
	cout << "Správa objednávok:\n";
	cout << "(0) Návrat do hlavného menu\n";
	cout << "(1) Zaeviduj objednávku\n";
	cout << "(2) Vypíš objednávky na nasledujúci deò\n";
	cout << "(3) Vypíš zrealizované objednávky\n";
	cout << "(4) Vypíš stornované objednávky\n";
	cout << "[5] Vypíš všetky objednávky\n";
	cout << endl;
}

void Menu::volbaObjednavka(ulong volba) {

	ulong od, po;

	switch (volba) {
	case 0: break;
	case 1: dbObjednavok.pridaj(dbVozidiel);
		break;
	case 2: dbObjednavok.vypisNaZrealizovanieZajtra(dbZakaznikov, dbNakupov);
		break;
	case 3:	cout << "Dnes je deò " << epochaNaPocetDni(dnesnyDatum) << endl;
		cout << "Den od ktorého mám vypísa objednávky: " << endl;
		cin >> od;
		cout << "Den po ktorý mám vypísa objednávky: " << endl;
		cin >> po;
		dbObjednavok.vypisZrealizovane(pocetDniNaEpochu(od), pocetDniNaEpochu(po), dbZakaznikov, dbNakupov);
		break;
	case 4: cout << "Dnes je deò " << epochaNaPocetDni(dnesnyDatum) << endl;
		cout << "Den od ktorého mám vypísa objednávky: " << endl;
		cin >> od;
		cout << "Den po ktorý mám vypísa objednávky: " << endl;
		cin >> po;
		dbObjednavok.vypisNezrealizovane(pocetDniNaEpochu(od), pocetDniNaEpochu(po), dbZakaznikov, dbNakupov);
		break;
	case 5: dbObjednavok.vypis(dbZakaznikov, dbNakupov);
		break;
	default: mObjednavka();
		cout << NEPLATNA_VOLBA << endl;
		volbaObjednavka(ziskajVolbu());
	}
}
