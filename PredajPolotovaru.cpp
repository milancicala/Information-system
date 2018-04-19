#include "PredajPolotovaru.h"
#include "Nakup.h"

// generovanie nahodneho cisla
// prevzate z
// https://stackoverflow.com/a/19728404
#include <random> 
#include <iostream>

PredajPolotovaru::PredajPolotovaru(
	const DruhyPolotovarov druh,
	const ulong MAX_MNOZSTVO,
	const ulong MAX_CENA)
	: druh(druh)
	, MAX_MNOZSTVO(MAX_MNOZSTVO)
	, MAX_CENA(MAX_CENA) {
	generujMnozstvo();
	generujCenu();
}

void PredajPolotovaru::generujMnozstvo() {
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(MIN_MNOZSTVO, MAX_MNOZSTVO); // guaranteed unbiased
	mnozstvo = uni(rng);
}

void PredajPolotovaru::generujCenu() {
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(MIN_CENA, MAX_CENA); // guaranteed unbiased
	cena = uni(rng);
}