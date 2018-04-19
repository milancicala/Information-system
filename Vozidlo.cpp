#include "Vozidlo.h"

#include <iostream>
using std::cout;
using std::flush;
using std::endl;

fstream& operator<<(fstream& file, const Vozidlo& v) {

	file
		<< v.getDatumZaradenia()
		<< endl
		<< v.getNosnost()
		<< endl
		<< v.getPrevadzkoveNaklady()
		<< endl
		<< v.getSPZ()
		<< endl
		<< (v.getDruh() == TypTovaru::hranolceky ? 'h' : 'l')
		<< endl
		<< flush;

	return file;
}

fstream& operator>>(fstream& file, Vozidlo& v) {

	char pismenko;

	file
		>> v.datumZaradenia
		>> v.NOSNOST
		>> v.prevadzkoveNaklady
		>> v.SPZ
		>> pismenko;

	if (!platnyTypTovaru(pismenko)) {
		cout << "Vložili ste nesprávne písmeno pre typ tovaru!" << endl;
		return file;
	}

	v.druh = (pismenko == 'h' ? TypTovaru::hranolceky : TypTovaru::lupienky);

	return file;
}

bool Vozidlo::naplnVozidlo(Objednavka* o) {

	if (mnozstvo + o->getMnozstvo() <= NOSNOST) {
		mnozstvo += o->getMnozstvo();
		objednavky.push(o);
		return true;
	}
	return false;
}

void Vozidlo::spracujObjednavku(Objednavka * o) {

	if (mnozstvo - o->getMnozstvo() >= 0) {
		mnozstvo -= o->getMnozstvo();
		o->zrealizovana = true;
	}
	else {
		cout << "Exception vo Vozidlo::spracujObjednavku()!!!" << endl;
		return;
	}
}
