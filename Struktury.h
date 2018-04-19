#pragma once

using ulong = unsigned long int;

#include <ctime>
using std::time_t;
using std::time;
using std::asctime;
using std::localtime;

// konvertuje Epochu v sekundach na pocet dni od Epochy
ulong epochaNaPocetDni(time_t);

// konvertuje pocet dni od Epochy na Epochu v sekundach
time_t pocetDniNaEpochu(ulong);

const time_t pocetSekundVdni = 24 * 60 * 60;