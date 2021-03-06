// Atlanteine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stack>
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <atlimage.h> 
#include <Gdiplusimaging.h> 

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <sstream>

#include<ctime>

#include <fstream>




using namespace std;

const int SIZE_MAP = 15;

//const int MAP[SIZE_MAP][SIZE_MAP] = 
/*{
	{ 0,0,0,0,5,0,0 },
	{ 0,4,1,1,1,1,0 },
	{ 0,1,1,5,1,1,0 },
	{ 0,1,1,1,1,5,0 },
	{ 0,1,1,1,1,1,0 },
	{ 0,1,5,1,1,2,0 },
	{ 0,0,0,0,0,0,0 } };*/
	int MAP[SIZE_MAP][SIZE_MAP] = {
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 1, 1, 3, 1, 1, 1, 1, 2, 0, 0, 0, 0 },
		{ 0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 0, 0 },
		{ 0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
		{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 0, 0 },
		{ 0, 1, 1, 1, 1, 1, 3, 1, 1, 3, 1, 1, 1, 0, 0 },
		{ 0, 1, 1, 1, 1, 1, 5, 3, 1, 5, 3, 3, 3, 0, 0 },
		{ 0, 3, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
		{ 0, 1, 3, 1, 3, 1, 1, 1, 1, 6, 1, 1, 1, 0, 0 },
		{ 0, 1, 1, 1, 3, 1, 1, 6, 1, 1, 1, 1, 1, 5, 0 },
		{ 0, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 0 },
		{ 0, 1, 1, 1, 1, 1, 3, 1, 1, 1, 0, 1, 1, 0, 0 },
		{ 0, 0, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
		{ 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

stack<string> pila;

int campoJuego[SIZE_MAP][SIZE_MAP];

const int UP = 1;
const int RIGHT = 2;
const int DOWN = 3;
const int LEFT = 4;

const int VACIO = 0;
const int SUELO = 1;
const int CALABAZA = 2;
const int CAJA = 3;
const int AGUJERO = 4;
const int PIEDRA = 5;
const int TELEPORT = 6;
int posicionInicial[2]= { -1,-1 };
int posicionTeleport1[2] = {-1,-1};
int posicionTeleport2[2] = { -1,-1 };
bool fin = false;
int recorrido[50][3];
const int MAX_REPETIDOS = 2;
int repeticiones = 0;
int posRecorrido = -1;
bool cajaMovida = false;

HWND hWnd = GetDesktopWindow();
HBITMAP hBitmap;
HBITMAP hBitmap_old;
time_t timestamp;


bool mover(int x, int y, int direccion);
bool moverCaja(int x, int y, int direccion);
void resolverAtlanteine();
HBITMAP ImageFromClipboard(HWND hWnd);
bool guardarImagen();
string exec(const char* cmd);

void mensaje(string txt) {
	pila.push(txt);
}

int main()
{
	while (true) {
		fin = false;
		cajaMovida = false;
		//hWnd = GetDesktopWindow();
		//hBitmap = NULL;
		cout << "Copie una imagen al portapapeles..." << endl;
		cout << guardarImagen() << endl;
		//char* cmd = "C:\\Users\\Chipi\\source\\repos\\Atlanteine\\Debug\\bash.exe -c \"/home/chipi/test_Atlanteine.sh\"";
		Sleep(1000);
		//string raw_matrix = exec(cmd);
		string raw_matrix, line;
		ifstream ifs("C:\\Users\\Chipi\\source\\repos\\Atlanteine\\Debug\\matriz.txt");
		Sleep(5000);
		while (getline(ifs, line))
			raw_matrix += line + "\n";
		ifs.close();
		cout << raw_matrix << endl;
		//cout << "SALIDA: " << endl;
		//cout << raw_matrix << endl;
		//cout << raw_matrix.length() << endl;
		// (14*15 + 14 + 1) * 3 - 1 + 14 * 2 = 702
		if (raw_matrix.length() >= 702) {
			for (int i = 0, n = 0, j = 1; i < SIZE_MAP; i++) {
				for (j = 0; j < SIZE_MAP; j++) {

					n = stoi(&raw_matrix[(i*SIZE_MAP + j + 1) * 3 - 1 + i * 2]);

					if (n == 0 && (i > 1 && i < 14) && (j > 1 && j < 14)) n = 1;
					//else if (n == 1) n = 0;
					cout << n << " ";
					MAP[i][j] = n;
				}
				cout << endl;
			}

			MAP[13][14] = 0;
			MAP[14][13] = 0;
			MAP[14][14] = 0;
			resolverAtlanteine();
		} // if length 
		else {
			cout << "No se ha encontrado campo de juego. Length: " << raw_matrix.length() << endl;
		}
	
	} //while true

	std::system("pause");
    return 0;
}

bool tocaSueloRight(int i, int j) {
	if (campoJuego[i][j + 1] == SUELO) return true;
	return false;
}
bool tocaPiedraRight(int i, int j) {
	if (campoJuego[i][j + 1] == PIEDRA) return true;
	return false;
}
bool tocaCajaRight(int i, int j) {
	if (campoJuego[i][j + 1] == CAJA) return true;
	return false;
}
bool tocaVacioRight(int i, int j) {
	if (campoJuego[i][j + 1] == VACIO) return true;
	return false;
}

bool tocaSueloLeft(int i, int j) {
	if (campoJuego[i][j - 1] == SUELO) return true;
	return false;
}
bool tocaPiedraLeft(int i, int j) {
	if (campoJuego[i][j - 1] == PIEDRA) return true;
	return false;
}
bool tocaCajaLeft(int i, int j) {
	if (campoJuego[i][j - 1] == CAJA) return true;
	return false;
}
bool tocaVacioLeft(int i, int j) {
	if (campoJuego[i][j - 1] == VACIO) return true;
	return false;
}

bool tocaSueloUp(int i, int j) {
	if (campoJuego[i - 1][j] == SUELO) return true;
	return false;
}
bool tocaPiedraUp(int i, int j) {
	if (campoJuego[i - 1][j] == PIEDRA) return true;
	return false;
}
bool tocaCajaUp(int i, int j) {
	if (campoJuego[i - 1][j] == CAJA) return true;
	return false;
}
bool tocaVacioUp(int i, int j) {
	if (campoJuego[i - 1][j] == VACIO) return true;
	return false;
}

bool tocaSueloDown(int i, int j) {
	if (campoJuego[i + 1][j] == SUELO) return true;
	return false;
}
bool tocaPiedraDown(int i, int j) {
	if (campoJuego[i + 1][j] == PIEDRA) return true;
	return false;
}
bool tocaCajaDown(int i, int j) {
	if (campoJuego[i + 1][j] == CAJA) return true;
	return false;
}
bool tocaVacioDown(int i, int j) {
	if (campoJuego[i + 1][j] == VACIO) return true;
	return false;
}

void resolverAtlanteine() {
	bool calabazaEnc = false;
	bool agujeroEnc = false;
	for (int x = 0; SIZE_MAP > x; x++) {
		for (int y = 0; SIZE_MAP > y; y++) {
			campoJuego[x][y] = MAP[x][y];
			if (campoJuego[x][y] == CALABAZA) {
				calabazaEnc = true;
				posicionInicial[0] = x;
				posicionInicial[1] = y;
				campoJuego[x][y] = SUELO;
				MAP[x][y] = SUELO;
			}
			else if (campoJuego[x][y] == AGUJERO) {
				agujeroEnc = true;
				cout << "Agujero en " << x << "," << y << endl;
			}
			else if (campoJuego[x][y] == TELEPORT && posicionTeleport1[0] == -1) {
				posicionTeleport1[0] = x;
				posicionTeleport1[1] = y;
			}
			else if (campoJuego[x][y] == TELEPORT && posicionTeleport2[0] == -1) {
				posicionTeleport2[0] = x;
				posicionTeleport2[1] = y;
			}
		}
	}
	if (!calabazaEnc) cout << "Calabaza no encontrada" << endl;
	else if (!agujeroEnc) cout << "Agujero no encontrado" << endl;
	else {
		int x = posicionInicial[0];
		int y = posicionInicial[1];
		cout << "Inicio: (" << y << "," << x << ")" << endl;
		if (tocaSueloRight(x, y) && !fin && mover(x, y, RIGHT)) {
			mensaje("RIGHT");
		}
		if (tocaSueloLeft(x, y) && !fin && mover(x, y, LEFT)) {
			mensaje("LEFT");
		}
		if (tocaSueloUp(x, y) && !fin && mover(x, y, UP)) {
			mensaje("UP");
		}
		if (tocaSueloDown(x, y) && !fin && mover(x, y, DOWN)) {
			mensaje("DOWN");
		}
	}
	cout << timestamp << endl;
	for (int i = 1; !pila.empty(); i++) {
		cout << i << "- " << pila.top() << endl;
		pila.pop();
	}

	
}

void reconstruirCampo() {
	for (int x = 0; SIZE_MAP > x; x++) {
		for (int y = 0; SIZE_MAP > y; y++) {
			campoJuego[x][y] = MAP[x][y];
		}
	}
}

bool mover(int x, int y, int direccion) {
	int i = x, j = y;
	//recorrido.insert(recorrido.begin(),{i, j, direccion});
	if (posRecorrido > 48) {
		posRecorrido = -1;
	}
	posRecorrido++;
	recorrido[posRecorrido][0] = i;
	recorrido[posRecorrido][1] = j;
	recorrido[posRecorrido][2] = direccion;
	if (direccion == RIGHT) {
		for (i = x, j = y; j < SIZE_MAP && tocaSueloRight(i,j) && !fin; j++) {}

		if (tocaVacioRight(i, j)) {
			return false;
		}
		else if (campoJuego[i][j + 1] == AGUJERO || fin) {
			fin = true;
			//cout << fin << endl;
			return true;
		}
		else if (tocaPiedraRight(i, j) || (tocaCajaRight(i, j))) {
			repeticiones = 0;
			for (int p = 0; p <= posRecorrido; p++) {
				if (i == recorrido[p][0] && j == recorrido[p][1]) {
					repeticiones++;
				}
			}
			if (!tocaPiedraUp(i, j) && !tocaCajaUp(i, j) && !tocaVacioUp(i, j) && MAX_REPETIDOS > repeticiones && (fin = mover(i, j, UP))) {
				mensaje("UP");
			}
			else if (!tocaPiedraDown(i, j) && !tocaCajaDown(i, j) && !tocaVacioDown(i, j) && MAX_REPETIDOS * 2 > repeticiones && (fin = mover(i, j, DOWN))) {
				mensaje("DOWN");
			}
		}
		else if (campoJuego[i][j + 1] == TELEPORT) {
			if (posicionTeleport1[0] == i && posicionTeleport1[1] == j + 1) mover(posicionTeleport2[0], posicionTeleport2[1], RIGHT);
			else if (posicionTeleport2[0] == i && posicionTeleport2[1] == j + 1) mover(posicionTeleport1[0], posicionTeleport1[1], RIGHT);
		}
	}
	else if (direccion == LEFT) {
		for (i = x, j = y; j > 0 && campoJuego[i][j - 1] == SUELO && !fin; j--) {}
		if (tocaVacioLeft(i, j)) {
			return false;
		}
		else if (campoJuego[i][j - 1] == AGUJERO || fin) {
			fin = true;
			//cout << fin << endl;
			return true;
		}
		else if (tocaPiedraLeft(i, j) || (tocaCajaLeft(i, j))) {
			repeticiones = 0;
			for (int p = 0; p <= posRecorrido; p++) {
				if (i == recorrido[p][0] && j == recorrido[p][1]) {
					repeticiones++;
				}
			}
			if (!tocaPiedraUp(i, j) && !tocaCajaUp(i, j) && !tocaVacioUp(i, j) && MAX_REPETIDOS > repeticiones && (fin = mover(i, j, UP))) {
				mensaje("UP");
			}
			else if (!tocaPiedraDown(i, j) && !tocaCajaDown(i, j) && !tocaVacioDown(i, j) && MAX_REPETIDOS * 2 > repeticiones && (fin = mover(i, j, DOWN))) {
				mensaje("DOWN");
			}
		}
		else if (campoJuego[i][j - 1] == TELEPORT) {
			if (posicionTeleport1[0] == i && posicionTeleport1[1] == j - 1) mover(posicionTeleport2[0], posicionTeleport2[1], LEFT);
			else if (posicionTeleport2[0] == i && posicionTeleport2[1] == j - 1) mover(posicionTeleport1[0], posicionTeleport1[1], LEFT);
		}
	}
	else if (direccion == UP) {
		for (i = x, j = y; i > 0 && campoJuego[i - 1][j] == SUELO && !fin; i--) {}

		if (tocaVacioUp(i, j)) {
			return false;
		}
		else if (campoJuego[i - 1][j] == AGUJERO || fin) {
			fin = true;
			//cout << fin << endl;
			return true;
		}
		else if (tocaPiedraUp(i, j) || tocaCajaUp(i, j)) {
			repeticiones = 0;
			for (int p = 0; p <= posRecorrido; p++) {
				if (i == recorrido[p][0] && j == recorrido[p][1]) {
					repeticiones++;
				}
			}
			if (!tocaPiedraLeft(i, j) && !tocaCajaLeft(i, j) && !tocaVacioLeft(i, j) && MAX_REPETIDOS > repeticiones && (fin = mover(i, j, LEFT))) {
				mensaje("LEFT");
			}
			else if (!tocaPiedraRight(i, j) && !tocaCajaRight(i, j) && !tocaVacioRight(i, j) && MAX_REPETIDOS * 2 > repeticiones && (fin = mover(i, j, RIGHT))) {
				mensaje("RIGHT");
			}
		}
		else if (campoJuego[i - 1][j] == TELEPORT) {
			if (posicionTeleport1[0] == i - 1 && posicionTeleport1[1] == j) mover(posicionTeleport2[0], posicionTeleport2[1], UP);
			else if (posicionTeleport2[0] == i - 1 && posicionTeleport2[1] == j) mover(posicionTeleport1[0], posicionTeleport1[1], UP);
		}
	}
	else if (direccion == DOWN) {
		for (i = x, j = y; i < SIZE_MAP && campoJuego[i + 1][j] == SUELO && !fin; i++) {}

		if (tocaVacioDown(i, j)) {
			return false;
		}
		else if (campoJuego[i + 1][j] == AGUJERO || fin) {
			fin = true;
			//cout << fin << endl;
			return true;
		}
		else if (tocaPiedraDown(i, j) || (tocaCajaDown(i, j))) {
			repeticiones = 0;
			for (int p = 0; p <= posRecorrido; p++) {
				if (i == recorrido[p][0] && j == recorrido[p][1]) {
					repeticiones++;
				}
			}
			if (!tocaPiedraLeft(i, j) && !tocaCajaLeft(i, j) && !tocaVacioLeft(i, j) && MAX_REPETIDOS > repeticiones && (fin = mover(i, j, LEFT))) {
				mensaje("LEFT");
			}
			else if (!tocaPiedraRight(i, j) && !tocaCajaRight(i, j) && !tocaVacioRight(i, j) && MAX_REPETIDOS * 2 > repeticiones && (fin = mover(i, j, RIGHT))) {
				mensaje("RIGHT");
			}
		}
		else if (campoJuego[i + 1][j] == TELEPORT) {
			if (posicionTeleport1[0] == i + 1 && posicionTeleport1[1] == j) mover(posicionTeleport2[0], posicionTeleport2[1], DOWN);
			else if (posicionTeleport2[0] == i + 1 && posicionTeleport2[1] == j) mover(posicionTeleport1[0], posicionTeleport1[1], DOWN);
		}
	}
	if (!cajaMovida && !fin) {
		if (j + 1 < SIZE_MAP && tocaCajaRight(i, j) && tocaSueloRight(i,j)) { //CAJA DERECHA
			if (fin = moverCaja(i, j, RIGHT)) mensaje("CAJA_RIGHT");
		}
		else if (j - 1 > 0 && tocaCajaLeft(i, j) && campoJuego[i][j - 2] == SUELO) { //CAJA IZQUIERDA
			if (fin = moverCaja(i, j, LEFT)) mensaje("CAJA_LEFT");
		}
		else if (i - 1 > 0 && tocaCajaUp(i, j) && campoJuego[i - 2][j] == SUELO) { //CAJA ARRIBA
			if (fin = moverCaja(i, j, UP)) mensaje("CAJA_UP");
		}
		else if (i + 1 < SIZE_MAP && tocaCajaDown(i, j) && campoJuego[i + 2][j] == SUELO) { //CAJA ABAJO
			if (fin = moverCaja(i, j, DOWN)) mensaje("CAJA_DOWN");
		}
	}
	
	//cout << fin << endl;

	return fin;
}

bool moverCaja(int x, int y, int direccion) {
	int i = x, j = y;

	if (direccion == RIGHT) {
		if (j + 1 < SIZE_MAP && tocaCajaRight(i, j) && campoJuego[i][j + 2] == SUELO || fin) {
			campoJuego[i][j + 1] = SUELO;
			campoJuego[i][j + 2] = PIEDRA;
			j++;
			cajaMovida = true;
		}
		if (tocaPiedraRight(i, j) || (cajaMovida && tocaCajaRight(i, j))) {
			if (fin = mover(i, j, LEFT)) mensaje("LEFT");
			else if (fin = mover(i, j, UP)) mensaje("UP");
			else if (fin = mover(i, j, DOWN)) mensaje("DOWN");
		}
	}
	else if (direccion == LEFT) {
		if (j - 1 > 0 && tocaCajaLeft(i, j) && campoJuego[i][j - 2] == SUELO || fin) {
			campoJuego[i][j - 1] = SUELO;
			campoJuego[i][j - 2] = PIEDRA;
			j--;
			cajaMovida = true;
		}
		if (tocaPiedraLeft(i, j) || (cajaMovida && tocaCajaLeft(i, j))) {
			if (fin = mover(i, j, RIGHT)) mensaje("RIGHT");
			else if (fin = mover(i, j, UP)) mensaje("UP");
			else if (fin = mover(i, j, DOWN)) mensaje("DOWN");
		}
	}
	else if (direccion == UP) {
		if (i - 1 > 0 && tocaCajaUp(i, j) && campoJuego[i - 2][j] == SUELO) {
			campoJuego[i - 1][j] = SUELO;
			campoJuego[i - 2][j] = PIEDRA;
			i--;
			cajaMovida = true;
		}
		if (tocaPiedraUp(i, j) || (cajaMovida && tocaCajaUp(i, j))) {
			if (fin = mover(i, j, LEFT)) mensaje("LEFT");
			else if (fin = mover(i, j, RIGHT)) mensaje("RIGHT");
			else if (fin = mover(i, j, DOWN)) mensaje("DOWN");
		}
	}
	else if (direccion == DOWN) {
		if (i + 1 < SIZE_MAP && tocaCajaDown(i, j) && campoJuego[i + 2][j] == SUELO) {
			campoJuego[i + 1][j] = SUELO;
			campoJuego[i + 2][j] = PIEDRA;
			i++;
			cajaMovida = true;
		}
		if (tocaPiedraDown(i, j) || (cajaMovida && tocaCajaDown(i, j))) {
			if (fin = mover(i, j, LEFT)) mensaje("LEFT");
			else if (fin = mover(i, j, RIGHT)) mensaje("RIGHT");
			else if (fin = mover(i, j, DOWN)) mensaje("UP");
		}
	}
	/*
	if (j + 1 < SIZE_MAP && campoJuego[i][j + 1] == CAJA && campoJuego[i][j + 1] == SUELO) { //CAJA DERECHA
		if (fin = moverCaja(i, j, RIGHT)) mensaje("CAJA_RIGHT");
		else if (fin = moverCaja(i, j, LEFT)) mensaje("CAJA_LEFT");
		else if (fin = moverCaja(i, j, UP)) mensaje("CAJA_UP");
		else if (fin = moverCaja(i, j, DOWN)) mensaje("CAJA_DOWN");
	}
	else if (j - 1 > 0 && campoJuego[i][j - 1] == CAJA && campoJuego[i][j - 2] == SUELO) { //CAJA IZQUIERDA
		if (fin = moverCaja(i, j, RIGHT)) mensaje("CAJA_RIGHT");
		else if (fin = moverCaja(i, j, LEFT)) mensaje("CAJA_LEFT");
		else if (fin = moverCaja(i, j, UP)) mensaje("CAJA_UP");
		else if (fin = moverCaja(i, j, DOWN)) mensaje("CAJA_DOWN");
	}
	else if (i - 1 > 0 && campoJuego[i - 1][j] == CAJA && campoJuego[i - 2][j] == SUELO) { //CAJA ARRIBA
		if (fin = moverCaja(i, j, RIGHT)) mensaje("CAJA_RIGHT");
		else if (fin = moverCaja(i, j, LEFT)) mensaje("CAJA_LEFT");
		else if (fin = moverCaja(i, j, UP)) mensaje("CAJA_UP");
		else if (fin = moverCaja(i, j, DOWN)) mensaje("CAJA_DOWN");
	}
	else if (i + 1 < SIZE_MAP && campoJuego[i + 1][j] == CAJA && campoJuego[i + 2][j] == SUELO) { //CAJA ABAJO
		if (fin = moverCaja(i, j, RIGHT)) mensaje("CAJA_RIGHT");
		else if (fin = moverCaja(i, j, LEFT)) mensaje("CAJA_LEFT");
		else if (fin = moverCaja(i, j, UP)) mensaje("CAJA_UP");
		else if (fin = moverCaja(i, j, DOWN)) mensaje("CAJA_DOWN");
	}
	*/

	if (!fin) {
		cajaMovida = false;
		reconstruirCampo();
	}
	//cout << fin << endl;
	return fin;
}

HBITMAP ImageFromClipboard(HWND hWnd)
{
	if (!OpenClipboard(hWnd))
		return NULL;

	hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
	CloseClipboard();
	return hBitmap;
}
bool guardarImagen() {
	//HWND hWnd = GetDesktopWindow();
	//HBITMAP hBitmap;
	while (true) {
		hBitmap = ImageFromClipboard(hWnd);
		if (hBitmap != NULL && hBitmap != hBitmap_old)
			break;
	}
	/*hBitmap = ImageFromClipboard(hWnd);
	if (hBitmap == NULL)
	return false;*/
	hBitmap_old = hBitmap;
	CImage image;
	image.Attach(hBitmap);
	timestamp = time(0);
	//cout << t << " seconds\n";
	if (image) {
		string s = "C:\\Users\\Chipi\\source\\repos\\Atlanteine\\Debug\\test_" + to_string(timestamp) + ".jpg";
		wstring url(s.begin(), s.end());
		const string s2 = "copy " + s + " C:\\Users\\Chipi\\source\\repos\\Atlanteine\\Debug\\test.jpg";
		remove("C:\\Users\\Chipi\\source\\repos\\Atlanteine\\Debug\\matriz.txt"); //Se borra la matriz anterior
		image.Save((LPCTSTR)(url.c_str()), Gdiplus::ImageFormatJPEG);
		cout << std::system(s2.c_str()) << endl;
		//image.Save(_T("C:\\Users\\Chipi\\source\\repos\\Atlanteine\\Debug\\test.jpg"), Gdiplus::ImageFormatJPEG);
		
	}
		
	return true;
}

string exec(const char* cmd) {
	array<char, 4096> buffer;
	string result;
	shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) throw runtime_error("popen() failed!");
	while (!feof(pipe.get())) {
		if (fgets(buffer.data(), 4096, pipe.get()) != nullptr)
			result += buffer.data();
	}
	return result += "\n";
}


