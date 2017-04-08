#include "stdafx.h"


#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <ctime>
#include <stdio.h>
#include <string>
#include <sstream>

class Objekt {
public:
	int x, y;
	Objekt(int X, int Y) : x(X), y(Y) {}
	Objekt() : x(0), y(0) {}

	bool operator ==(const Objekt & v) {
		if ((this->x == v.x) && (this->y == v.y))
			return true;
		else
			return false;
	}
};

Objekt jablko;
std::vector<Objekt> waz;
static int szerokosc = 78;
static int wysokosc = 23;
std::string* znaki = new std::string[3];
HANDLE hOut;

void wyswietl() {
	for (int Ty = 0; Ty<wysokosc; Ty++) {
		bool jest = false;
		for (size_t i = 0; i<waz.size(); i++)
			if (Ty == waz[i].y)
				jest = true;
		if (Ty == jablko.y)
			jest = true;
		if (jest == true)
			for (int Tx = 0; Tx<szerokosc; Tx++) {
				bool t = false;
				for (size_t i = 0; i<waz.size(); i++) {
					if (Tx == waz[i].x&&Ty == waz[i].y)
						t = true;
				}
				if (t) {
					SetConsoleTextAttribute(hOut, BACKGROUND_RED | FOREGROUND_INTENSITY);
					std::cout << " ";
				}
				else {
					if (Tx == jablko.x&&Ty == jablko.y) {
						SetConsoleTextAttribute(hOut, BACKGROUND_GREEN | FOREGROUND_INTENSITY);
						std::cout << " ";
					}
					else {
						SetConsoleTextAttribute(hOut, BACKGROUND_BLUE);
						std::cout << " ";
					}
				}
			}
		SetConsoleTextAttribute(hOut, BACKGROUND_BLUE);
		std::cout << "\n";
	}
	SetConsoleTextAttribute(hOut, BACKGROUND_BLUE);
	std::cout << "wynik=";
	std::stringstream ss;
	ss << waz.size();
	std::cout << ss.str() << "\n";
}

bool przejdz(int strona) {
	Objekt w = waz[0];
	switch (strona) {
	case 0: //góra
		w.y -= 1;
		break;
	case 1: //lewo
		w.x -= 1;
		break;
	case 2: //dó³
		w.y += 1;
		break;
	case 3: //prawo
		w.x += 1;
		break;
	}

	if (w.x == szerokosc)
		w.x = 0;
	if (w.x == -1)
		w.x = szerokosc - 1;
	if (w.y == wysokosc)
		w.y = 0;
	if (w.y == -1)
		w.y = wysokosc - 1;

	for (size_t i = waz.size() - 1; i>0; i--)
		if (waz[i] == w)//uderzenie
			return false;

	for (size_t i = waz.size() - 1; i>0; i--) {
		waz[i] = waz[i - 1];
	}
	waz[0] = w;

	if (w == jablko) {
		waz.push_back(Objekt(0, 0));

		bool k = true;	//losowanie pozycji dla jablka
		while (k) {
			jablko = Objekt(rand() % szerokosc, rand() % wysokosc);
			for (size_t i = waz.size() - 1; i>0; i--)
				if (jablko == waz[i]) {	 //byle nie tam gdzie waz
					jablko = Objekt(rand() % szerokosc, rand() % wysokosc);
					i = waz.size();
				}
			k = false;
		}
	}

	wyswietl();
	return true;
}


int main() {
	srand(time(NULL));

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::ios::sync_with_stdio(false);

	waz.push_back(Objekt(szerokosc / 2, wysokosc / 2));
	jablko = Objekt(rand() % szerokosc, rand() % wysokosc);

	bool koniec = false;
	int kierunek = 3;
	while (koniec == false) {
		unsigned char Pznak;
		unsigned char znak = 0;

		if (_kbhit())  Pznak = _getch();
		while (_kbhit())
			if (Pznak == 0 || Pznak == 224)
				znak = _getch();
			else
				Pznak = _getch();

		switch (znak) {
		case 72: //strza³ka w górê
			kierunek = 0;
			break;
		case 75: //strza³ka w lewo
			kierunek = 1;
			break;
		case 80: //strza³ka w dó³
			kierunek = 2;
			break;
		case 77: //strza³ka w prawo
			kierunek = 3;
			break;
		}

		if (!przejdz(kierunek)) {
			koniec = true;
		}
		wyswietl();

		Sleep(100);
	}
	std::cout << "Koniec gry, wynik:" << waz.size();
	_getch();
	return(0);
}
