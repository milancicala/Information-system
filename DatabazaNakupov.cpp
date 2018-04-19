#include "DatabazaNakupov.h"
#include "structures\priority_queue\priority_queue_unsorted_array_list.h"
#include "structures\list\linked_list.h"

#include <iostream>
using std::cout;
using std::endl;
using std::cin;

void DatabazaNakupov::pridaj(const Nakup& nakup) {

	switch (nakup.druh) {
	case DruhyPolotovarov::olej:
		mnozstvoOleja += nakup.mnozstvo;
		break;
	case DruhyPolotovarov::zemiaky:
		mnozstvoZemiakov += nakup.mnozstvo;
		break;
	case DruhyPolotovarov::ochucovadla:
		mnozstvoOchucovadla += nakup.mnozstvo;
		break;
	}

	zoznam->add(new Nakup(nakup));
}

void DatabazaNakupov::vypis() {
	if (zoznam->isEmpty()) {
		cout << "Od žiadneho farmára sa doteraz nenakupovalo." << endl;
		return;
	}

	// vypisujem zoznam nákupov od biofarmárov
	for (long i = 0; i < zoznam->size(); i++) {
		cout << "Biofarmar:" << endl;
		cout << "obchodný názov '" << (*zoznam)[i]->obchodnyNazov << endl;
		cout << "deò nákupu '" << (*zoznam)[i]->datum << endl;
		cout << "nakúpené množstvo '" << (*zoznam)[i]->mnozstvo << endl;
		cout << "za cenu '" << (*zoznam)[i]->cena << endl;

		cout << "zakúpený polotovar '";
		switch ((*zoznam)[i]->druh) {
		case DruhyPolotovarov::olej:
			cout << "olej";
			break;
		case DruhyPolotovarov::zemiaky:
			cout << "zemiaky";
			break;
		case DruhyPolotovarov::ochucovadla:
			cout << "ochucovadlá";
			break;
		default:
			cout << "neznámeho druhu'!";
			return;
		}
		cout << "'" << endl << endl;
	}
}

bool platnyPolotovar(char pismenko) {
	return pismenko == 'z' || pismenko == 'j' || pismenko == 'o';
}

void DatabazaNakupov::vypisNajBiofarmara() const {
	char pismenko;

	cout << "Vložte prosím druh polotovaru" << endl;
	cout << "['j' pre olej, 'o' pre ochucovadlo a 'z' pre zemiaky]: ";
	cin >> pismenko;

	if (!platnyPolotovar(pismenko)) {
		while (!platnyPolotovar(pismenko)) {
			cout << "Vložili ste nesprávne písmeno." << endl;
			cout << "Vložte prosím druh polotovaru" << endl;
			cout << "['j' pre olej, 'o' pre ochucovadlo a 'z' pre zemiaky]: ";
			cin >> pismenko;
		}
	}

	DruhyPolotovarov druh;
	switch (pismenko) {
	case 'j': druh = DruhyPolotovarov::olej;
		break;
	case 'o': druh = DruhyPolotovarov::ochucovadla;
		break;
	case 'z': druh = DruhyPolotovarov::zemiaky;
		break;
	}

	// kazdy prvok kontajneru biofarmari je totiz unikatny
	structures::LinkedList<string> biofarmari;

	for (auto pNakup : *zoznam) {
		if (epochaNaPocetDni(pNakup->datum) + 30 >= epochaNaPocetDni(dnesnyDatum))
		{
			// ak v databaze este nemame nazov daneho farmara, tak ho pridame
			if (biofarmari.getIndexOf(pNakup->obchodnyNazov) == -1) {
				biofarmari.add(pNakup->obchodnyNazov);
			}
		}
	}

	// najdi obchodny nazov biofarmara s najvacsim
	// mnozstvom daneho nakupeneho polotovaru
	string najlepsiFarmar = "";
	float maximalneMnozstvo = 0;
	float maximalnaCena = 0;

	for (auto bf : biofarmari) {
		ulong celkoveMnozstvo = 0;
		float celkovaCena = 0;

		for (auto pNakup : *zoznam) {
			if (druh == pNakup->druh) {
				celkoveMnozstvo += pNakup->mnozstvo;
				celkovaCena += pNakup->cena;
			}
		}

		if (celkoveMnozstvo > maximalneMnozstvo) {
			najlepsiFarmar = bf;
			maximalneMnozstvo = celkoveMnozstvo;
			maximalnaCena = celkovaCena;
		}
	}

	cout << "Najlepší biofarmár:" << endl;
	cout << "obchodný názov: " << najlepsiFarmar << endl;
	cout << "celkové množstvo: " << maximalneMnozstvo << endl;
	cout << "celková cena: " << maximalnaCena << endl;
	cout << "priemerná cena: " << (float)maximalnaCena / maximalneMnozstvo << endl;
}

float DatabazaNakupov::priemer(string obchodnyNazov, DruhyPolotovarov druh) const {

	if (zoznam->isEmpty()) {
		return 0;
	}

	float celkovaCena = 0;
	ulong pocetKusov = 0;

	for (size_t i = 0; i < zoznam->size(); i++) {

		if (epochaNaPocetDni((*zoznam)[i]->datum) + 30 >= epochaNaPocetDni(dnesnyDatum)) //za posledych 30 dni
		{
			if ((*zoznam)[i]->obchodnyNazov == obchodnyNazov && (*zoznam)[i]->datum >= 30 * pocetSekundVdni) {
				celkovaCena += (*zoznam)[i]->mnozstvo * (*zoznam)[i]->cena;
				pocetKusov += (*zoznam)[i]->mnozstvo;
			}
		}
	}

	//ak spolocnost u farmara nenakupovala
	//priemerna cena nakupu u farmara za poslednych 30 dni je 0
	if (pocetKusov == 0) {
		return 0;
	}

	return (float)celkovaCena / pocetKusov;
}

float DatabazaNakupov::cenaPolotovarov() const
{
	float celkoveMnozstvo = 0;
	float celkovaCena = 0;

	for (auto pNakup : *zoznam) {
		celkoveMnozstvo += pNakup->mnozstvo;
		celkovaCena += pNakup->cena;
	}

	return celkoveMnozstvo * celkovaCena;
}

DatabazaNakupov::DatabazaNakupov()
	: zoznam(new structures::LinkedList<Nakup*>) {}

DatabazaNakupov::DatabazaNakupov(
	const structures::LinkedList<Nakup*>*& inyZoznam
) {
	zoznam = new structures::LinkedList<Nakup*>(*inyZoznam);
}

DatabazaNakupov::~DatabazaNakupov() noexcept {

	for (auto prvok : *zoznam) {
		delete prvok;
	}

	delete zoznam;

}

bool DatabazaNakupov::existujeDostatokPolotovarov(
	const Objednavka& o
) const {
	ulong potrebujemHranolcekov = 0;
	ulong potrebujemLupienkov = 0;

	switch (o.getTypTovaru()) {
	case TypTovaru::hranolceky:
		potrebujemHranolcekov = o.getMnozstvo();
		break;
	case TypTovaru::lupienky:
		potrebujemLupienkov = o.getMnozstvo();
		break;
	}

	float potrebujemZemiakov = potrebujemHranolcekov * 1.5 + potrebujemLupienkov * 2; //kg
	float potrebujemOleja = potrebujemHranolcekov * 0.2 + potrebujemLupienkov * 0.4; //l
	float potrebujemOchucovadiel = potrebujemLupienkov * 20; //g

	return (mnozstvoZemiakov >= potrebujemZemiakov) && (mnozstvoOleja >= potrebujemOleja) && (mnozstvoOchucovadla >= potrebujemOchucovadiel);
}

//odpocita mnozstvo tovaru zo skladu po zrealizovani objednavky
void DatabazaNakupov::odpocitajMnozstvoTovaru(const Objednavka& o){
	float spotrebaZemiakov = 0;
	float spotrebaOleja = 0;
	float spotrebaOchucovadla = 0;
	if (o.getTypTovaru() == TypTovaru::hranolceky)
	{
		spotrebaZemiakov = o.getMnozstvo() * 1.5;
		spotrebaOleja = o.getMnozstvo() * 0.2;
	}
	if (o.getTypTovaru() == TypTovaru::lupienky)
	{
		spotrebaZemiakov = o.getMnozstvo() * 2;
		spotrebaOleja = o.getMnozstvo() * 0.4;
		spotrebaOchucovadla = o.getMnozstvo() * 20;
	}
	mnozstvoZemiakov -= spotrebaZemiakov;
	mnozstvoOleja -= spotrebaOleja;
	mnozstvoOchucovadla -= spotrebaOchucovadla;
}

void DatabazaNakupov::zabezpecDostatokPolotovarov(
	const DatabazaObjednavok& dbObjednavok,
	DatabazaBiofarmarov& dbBiofarmarov
) {
	//pocet hranolkov v kg, ktore potrebujem na najblizsi tyzden
	ulong potrebujemHranolcekov = dbObjednavok.kDodaniuDoTyzdna(TypTovaru::hranolceky);
	cout << "Potrebujem hranolèekov: " << potrebujemHranolcekov << endl;

	//pocet lupienkov v kg, ktore potrebujem na najblizsi tyzden
	ulong potrebujemLupienkov = dbObjednavok.kDodaniuDoTyzdna(TypTovaru::lupienky);
	cout << "Potrebujem lupienkov: " << potrebujemLupienkov << endl;

	float potrebujemZemiakov = potrebujemHranolcekov * 1.5 + potrebujemLupienkov * 2; //kg
	float potrebujemOleja = potrebujemHranolcekov * 0.2 + potrebujemLupienkov * 0.4; //l
	float potrebujemOchucovadiel = potrebujemLupienkov * 20; //g
	cout << "K výrobe hranolèekov a lupienkov na najbližších 7 dní budem potrebova: " << potrebujemZemiakov << " zemiakov, " << potrebujemOleja << " oleja, " << potrebujemOchucovadiel << " ochucovadiel." << endl;

	// Prvy polotovar
	if (mnozstvoZemiakov < potrebujemZemiakov) {

		structures::PriorityQueueUnsortedArrayList<Biofarmar*> pqZemiaky;

		for (auto pBiofarmar : *dbBiofarmarov.zoznam) {
			pqZemiaky.push(
				priemer(pBiofarmar->getObchodnyNazov(), DruhyPolotovarov::zemiaky),
				pBiofarmar
			);
		}

		while (
			mnozstvoZemiakov < potrebujemZemiakov
			&&
			!pqZemiaky.isEmpty()
			) {
			auto pBiofarmar = pqZemiaky.pop();
			float nakupeneMnozstvo = pBiofarmar->predajMi(potrebujemZemiakov, DruhyPolotovarov::zemiaky, *this);
			potrebujemZemiakov -= nakupeneMnozstvo;
			mnozstvoZemiakov += nakupeneMnozstvo - nakupeneMnozstvo;
			cout << "Nakúpil som zemiaky od bifarmára s obchodným názvom: " << pBiofarmar->getObchodnyNazov() << "." << ". Množstvo: " << nakupeneMnozstvo << "." << endl;
		}
	}

	// Druhy polotovar
	if (mnozstvoOleja < potrebujemOleja) {

		structures::PriorityQueueUnsortedArrayList<Biofarmar*> pqOlej;

		for (auto pBiofarmar : *dbBiofarmarov.zoznam) {
			pqOlej.push(
				priemer(pBiofarmar->getObchodnyNazov(), DruhyPolotovarov::olej),
				pBiofarmar
			);
		}

		while (
			mnozstvoOleja < potrebujemOleja
			&&
			!pqOlej.isEmpty()
			) {
			auto pBiofarmar = pqOlej.pop();
		    float nakupeneMnozstvo = pBiofarmar->predajMi(potrebujemOleja, DruhyPolotovarov::olej, *this);
			potrebujemOleja -= nakupeneMnozstvo;
			mnozstvoOleja += nakupeneMnozstvo;
			cout << "Nakúpil som olej od bifarmára s obchodným názvom: " << pBiofarmar->getObchodnyNazov() << ". Množstvo: " << nakupeneMnozstvo << "." << endl;
		}
	}

	// Treti polotovar
	if (mnozstvoOchucovadla < potrebujemOchucovadiel) {

		structures::PriorityQueueUnsortedArrayList<Biofarmar*> pqOchucovadlo;

		for (auto pBiofarmar : *dbBiofarmarov.zoznam) {
			pqOchucovadlo.push(
				priemer(pBiofarmar->getObchodnyNazov(), DruhyPolotovarov::ochucovadla),
				pBiofarmar
			);
		}

		while (
			mnozstvoOchucovadla < potrebujemOchucovadiel
			&&
			!pqOchucovadlo.isEmpty()
			) {
			auto pBiofarmar = pqOchucovadlo.pop();
			float nakupeneMnozstvo = pBiofarmar->predajMi(potrebujemOchucovadiel, DruhyPolotovarov::ochucovadla, *this);
			potrebujemOchucovadiel -= nakupeneMnozstvo;
			mnozstvoOchucovadla += nakupeneMnozstvo;
			cout << "Nakúpil som ochucovadlo od bifarmára s obchodným názvom: " << pBiofarmar->getObchodnyNazov() << ". Množstvo: " << nakupeneMnozstvo << "." << endl;
		}
	}
}