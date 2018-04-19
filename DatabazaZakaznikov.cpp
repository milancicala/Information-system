#include "DatabazaZakaznikov.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

DatabazaZakaznikov::DatabazaZakaznikov()
	:zoznam(new structures::LinkedList<Zakaznik*>) {}

DatabazaZakaznikov::~DatabazaZakaznikov() noexcept /* explicitly specified destructors should be annotated noexcept as best-practice */
{
	for (auto pZakaznik : *zoznam) {
		delete pZakaznik;
		pZakaznik = nullptr;
	}

	delete zoznam;
	zoznam = nullptr;
}

//vrati true ak je parameter platnou adresou zakaznika
bool platnyRegion(ulong region) {
	return region >= 1 && region <= 8;
}

bool DatabazaZakaznikov::Registruj()
{
	string obchodnyNazov;
	cout << "Vlo�te obchodn� n�zov: ";
	cin >> obchodnyNazov;
	cout << endl;

	ulong region;
	cout << "Vlo�te adresu (��slo regi�nu): ";
	cin >> region;
	cout << endl;

	if (!platnyRegion(region)) {
		while (!platnyRegion(region)) {
			cout << "Vlo�ili ste nespr�vne ��slo regi�nu." << endl;
			cout << "Vlo�ili pros�m hodnotu od 1 do 8." << endl;
			cout << "Vlo�te adresu (��slo regi�nu): ";
			cin >> region;
			cout << endl;
		}
	}

	Zakaznik z(obchodnyNazov, region);

	pridajAbecedne(z);

	return true;
}

bool DatabazaZakaznikov::pridajAbecedne(const Zakaznik& vkladany) {

	for (auto prvok : *zoznam) {

		if (vkladany.getObchodnyNazov() == prvok->getObchodnyNazov()) {
			cout << "Chyba: zvolen� obchodn� n�zov u� existuje!" << endl;
			cout << "Zadan� z�kazn�k nebol vlo�en�" << endl;
			return false;
		}

		if (
			vkladany.getObchodnyNazov().compare(prvok->getObchodnyNazov()) < 0
			) {
			zoznam->insert(
				new Zakaznik(
					vkladany.getObchodnyNazov(),
					vkladany.getRegion()
				),
				zoznam->getIndexOf(prvok)
			);
			return true;
		}
	}

	zoznam->add(
		new Zakaznik(
			vkladany.getObchodnyNazov(),
			vkladany.getRegion()
		)
	);

	return true;
}

void DatabazaZakaznikov::vypis() const {

	if (zoznam->isEmpty()) {
		cout << "Datab�za z�kazn�kov je pr�zdna." << endl;
		return;
	}

	for (auto prvok : *zoznam) {
		cout << "Z�kazn�k:" << endl;
		cout << "obchodn� n�zov: " << prvok->getObchodnyNazov() << endl;
		cout << "adresa (��slo regi�nu): " << prvok->getRegion() << endl;
		cout << endl;
	}
}

ulong DatabazaZakaznikov::ziskajRegion(string nazovZakaznika) const {
	for (auto pZakaznik : *zoznam) {
		if (nazovZakaznika == pZakaznik->getObchodnyNazov()) {
			return pZakaznik->getRegion();
			break;
		}
	}

	return 0;
}