#include "c_Job.h"

Job::Job(void)
{
	//std::cout << "+";
}

Job::~Job(void)
{
	//std::cout << "-";
}

void Job::set(int id, Rect pos, int priority, int services, bool impatient, clock_t resign_time, int get_impatient_in,  int action){
	this->id = id;
	this->pos.set(pos.x,pos.y,pos.w,pos.h);
	this->priority = priority;
	this->services = services;
	this->impatient = impatient;
	this->resign_time = resign_time;
	this->get_impatient_in = get_impatient_in;
	this->action = action;
	this->choice = NULL;
	this->just_resigned = false;
	this->already_serviced = false;
}

void Job::set(int id, int priority, int services, bool impatient, clock_t resign_time, int get_impatient_in, int action, clock_t service_end_time){
	this->id = id;
	this->priority = priority;
	this->services = services;
	this->impatient = impatient;
	this->resign_time = resign_time;
	this->get_impatient_in = get_impatient_in;
	this->action = action;
	this->service_end_time = service_end_time;


}