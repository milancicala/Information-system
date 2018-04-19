#include "Nakup.h"

#include <iostream>
using std::endl;

Nakup::Nakup(
	time_t datum,
	string obchodnyNazov,
	DruhyPolotovarov druh,
	ulong mnozstvo,
	ulong cena
) :
	datum(datum),
	obchodnyNazov(obchodnyNazov),
	druh(druh),
	mnozstvo(mnozstvo),
	cena(cena) {
}

fstream& operator<<(fstream& file, const Nakup& n) {
	file
		<< n.obchodnyNazov
		<< endl
		<< n.datum
		<< endl
		<< n.mnozstvo
		<< endl
		<< n.cena
		<< endl;

	return file;
}

fstream& operator>>(fstream& file, Nakup& n) {
	file
		>> n.obchodnyNazov
		>> n.datum
		>> n.mnozstvo
		>> n.cena;

	return file;
}