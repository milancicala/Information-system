#ifndef BIOFARMAR_H
#define BIOFARMAR_H

#include "PredajPolotovaru.h"
#include "DatabazaNakupov.h"

#include <string>
using std::string;

#include <fstream>
using std::fstream;

class Biofarmar {
	friend class Perzistencia;

	string obchodnyNazov = "";

	PredajOleja* predajOleja = nullptr;
	PredajZemiakov* predajZemiakov = nullptr;
	PredajOchucovadiel* predajOchucovadiel = nullptr;

public:
	Biofarmar() {}
	Biofarmar(string obchodnyNazov, produkty dodava);

	/** Kopirovaci constructor */
	Biofarmar(const Biofarmar&);

	/** Destructor */
	~Biofarmar() noexcept;

	string getObchodnyNazov() const;
	void setObchodnyNazov(string obchodnyNazov);

	bool dodavaOlej() const;
	bool dodavaZemiaky() const;
	bool dodavaOchucovadla() const;

	// sluzi ku kupe produktu od biofarmara
	// ak dojde k predaju, bude o tom vlozeny zaznam do databazy
	// vrati mnozstvo tovaru, ktore bolo predane
	ulong predajMi(float mnozstvo,
		DruhyPolotovarov druh,
		DatabazaNakupov& dbNakupov);

	// vypis objekt Biofarmar do suboru
	// umozni napriklad nasledovny prikaz
	// file << biofarmar;
	friend fstream& operator<<(fstream&, const Biofarmar&);

	// nacitanie objektu Biofarmar zo suboru
	// umozni napriklad nasledovny prikaz
	// file >> biofarmar;
	friend fstream& operator>>(fstream&, Biofarmar&);
};

#endif