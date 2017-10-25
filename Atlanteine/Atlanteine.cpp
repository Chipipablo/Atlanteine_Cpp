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






using namespace std;

const static int SIZE_MAP = 15;

//const static int MAP[SIZE_MAP][SIZE_MAP] = 
/*{
	{ 0,0,0,0,5,0,0 },
	{ 0,4,1,1,1,1,0 },
	{ 0,1,1,5,1,1,0 },
	{ 0,1,1,1,1,5,0 },
	{ 0,1,1,1,1,1,0 },
	{ 0,1,5,1,1,2,0 },
	{ 0,0,0,0,0,0,0 } };*/
	static int MAP[SIZE_MAP][SIZE_MAP] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 3, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 5, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 5, 5, 5, 5, 5, 1, 5, 5, 5, 1, 1, 1, 1, 0 },
	{ 0, 0, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 0, 0 },
	{ 0, 0, 1, 1, 1, 5, 1, 5, 1, 1, 1, 1, 2, 5, 0 },
	{ 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 } };

static stack<string> pila;

static int campoJuego[SIZE_MAP][SIZE_MAP];

const static int UP = 1;
const static int RIGHT = 2;
const static int DOWN = 3;
const static int LEFT = 4;

const static int VACIO = 0;
const static int SUELO = 1;
const static int CALABAZA = 2;
const static int CAJA = 3;
const static int AGUJERO = 4;
const static int PIEDRA = 5;
const static int TELEPORT = 6;
static int posicionInicial[2]= { -1,-1 };
static int posicionTeleport1[2] = {-1,-1};
static int posicionTeleport2[2] = { -1,-1 };
static bool fin = false;
static int recorrido[50][3];
const static int MAX_REPETIDOS = 3;
static int repeticiones = 0;
static int posRecorrido = -1;
static bool cajaMovida = false;


static bool mover(int x, int y, int direccion);
static bool moverCaja(int x, int y, int direccion);
static void resolverAtlanteine();
HBITMAP ImageFromClipboard(HWND hWnd);
bool guardarImagen();
string exec(const char* cmd);

static void mensaje(string txt) {
	pila.push(txt);
}

int main()
{
	cout << "Copie una imagen al portapapeles..." << endl;
	cout << guardarImagen() << endl;
	char* cmd = "C:\\Users\\Chipi\\source\\repos\\Atlanteine\\Debug\\bash.exe -c \"/home/chipi/test_Atlanteine.sh\"";
	string raw_matrix = exec(cmd);
	//cout << "SALIDA: " << endl;
	//cout << raw_matrix << endl;

	for (int i=0, n=0, j=1; i < SIZE_MAP; i++) {
		for (j=0; j < SIZE_MAP; j++) {

			n=stoi(&raw_matrix[(i*SIZE_MAP + j+1) * 3 - 1 + i * 2]);
			
			//if (n == 0) n = 1;
			//else if (n == 1) n = 0;
			cout << n << " ";
			MAP[i][j] = n;
		}
		cout << endl;
	}

	resolverAtlanteine();

	system("pause");
    return 0;
}

static void resolverAtlanteine() {
	for (int x = 0; SIZE_MAP > x; x++) {
		for (int y = 0; SIZE_MAP > y; y++) {
			campoJuego[x][y] = MAP[x][y];
			if (campoJuego[x][y] == CALABAZA) {
				posicionInicial[0] = x;
				posicionInicial[1] = y;
				campoJuego[x][y] = SUELO;
				//MAP[x][y] = SUELO;
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
	int x = posicionInicial[0];
	int y = posicionInicial[1];
	cout << "Inicio: (" << y << "," << x << ")" << endl;
	if (fin = mover(x, y, RIGHT)) mensaje("RIGHT");
	else if (fin = mover(x, y, LEFT)) mensaje("LEFT");
	else if (fin = mover(x, y, RIGHT)) mensaje("RIGHT");
	else if (fin = mover(x, y, DOWN)) mensaje("DOWN");


	while (!pila.empty()) {
		cout << pila.top() << endl;
		pila.pop();
	}

	
}

static bool mover(int x, int y, int direccion) {
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
		for (i = x, j = y; j < SIZE_MAP && campoJuego[i][j + 1] == SUELO && !fin; j++) {}

		if (campoJuego[i][j + 1] == VACIO) {
			return false;
		}
		else if (campoJuego[i][j + 1] == AGUJERO || fin) {
			fin = true;
			//cout << fin << endl;
			return true;
		}
		else if (campoJuego[i][j + 1] == PIEDRA || (cajaMovida && campoJuego[i][j + 1] == CAJA)) {
			repeticiones = 0;
			for (int p = 0; p <= posRecorrido; p++) {
				if (i == recorrido[p][0] && j == recorrido[p][1]) {
					repeticiones++;
				}
			}
			if (MAX_REPETIDOS > repeticiones && (fin = mover(i, j, UP))) {
				mensaje("UP");
			}
			else if (MAX_REPETIDOS * 2 > repeticiones && (fin = mover(i, j, DOWN))) {
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
		if (campoJuego[i][j - 1] == VACIO) {
			return false;
		}
		else if (campoJuego[i][j - 1] == AGUJERO || fin) {
			fin = true;
			//cout << fin << endl;
			return true;
		}
		else if (campoJuego[i][j - 1] == PIEDRA || (cajaMovida && campoJuego[i][j - 1] == CAJA)) {
			repeticiones = 0;
			for (int p = 0; p <= posRecorrido; p++) {
				if (i == recorrido[p][0] && j == recorrido[p][1]) {
					repeticiones++;
				}
			}
			if (MAX_REPETIDOS > repeticiones && (fin = mover(i, j, UP))) {
				mensaje("UP");
			}
			else if (MAX_REPETIDOS * 2 > repeticiones && (fin = mover(i, j, DOWN))) {
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

		if (campoJuego[i - 1][j] == VACIO) {
			return false;
		}
		else if (campoJuego[i - 1][j] == AGUJERO || fin) {
			fin = true;
			//cout << fin << endl;
			return true;
		}
		else if (campoJuego[i - 1][j] == PIEDRA || (cajaMovida && campoJuego[i - 1][j] == CAJA)) {
			repeticiones = 0;
			for (int p = 0; p <= posRecorrido; p++) {
				if (i == recorrido[p][0] && j == recorrido[p][1]) {
					repeticiones++;
				}
			}
			if (MAX_REPETIDOS > repeticiones && (fin = mover(i, j, LEFT))) {
				mensaje("LEFT");
			}
			else if (MAX_REPETIDOS > repeticiones && (fin = mover(i, j, RIGHT))) {
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

		if (campoJuego[i + 1][j] == VACIO) {
			return false;
		}
		else if (campoJuego[i + 1][j] == AGUJERO || fin) {
			fin = true;
			//cout << fin << endl;
			return true;
		}
		else if (campoJuego[i + 1][j] == PIEDRA || (cajaMovida && campoJuego[i + 1][j] == CAJA)) {
			repeticiones = 0;
			for (int p = 0; p <= posRecorrido; p++) {
				if (i == recorrido[p][0] && j == recorrido[p][1]) {
					repeticiones++;
				}
			}
			if (MAX_REPETIDOS > repeticiones && (fin = mover(i, j, LEFT))) {
				mensaje("LEFT");
			}
			else if (MAX_REPETIDOS > repeticiones && (fin = mover(i, j, RIGHT))) {
				mensaje("RIGHT");
			}
		}
		else if (campoJuego[i + 1][j] == TELEPORT) {
			if (posicionTeleport1[0] == i + 1 && posicionTeleport1[1] == j) mover(posicionTeleport2[0], posicionTeleport2[1], DOWN);
			else if (posicionTeleport2[0] == i + 1 && posicionTeleport2[1] == j) mover(posicionTeleport1[0], posicionTeleport1[1], DOWN);
		}
	}
	if (!cajaMovida) {
		if (j + 1 < SIZE_MAP && campoJuego[i][j + 1] == CAJA && campoJuego[i][j + 1] == SUELO) { //CAJA DERECHA
			if (fin = moverCaja(i, j, RIGHT)) mensaje("CAJA_RIGHT");
		}
		else if (j - 1 > 0 && campoJuego[i][j - 1] == CAJA && campoJuego[i][j - 2] == SUELO) { //CAJA IZQUIERDA
			if (fin = moverCaja(i, j, LEFT)) mensaje("CAJA_LEFT");
		}
		else if (i - 1 > 0 && campoJuego[i - 1][j] == CAJA && campoJuego[i - 2][j] == SUELO) { //CAJA ARRIBA
			if (fin = moverCaja(i, j, UP)) mensaje("CAJA_UP");
		}
		else if (i + 1 < SIZE_MAP && campoJuego[i + 1][j] == CAJA && campoJuego[i + 2][j] == SUELO) { //CAJA ABAJO
			if (fin = moverCaja(i, j, DOWN)) mensaje("CAJA_DOWN");
		}
	}
	
	//cout << fin << endl;

	return fin;
}

static bool moverCaja(int x, int y, int direccion) {
	int i = x, j = y;

	if (direccion == RIGHT) {
		if (j + 1 < SIZE_MAP && campoJuego[i][j + 1] == CAJA && campoJuego[i][j + 2] == SUELO || fin) {
			campoJuego[i][j + 1] = SUELO;
			campoJuego[i][j + 2] = PIEDRA;
			j++;
			cajaMovida = true;
		}
		if (campoJuego[i][j + 1] == PIEDRA || (cajaMovida && campoJuego[i][j + 1] == CAJA)) {
			if (fin = mover(i, j, LEFT)) mensaje("LEFT");
			else if (fin = mover(i, j, UP)) mensaje("UP");
			else if (fin = mover(i, j, DOWN)) mensaje("DOWN");
		}
	}
	else if (direccion == LEFT) {
		if (j - 1 > 0 && campoJuego[i][j - 1] == CAJA && campoJuego[i][j - 2] == SUELO || fin) {
			campoJuego[i][j - 1] = SUELO;
			campoJuego[i][j - 2] = PIEDRA;
			j--;
			cajaMovida = true;
		}
		if (campoJuego[i][j - 1] == PIEDRA || (cajaMovida && campoJuego[i][j - 1] == CAJA)) {
			if (fin = mover(i, j, RIGHT)) mensaje("RIGHT");
			else if (fin = mover(i, j, UP)) mensaje("UP");
			else if (fin = mover(i, j, DOWN)) mensaje("DOWN");
		}
	}
	else if (direccion == UP) {
		if (i - 1 > 0 && campoJuego[i - 1][j] == CAJA && campoJuego[i - 2][j] == SUELO) {
			campoJuego[i - 1][j] = SUELO;
			campoJuego[i - 2][j] = PIEDRA;
			i--;
			cajaMovida = true;
		}
		if (campoJuego[i - 1][j] == PIEDRA || (cajaMovida && campoJuego[i - 1][j] == CAJA)) {
			if (fin = mover(i, j, LEFT)) mensaje("LEFT");
			else if (fin = mover(i, j, RIGHT)) mensaje("RIGHT");
			else if (fin = mover(i, j, DOWN)) mensaje("DOWN");
		}
	}
	else if (direccion == DOWN) {
		if (i + 1 < SIZE_MAP && campoJuego[i + 1][j] == CAJA && campoJuego[i + 2][j] == SUELO) {
			campoJuego[i + 1][j] = SUELO;
			campoJuego[i + 2][j] = PIEDRA;
			i++;
			cajaMovida = true;
		}
		if (campoJuego[i + 1][j] == PIEDRA || (cajaMovida && campoJuego[i + 1][j] == CAJA)) {
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

	if (!fin) cajaMovida = false;
	//cout << fin << endl;
	return fin;
}

HBITMAP ImageFromClipboard(HWND hWnd)
{
	if (!OpenClipboard(hWnd))
		return NULL;

	HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
	CloseClipboard();
	return hBitmap;
}
bool guardarImagen() {
	HWND hWnd = GetDesktopWindow();
	HBITMAP hBitmap;
	while (true) {
		hBitmap = ImageFromClipboard(hWnd);
		if (hBitmap != NULL)
			break;
	}
	/*hBitmap = ImageFromClipboard(hWnd);
	if (hBitmap == NULL)
	return false;*/

	CImage image;
	image.Attach(hBitmap);
	image.Save(_T("C:\\Users\\Chipi\\source\\repos\\Atlanteine\\Debug\\test.jpg"), Gdiplus::ImageFormatJPEG);
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


