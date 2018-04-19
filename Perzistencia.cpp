#include "Perzistencia.h"

#include <fstream>
using std::fstream;
using std::ofstream;

#include <iostream>
using std::cout;
using std::endl;
using std::flush;

void Perzistencia::ulozObjednavku(const Objednavka& o) {
	objednavky << o << flush;
	cout << "Objedn�vka �spe�ne ulo�en� do s�boru." << endl;
}

void Perzistencia::ulozZakaznika(const Zakaznik& z) {
	zakaznici << z << flush;
	cout << "Z�kazn�k/ca �spe�ne ulo�en�/� do s�boru." << endl;
}

void Perzistencia::ulozVozidlo(const Vozidlo& v) {
	vozidla << v << flush;
	cout << "Vozidlo �spe�ne ulo�en� do s�boru." << endl;
}

void Perzistencia::ulozBiofarmara(const Biofarmar& b) {
	biofarmari << b << flush;
	cout << "Biafarm�r �spe�ne ulo�en� do s�boru." << endl;
}

void Perzistencia::ulozNakup(const Nakup& n) {
	nakupy << n << flush;
	cout << "N�kup �spe�ne ulo�en� do s�boru." << endl;
}

bool Perzistencia::nacitajObjednavku(DatabazaObjednavok& dbObjednavok, const DatabazaVozidiel& dbVozidiel) {

	if (!objednavky.is_open()) {
		cout << "Objedn�vka nemohla by� vlo�en� do datab�zy!" << endl;
		cout << "Probl�m pri pr�ci s extern�m s�borom." << endl;
		return false;
	}

	Objednavka o;
	objednavky >> o;

	//kontroly stavu objednavky

	if (o.getDatumDorucenia() < dnesnyDatum + pocetSekundVdni * 7) {
		cout << "Objedn�vka nem��e by� doru�en�!" << endl;
		cout << "Po�et dn� do doru�enia mus� by� aspo� 7!" << endl;

		return false;
	}

	if (!dbVozidiel.volnaKapacita(o.getTypTovaru(), dbObjednavok, o.getDatumDorucenia(), o.getMnozstvo())) {
		cout << "Objedn�vka ozna�en� ako zamietnut�!" << endl;
		cout << "Prekro�en� celkov� kapacita vozidiel pre rozvoz dan�ho tovaru." << endl;
		o.setZamietnuta();
	}

	//ukoncenie kontrol

	dbObjednavok.pridajPodlaDataRealizacie(o);

	cout << "Objedn�vka �spe�ne na��tan�." << endl;
	return true;
}

bool Perzistencia::nacitajZakaznika(DatabazaZakaznikov& dbZakaznikov) {
	if (!zakaznici.is_open()) {
		cout << "Z�kazn�k nemohol by� vlo�en� do datab�zy!" << endl;
		cout << "Probl�m pri pr�ci s extern�m s�borom." << endl;
		return false;
	}

	Zakaznik z;
	zakaznici >> z;

	//kontroly stavu zakaznika

	if (z.getRegion() < 1 || z.getRegion() > 8) {
		cout << "Neplatn� ��slo regi�nu!" << endl;
		return false;
	}

	//ukoncenie kontrol

	if (!dbZakaznikov.pridajAbecedne(z)) {
		cout << "Z�kazn�k nebol pridan�!" << endl;
		return false;
	}

	return true;
}

bool Perzistencia::nacitajVozidlo(DatabazaVozidiel& dbVozidiel) {
	if (!vozidla.is_open()) {
		cout << "Vozidlo nemohlo by� vlo�en� do datab�zy!" << endl;
		cout << "Probl�m pri pr�ci s extern�m s�borom." << endl;
		return false;
	}

	Vozidlo v;
	vozidla >> v;

	//kontroly stavu vozidla
	//...nie je potrebna
	//ukoncenie kontrol

	if (v.getDruh() == TypTovaru::hranolceky) {
		dbVozidiel.pridajChronologicky(new VozidloHranolceky(v.getSPZ()));
	}
	else
		if (v.getDruh() == TypTovaru::lupienky) {
			dbVozidiel.pridajChronologicky(new VozidloLupienky(v.getSPZ()));
		}
		else {
			cout << "Typ vozidla nebol platn�, vozidlo nebolo pridan�!" << endl;
			return false;
		}

		return true;
}

bool Perzistencia::nacitajBiofarmara(DatabazaBiofarmarov& dbBiofarmarov) {
	if (!biofarmari.is_open()) {
		cout << "Biofarmar nemohol by� vlo�en� do datab�zy!" << endl;
		cout << "Probl�m pri pr�ci s extern�m s�borom." << endl;
		return false;
	}

	Biofarmar* b = new Biofarmar;
	biofarmari >> *b;

	//kontrola stavu biofarmara
	//...nie je potrebna
	//ukoncenie kontrol

	dbBiofarmarov.pridajAbecedne(b);

	return true;
}

bool Perzistencia::nacitajNakup(DatabazaNakupov& dbNakupov) {
	if (!nakupy.is_open()) {
		cout << "N�kup nemohol by� vlo�en� do datab�zy!" << endl;
		cout << "Probl�m pri pr�ci s extern�m s�borom." << endl;
		return false;
	}

	Nakup n;
	nakupy >> n;

	//kontrola stavu nakupu
	//...nie je potrebna
	//ukoncenie kontrol

	dbNakupov.pridaj(n);
	return true;
}

bool Perzistencia::asociujSubor(const string& nazov, fstream& file) {
	file.open(nazov, fstream::in | fstream::out | fstream::app);

	//ak subor neexistuje
	if (!file.is_open()) {
		cout << "Vytv�ram nov� s�bor '" << nazov << "'." << endl;

		//vytvor novy subor
		ofstream newFile(nazov);
		newFile << flush;

		//asociuj ho s paramentrom objektu
		file.open(nazov);
	}

	if (!file.is_open()) {
		cout << "Vytvorenie s�boru zlyhalo!" << endl;
		return false;
	}

	cout << "S�bor " << nazov << " �spe�ne na��tan�." << endl;

	return true;
}

Perzistencia::Perzistencia() {
	asociujSubor("objednavky.dat", objednavky);
	asociujSubor("zakaznici.dat", zakaznici);
	asociujSubor("vozidla.dat", vozidla);
	asociujSubor("biofarmari.dat", biofarmari);
	asociujSubor("nakupy.dat", nakupy);
}

Perzistencia::~Perzistencia() {
	objednavky.close();
	zakaznici.close();
	vozidla.close();
	biofarmari.close();
	nakupy.close();
}

void Perzistencia::uloz(
	DatabazaObjednavok& dbObjednavok,
	DatabazaZakaznikov& dbZakaznikov,
	DatabazaVozidiel& dbVozidiel,
	DatabazaBiofarmarov& dbBiofarmarov,
	DatabazaNakupov& dbNakupov
) {
	for (auto pObjednavka : *dbObjednavok.zoznam) {
		ulozObjednavku(*pObjednavka);
		cout << "Vlo�en� objedn�vka." << endl;
	}

	for (auto pNakup : *dbNakupov.zoznam) {
		ulozNakup(*pNakup);
		cout << "Vlo�en� n�kup." << endl;
	}

	for (auto pBiofarmar : *dbBiofarmarov.zoznam) {
		ulozBiofarmara(*pBiofarmar);
		cout << "Vlo�en� biofarm�r." << endl;
	}

	for (auto pVozidlo : *dbVozidiel.zoznam) {
		ulozVozidlo(*pVozidlo);
		cout << "Vlo�en� vozidlo." << endl;
	}

	for (auto pZakaznik : *dbZakaznikov.zoznam) {
		ulozZakaznika(*pZakaznik);
		cout << "Vlo�en� z�kazn�k." << endl;
	}
}

void Perzistencia::nacitaj(
	DatabazaObjednavok& dbObjednavok,
	DatabazaZakaznikov& dbZakaznikov,
	DatabazaVozidiel& dbVozidiel,
	DatabazaBiofarmarov& dbBiofarmarov,
	DatabazaNakupov& dbNakupov
) {
	cout << "Na��tavam vozidl�." << endl;

	vozidla >> std::ws;
	if (vozidla.eof()) {
		cout << "Datab�za je pr�zdna." << endl;
	}

	while (vozidla.good()) {
		if (nacitajVozidlo(dbVozidiel)) {
			cout << "Na��tan� 1 objekt." << endl;
		}
		vozidla >> std::ws;
	}

	cout << "Na��tavam n�kupy." << endl;

	nakupy >> std::ws;
	if (nakupy.eof()) {
		cout << "Datab�za je pr�zdna." << endl;
	}

	while (nakupy.good()) {
		if (nacitajNakup(dbNakupov)) {
			cout << "Na��tan� 1 objekt." << endl;
		}
		nakupy >> std::ws;
	}

	cout << "Na��tavam z�kazn�kov." << endl;

	zakaznici >> std::ws;
	if (zakaznici.eof()) {
		cout << "Datab�za je pr�zdna." << endl;
	}

	while (zakaznici.good()) {
		if (nacitajZakaznika(dbZakaznikov)) {
			cout << "Na��tan� 1 objekt." << endl;
		}
		zakaznici >> std::ws;
	}

	cout << "Na��tavam biofarm�rov." << endl;

	biofarmari >> std::ws;
	if (biofarmari.eof()) {
		cout << "Datab�za je pr�zdna." << endl;
	}

	while (biofarmari.good()) {
		if (nacitajBiofarmara(dbBiofarmarov)) {
			cout << "Na��tan� 1 objekt." << endl;
		}
		biofarmari >> std::ws;
	}

	cout << "Na��tavam objedn�vky." << endl;

	objednavky >> std::ws;
	if (objednavky.eof()) {
		cout << "Datab�za je pr�zdna." << endl;
	}

	while (objednavky.good()) {
		if (nacitajObjednavku(dbObjednavok, dbVozidiel)) {
			cout << "Na��tan� 1 objekt." << endl;
		}
		objednavky >> std::ws;
	}
}

void Perzistencia::vratSaNaZaciatok() {
	objednavky.clear();
	objednavky.seekg(0, std::ios::beg);

	zakaznici.clear();
	zakaznici.seekg(0, std::ios::beg);

	vozidla.clear();
	vozidla.seekg(0, std::ios::beg);

	biofarmari.clear();
	biofarmari.seekg(0, std::ios::beg);

	nakupy.clear();
	nakupy.seekg(0, std::ios::beg);
}