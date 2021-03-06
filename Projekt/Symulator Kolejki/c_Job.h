#pragma once
#include <ctime>
#include "s_Rect.h"
#include <iostream>//tego moze nie byc
using namespace std;//tego tez nie 

class Server;


//Zg這szenie
class Job
{
public:
	int id;					//unikalny identyfikator zgloszenia
	Rect pos;				//pozycja zg這szenia na ekranie symulacji
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
							* 1 - zg這szenie idzie do kolejki 
							* 2 - zg這szenie idzie do kana逝
							* 3 + zg這szenie jest obs逝giwane
							* 4 - zg這szenie opuszcza system
							* 5 - zg這szenie wraca do kolejki (mo瞠 wystarczy punkt 1) po obsluzeniu
							* 6 - zg這szenie jest odrzucane lub rezygnuje z kolejki (niecierpliwe)
							* 7 + zg這szenie jest w kolejce
							* 8 + zgloszenie jest w obrabiarce, system kolejkowy zamkniety
							* 9 - zgloszenie idzie od rozrabiarki do kolejki
							* 10 - zgloszenie idzie do rozrabiarki, zamkniety system
							*/

	Server* choice;		//wka�nik do kana逝, kt鏎y zosta� wybrany przez nasze zg這szenie

	bool just_resigned;		//domyslnie false, true jezeli zgloszenie wlasnie zrezygnowalo
	bool already_serviced;	//domyslnie false, true jezeli zgloszenie wchodzi kolejny raz do kolejki

	clock_t service_end_time;	//czas kiedy kana� sko鎍zy obs逝giwa� zg這szenie
								//tylko do celow symulacji naukowej



	//POTRZEBNE DO WZOR紟 LITTLE'A, TUDZIE� RYSOWANIA WYKRES紟
	//czasu przebywania jenostki zar闚no w systemie, jak i w kolejce, oraz ich �rednich
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
