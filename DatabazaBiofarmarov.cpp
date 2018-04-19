#include "DatabazaBiofarmarov.h"
#include "StrukturyPrePolotovary.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

void DatabazaBiofarmarov::pridaj() {

	string obchodnyNazov;
	cout << "Vlo�te obchodn� n�zov: ";
	cin >> obchodnyNazov;
	cout << endl;

	char odpoved = 'n';
	produkty dodava;

	cout << "Dod�va tento farm�r zemiaky? (a/n): ";
	cin >> odpoved;
	if (odpoved == 'a') dodava.zemiaky = true;

	cout << "Dod�va tento farm�r olej? (a/n): ";
	cin >> odpoved;
	if (odpoved == 'a') dodava.olej = true;

	cout << "Dod�va tento farm�r ochucovadl�? (a/n): ";
	cin >> odpoved;
	if (odpoved == 'a') dodava.ochucovadla = true;

	cout << endl;

	pridajAbecedne(new Biofarmar(obchodnyNazov, dodava));
}

DatabazaBiofarmarov::DatabazaBiofarmarov()
	: zoznam(new structures::LinkedList<Biofarmar*>) {}

DatabazaBiofarmarov::~DatabazaBiofarmarov() noexcept {
	for (auto pBiofarmar : *zoznam) {
		delete pBiofarmar;
		pBiofarmar = nullptr;
	}

	delete zoznam;
	zoznam = nullptr;
}

void DatabazaBiofarmarov::pridajAbecedne(Biofarmar* bf) {

	if (zoznam->isEmpty()) {
		zoznam->add(bf);
		return;
	}

	// vloz do zoznamu abecedne
	bool bolVlozeny = false;
	string nazovVlozeneho, nazovVkladaneho;

	for (long i = 0; i < zoznam->size(); i++) {

		nazovVlozeneho = (*zoznam)[i]->getObchodnyNazov();
		nazovVkladaneho = bf->getObchodnyNazov();

		// obchodny nazov ma byt unikatny 		
		if (nazovVlozeneho == nazovVkladaneho) {
			cout << "Chyba: zvolen� obchodn� n�zov u� existuje!" << endl;
			cout << "Zadan� biofarm�r nebol vlo�en�" << endl;
			return;
		}

		if (nazovVlozeneho.compare(nazovVkladaneho) > 0) {
			zoznam->insert(bf, i);
			bolVlozeny = true;
			break;
		}
		else {
			continue;
		}
	}

	if (!bolVlozeny) {
		zoznam->add(bf);
	}

	cout << endl;
}

void DatabazaBiofarmarov::vypis(const DatabazaNakupov& dbNakupov) const {

	if (zoznam->isEmpty()) {
		cout << "Zoznam biofarm�rov je pr�zdny." << endl << endl;
		return;
	}

	// vypisujem zoznam biofarmarov
	for (long i = 0; i < zoznam->size(); i++) {
		cout << endl;
		cout << "Obchodn� n�zov = '" << (*zoznam)[i]->getObchodnyNazov() << "'." << endl;

		if (!(*zoznam)[i]->dodavaOchucovadla() &&
			!(*zoznam)[i]->dodavaZemiaky() &&
			!(*zoznam)[i]->dodavaOlej()) {
			cout << "Tento farm�r ni� nedod�va.";
			continue;
		}

		cout << "Tento farm�r dod�va ";

		if ((*zoznam)[i]->dodavaOchucovadla()) {
			cout << "ochucovadl� za priemern� cenu "
				<< dbNakupov.priemer((*zoznam)[i]->getObchodnyNazov(),
					DruhyPolotovarov::ochucovadla)
				<< endl;
		}

		if ((*zoznam)[i]->dodavaOlej()) {
			cout << "olej za priemern� cenu "
				<< dbNakupov.priemer((*zoznam)[i]->getObchodnyNazov(),
					DruhyPolotovarov::olej)
				<< endl;
		}

		if ((*zoznam)[i]->dodavaZemiaky()) {
			cout << "zemiaky za priemern� cenu "
				<< dbNakupov.priemer((*zoznam)[i]->getObchodnyNazov(),
					DruhyPolotovarov::zemiaky)
				<< endl;
		}
	}

	cout << endl;
}