#include "vector.h"
#include "../ds_routines.h"
#include <cstdlib>
#include <cstring>

namespace structures {

	//parametricky konstruktor
	//preberá v parametri size informáciu o ve¾kosti pamäte. Takto ve¾kú pamä vyalokuje a inicializuje.
	Vector::Vector(size_t size) :
		memory_(calloc(size, 1)),	// calloc - Alokuje a vráti odkaz na pamä, ktorá pojme size prvkov o ve¾kosti 1 bajtu
		size_(size)
	{
	}

	//Kopírovací konštruktor vytvorí kópiu pamäte spravovanej vektorom other.
	Vector::Vector(const Vector& other) :
		Vector(other.size_)
	{
		memory_ = memcpy(memory_, other.memory_, size_);	//void * memcpy(.,.,.) - Binárne skopíruje size_ bytov z pamäte 
															//zaèínajúcej na adrese other.memory_ do pamäte zaèínajúcej na adrese memory_.
	}

	//Deštruktor uvo¾ní spravovanú, predtım alokovanú pamä, potom upratat, size na nulu a memory_ na nullptr, inak ukazuje na neplatnu pamat
	Vector::~Vector()
	{
		free(memory_);
		size_ = 0;
		memory_ = nullptr;
	}

	Structure* Vector::clone() const
	{
		return new Vector(*this);
	}

	size_t Vector::size() const
	{
		return size_;
	}

	Structure & Vector::operator=(const Structure & other)
	{
		if (this != &other)
		{
			*this = dynamic_cast<const Vector&>(other);
		}
		return *this;
	}

	// Operátor priradenia, najskôr kontrola identity, aby sa nerovnal sam sebe, nastavim velkost
	// realloc (void * ptr, size_t size) -	Zmení ve¾kos pamäte ptr na ve¾kos danú parametrom size.Pamä môe by
	//										presunutá – adresa novej pamäte je v návratovej hodnote.Hodnoty prvkov, ktoré
	//										sa do realokovanej pamäte zmestia, ostanú zachované.Ak pamä presunutá
	//										nebola, návratová hodnota je zhodná s parametrom ptr.Ak je parameter ptr
	//										rovnı null, funkcia sa správa ako funkcia malloc.Ak je parameter size rovnı
	//										0, funkcia sa správa ako funkcia free a návratová hodnota je null.
	Vector& Vector::operator=(const Vector& other)
	{
		if (this != &other) {
			size_ = other.size_;
			memory_ = realloc(memory_, size_);
			memcpy(memory_, other.memory_, size_);
		}
		return *this;

	}

	// Porovnávací operátor
	// int memcmp(const void* ptr1, const void* ptr2, size_t num)
	//			- Binárne porovná pamäte o ve¾kosti num bytov zaèínajúcich na adresách ptr1 a
	//			ptr2. Vráti 0, ak sú pamäte zhodné, iná hodnota znamená, e pamäte nie sú
	//			zhodné. Nekontroluje platnosti pamätí (preteèenie)!
	bool Vector::operator==(const Vector& other) const
	{
		return other.size_ == size_ && memcmp(memory_, other.memory_, size_) == 0;
	}

	//Vracia odkaz na danom indexe
	byte& Vector::operator[](const int index)
	{
		DSRoutines::rangeCheckExcept(index, size_, "Index out of range");
		return *(static_cast <byte*>(memory_) + index);
	}

	//Vráti objekt na indexe
	byte Vector::operator[](const int index) const
	{
		DSRoutines::rangeCheckExcept(index, size_, "Index out of range");
		return *(static_cast <byte*>(memory_) + index);
	}

	// Do pamäte zaèínajúcej na adrese dest skopíruje count bytov od indexu index. Vráti dest.
	byte& Vector::readBytes(const int index, const int count, byte& dest)
	{
		DSRoutines::rangeCheckExcept(index, size_, "Index out of range");
		DSRoutines::rangeCheckExcept(index + count, size_ + 1, "Index out of range");

		memcpy(&dest, getBytePointer(index), count);
		return dest;
	}

	void Vector::copy(const Vector& src, const int srcStartIndex, Vector& dest, const int destStartIndex, const int length)
	{
		DSRoutines::rangeCheckExcept(srcStartIndex, src.size_, "Index out of range");
		DSRoutines::rangeCheckExcept(srcStartIndex + length, src.size_, "Index out of range");
		DSRoutines::rangeCheckExcept(destStartIndex, dest.size_, "Index out of range");
		DSRoutines::rangeCheckExcept(destStartIndex + length, dest.size_, "Index out of range");
		if (&src == &dest || srcStartIndex + length <= destStartIndex || destStartIndex + length <= srcStartIndex) {
			memmove(src.getBytePointer(destStartIndex), src.getBytePointer(srcStartIndex), length);
		}
		else {
			memcpy(dest.getBytePointer(srcStartIndex), src.getBytePointer(destStartIndex), length);
		}
	}

	byte* Vector::getBytePointer(const int index) const
	{
		DSRoutines::rangeCheckExcept(index, size_, "Index out of range");
		return (static_cast <byte*>(memory_) + index);
	}

}