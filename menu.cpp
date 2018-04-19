#include "menu.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

const string ODDELOVAC = "- - - - - - - - - - - - - - - - - - - -";
const string NEPLATNA_VOLBA = "Va�a predo�l� vo�ba bola neplatn�. Pros�m zadajte ��slo z po�adovan�ho intervalu a potvr�te stla�en�m kl�vesu ENTER.\n";

ulong Menu::ziskajVolbu() {
	ulong volba = 0;
	cout << "�o si �el�te spravi�?\n";
	cout << "Va�a vo�ba: ";
	cin >> volba;
	cout << endl;

	return volba;
}

void Menu::mHlavne() {
	cout << ODDELOVAC << endl << endl;
	cout << "Hlavn� menu:\n";
	cout << "(0) Ukon�i� program\n";
	cout << "(1) Spr�va biofarm�rov\n";
	cout << "(2) Spr�va vozidiel\n";
	cout << "(3) Spr�va z�kazn�kov\n";
	cout << "(4) Spr�va objedn�vok\n";
	cout << "(5) Kontrola po�iadaviek na najbli���ch 7 dn�\n";
	cout << "(6) Odovzdanie tovaru z�kazn�kom\n";
	cout << "(7) Vyp�sanie zisku za �asov� obdobie\n";
	cout << "(8) Vyp�sanie aktu�lneho d�tumu\n";
	cout << "(9) Prechod na �a�� de�\n";
	cout << "(10) Ulo�enie aktu�lneho stavu evidencie do s�boru\n";
	cout << "(11) Na��tanie aktu�lneho stavu evidencie zo s�boru\n";
	cout << "[12] Prechod na �a�� t��de�\n";
	cout << "(13) Celkov� zisk spolo�nosti\n";
	cout << "(14) V�pis aktu�lnych skladov�ch z�sob\n";
	cout << "(15) Nachystaj objedn�vky pre v�robu na zajtraj�� de�(odpo��taj mno�stvo skladov�ch z�sob pre v�robu objedn�vky), " << "ak je nedostatok polotvarov zru� objedn�vku.\n";
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
	case 8: cout << "Aktu�lny d�tum/epocha: " << ctime(&dnesnyDatum) << "/" << epochaNaPocetDni(dnesnyDatum) << endl;
		break;
	case 9: dnesnyDatum += pocetSekundVdni;
		cout << "D�tum posunut� o de� nesk�r." << endl;
		break;
	case 10: persist.uloz(dbObjednavok, dbZakaznikov, dbVozidiel, dbBiofarmarov, dbNakupov);
		break;
	case 11: persist.vratSaNaZaciatok();
		persist.nacitaj(dbObjednavok, dbZakaznikov, dbVozidiel, dbBiofarmarov, dbNakupov);
		break;
	case 12: dnesnyDatum += 7 * pocetSekundVdni;
		cout << "D�tum posunut� o t��de� nesk�r." << endl;
		break;
	case 13: cout << "Celkov� zisk spolo�nosti: " << dbObjednavok.zisk() - dbNakupov.cenaPolotovarov() - dbVozidiel.prevadzkoveNaklady() << endl;
		break;
	case 14: cout << "Aktu�lne skladov� z�soby: " << endl << "Zemiaky: " << dbNakupov.getMnozstvoZemiakov() << " |Olej: " << dbNakupov.getMnozstvOleja() << " |Ochucovadlo: " << dbNakupov.getMnozstvoOchucovadla() << endl;
		break;
	case 15: dbObjednavok.zrusObjednavkyPreNedostatokPolotovarov(dbNakupov);
		break;
	default: mHlavne();
		cout << NEPLATNA_VOLBA << endl;
		volbaHlavne(ziskajVolbu());
	}
}

void Menu::mBiofarmar() {
	cout << "Spr�va biofarm�rov:\n";
	cout << "(0) N�vrat do hlavn�ho menu\n";
	cout << "(1) Pridaj biofarm�ra\n";
	cout << "(2) Vyp� biofarm�rov\n";
	cout << "(3) N�jdi biofarm�ra od ktor�ho spolo�nos� nak�pila najviac\n";
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
	cout << "Spr�va vozidiel:\n";
	cout << "(0) N�vrat do hlavn�ho menu\n";
	cout << "(1) Pridaj vozidlo\n";
	cout << "(2) Vyp� vozidl�\n";
	cout << "(3) Napl� vozidl�\n";
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
	cout << "Spr�va z�kazn�kov:\n";
	cout << "(0) N�vrat do hlavn�ho menu\n";
	cout << "(1) Registruj z�kazn�ka\n";
	cout << "(2) Vyp� z�kazn�kov\n";
	cout << "(3) Vyp� z�kazn�kov z dan�ho regi�nu\n";
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
		cout << "Vlo�te regi�n: ";
		cin >> region;
		cout << "Dnes je de� " << epochaNaPocetDni(dnesnyDatum) << endl;
		cout << "Den od ktor�ho m�m vyp�sa� z�kazn�kov: " << endl;
		cin >> od;
		cout << "Den po ktor� m�m vyp�sa� z�kazn�kov: " << endl;
		cin >> po;
		dbObjednavok.vypisPocetHmotnostTrzbyZrusenychAzamietnutych(region, od, po, dbZakaznikov);
		break;
	default: mZakaznik();
		cout << NEPLATNA_VOLBA << endl;
		volbaZakaznik(ziskajVolbu());
	}
}

void Menu::mObjednavka() {
	cout << "Spr�va objedn�vok:\n";
	cout << "(0) N�vrat do hlavn�ho menu\n";
	cout << "(1) Zaeviduj objedn�vku\n";
	cout << "(2) Vyp� objedn�vky na nasleduj�ci de�\n";
	cout << "(3) Vyp� zrealizovan� objedn�vky\n";
	cout << "(4) Vyp� stornovan� objedn�vky\n";
	cout << "[5] Vyp� v�etky objedn�vky\n";
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
	case 3:	cout << "Dnes je de� " << epochaNaPocetDni(dnesnyDatum) << endl;
		cout << "Den od ktor�ho m�m vyp�sa� objedn�vky: " << endl;
		cin >> od;
		cout << "Den po ktor� m�m vyp�sa� objedn�vky: " << endl;
		cin >> po;
		dbObjednavok.vypisZrealizovane(pocetDniNaEpochu(od), pocetDniNaEpochu(po), dbZakaznikov, dbNakupov);
		break;
	case 4: cout << "Dnes je de� " << epochaNaPocetDni(dnesnyDatum) << endl;
		cout << "Den od ktor�ho m�m vyp�sa� objedn�vky: " << endl;
		cin >> od;
		cout << "Den po ktor� m�m vyp�sa� objedn�vky: " << endl;
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
