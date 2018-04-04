#pragma once
#include <iostream>
#include <ctime>
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

/*
set of functions needed
during the simulation phase
*/
namespace Simulation
{
	void start();
	void mainLoop(int val);
	void render();
	void update();
	void keysPress( unsigned char key, int x, int y );
	void keysSpecialPress(int key,int x, int y);
	void mousePress(int button, int state, int x, int y);
	void mouseMovement(int x, int y);
	void mousePressedMovement(int x, int y);

	double r_wykladniczy(double EX);
	
	//int getPixel ( int x , int y );
};
bool prob(int chance);	//there's a chance chance that this function will return true;
void update_average_business(); //dodajemy przed kazdym wejsciem zgloszenia do kolejki
void update_average_queue_size();
bool equals(double d1, double d2);
//funkcja losuje jeden element z tablicy na podstawie prawdopodobienstw i 
//zwraca indeks elementu, ktory wylosowano
int get_services_number(int* tab, int size);

