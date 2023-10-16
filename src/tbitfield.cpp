// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int _BitLen = 1)
{
	if (_BitLen <= 0) { throw "wrong len"; }
	else {
		MemLen = _BitLen / 32 + 1;
		pMem = new TELEM[MemLen];
		BitLen = _BitLen;
		for (int i = 0; i < MemLen; i++) {
			pMem[i] = 0;
		}
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	BitLen = bf.BitLen;
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	// <0?
	return n/32;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	// <0?
	TELEM t = 1;
	char shift = n % 32; //char- целый тип данных -128 - 128)
	TELEM result = t << shift;
	return result;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0) { throw "negative n"; }
	else {
		if (n >= BitLen) { throw "too large n"; }
		else {
			TELEM m = GetMemMask(n);
			int idx = GetMemIndex(n);
			pMem[idx] = pMem[idx] | m;
		}
	}
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0) { throw "negative n"; }
	else {
		if (n >= BitLen) { throw "too large n"; }
		else {
			TELEM m = ~(GetMemMask(n));
			int idx = GetMemIndex(n);
			pMem[idx] = pMem[idx] & m;
		}
	}
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0) { throw "negative n"; }
	else {
		if (n >= BitLen) { throw "too large n"; }
		else {
			TELEM m = GetMemMask(n);
			int idx = GetMemIndex(n);
			TELEM res = m & pMem[idx];
			return res;
		}
	}
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (MemLen != bf.MemLen) {
		delete[] pMem;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
	}
	BitLen = bf.BitLen;
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) {
		return false;
	}
	for (int i = 0; i < MemLen-1; i++) {
		if (pMem[i] != bf.pMem[i]) {
			return false;
		}
	}
	for (int i = (MemLen - 1) * 32; i < BitLen; i++) {
		if (GetBit(i) != bf.GetBit(i)) {
			return false;
		}
	}
  return true;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) {
		return true;
	}
	for (int i = 0; i < MemLen - 1; i++) {
		if (pMem[i] != bf.pMem[i]) {
			return true;
		}
	}
	for (int i = (MemLen - 1) * 32; i < BitLen; i++) {
		if (GetBit(i) != bf.GetBit(i)) {
			return true;
		}
	}
	return false;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int max_L = bf.BitLen;
	if (BitLen > bf.BitLen) { max_L = BitLen; }
	TBitField res(max_L);
	for (int i = 0; i < MemLen; i++) { res.pMem[i] = pMem[i]; }
	for (int i = 0; i < bf.MemLen; i++) { res.pMem[i] = bf.pMem[i] | res.pMem[i]; }
	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (BitLen > bf.BitLen) {
		TBitField res(*this);
		TBitField pr(BitLen);
		for (int i = 0; i < res.MemLen; i++) { pr.pMem[i] = bf.pMem[i]; }
		for (int i = 0; i < res.MemLen; i++) { res.pMem[i] = pr.pMem[i] & res.pMem[i]; }
		return res;
	}
	else {
		TBitField res(bf);
		TBitField pr(bf.BitLen);
		for (int i = 0; i < res.MemLen; i++) { pr.pMem[i] = pMem[i]; }
		for (int i = 0; i < res.MemLen; i++) { res.pMem[i] = pr.pMem[i] & res.pMem[i]; }
		return res;
	}
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i = 0; i < MemLen; i++) {
		res.pMem[i] = ~pMem[i];
	}
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char a;
	for (int i = 0; i < bf.MemLen; i++) {
		bf.pMem[i] = 0;
	}
	for (int i = 0; i < bf.BitLen; i++) {
		istr >> a;
		if (a == '1') { bf.SetBit(i); }
		else { if (a == '0') { bf.ClrBit(i); }
		else { break; }
		}
	}
	return istr;

}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		if (bf.GetBit(i)) {
			ostr << '1';
		}
		else {
			ostr << '0';
		}
	}
	return ostr;

}
