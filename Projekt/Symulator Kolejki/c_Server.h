#pragma once
#include "c_Job.h"

//Kana³
class Server
{
public:
	int id;						//unikalny identyfikator kana³u
	Rect pos;					//pozycja kana³u na ekranie symulacji	
	Job* servicing;				//wskaznik do aktualnie obslugiwanego zgloszenia
	bool saved;					//czy dany kanal jest zajety, tzn. czy wlasnie ktos do niego zmierza
	time_t service_end_time;	//czas kiedy kana³ skoñczy obs³ugiwaæ zg³oszenie
public:
	Server(void);
	~Server(void);
	void set(int id, Rect pos, Job* servicing, time_t service_end_time);
};
