#ifndef DATABAZABIOFARMAROV_H
#define DATABAZABIOFARMAROV_H

#include "structures\list\linked_list.h"
#include "Biofarmar.h"

class DatabazaBiofarmarov {
	friend class Perzistencia;
	friend class DatabazaNakupov;

private:
	structures::LinkedList<Biofarmar*>* zoznam;

public:
	/** Konstruktor */
	DatabazaBiofarmarov();

	/** Kopirovaci constructor */
	DatabazaBiofarmarov(const DatabazaBiofarmarov&) = delete;

	/** Destructor */
	~DatabazaBiofarmarov() noexcept;

	void pridajAbecedne(Biofarmar*);

	void pridaj(); // bod 1

	// bod 2
	// vypise pre kazdeho biofarmara
	// obchodny nazov (abecedne)
	// typy vsetkych polotovarov, ktore dodava
	// priemerna nakupna cena polotovarov za poslednych 30 dni
	// (ktoru ziska z databazy nakupov predanej ako parameter)
	void vypis(const DatabazaNakupov&) const;
};

#endif
