#pragma once

#include "Struktury.h" //datove typy
#include "menu.h"
#include "DatabazaBiofarmarov.h"
#include "PredajPolotovaru.h"
#include "DatabazaNakupov.h"
#include "StrukturyPrePolotovary.h"
#include "DatabazaVozidiel.h"
#include "DatabazaZakaznikov.h"
#include "DatabazaObjednavok.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

// podpora diakritiky pre konzolu vo Windows
// (Linuxovy terminal to prirodzene zvlada nativne)
// potrebne v nastaveniach MS VS C++:
// Project -> Properties -> General -> Character Set -> Use Unicode Character Set
#include <windows.h>
#pragma execution_character_set( "utf-8" )

//dnesny datum (globalna premenna)
//myslím si, ze v tomto kontretnom pripade
//ide o odovodnene pouzitie globalnej premennej
//pretoze aktualny datum je vyuzivany mnohymi funkciami
//a zneprehladnoval by ich rozhrania
time_t dnesnyDatum = time(nullptr);
