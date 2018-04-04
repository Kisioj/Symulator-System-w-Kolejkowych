#pragma once
#include "c_Job.h"

//Kana�
class Server
{
public:
	int id;						//unikalny identyfikator kana�u
	Rect pos;					//pozycja kana�u na ekranie symulacji	
	Job* servicing;				//wskaznik do aktualnie obslugiwanego zgloszenia
	bool saved;					//czy dany kanal jest zajety, tzn. czy wlasnie ktos do niego zmierza
	time_t service_end_time;	//czas kiedy kana� sko�czy obs�ugiwa� zg�oszenie
public:
	Server(void);
	~Server(void);
	void set(int id, Rect pos, Job* servicing, time_t service_end_time);
};
