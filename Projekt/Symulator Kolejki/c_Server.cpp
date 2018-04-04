#include "c_Server.h"

Server::Server(void)
{
	id	= 0;
	pos.set(0,0,0,0);
	servicing = NULL;
	service_end_time = 0;
	saved = false;
}

Server::~Server(void)
{
}

void Server::set(int id, Rect pos, Job* servicing, time_t service_end_time){
	this->id = id;
	this->pos.set(pos.x,pos.y,pos.w,pos.h);
	this->servicing = servicing;
	this->service_end_time = service_end_time;

}