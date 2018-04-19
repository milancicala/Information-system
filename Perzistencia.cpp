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
	cout << "Objednávka úspešne uložená do súboru." << endl;
}

void Perzistencia::ulozZakaznika(const Zakaznik& z) {
	zakaznici << z << flush;
	cout << "Zákazník/ca úspešne uložený/á do súboru." << endl;
}

void Perzistencia::ulozVozidlo(const Vozidlo& v) {
	vozidla << v << flush;
	cout << "Vozidlo úspešne uložené do súboru." << endl;
}

void Perzistencia::ulozBiofarmara(const Biofarmar& b) {
	biofarmari << b << flush;
	cout << "Biafarmár úspešne uložený do súboru." << endl;
}

void Perzistencia::ulozNakup(const Nakup& n) {
	nakupy << n << flush;
	cout << "Nákup úspešne uložený do súboru." << endl;
}

bool Perzistencia::nacitajObjednavku(DatabazaObjednavok& dbObjednavok, const DatabazaVozidiel& dbVozidiel) {

	if (!objednavky.is_open()) {
		cout << "Objednávka nemohla by vložená do databázy!" << endl;
		cout << "Problém pri práci s externým súborom." << endl;
		return false;
	}

	Objednavka o;
	objednavky >> o;

	//kontroly stavu objednavky

	if (o.getDatumDorucenia() < dnesnyDatum + pocetSekundVdni * 7) {
		cout << "Objednávka nemôže by doruèená!" << endl;
		cout << "Poèet dní do doruèenia musí by aspoñ 7!" << endl;

		return false;
	}

	if (!dbVozidiel.volnaKapacita(o.getTypTovaru(), dbObjednavok, o.getDatumDorucenia(), o.getMnozstvo())) {
		cout << "Objednávka oznaèená ako zamietnutá!" << endl;
		cout << "Prekroèená celková kapacita vozidiel pre rozvoz daného tovaru." << endl;
		o.setZamietnuta();
	}

	//ukoncenie kontrol

	dbObjednavok.pridajPodlaDataRealizacie(o);

	cout << "Objednávka úspešne naèítaná." << endl;
	return true;
}

bool Perzistencia::nacitajZakaznika(DatabazaZakaznikov& dbZakaznikov) {
	if (!zakaznici.is_open()) {
		cout << "Zákazník nemohol by vložený do databázy!" << endl;
		cout << "Problém pri práci s externým súborom." << endl;
		return false;
	}

	Zakaznik z;
	zakaznici >> z;

	//kontroly stavu zakaznika

	if (z.getRegion() < 1 || z.getRegion() > 8) {
		cout << "Neplatné èíslo regiónu!" << endl;
		return false;
	}

	//ukoncenie kontrol

	if (!dbZakaznikov.pridajAbecedne(z)) {
		cout << "Zákazník nebol pridaný!" << endl;
		return false;
	}

	return true;
}

bool Perzistencia::nacitajVozidlo(DatabazaVozidiel& dbVozidiel) {
	if (!vozidla.is_open()) {
		cout << "Vozidlo nemohlo by vložené do databázy!" << endl;
		cout << "Problém pri práci s externým súborom." << endl;
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
			cout << "Typ vozidla nebol platný, vozidlo nebolo pridané!" << endl;
			return false;
		}

		return true;
}

bool Perzistencia::nacitajBiofarmara(DatabazaBiofarmarov& dbBiofarmarov) {
	if (!biofarmari.is_open()) {
		cout << "Biofarmar nemohol by vložený do databázy!" << endl;
		cout << "Problém pri práci s externým súborom." << endl;
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
		cout << "Nákup nemohol by vložený do databázy!" << endl;
		cout << "Problém pri práci s externým súborom." << endl;
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
		cout << "Vytváram nový súbor '" << nazov << "'." << endl;

		//vytvor novy subor
		ofstream newFile(nazov);
		newFile << flush;

		//asociuj ho s paramentrom objektu
		file.open(nazov);
	}

	if (!file.is_open()) {
		cout << "Vytvorenie súboru zlyhalo!" << endl;
		return false;
	}

	cout << "Súbor " << nazov << " úspešne naèítaný." << endl;

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
		cout << "Vložená objednávka." << endl;
	}

	for (auto pNakup : *dbNakupov.zoznam) {
		ulozNakup(*pNakup);
		cout << "Vložený nákup." << endl;
	}

	for (auto pBiofarmar : *dbBiofarmarov.zoznam) {
		ulozBiofarmara(*pBiofarmar);
		cout << "Vložený biofarmár." << endl;
	}

	for (auto pVozidlo : *dbVozidiel.zoznam) {
		ulozVozidlo(*pVozidlo);
		cout << "Vložené vozidlo." << endl;
	}

	for (auto pZakaznik : *dbZakaznikov.zoznam) {
		ulozZakaznika(*pZakaznik);
		cout << "Vložený zákazník." << endl;
	}
}

void Perzistencia::nacitaj(
	DatabazaObjednavok& dbObjednavok,
	DatabazaZakaznikov& dbZakaznikov,
	DatabazaVozidiel& dbVozidiel,
	DatabazaBiofarmarov& dbBiofarmarov,
	DatabazaNakupov& dbNakupov
) {
	cout << "Naèítavam vozidlá." << endl;

	vozidla >> std::ws;
	if (vozidla.eof()) {
		cout << "Databáza je prázdna." << endl;
	}

	while (vozidla.good()) {
		if (nacitajVozidlo(dbVozidiel)) {
			cout << "Naèítaný 1 objekt." << endl;
		}
		vozidla >> std::ws;
	}

	cout << "Naèítavam nákupy." << endl;

	nakupy >> std::ws;
	if (nakupy.eof()) {
		cout << "Databáza je prázdna." << endl;
	}

	while (nakupy.good()) {
		if (nacitajNakup(dbNakupov)) {
			cout << "Naèítaný 1 objekt." << endl;
		}
		nakupy >> std::ws;
	}

	cout << "Naèítavam zákazníkov." << endl;

	zakaznici >> std::ws;
	if (zakaznici.eof()) {
		cout << "Databáza je prázdna." << endl;
	}

	while (zakaznici.good()) {
		if (nacitajZakaznika(dbZakaznikov)) {
			cout << "Naèítaný 1 objekt." << endl;
		}
		zakaznici >> std::ws;
	}

	cout << "Naèítavam biofarmárov." << endl;

	biofarmari >> std::ws;
	if (biofarmari.eof()) {
		cout << "Databáza je prázdna." << endl;
	}

	while (biofarmari.good()) {
		if (nacitajBiofarmara(dbBiofarmarov)) {
			cout << "Naèítaný 1 objekt." << endl;
		}
		biofarmari >> std::ws;
	}

	cout << "Naèítavam objednávky." << endl;

	objednavky >> std::ws;
	if (objednavky.eof()) {
		cout << "Databáza je prázdna." << endl;
	}

	while (objednavky.good()) {
		if (nacitajObjednavku(dbObjednavok, dbVozidiel)) {
			cout << "Naèítaný 1 objekt." << endl;
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