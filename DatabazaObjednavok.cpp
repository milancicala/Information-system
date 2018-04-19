#include "DatabazaObjednavok.h"

#include <ctime>
using std::ctime;

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

const string ODDELOVAC = "- - - - - - - - - -";
extern time_t dnesnyDatum;

DatabazaObjednavok::DatabazaObjednavok()
	: zoznam(new structures::LinkedList<Objednavka*>) {}

DatabazaObjednavok::DatabazaObjednavok(
	const structures::LinkedList<Objednavka*>*& inyZoznam
) {
	zoznam = new structures::LinkedList<Objednavka*>(*inyZoznam);
}

DatabazaObjednavok::~DatabazaObjednavok() noexcept {

	for (auto prvok : *zoznam) {
		delete prvok;
	}

	delete zoznam;

}

void DatabazaObjednavok::pridajPodlaDataRealizacie(const Objednavka& vkladany) {

	Objednavka* ukVkladany = new Objednavka(
		vkladany.getDatumDorucenia(),
		vkladany.getDatumZaevidovania(),
		vkladany.getTypTovaru(),
		vkladany.getMnozstvo(),
		vkladany.getCena(),
		vkladany.getZakaznik()
	);

	vkladany.getZamietnuta() ? ukVkladany->setZamietnuta() : false;
	vkladany.getZrusena() ? ukVkladany->setZrusena() : false;

	// pokial vkladany datum je neskorsi ako vlozeny tak preskakuj
	for (size_t i = 0; i < zoznam->size(); i++) {
		if (
			vkladany.getDatumDorucenia()
	>
			(*zoznam)[i]->getDatumDorucenia()
			) {
			continue;
		}
		else {
			zoznam->insert(ukVkladany, i);
			return;
		}
	}

	zoznam->add(ukVkladany);
}

bool DatabazaObjednavok::pridaj(const DatabazaVozidiel& dbVozidiel) {
	string zakaznik;
	cout << "Vlo�te obchodn� n�zov z�kazn�ka: ";
	cin >> zakaznik;
	cout << endl;

	char vlozenyTyp;
	cout << "Vlo�te typ tovaru ('h' pre hranolceky, 'l' pre lupienky): ";
	cin >> vlozenyTyp;

	if (!platnyTypTovaru(vlozenyTyp)) {
		while (!platnyTypTovaru(vlozenyTyp)) {
			cout << endl;
			cout << "Vlo�ili ste nespr�vne p�smeno pre typ tovaru." << endl;
			cout << "Vlo�te typ tovaru ('h' pre hranolceky, 'l' pre lupienky): ";
			cin >> vlozenyTyp;
			cout << endl;
		}
	}

	TypTovaru typ;
	switch (vlozenyTyp) {
	case 'h':
		typ = TypTovaru::hranolceky;
		break;
	case 'l':
		typ = TypTovaru::lupienky;
		break;
	default:
		cout << "Typ tovaru nebol platn�, objedn�vka nebola pridan�!" << endl;
		return false;
	}

	ulong mnozstvo;
	cout << "Vlozte mno�stvo objedn�van�ho tovaru: ";
	cin >> mnozstvo;

	ulong cena;
	cout << "Vlozte predajn� cenu: ";
	cin >> cena;

	ulong pocetDniKuDoruceniu;
	cout << "O ko�ko dn� m� by� objedn�vka doru�en�? ";
	cin >> pocetDniKuDoruceniu;

	if (pocetDniKuDoruceniu < 7) {
		cout << endl;
		cout << "Objedn�vka nem��e by� doru�en�!" << endl;
		cout << "Po�et dn� do doru�enia mus� by� aspo� 7!" << endl;
		cout << endl;

		return false;
	}

	time_t datumDorucenia;
	datumDorucenia = dnesnyDatum + pocetDniNaEpochu(pocetDniKuDoruceniu);

	Objednavka o(
		datumDorucenia,
		dnesnyDatum,
		typ,
		mnozstvo,
		cena,
		zakaznik
	);

	bool zamietnuta = false;
	if (!dbVozidiel.volnaKapacita(typ, *this, datumDorucenia, mnozstvo)) {
		cout << "Objedn�vka ozna�en� ako zamietnut�!" << endl;
		cout << "Prekro�en� celkov� kapacita vozidiel pre rozvoz dan�ho tovaru." << endl;
		zamietnuta = true;
		o.setZamietnuta();
	}

	pridajPodlaDataRealizacie(o);

	zamietnuta == false ? cout << "Objedn�vka bola �spe�ne zadan� do syst�mu." << endl : cout << "Objedn�vka bola �spe�ne zadan� do syst�mu a ozna�en� ako zamietnut�." << endl;
	cout << endl;

	return true;
}

void DatabazaObjednavok::vypis(const DatabazaZakaznikov& dbZakaznikov, const DatabazaNakupov& dbNakupov) const {

	if (zoznam->isEmpty()) {
		cout << "Datab�za objedn�vok je pr�zdna." << endl;
		cout << endl;

		return;
	}

	for (auto pObjednavka : *zoznam) {
		pObjednavka->vypis(dbZakaznikov, dbNakupov);
	}
}

//zrusenie objednavky na dalsi den, ak je nedostacujuci pocet polotovarov a rezervovanie polotovarov na sklade pre danu objednavku
void DatabazaObjednavok::zrusObjednavkyPreNedostatokPolotovarov(DatabazaNakupov& dbNakupov) {
	ulong pocetZrusenych = 0;
	for (auto pObjednavka : *zoznam) {
		ulong dnesnyDen = epochaNaPocetDni(dnesnyDatum);
		ulong denDorucenia = epochaNaPocetDni(pObjednavka->getDatumDorucenia());

		if (denDorucenia == dnesnyDen + 1) {
			bool existuje = dbNakupov.existujeDostatokPolotovarov(*pObjednavka);
			if (!existuje)
			{
				cout << "Prieha zru�enie objedn�vky" << endl;
				pocetZrusenych++;
				pObjednavka->setZrusena();
			}
			else 
			{
				dbNakupov.odpocitajMnozstvoTovaru(*pObjednavka);
			}
		}
	}
	cout << "Po�et zru�en�ch objedn�vok na zajtra: " << pocetZrusenych << endl << endl;
}

void DatabazaObjednavok::vypisNaZrealizovanieZajtra(const DatabazaZakaznikov& dbZakaznikov, const DatabazaNakupov& dbNakupov) const {
	for (auto pObjednavka : *zoznam) {
		ulong dnesnyDen = epochaNaPocetDni(dnesnyDatum);
		ulong denDorucenia = epochaNaPocetDni(pObjednavka->getDatumDorucenia());

		if (denDorucenia == dnesnyDen + 1 && !pObjednavka->getZamietnuta() && !pObjednavka->getZrusena()) {
			pObjednavka->vypis(dbZakaznikov, dbNakupov);
		}
	}
}

ulong DatabazaObjednavok::zisk() const {

	ulong celkoveMnozstvo = 0;
	ulong celkovaCena = 0;

	for (auto pObjednavka : *zoznam) {
		celkoveMnozstvo += pObjednavka->getMnozstvo();
		celkovaCena += pObjednavka->getCena();
	}

	return celkoveMnozstvo * celkovaCena;
}

ulong DatabazaObjednavok::kDodaniuDoTyzdna(TypTovaru typTovaru) const {
	ulong mnozstvoKdodaniu = 0;

	for (auto objednavka : *zoznam) {
		if (typTovaru == objednavka->getTypTovaru()) {
			if (
				objednavka->getDatumDorucenia() >= dnesnyDatum
				&&
				objednavka->getDatumDorucenia() <= dnesnyDatum + pocetSekundVdni * 7
				&&
				!objednavka->getZamietnuta()
				&&
				!objednavka->getZrusena()
				) {
				if (objednavka->getDatumDorucenia() > dnesnyDatum + pocetSekundVdni * 1) //objednavky na zajtrajsi den budu zrusene
				{
					mnozstvoKdodaniu += objednavka->getMnozstvo();
				}
				else 
				{
					objednavka->setZrusena();
					cout << "Objedn�vka pre z�kazn�ka: " << objednavka->getZakaznik() << " ,ktor� mala by� doru�en� zajtra, bude zru�en� z d�vodu nedostatku polotvarov." << endl;
				}
				
			}
		}
	}

	return mnozstvoKdodaniu;
}

void DatabazaObjednavok::vypisZrealizovane(time_t od, time_t po, const DatabazaZakaznikov& dbZakaznikov, const DatabazaNakupov& dbNakupov) const {
	for (auto pObjednavka : *zoznam) {
		if (pObjednavka->getDatumDorucenia() >= od && pObjednavka->getDatumDorucenia() <= po) {
			if (!pObjednavka->getZamietnuta() && !pObjednavka->getZrusena()) {
				pObjednavka->vypis(dbZakaznikov, dbNakupov);
				cout << "Celkov� tr�by zisk:" << pObjednavka->getCena() * pObjednavka->getMnozstvo() << endl;
			}
		}
	}
}

void DatabazaObjednavok::vypisNezrealizovane(time_t od, time_t po, const DatabazaZakaznikov& dbZakaznikov, const DatabazaNakupov& dbNakupov) const {
	for (auto pObjednavka : *zoznam) {
		if (pObjednavka->getDatumDorucenia() >= od && pObjednavka->getDatumDorucenia() <= po) {
			if (pObjednavka->getZamietnuta() || pObjednavka->getZrusena()) {
				pObjednavka->vypis(dbZakaznikov, dbNakupov);
				cout << "U�l� zisk:" << pObjednavka->getCena() * pObjednavka->getMnozstvo() << endl;
			}
		}
	}
}

void DatabazaObjednavok::vypisPocetHmotnostTrzbyZrusenychAzamietnutych(ulong region, time_t od, time_t po, const DatabazaZakaznikov& dbZakaznikov) const {
	for (auto pZakaznik : *dbZakaznikov.zoznam) {
		if (region == pZakaznik->getRegion()) {
			ulong celkovyPrijem = 0;

			ulong pocetZrusenych = 0;
			ulong hmotnostZrusenych = 0;
			ulong cenaZrusenych = 0;

			ulong pocetZamietnutych = 0;
			ulong hmotnostZamietnutych = 0;
			ulong cenaZamietnutych = 0;

			for (auto pObjednavka : *zoznam) {
				if (epochaNaPocetDni(pObjednavka->getDatumDorucenia()) >= od && epochaNaPocetDni(pObjednavka->getDatumDorucenia()) <= po && pZakaznik->getObchodnyNazov() == pObjednavka->getZakaznik()) {
					if (pObjednavka->getZrealizovana()) {
						celkovyPrijem += pObjednavka->getMnozstvo() * pObjednavka->getCena();
					}

					if (pObjednavka->getZrusena()) {
						++pocetZrusenych;
						hmotnostZrusenych += pObjednavka->getMnozstvo();
						cenaZrusenych += pObjednavka->getCena();
					}

					if (pObjednavka->getZamietnuta()) {
						++pocetZamietnutych;
						hmotnostZamietnutych += pObjednavka->getMnozstvo();
						cenaZamietnutych += pObjednavka->getCena();
					}
				}
			}

			cout << "z�kazn�k: " << pZakaznik->getObchodnyNazov() << endl;

			cout << "celkov� pr�jem: " << celkovyPrijem << endl;

			cout << "zru�en� objedn�vky:" << endl;
			cout << "po�et: " << pocetZrusenych << endl;
			cout << "hmotnos�: " << hmotnostZrusenych << endl;
			cout << "tr�by: " << hmotnostZrusenych * cenaZrusenych << endl;

			cout << "zamietnut� objedn�vky:" << endl;
			cout << "po�et: " << pocetZamietnutych << endl;
			cout << "hmotnos�: " << hmotnostZamietnutych << endl;
			cout << "tr�by: " << hmotnostZamietnutych * cenaZamietnutych << endl;
		}
	}
}