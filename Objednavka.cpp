#include "Objednavka.h"

#include <iostream>
using std::cout;
using std::flush;
using std::endl;

const string ODDELOVAC = "- - - - - - - - - -";

void Objednavka::vypis(const DatabazaZakaznikov& dbZakaznikov, const DatabazaNakupov& dbNakupov) {

	cout << "Objednávka:" << endl;

	cout << "obchodný názov zákazníka: " << zakaznik << endl;
	ulong region = dbZakaznikov.ziskajRegion(this->getZakaznik());

	if (region == 0) {
		cout << "Zákazník nebol nájdený v databáze zákazníkov!" << endl;
	}
	else {
		cout << "región: " << region << endl;
	}

	time_t datumRealizacie = datumDorucenia;
	cout << "dátum realizácie: " << endl << ctime(&datumRealizacie) << endl;

	time_t datumVytvorenia = datumZaevidovania;
	cout << "dátum vytvorenia: " << endl << ctime(&datumVytvorenia) << endl;

	cout << "množstvo: " << mnozstvo << endl;
	cout << "cena: " << cena << endl;

	cout << "typ tovaru: ";
	switch (typTovaru) {
	case TypTovaru::hranolceky:
		cout << "na hranolèeky" << endl;
		break;
	case TypTovaru::lupienky:
		cout << "na lupienky" << endl;
		break;
	default:
		cout << "Neznámy typ tovaru!" << endl;
	}

	bool nedostatokPolotovarov = dbNakupov.existujeDostatokPolotovarov(*this);
	cout << "Existuje dostatok polotovarov: [0-nie | 1-ano] " << "-" <<nedostatokPolotovarov << endl; //vhodne pre testovaci vypis

	zrealizovana ? cout << "Objednávka bola zrealizovaná." << endl : cout << "Objednávka nie je zrealizovaná.";
	cout << "Objednávka " << (!zrusena ? "ne" : "") << "bude zrušená" << endl;
	cout << "Objednávka " << (!zamietnuta ? "ne" : "") << "bude zamietnutá" << endl;

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
		cout << "Vložili ste nesprávne písmeno pre typ tovaru!" << endl;
		return file;
	}

	o.typTovaru = (pismenko == 'h' ? TypTovaru::hranolceky : TypTovaru::lupienky);

	return file;
}