#pragma once
#include <list>
#include "c_Job.h"
#include "n_Simulation.h"
#include "h_Includes.h"
//#include "c_System.h"
#include <limits>
using namespace std;

class Scientific
{
public:
	list<Job*> destroyers;				//obrabiarki, TYLKO system kolejkowy ZAMKNIETY
	list<Job*> queue;
	list<Job*> servers;
	list<Job*> impatients;
	int max_queue_size;
	int max_servers_size;

	int busy_servers;					//domyslnie 0, liczba zajetych serverow

	bool closed_queue_system;			//domyslnie false - system kolejkowy zamkniety
	bool write_data_to_file;			//domyslnie true, zapisywanie danych wykresow do pliku txt

	int service_time_distribution;		//1 - rozklad wykladniczy, 2 - rozklad jednostajny, 3 - bez rozkladu; stale przyjscia
	double average_service_time;		//in seconds, ale ulamek, for ex. 3,5s
	double min_service_time;			//jezeli wybrano rozklad jednostajny
	double max_service_time;			//jezeli wybrano rozklad jednostajny

	int new_job_time_distribution;		//1 - rozklad wykladniczy, 2 - rozklad jednostajny, 3 - bez rozkladu; stale przyjscia
	double average_new_job_time;		//in 1/10 seconds, max w ciagu minuty 600, czyli 1/10 sekundy najmniej
	double min_new_job_time;			//jezeli wybrano rozklad jednostajny
	double max_new_job_time;			//jezeli wybrano rozklad jednostajny

	int vip_probability;				//from 0 to 100, probability that new job is vip (has high priority)
	//int vip_queue_size;					//rozmiar kolejki dla zgloszen z wysokim priorytetem

	int impatient_probability;			//szansa, ze nowe zgloszenie bedzie niecierpliwe
	//int extra_service_probability;		//szansa, ze nowe zgloszenie, bedzie chcialo byc obslugiwane 2 tazy
										//tymczasowo, potem trzeba zmienic, jaka szansa na konkretna ilosc obsluzen
	
	int impatient_time_distribution;	//1 - rozklad wykladniczy, 2 - rozklad jednostajny, 3 - bez rozkladu; stale przyjscia
	double average_impatient_time;		//domyslnie 10.0
	double min_impatient_time;
	double max_impatient_time;

	clock_t end_time;
	//int simulation_time;
	
	int jobs_serviced;		//ilosc zgloszen, ktore juz obsluzono
	int jobs_rejected;		//ilosc zgloszen, ktore odrzucono
	int jobs_resigned;		//ilosc zgloszen, ktore same zrezygnowaly

	int jobs_repaired;		//ilosc maszyn, ktore naprawiono - SYSTEM KOLEJKOWY ZAMKNIETY
	int jobs_destroyed;		//ilosc maszyn, ktore zepsuto - SYSTEM KOLEJKOWY ZAMKNIETY

	int services_probabilities[5];


public:
	void update();
	void render_info();
	void render();
	void set();
	Scientific(void);
	~Scientific(void);
};
