#include "Struktury.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

ulong epochaNaPocetDni(time_t epocha) {
	return epocha / (60 * 60 * 24);
}

time_t pocetDniNaEpochu(ulong pocetDni) {
	return pocetDni * 24 * 60 * 60;
}

time_t nacitajCas() {
	tm tm;
	tm.tm_sec = 0;
	tm.tm_min = 0;
	tm.tm_hour = 0;
	tm.tm_isdst = 0;

	cout << "Vložte deñ [1-31]: ";
	cin >> tm.tm_mday;

	cout << "Vložte mesiac [1-12]: ";
	cin >> tm.tm_mon;
	--tm.tm_mon;

	int rok;
	cout << "Vložte rok [>=1900]: ";
	cin >> rok;
	tm.tm_year = rok - 1900;

	return mktime(&tm);
}