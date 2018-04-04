#include "c_Queue.h"

Queue::Queue(void)
{
	max_size = numeric_limits<unsigned long long>::max();
	max_size = 50;
}

Queue::~Queue(void)
{
}
void Queue::set(Rect pos, unsigned long long max_size){
	this->pos.set(pos.x,pos.y,pos.w,pos.h);
	this->max_size = max_size;
}
