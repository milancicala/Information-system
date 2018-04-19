#include "vector.h"
#include "../ds_routines.h"
#include <cstdlib>
#include <cstring>

namespace structures {

	//parametricky konstruktor
	//preber� v parametri size inform�ciu o ve�kosti pam�te. Takto ve�k� pam� vyalokuje a inicializuje.
	Vector::Vector(size_t size) :
		memory_(calloc(size, 1)),	// calloc - Alokuje a vr�ti odkaz na pam�, ktor� pojme size prvkov o ve�kosti 1 bajtu
		size_(size)
	{
	}

	//Kop�rovac� kon�truktor vytvor� k�piu pam�te spravovanej vektorom other.
	Vector::Vector(const Vector& other) :
		Vector(other.size_)
	{
		memory_ = memcpy(memory_, other.memory_, size_);	//void * memcpy(.,.,.) - Bin�rne skop�ruje size_ bytov z pam�te 
															//za��naj�cej na adrese other.memory_ do pam�te za��naj�cej na adrese memory_.
	}

	//De�truktor uvo�n� spravovan�, predt�m alokovan� pam�, potom upratat, size na nulu a memory_ na nullptr, inak ukazuje na neplatnu pamat
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

	// Oper�tor priradenia, najsk�r kontrola identity, aby sa nerovnal sam sebe, nastavim velkost
	// realloc (void * ptr, size_t size) -	Zmen� ve�kos� pam�te ptr na ve�kos� dan� parametrom size.Pam� m��e by�
	//										presunut� � adresa novej pam�te je v n�vratovej hodnote.Hodnoty prvkov, ktor�
	//										sa do realokovanej pam�te zmestia, ostan� zachovan�.Ak pam� presunut�
	//										nebola, n�vratov� hodnota je zhodn� s parametrom ptr.Ak je parameter ptr
	//										rovn� null, funkcia sa spr�va ako funkcia malloc.Ak je parameter size rovn�
	//										0, funkcia sa spr�va ako funkcia free a n�vratov� hodnota je null.
	Vector& Vector::operator=(const Vector& other)
	{
		if (this != &other) {
			size_ = other.size_;
			memory_ = realloc(memory_, size_);
			memcpy(memory_, other.memory_, size_);
		}
		return *this;

	}

	// Porovn�vac� oper�tor
	// int memcmp(const void* ptr1, const void* ptr2, size_t num)
	//			- Bin�rne porovn� pam�te o ve�kosti num bytov za��naj�cich na adres�ch ptr1 a
	//			ptr2. Vr�ti 0, ak s� pam�te zhodn�, in� hodnota znamen�, �e pam�te nie s�
	//			zhodn�. Nekontroluje platnosti pam�t� (prete�enie)!
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

	//Vr�ti objekt na indexe
	byte Vector::operator[](const int index) const
	{
		DSRoutines::rangeCheckExcept(index, size_, "Index out of range");
		return *(static_cast <byte*>(memory_) + index);
	}

	// Do pam�te za��naj�cej na adrese dest skop�ruje count bytov od indexu index. Vr�ti dest.
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