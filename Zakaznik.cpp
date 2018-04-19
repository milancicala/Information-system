#include "Zakaznik.h"

#include <iostream>
using std::endl;

fstream& operator<<(fstream& file, const Zakaznik& z) {

	file
		<< z.getObchodnyNazov()
		<< endl
		<< z.getRegion()
		<< endl;

	return file;
}

fstream& operator>>(fstream& file, Zakaznik& z) {

	file
		>> z.obchodnyNazov
		>> z.region;

	return file;
}