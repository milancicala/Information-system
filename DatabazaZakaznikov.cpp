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
	cout << "Vložte obchodný názov: ";
	cin >> obchodnyNazov;
	cout << endl;

	ulong region;
	cout << "Vložte adresu (èíslo regiónu): ";
	cin >> region;
	cout << endl;

	if (!platnyRegion(region)) {
		while (!platnyRegion(region)) {
			cout << "Vložili ste nesprávne èíslo regiónu." << endl;
			cout << "Vložili prosím hodnotu od 1 do 8." << endl;
			cout << "Vložte adresu (èíslo regiónu): ";
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
			cout << "Chyba: zvolený obchodný názov už existuje!" << endl;
			cout << "Zadaný zákazník nebol vložený" << endl;
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
		cout << "Databáza zákazníkov je prázdna." << endl;
		return;
	}

	for (auto prvok : *zoznam) {
		cout << "Zákazník:" << endl;
		cout << "obchodný názov: " << prvok->getObchodnyNazov() << endl;
		cout << "adresa (èíslo regiónu): " << prvok->getRegion() << endl;
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