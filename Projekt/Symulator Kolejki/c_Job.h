#pragma once
#include <ctime>
#include "s_Rect.h"
#include <iostream>//tego moze nie byc
using namespace std;//tego tez nie 

class Server;


//Zg³oszenie
class Job
{
public:
	int id;					//unikalny identyfikator zgloszenia
	Rect pos;				//pozycja zg³oszenia na ekranie symulacji
	int priority;			//poki co mamy systemy bez pierwszenstwa - domyslnie 0
	int services;			//ile razy jedno zgloszniee ma byc obsluzone - domyslnie 1
	bool impatient;			//czy zgloszenie jest niecierpliwe, tzn. moze zrezygnowac
	clock_t resign_time;	//czas bezwgledny po jakim zgloszenie opusci kolejke
	int get_impatient_in;	/*
								czas w sekundach po jakim ogloszenie od wejscia do systemu sie zniecierpliwia
								potrzebne do obliczenia ile juz czasu zgloszenie sie niecierpliwi, aby
								nadac mu odpowiedni kolor - czym dluzej sie niecierpliwi, tym czerwienszy
							*/

	int action;			/**
							* action:
							* 1 - zg³oszenie idzie do kolejki 
							* 2 - zg³oszenie idzie do kana³u
							* 3 + zg³oszenie jest obs³ugiwane
							* 4 - zg³oszenie opuszcza system
							* 5 - zg³oszenie wraca do kolejki (mo¿e wystarczy punkt 1) po obsluzeniu
							* 6 - zg³oszenie jest odrzucane lub rezygnuje z kolejki (niecierpliwe)
							* 7 + zg³oszenie jest w kolejce
							* 8 + zgloszenie jest w obrabiarce, system kolejkowy zamkniety
							* 9 - zgloszenie idzie od rozrabiarki do kolejki
							* 10 - zgloszenie idzie do rozrabiarki, zamkniety system
							*/

	Server* choice;		//wkaŸnik do kana³u, który zosta³ wybrany przez nasze zg³oszenie

	bool just_resigned;		//domyslnie false, true jezeli zgloszenie wlasnie zrezygnowalo
	bool already_serviced;	//domyslnie false, true jezeli zgloszenie wchodzi kolejny raz do kolejki

	clock_t service_end_time;	//czas kiedy kana³ skoñczy obs³ugiwaæ zg³oszenie
								//tylko do celow symulacji naukowej



	//POTRZEBNE DO WZORÓW LITTLE'A, TUDZIE¯ RYSOWANIA WYKRESÓW
	//czasu przebywania jenostki zarówno w systemie, jak i w kolejce, oraz ich œrednich
	clock_t system_join_time;	//czas kiedy zgloszenie weszlo do systemu/kolejki
	clock_t queue_join_time;	//potrzebne rozroznienie w przypadku, gdy zgloszenie chce byc ponownie obslugiwane
								//wtedy znow wchodzi do kolejki wiec queue_join_time sie zmienia, ale
								//system_join_time nie ulega zmianie

public:
	Job(void);
	~Job(void);
	
	void set(int id, Rect pos, int priority, int services, bool impatient, clock_t resign_time, int get_impatient_in, int action);
	void set(int id, int priority, int services, bool impatient, clock_t resign_time, int get_impatient_in, int action, clock_t service_end_time);
};
