#pragma once
#include <queue>
#include <limits>
#include "c_Job.h"
using namespace std;

class Queue
{
public:
	Rect pos;						//pozycja kolejki na ekranie symulacji
	queue<Job*> jobs;				//kolejka wskaźników do zgłoszeń w niej oczekujących
	unsigned long long max_size;	//maxymalny rozmiar kolejki, max 18446744073709551615
public:
	Queue(void);
	~Queue(void);
	void set(Rect pos, unsigned long long max_size);
};