#include "Objednavka.h"

#include <iostream>
using std::cout;
using std::flush;
using std::endl;

const string ODDELOVAC = "- - - - - - - - - -";

void Objednavka::vypis(const DatabazaZakaznikov& dbZakaznikov, const DatabazaNakupov& dbNakupov) {

	cout << "Objedn�vka:" << endl;

	cout << "obchodn� n�zov z�kazn�ka: " << zakaznik << endl;
	ulong region = dbZakaznikov.ziskajRegion(this->getZakaznik());

	if (region == 0) {
		cout << "Z�kazn�k nebol n�jden� v datab�ze z�kazn�kov!" << endl;
	}
	else {
		cout << "regi�n: " << region << endl;
	}

	time_t datumRealizacie = datumDorucenia;
	cout << "d�tum realiz�cie: " << endl << ctime(&datumRealizacie) << endl;

	time_t datumVytvorenia = datumZaevidovania;
	cout << "d�tum vytvorenia: " << endl << ctime(&datumVytvorenia) << endl;

	cout << "mno�stvo: " << mnozstvo << endl;
	cout << "cena: " << cena << endl;

	cout << "typ tovaru: ";
	switch (typTovaru) {
	case TypTovaru::hranolceky:
		cout << "na hranol�eky" << endl;
		break;
	case TypTovaru::lupienky:
		cout << "na lupienky" << endl;
		break;
	default:
		cout << "Nezn�my typ tovaru!" << endl;
	}

	bool nedostatokPolotovarov = dbNakupov.existujeDostatokPolotovarov(*this);
	cout << "Existuje dostatok polotovarov: [0-nie | 1-ano] " << "-" <<nedostatokPolotovarov << endl; //vhodne pre testovaci vypis

	zrealizovana ? cout << "Objedn�vka bola zrealizovan�." << endl : cout << "Objedn�vka nie je zrealizovan�.";
	cout << "Objedn�vka " << (!zrusena ? "ne" : "") << "bude zru�en�" << endl;
	cout << "Objedn�vka " << (!zamietnuta ? "ne" : "") << "bude zamietnut�" << endl;

	cout << ODDELOVAC << endl;
}

fstream& operator<<(fstream& file, const Objednavka& o) {

	file
		<< o.getDatumDorucenia()
		<< endl
		<< o.getDatumZaevidovania()
		<< endl
		<< o.getMnozstvo()
		<< endl
		<< o.getCena()
		<< endl
		<< o.getZakaznik()
		<< endl
		<< o.getZamietnuta()
		<< endl
		<< o.getZrusena()
		<< endl
		<< (o.getTypTovaru() == TypTovaru::hranolceky ? 'h' : 'l')
		<< endl;

	return file;
}

fstream& operator>>(fstream& file, Objednavka& o) {

	char pismenko;

	file
		>> o.datumDorucenia
		>> o.datumZaevidovania
		>> o.mnozstvo
		>> o.cena
		>> o.zakaznik
		>> o.zamietnuta
		>> o.zrusena
		>> pismenko;

	if (!platnyTypTovaru(pismenko)) {
		cout << "Vlo�ili ste nespr�vne p�smeno pre typ tovaru!" << endl;
		return file;
	}

	o.typTovaru = (pismenko == 'h' ? TypTovaru::hranolceky : TypTovaru::lupienky);

	return file;
}