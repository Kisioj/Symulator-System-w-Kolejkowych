#pragma once
#include "s_Label.h"
#include "s_Rect.h"
#include "s_Point.h"
#include "c_Server.h"
#include "c_Queue.h"
#include "n_Simulation.h"
#include <list>
#include <vector>
#include <ctime>
#include <sstream>
//#include <cmath>
using namespace std;

//Symulacja
class Simulation_Box
{
public:
	Label title;
	Rect position;
	bool can_render;

	bool closed_queue_system;			//domyslnie false - system kolejkowy zamkniety
	bool write_data_to_file;			//domyslnie true, zapisywanie danych


	vector<Server> destroyers;			//obrabiarki, tylko do modelu zamknietego, wtedy servers to konserwatorzy
	
	vector<Server> servers;
	int available_servers;			//w przypadku animacji drogi do servera, server jest juz wtedy uznawany za niedostepny
									//mimo, iz nie znajduje sie w nim zadne zgloszenie - lecz do niego idzie
									// w takim razie nie mozna obliczyc zajetych serverow odejmujac od liczby serverow
									// liczbe wolnych serverow: przyda sie nowa zmienna: busy_servers
	int busy_servers;				//domyslnie 0 - liczba zajetych serverow

	int max_servers_size;			//CURRENTLY NOT USED, 10 BY DEFAULT

	int queue_size;
	int max_queue_size;
	int queue_x;	//wartosc w pixelach od ktorego zaczyna sie kolejka
	int queue_y;	//wartosc w pixelach gdzie jest srodek kolejki

	int queue_end_x;	//na mojej rozdzielczosci to jest 490, ale na innej bedzie inaczej, czyli prawy skraj poczekalni

	list<Job> jobs;
	list<Job> leaving_jobs;		//zgloszenia odrzucone, lub zniecierpiwione, ktore opuszczaja kolejke
	Queue queue;

	clock_t end_time;
	//int simulation_time;
	
	int jobs_serviced;		//ilosc zgloszen, ktore juz obsluzono
	int jobs_rejected;		//ilosc zgloszen, ktore odrzucono
	int jobs_resigned;		//ilosc zgloszen, ktore same zrezygnowaly

	int jobs_repaired;		//ilosc maszyn, ktore naprawiono - SYSTEM KOLEJKOWY ZAMKNIETY
	int jobs_destroyed;		//ilosc maszyn, ktore zepsuto - SYSTEM KOLEJKOWY ZAMKNIETY

	//domyslnie false, jezeli true to nie bedzie animacji jak zgloszenia ida do kana³ów, 
	//bedzie to wygladalo jakby sie teleportowaly
	bool instant_service;

	//domyslnie false, jezeli true to nie bedzie animacji jak zlogszenia ida do kolejki,
	//oraz jezeli zgloszenie chce zostac ponownie obsluzone, to tez ta zmienna na true
	//pokze animacje jak to zgloszenie idzie do kolejki
	//bedzie to wygladalo jakby sie teleportowaly
	bool instant_queue;

	//domyslnie false, jezeli true to nie bedzie animacji jak zgloszenia zostaja odrzucone
	bool instant_reject;

	//domyslnie false, jezeli true to nie bedzie animacji jak zgloszenia wychodza z kolejki
	bool instant_leave;

	//domyslnie false, jezeli true to nie bedzie animacji jak zgloszenia rezygnuja z kolejki
	bool instant_resign;

	

	//bool map[203][103];
	bool map[609][309];

	int service_time_distribution;		//+1 - rozklad wykladniczy, 2 - rozklad jednostajny, 3 - bez rozkladu; stale przyjscia
	double average_service_time;		//in 1/10 seconds, for ex. 3,5s
	double min_service_time;			//+jezeli wybrano rozklad jednostajny
	double max_service_time;			//+jezeli wybrano rozklad jednostajny

	int new_job_time_distribution;		//+1 - rozklad wykladniczy, 2 - rozklad jednostajny, 3 - bez rozkladu; stale przyjscia
	double average_new_job_time;		//in 1/10 seconds, max w ciagu minuty 600, czyli 1/10 sekundy najmniej
	double min_new_job_time;			//+jezeli wybrano rozklad jednostajny
	double max_new_job_time;			//+jezeli wybrano rozklad jednostajny

	int vip_probability;				//from 0 to 100, probability that new job is vip (has high priority)
	int vip_queue_size;					//rozmiar kolejki dla zgloszen z wysokim priorytetem

	int impatient_probability;			//szansa, ze nowe zgloszenie bedzie niecierpliwe
	//int extra_service_probability;		//szansa, ze nowe zgloszenie, bedzie chcialo byc obslugiwane 2 tazy
										//tymczasowo, potem trzeba zmienic, jaka szansa na konkretna ilosc obsluzen
	
	int impatient_time_distribution;	//1 - rozklad wykladniczy, 2 - rozklad jednostajny, 3 - bez rozkladu; stale przyjscia
	double average_impatient_time;		//domyslnie 10.0
	double min_impatient_time;
	double max_impatient_time;

	int services_probabilities[5];
	
public:
	Simulation_Box(void);
	~Simulation_Box(void);

	 //zwraca wskaŸnik na kana³, znajduj¹cy siê najbli¿esz wspó³rzêdnych podanych w argumencie
	Server* get_available_server(int x, int y);
	//zwraca wskaznik na destroyer(obrabiarke) znajdujaca sie najblizej podanych wspolrzednych (SYSTEM ZAMKNIETY)
	Server* get_available_destroyer(int x, int y);
	bool can_move(int x, int y, int dir);
	//list<Job>::iterator get_next_element(list<Job>::iterator it, list<Job>::iterator begin, list<Job>::iterator end);
	int get_next_element_x(list<Job>::iterator it);
	void set(Label title, int x, int y, int w, int h);
	float impatient_level(list<Job>::iterator it);	/*
													zwraca wartosc zniecierpliwienia od 0.0f (niezniecierpliwiony)
													do 1.0f (maksymalnie zniecierpliwony; opuszcza kolejke)
													potrzebne do kolorowania na czerwono zniecierpliwionych zgloszen
													*/
	void update();
	void render();
	void render_info();
	void init_info();
	
};
