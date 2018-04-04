#pragma once
#pragma warning (disable : 4244)
#pragma warning (disable : 4305)
#include <sstream>
#include <ttmath.h>
#include <assert.h>
#include "h_Includes.h"
#include "n_Simulation.h"
#include "c_Chart.h"
#include "c_Simulation_Box.h"
#include "c_Scientific.h"
#include "LTexture.h"
#include "c_Menu.h"


#include <vector>
using namespace std;


class System
{
public:
	double SCREEN_WIDTH;
	double SCREEN_HEIGHT;
	int SCREEN_COLOR_DEPTH;		//in bits
	int SCREEN_FPS;				//60 frames per second
	int mouse_x, mouse_y;
	vector<Chart> charts;
	
	Simulation_Box sim;
	Scientific sci;
	Menu menu;

	int type;					//1 - educational, 2 - scientific
	LTexture logo;



	int phase;						//1 - logo, 2 - options, 3 - simulation
	clock_t start_simulation_time;


	//ZMIENNE POTRZEBNE DO WYKRESOW:

	//WYKRES ZAJETOSCI KANALOW:
	double average_busy_servers;	//srednia zajetosc, w sumi to suma zajetosci, a nie srednia
	clock_t busy_last_time;	
	//WYKRES SREDNIEJ DLUGOSCI KOLEJKI:
	double average_queue_size;		//srednia dlugosc kolejki, w sumi to suma stedniej dlugosci kolejki
	clock_t queue_last_time;


	double sum_of_waiting_in_queue_times;		//suma czasow pobytu w kolejce wszystkich zgloszen
	double sum_of_waiting_in_system_times;		//suma czasow pobytu w systemie wszystkich zgloszen
	int jobs_that_left_queue;					//zgloszenia, ktore weszly do kanalow/ida do kanalow (w edukacyjnym)
	int jobs_that_left_system;					//zgloszenia, ktore po obsluzeniu opuscily system naturalnie



	stringstream queue_system;	//pomocniczy strumien z zapisanym rodzajem kolejki
	bool options[7];			//pomocnicza tablica do wypisywania rodzaju kolejki na ekranie
	//[0] - system kolejkowy zamkniety										M/M/m/FIFO/N/F
	//[1] - system kolejkowy ze stratami									M/M/m/-/M
	//[2] - system kolejkowy z oczekiwaniem									M/M/m/FIFO/INF
	//[3] - system kolejkowy mieszany										M/M/m/FIFO/m+N
	//[4] - system kolejkowyu o nieograniczonej ilosci kanalow obslugi		M/M/INF
	//[5] - system kolejkowy X z niecierpliwymi klientami
	//[6] - system kolejkowy X z priorytetowymi klientami


	//Zmienne obliczone z Analizy Systemu Kolejkowego
	double pm;			//Prawdopodobienstwo, ze wszystkie kanaly sa zajete
	double q;			//Wzgledna zdolnosc obslugi systemu
	double A;			//Bezwzgledna zdolnosc obslugi systemu
	double m0;			//Srednia liczba zajetych kanalow
	double v;			//Srednia liczba klientow w kolejce
	double n;			//Srednia liczba zgloszen w systemie
	double ts;			//Sredni czas przebywania zgloszenia w systemie

public:
	void Init( int argc, char* args[] );
	void StartSimulation();
	void CreateSimulationBox();
	void CreateCharts();
	void InitSystemQueueType();
	System(void);
	~System(void);
};
