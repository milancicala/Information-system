#include "Biofarmar.h"

#include <iostream>
using std::cout;
using std::endl;

#include "Struktury.h"
extern time_t dnesnyDatum;

Biofarmar::Biofarmar(string obchodnyNazov, produkty dodava)
	:obchodnyNazov(obchodnyNazov) {
	if (dodava.olej) {
		predajOleja = new PredajOleja;
	}

	if (dodava.zemiaky) {
		predajZemiakov = new PredajZemiakov;
	}

	if (dodava.ochucovadla) {
		predajOchucovadiel = new PredajOchucovadiel;
	}
};

string Biofarmar::getObchodnyNazov() const {
	return obchodnyNazov;
}

void Biofarmar::setObchodnyNazov(string obchodnyNazov) {
	this->obchodnyNazov = obchodnyNazov;
}

bool Biofarmar::dodavaOlej() const {
	return predajOleja != nullptr;
}

bool Biofarmar::dodavaZemiaky() const {
	return predajZemiakov != nullptr;
}

bool Biofarmar::dodavaOchucovadla() const {
	return predajOchucovadiel != nullptr;
}

ulong Biofarmar::predajMi(float mnozstvo,
	DruhyPolotovarov druh,
	DatabazaNakupov& dbNakupov) {
	if (
		(druh == DruhyPolotovarov::olej) && !dodavaOlej()
		||
		(druh == DruhyPolotovarov::zemiaky) && !dodavaZemiaky()
		||
		(druh == DruhyPolotovarov::ochucovadla) && !dodavaOchucovadla()
		) {
		cout << "Tento biofarmar nedodava dany polotovar." << endl;
		return 0;
	}

	PredajPolotovaru* predajPolotovaru;

	switch (druh) {
	case DruhyPolotovarov::olej:
		predajPolotovaru = predajOleja;
		break;
	case DruhyPolotovarov::zemiaky:
		predajPolotovaru = predajZemiakov;
		break;
	case DruhyPolotovarov::ochucovadla:
		predajPolotovaru = predajOchucovadiel;
	default:
		cout << "Zadany druh polotovaru neexistuje!";
		return false;
	}

	ulong mnozstvoPred = predajPolotovaru->mnozstvo;
	if (mnozstvo <= predajPolotovaru->mnozstvo) {
		predajPolotovaru->mnozstvo -= mnozstvo;
	}
	else {
		predajPolotovaru->mnozstvo = 0;
	}
	ulong mnozstvoPo = predajPolotovaru->mnozstvo;

	// vloz do databazy zaznam o predaji
	dbNakupov.pridaj(
		Nakup(
			dnesnyDatum,
			this->obchodnyNazov,
			druh,
			mnozstvo,
			predajPolotovaru->cena
		)
	);

	return mnozstvoPred - mnozstvoPo;
}

fstream& operator<<(fstream& file, const Biofarmar& b) {

	file
		<< b.getObchodnyNazov()
		<< endl
		<< b.dodavaOchucovadla()
		<< endl
		<< b.dodavaOlej()
		<< endl
		<< b.dodavaZemiaky()
		<< endl;

	return file;
}

fstream& operator>> (fstream& file, Biofarmar& b) {

	struct produkty dodava;

	file
		>> b.obchodnyNazov
		>> dodava.ochucovadla
		>> dodava.olej
		>> dodava.zemiaky;

	if (dodava.ochucovadla) {
		b.predajOchucovadiel = new PredajOchucovadiel;
	}

	if (dodava.olej) {
		b.predajOleja = new PredajOleja;
	}

	if (dodava.zemiaky) {
		b.predajZemiakov = new PredajZemiakov;
	}

	return file;
}

Biofarmar::Biofarmar(const Biofarmar& iny) {
	obchodnyNazov = iny.obchodnyNazov;

	if (iny.dodavaOchucovadla()) {
		predajOchucovadiel = new PredajOchucovadiel;
	}

	if (iny.dodavaOlej()) {
		predajOleja = new PredajOleja;
	}

	if (iny.dodavaZemiaky()) {
		predajZemiakov = new PredajZemiakov;
	}
}

Biofarmar::~Biofarmar() noexcept {
	if (dodavaOchucovadla()) {
		delete predajOchucovadiel;
		predajOchucovadiel = nullptr;
	}

	if (dodavaOlej()) {
		delete predajOleja;
		predajOleja = nullptr;
	}

	if (dodavaZemiaky()) {
		delete predajZemiakov;
		predajZemiakov = nullptr;
	}
}