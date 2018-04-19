#pragma once

#include "Struktury.h"
#include "StrukturyPrePolotovary.h"

class PredajPolotovaru {
	friend class Biofarmar;

protected:

	DruhyPolotovarov druh;

	ulong mnozstvo;
	ulong MAX_MNOZSTVO;
	const ulong MIN_MNOZSTVO = 0;

	ulong cena;
	ulong MAX_CENA;
	const ulong MIN_CENA = 1;

public:

	void generujMnozstvo();
	void generujCenu();

	PredajPolotovaru(const DruhyPolotovarov druh,
		const ulong MAX_MNOZSTVO,
		const ulong MAX_CENA);

	// cena za ktoru biofarmar aktualne predava dany polotovar
	ulong getCena() {
		return cena;
	}

	// mnozstvo, ktore je biofarmar schopny dodat
	ulong getMnozstvo() {
		return mnozstvo;
	}
};

class PredajOleja : public PredajPolotovaru {
public:
	PredajOleja()
		:PredajPolotovaru(DruhyPolotovarov::olej, 1000, 4) {
	}
};


class PredajZemiakov : public PredajPolotovaru {
public:
	PredajZemiakov()
		:PredajPolotovaru(DruhyPolotovarov::zemiaky, 5000, 3) {
	}
};

class PredajOchucovadiel : public PredajPolotovaru {
public:
	PredajOchucovadiel()
		:PredajPolotovaru(DruhyPolotovarov::ochucovadla, 1000, 2) {
	}
};