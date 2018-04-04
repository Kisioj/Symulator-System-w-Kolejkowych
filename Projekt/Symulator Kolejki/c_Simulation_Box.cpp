#include "c_Simulation_Box.h"
#include "c_System.h"

extern System sys;

#define NORTH 1
#define SOUTH 2
#define WEST 3
#define EAST 4

Simulation_Box::Simulation_Box(void)
{
}

Simulation_Box::~Simulation_Box(void)
{
}

void Simulation_Box::set(Label title, int x, int y, int w, int h){
	this->title.set(x+w/2,y,title.s,title.font);
	this->position.set(x,y,w,h);

	this->end_time = 600;		//seconds
	this->can_render = true;
	this->jobs_serviced = 0;
	this->jobs_rejected = 0;
	this->jobs_resigned = 0;
	this->jobs_repaired = 0;
	this->jobs_destroyed = 0;

	this->busy_servers = 0;

	int px_b = 36;//40		//px_between pixele miedzy kanalami, np. jezeli 25, to 45 miedzy srodkami
	int px_b_min = 14;		//minimalne odstepy miedzy kanalami jezeli jest ich duzo
	int a = 22;				//dlugosc boku kwadratu, ktory przedstawia kana³ (server)
	int a_min = 16;			//minimalna dlugosc boku kwadratu, ktory przedstawia server

	int max_servers = 0;	//jest obliczane na dole, dla nas 10
	int servers_amount = max_servers_size;

	queue_end_x = (w-w/14)-75;
	queue_size = 0;
	queue_x = queue_end_x-1-9*max_queue_size;
	queue_y = position.h / 2;

	/*
	¯eby zmieœci³o siê wiêcej kana³ów na ekranie,
	jesteœmy zmuszeni zmniejszaæ odstêpy w pixelach
	pomiêdzy nimi, oraz zmniejszaæ ich rozmiar, a¿
	do minimalnego lub dopóki oka¿e siê, ¿e obecne
	odstêpy i d³ugoœæ boku kana³u wystarczaj¹,
	aby zmieœci³y siê one wszystkie na ekranie.
	*/
	while(servers_amount > max_servers){
		max_servers = h/(px_b+a);
		if(px_b > px_b_min){
			px_b--;
		}else if(a > a_min){
			a--;
		}else{
			servers_amount = max_servers;
		}
	};

	for(int i=0;i<servers_amount;i++){
		int x = w-w/14;
		int y = (h/2)-( (i+1) - ((servers_amount/2.0) + 0.5) )*(px_b+a);
		Rect r = {x,y,a,a};
		Server s;
		s.set(i,r,NULL,0);
		servers.push_back(s);
	}
	available_servers = servers_amount;



	/*
	OBRABIARKI, TYLKO DO SYSTEMU KOLEJKOWEGO ZAMKNIETEGO
	*/
	if(this->closed_queue_system){
		px_b = 36;
		a = 22;
		int destroyers_amount = this->max_queue_size;
		int max_destroyers = 0;
		while(destroyers_amount > max_destroyers){
			max_destroyers = h/(px_b+a);
			if(px_b > px_b_min){
				px_b--;
			}else if(a > a_min){
				a--;
			}else{
				destroyers_amount = max_destroyers;
			}
		};

		destroyers.reserve(10);//zeby nie bylo realokacji zadnych
		for(int i=0;i<destroyers_amount;i++){
			int x = w/14;
			int y = (h/2)-( (i+1) - ((destroyers_amount/2.0) + 0.5) )*(px_b+a);
			Rect r = {x,y,a,a};

			clock_t uptime = clock() / (CLOCKS_PER_SEC / 1000);

			clock_t job_destroy_time;
			if(new_job_time_distribution==1){
				job_destroy_time = uptime + long(Simulation::r_wykladniczy(average_new_job_time)*1000);
			}else if(new_job_time_distribution==2){
				job_destroy_time = uptime + ((rand()%int(max_new_job_time*1000-min_new_job_time*1000) + int(min_new_job_time*1000))/1000.0)*1000;
			}else{
				job_destroy_time = uptime + average_new_job_time*1000;
			}


			static int new_id = 0;
			Job j;
			j.set(new_id++,r, 0, 1, false, numeric_limits<long>::max(), 0, 8);
			jobs.push_back(j);

			Server s;
			s.set(i,r,&jobs.back(),job_destroy_time);
			s.saved = true;
			destroyers.push_back(s);

			jobs.back().choice = &destroyers.back();

		}
	}



	for(int y = 0;y<309;y++){
		for(int x=0;x<609;x++){
			map[x][y] = false;
		}
	}




}



void Simulation_Box::render(){
	//PROSTOK¥T Z SYMULACJ¥
	glBegin(GL_LINE_LOOP);
	glColor3f(0.f, 0.f, 0.f );
	glVertex2f(position.x-1, position.y);
	glVertex2f(position.x, position.y+position.h);
	glVertex2f(position.x+position.w, position.y+position.h);
	glVertex2f(position.x+position.w, position.y);
	glEnd();

	//SZARE ŒCIANY NAD I POD KOLEJK¥
	glColor3f(0.9f,0.9f,0.9f);
	glBegin(GL_QUADS);
	glVertex2f(position.x+queue_x, position.y);
	glVertex2f(position.x+queue_end_x+10, position.y);
	glVertex2f(position.x+queue_end_x+10, position.y+position.h-1);
	glVertex2f(position.x+queue_x, position.y+position.h-1);
	glEnd();
	glColor3f(0.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	glVertex2f(position.x+queue_x+1, position.y);
	glVertex2f(position.x+queue_x+1, position.y+position.h-1);
	glVertex2f(position.x+queue_end_x+10, position.y);
	glVertex2f(position.x+queue_end_x+10, position.y+position.h-1);
	glEnd();

	//KOLEJKA JAKO BIA£A DROGA PRZEZ ŒCIANY
	glColor3f(1.f,1.f,1.f);
	glBegin(GL_QUADS);
	glVertex2f(position.x+queue_x, position.y+(position.h/2)-25);
	glVertex2f(position.x+queue_end_x+10, position.y+(position.h/2)-25);
	glVertex2f(position.x+queue_end_x+10, position.y+(position.h/2)+25);
	glVertex2f(position.x+queue_x, position.y+(position.h/2)+25);
	glEnd();
	glColor3f(0.f,0.f,0.f);
	glBegin(GL_LINES);
	glVertex2f(position.x+queue_x, position.y+(position.h/2)-25);
	glVertex2f(position.x+queue_end_x+10, position.y+(position.h/2)-25);
	glVertex2f(position.x+queue_x, position.y+(position.h/2)+25);
	glVertex2f(position.x+queue_end_x+10, position.y+(position.h/2)+25);
	glEnd();
	if(available_servers){
		glColor3f(0.f, 0.8f, 0.f );
	}else{
		glColor3f(0.9f, 0.f, 0.f );
	}
	glBegin(GL_LINES);
	glVertex2f(position.x+queue_end_x, position.y+(position.h/2)-25);
	glVertex2f(position.x+queue_end_x, position.y+(position.h/2)+24);
	glEnd();

	for(unsigned int i=0;i<servers.size();i++){
		Rect* r = &servers[i].pos;
		if(servers[i].servicing!=NULL){
			glColor3f(0.9f, 0.f, 0.f );
		}else{
			glColor3f(0.f, 0.8f, 0.f );
		}
		glBegin(GL_QUADS);
		glVertex2f(position.x-(r->w/2) + r->x, position.y-(r->h/2) + r->y);
		glVertex2f(position.x-(r->w/2) + r->x, position.y+(r->h/2) + r->y);
		glVertex2f(position.x+(r->w/2) + r->x, position.y+(r->h/2) + r->y);
		glVertex2f(position.x+(r->w/2) + r->x, position.y-(r->h/2) + r->y);
		glEnd();
		glColor3f(0.f, 0.f, 0.f );
		glBegin(GL_LINE_LOOP);
		glVertex2f(position.x-(r->w/2) + r->x-1, position.y-(r->h/2) + r->y);
		glVertex2f(position.x-(r->w/2) + r->x, position.y+(r->h/2) + r->y);
		glVertex2f(position.x+(r->w/2) + r->x, position.y+(r->h/2) + r->y);
		glVertex2f(position.x+(r->w/2) + r->x, position.y-(r->h/2) + r->y);
		glEnd();

		Label l;
		stringstream id_string;
		id_string << servers[i].id;
		l.set(position.x+r->x,position.y+r->y,id_string.str(),GLUT_BITMAP_TIMES_ROMAN_10,CENTER);
		l.render();
	}
	if(this->closed_queue_system){
		for(unsigned int i=0;i<destroyers.size();i++){
			Rect* r = &destroyers[i].pos;
			if(destroyers[i].servicing!=NULL){
				glColor3f(0.9f, 0.f, 0.f );
			}else{
				glColor3f(0.f, 0.8f, 0.f );
			}
			glBegin(GL_QUADS);
			glVertex2f(position.x-(r->w/2) + r->x, position.y-(r->h/2) + r->y);
			glVertex2f(position.x-(r->w/2) + r->x, position.y+(r->h/2) + r->y);
			glVertex2f(position.x+(r->w/2) + r->x, position.y+(r->h/2) + r->y);
			glVertex2f(position.x+(r->w/2) + r->x, position.y-(r->h/2) + r->y);
			glEnd();
			glColor3f(0.f, 0.f, 0.f );
			glBegin(GL_LINE_LOOP);
			glVertex2f(position.x-(r->w/2) + r->x-1, position.y-(r->h/2) + r->y);
			glVertex2f(position.x-(r->w/2) + r->x, position.y+(r->h/2) + r->y);
			glVertex2f(position.x+(r->w/2) + r->x, position.y+(r->h/2) + r->y);
			glVertex2f(position.x+(r->w/2) + r->x, position.y-(r->h/2) + r->y);
			glEnd();

			Label l;
			stringstream id_string;
			id_string << destroyers[i].id;
			l.set(position.x+r->x,position.y+r->y,id_string.str(),GLUT_BITMAP_TIMES_ROMAN_10,CENTER);
			l.render();
		}
	}

	for (list<Job>::iterator it=jobs.begin(); it != jobs.end(); ++it){
		float red = 0.f;
		if(it->impatient && it->action==7){
			/*
			CZYM BARDZIEJ ZNIECIERPLIWIONE ZG£OSZENIE
			TYM BARDZIEJ CZERWONY KOLOR ZG£OSZENIA
			*/
			red = impatient_level(it);
		}
		/*
		W ZAMKNIÊTYM SYSTEMIE KOLEJKOWYM
		INNE ZASADY KOLOROWANIA
		*/
		if(this->closed_queue_system){
			glColor3f(0.f,0.f,0.f);
		}else{
			if(instant_resign){
				glColor3f(red, red, red);
			}else{
				glColor3f(red, 0.f, 0.f );
			}
			if(it->action==6){
				glColor3f(0.9f, 0.f, 0.f );
			}else if(it->action==4){
				glColor3f(0.f, 0.8f, 0.f );
			}else if(it->action==5 || it->already_serviced){
				glColor3f(0.8f, 0.8f, 0.f );
			}else if(it->priority==1){
				if(instant_resign){
					glColor3f(red, red, 1.f);
				}else{
					glColor3f(red, 0.0f, 1.f - red );
				}
			}
		}
		glBegin(GL_QUADS);
		glVertex2f(position.x-3 + it->pos.x, position.y-3 + it->pos.y);
		glVertex2f(position.x-3 + it->pos.x, position.y+3 + it->pos.y);
		glVertex2f(position.x+3 + it->pos.x, position.y+3 + it->pos.y);
		glVertex2f(position.x+3 + it->pos.x, position.y-3 + it->pos.y);
		glEnd();

		if(it->impatient && instant_resign){
			if(it->priority==0){
				glColor3f(0.f, 0.f, 0.f );
			}else if(it->priority==1){
				glColor3f(0.f, 0.f, 1.f );
			}
			glBegin(GL_LINE_LOOP);
			glVertex2f(position.x-2 + it->pos.x-1, position.y-2 + it->pos.y);
			glVertex2f(position.x-2 + it->pos.x, position.y+3 + it->pos.y);
			glVertex2f(position.x+3 + it->pos.x, position.y+3 + it->pos.y);
			glVertex2f(position.x+3 + it->pos.x, position.y-2 + it->pos.y);
			glEnd();
		}
		if(it->choice){
			Label l;
			stringstream id_string;
			id_string << it->choice->id;
			l.set(position.x+it->pos.x,position.y+it->pos.y-5,id_string.str(),GLUT_BITMAP_TIMES_ROMAN_10,CENTER);
			l.render();
		}
	}
	render_info();
}


void Simulation_Box::update(){

	clock_t uptime = clock() / (CLOCKS_PER_SEC / 1000);

	clock_t tmp_time;
	if(new_job_time_distribution==1){
		tmp_time = long(Simulation::r_wykladniczy(average_new_job_time)*1000);
	}else if(new_job_time_distribution==2){
		tmp_time = ((rand()%int(max_new_job_time*1000-min_new_job_time*1000) + int(min_new_job_time*1000))/1000.0)*1000;
	}else{
		tmp_time = average_new_job_time*1000;
	}

	static clock_t new_job_time = uptime + tmp_time;
	static int new_id = 1;

	/*
	nowe zgloszenia przychodza co jakis czas tylko jezeli system kolejkowy nie jest systemem zamknietym;
	jezeli jest to tworzymy na poczatku tyle zgloszen, jak dluga kolejka jest (dokladnie tyle jest destroyerow).
	*/
	if(!this->closed_queue_system){
		if(uptime>new_job_time){
			Rect r = {3,position.h/2,6,6};
			int priority = 0;
			if(prob(vip_probability)){
				priority = 1;
				r.y = (position.h/2)-9;
			}		
			Job j;
			bool impatient = 0;
			if(prob(impatient_probability)){
				impatient = 1;
				if(impatient_time_distribution==1){
					j.get_impatient_in = Simulation::r_wykladniczy(average_impatient_time);
				}if(impatient_time_distribution==2){
					j.get_impatient_in = (rand()% int(max_impatient_time*100-min_impatient_time*100) + int(min_impatient_time*100) )/100.0;
				}else{
					j.get_impatient_in = average_impatient_time;
				}
			}


			int services = get_services_number(services_probabilities,5) + 1;

			j.set(new_id++,r, priority, services, impatient, numeric_limits<long>::max(), j.get_impatient_in, 1);
			jobs.push_back(j);

			if(new_job_time_distribution==1){
				new_job_time = uptime + long(Simulation::r_wykladniczy(average_new_job_time)*1000);
			}else if(new_job_time_distribution==2){
				new_job_time = uptime + ((rand()% int(max_new_job_time*1000-min_new_job_time*1000) + int(min_new_job_time*1000) )/1000.0)*1000;
			}else{
				new_job_time = uptime + average_new_job_time*1000;
			}
		}
	}

	for(unsigned int i =0;i<servers.size();i++){
		if(servers[i].servicing && servers[i].service_end_time < uptime){
			servers[i].saved = false;
			servers[i].servicing->services--;
			servers[i].servicing->choice = NULL;
			if(servers[i].servicing->services>0){	//jezeli dane zgloszenie chce byc jeszcze przynajmniej raz obsluzone
				servers[i].servicing->action = 5;
				servers[i].servicing->already_serviced = true;
			}else{
				servers[i].servicing->action = 4;
				//DO WYKRESOW I ANALIZY
				sys.sum_of_waiting_in_system_times+=clock() - servers[i].servicing->system_join_time; 
				sys.jobs_that_left_system++;	
				////////////////////////
			}
			servers[i].servicing = NULL;
			available_servers++;
			update_average_business();
			busy_servers--;
			if(closed_queue_system){
				jobs_repaired++;
			}else{
				jobs_serviced++;
			}
		}
	}

	for(unsigned int i =0;i<destroyers.size();i++){
		if(destroyers[i].servicing && destroyers[i].service_end_time < uptime){
			destroyers[i].saved = false;
			destroyers[i].servicing->choice = NULL;
			destroyers[i].servicing->action = 9; //idzie do kolejki
			destroyers[i].servicing = NULL;
			jobs_destroyed++;
		}
	}




	if(!jobs.empty()){
		for (list<Job>::iterator it=jobs.begin(); it != jobs.end(); ++it){
			if(it->action == 7 && it->impatient == true){	//rezygnacja z kolejki
				if( (clock() / CLOCKS_PER_SEC) > it->resign_time ){
					it->action = 6;
					it->just_resigned = false;
					if(it->priority==1){
						vip_queue_size--;
					}else{
						update_average_queue_size();
						queue_size--;
					}
					jobs_resigned++;
					if(instant_resign){
						map[it->pos.x][it->pos.y] = false;
						it = jobs.erase(it);
						if(it==jobs.end()){
							break;
						}
					}else if(it->priority==0){
						if(can_move(it->pos.x,it->pos.y,SOUTH)){
							map[it->pos.x][it->pos.y] = false;
							it->pos.y+=3;
							map[it->pos.x][it->pos.y] = true;
						}
					}else if(it->priority==1){
						if(can_move(it->pos.x,it->pos.y,NORTH)){
							map[it->pos.x][it->pos.y] = false;
							it->pos.y-=3;
							map[it->pos.x][it->pos.y] = true;
						}
					}
				}
			}

			if(it->action==1 || it->action==7){				//jezeli zgloszenie idzie do kolejki lub juz w niej jest
				if(it->action==1 && it->pos.x >= queue_x){	//jezeli zg³oszenie idzie, ale jest ju¿ w poczekalni
					it->queue_join_time = clock();			//przypisujemy czas wejscia do kolejki
					it->system_join_time = clock();			//przypisujemy czas wejscia do systemu
					it->action=7;							//to dodajemy je do kolejki
					if(it->impatient==true){
						it->resign_time = (clock() / CLOCKS_PER_SEC) + it->get_impatient_in;
					}
					
					if(it->priority==1){
						vip_queue_size++;
					}else{
						update_average_queue_size();
						queue_size++;
					}
					if(instant_queue){
						map[it->pos.x][it->pos.y] = false;
						if(it->priority==1){
							it->pos.x = queue_end_x-1-9*vip_queue_size;
						}else{
							it->pos.x = queue_end_x-1-9*queue_size;
						}
						map[it->pos.x][it->pos.y] = true;
					}
				}

				if( can_move(it->pos.x,it->pos.y,EAST) ){	//jezeli mozemy sie ruszyc w prawo
					if(it->pos.x+9<queue_end_x){			//jezeli znajdujemy sie na lewo od prawego poczatku poczekalni
						if(instant_queue && it->action==7){
							for(int i =0;i<3;i++){
								if( can_move(it->pos.x,it->pos.y,EAST) ){
									if(it->pos.x+9<queue_end_x){
										map[it->pos.x][it->pos.y] = false;
										it->pos.x+=3;
										map[it->pos.x][it->pos.y] = true;
									}else{
										break;
									}
								}
							}

						}else{
							map[it->pos.x][it->pos.y] = false;
							it->pos.x+=3;
							map[it->pos.x][it->pos.y] = true;
						}
					}else if(available_servers){	//jezeli jest jakis wolny server, to do niego idziemy
						if( !vip_queue_size || it->priority==1 || can_move(it->pos.x,it->pos.y,NORTH) ){
							it->action = 2;
							it->choice = get_available_server(it->pos.x, it->pos.y);
							it->choice->saved = true;

							//DO WYKRESÓW I ANALIZY
							sys.sum_of_waiting_in_queue_times+=clock() - it->queue_join_time;
							sys.jobs_that_left_queue++;
							//////////////////////

							available_servers--;
							if(max_queue_size){
								
								if(it->priority==1){
									vip_queue_size--;
								}else{
									update_average_queue_size();
									queue_size--;
								}
							}
						}
					}else if(max_queue_size==0){
						it->action = 6;
						jobs_rejected++;
						if(instant_reject){
							map[it->pos.x][it->pos.y] = false;
							it = jobs.erase(it);
							if(it==jobs.end()){
								break;
							}
						}else if(it->priority==0){
							if(can_move(it->pos.x,it->pos.y,SOUTH)){
								map[it->pos.x][it->pos.y] = false;
								it->pos.y+=3;
								map[it->pos.x][it->pos.y] = true;
							}
						}else if(it->priority==1){
							if(can_move(it->pos.x,it->pos.y,NORTH)){
								map[it->pos.x][it->pos.y] = false;
								it->pos.y-=3;
								map[it->pos.x][it->pos.y] = true;
							}
						}
					}
				}else if(it->pos.x > queue_x-9 && it->pos.x < queue_x){
					if(!available_servers){//jak sa jakies wolne servery, znaczy, ze miejsce sie zrobi w tej iteracji, dodano ifa 31 maja o 21:41
						it->action = 6;
						jobs_rejected++;
						if(instant_reject){
							map[it->pos.x][it->pos.y] = false;
							it = jobs.erase(it);
							if(it==jobs.end()){
								break;
							}
						}else if(it->priority==0){
							if(can_move(it->pos.x,it->pos.y,SOUTH)){
								map[it->pos.x][it->pos.y] = false;
								it->pos.y+=3;
								map[it->pos.x][it->pos.y] = true;
							}
						}else if(it->priority==1){
							if(can_move(it->pos.x,it->pos.y,NORTH)){
								map[it->pos.x][it->pos.y] = false;
								it->pos.y-=3;
								map[it->pos.x][it->pos.y] = true;
							}
						}
					}

				}

			}else if(it->action==2){
				map[it->pos.x][it->pos.y] = false;
				if(instant_service){
					it->pos.x = it->choice->pos.x;
					it->pos.y = it->choice->pos.y;
				}

				if(it->pos.x+7<queue_end_x+30 && (it->pos.y != it->choice->pos.y) ){
					it->pos.x+=3;
				}else {
					if(it->pos.y != it->choice->pos.y){
						if(it->pos.y < it->choice->pos.y){
							it->pos.y+=3;
						}else if(it->pos.y > it->choice->pos.y){
							it->pos.y-=3;
						}
						if(abs(it->pos.y - it->choice->pos.y) < 3){
							it->pos.y = it->choice->pos.y;
						}
					}else{
						if(it->pos.x < it->choice->pos.x){
							it->pos.x+=3;
						}else if(it->pos.x > it->choice->pos.x){
							it->pos.x-=3;
						}
						if(abs(it->pos.x - it->choice->pos.x) < 3){
							it->pos.x = it->choice->pos.x;
							it->action = 3;
							it->choice->servicing = &(*it);
							if(service_time_distribution==1){//rozklad wykladniczy
								it->choice->service_end_time = uptime + long(Simulation::r_wykladniczy(average_service_time)*1000);
							}else if(service_time_distribution==2){//rozklad jednostajny
								it->choice->service_end_time = uptime + ((rand()% int(max_service_time*100-min_service_time*100) + int(min_service_time*100))/100.0)*1000;
							}else{//wartosc nielosowa
								it->choice->service_end_time = uptime + average_service_time*1000;
							}
							update_average_business();
							busy_servers++;
						}						
					}

				}
			}else if(it->action==4){		//jezeli zgloszenie zostalo obsluzone i opuszcza system

				if(instant_leave){
					if(this->closed_queue_system){
						map[it->pos.x][it->pos.y] = false;
						it->pos.x = 3;
						it->action = 10;
						it->choice = get_available_destroyer(it->pos.x,it->pos.y);
						it->choice->saved = true;
						map[it->pos.x][it->pos.y] = true;
					}else{
						it = jobs.erase(it);
						if(it==jobs.end()){
							break;
						}
					}
				}else{
					it->pos.x+=1;
				}
			}else if(it->action==6){		//jezeli zgloszenie jest odrzucone lub rezygnuje z kolejki i wychodzi
				if( it->pos.y < queue_y + 7 && it->priority==0 ){
					if(can_move(it->pos.x,it->pos.y,SOUTH)){
						map[it->pos.x][it->pos.y] = false;
						it->pos.y+=3;
						map[it->pos.x][it->pos.y] = true;
					}
				}else if( it->pos.y > queue_y - 16 && it->priority==1 ){
					if(can_move(it->pos.x,it->pos.y,NORTH)){
						map[it->pos.x][it->pos.y] = false;
						it->pos.y-=3;
						map[it->pos.x][it->pos.y] = true;
					}
				}else{
					if(can_move(it->pos.x,it->pos.y,WEST)){
						map[it->pos.x][it->pos.y] = false;
						it->pos.x-=3;
						map[it->pos.x][it->pos.y] = true;
					}
				}
			}else if(it->action==5){		//zgloszenie wychodzi z kanalu i wraca do kolejki
				if(max_queue_size==0){//jezeli w systemie nie ma poczekalni to zgloszenie opuszcza system
					it->action=4;
				}
				if(instant_queue){
					if( (it->priority==1 && vip_queue_size < max_queue_size) || 
						(it->priority==0 && queue_size < max_queue_size)){

							int destination_y;		//wspolrzedna y, gdzie ma isc zgloszenie, zalezy czy VIP czy nie
							if(it->priority==1){
								destination_y = queue_y - 9;
							}else if(it->priority==0){
								destination_y = queue_y;
							}
							map[it->pos.x][it->pos.y] = false;//not necessary
							
							if(it->priority==1){
								vip_queue_size++;
								it->pos.x = queue_end_x-1-9*vip_queue_size;
							}else{
								update_average_queue_size();
								queue_size++;
								it->pos.x = queue_end_x-1-9*queue_size;
							}
							it->pos.y = destination_y;
							map[it->pos.x][it->pos.y] = true;
							it->action = 7;
							it->queue_join_time = clock();		//zgloszenie ponownie wchodzi do kolejki by byc
																//obsluzone, wiec ponownie potrzebuje nowego
																//czasu wejscia do kolejki
					}else{
						if(instant_leave){
							it = jobs.erase(it);
							if(it==jobs.end()){
								break;
							}
						}else{
							int destination_x = queue_end_x + 12;
							int destination_y;		//wspolrzedna y, gdzie ma isc zgloszenie, zalezy czy VIP czy nie
							if(it->priority==1){
								destination_y = queue_y - 18;
							}else if(it->priority==0){
								destination_y = queue_y + 9;
							}
							it->pos.x = destination_x;
							it->pos.y = destination_y;
							it->action=6;
						}
					}

				}else{

					/*
					zgloszenie chce wejsc do kolejki, wiec idzie na jej koniec, ale okazuje sie
					np., ¿e wysz³o z poszekalni tzn. kolejka jest pe³na, wtedy zg³oszenie zostaje odrzucone
					stad it->action=6;
					*/
					if(it->pos.x<queue_x){		
						it->action=6;
					}else{
						if(it->pos.x>queue_end_x+15 ){
							it->pos.x-=3;
						}else{
							int destination_y;		//wspolrzedna y, gdzie ma isc zgloszenie, zalezy czy VIP czy nie
							if(it->priority==1){
								destination_y = queue_y - 18;
							}else if(it->priority==0){
								destination_y = queue_y + 9;
							}

							if(it->pos.y != destination_y && it->pos.x > queue_end_x){
								if(it->pos.y < destination_y){
									it->pos.y+=3;
								}else if(it->pos.y > destination_y){
									it->pos.y-=3;
								}
								if(abs(it->pos.y - destination_y) < 3){
									it->pos.y = destination_y;
								}
							}else if(it->pos.y != destination_y){
								if( it->priority==1 && can_move(it->pos.x,it->pos.y,SOUTH) ){
									if(it->pos.y != destination_y+9){
										map[it->pos.x][it->pos.y] = false;
										it->pos.y+=3;
										map[it->pos.x][it->pos.y] = true;
									}
									if( it->pos.y == destination_y+9) {
										it->action=7;
										vip_queue_size++;
										if(it->impatient){
											it->resign_time = (clock() / CLOCKS_PER_SEC) + it->get_impatient_in;
										}
									}
								}else if( it->priority==0 && can_move(it->pos.x,it->pos.y,NORTH) ){
									if(it->pos.y != destination_y-9){
										map[it->pos.x][it->pos.y] = false;
										it->pos.y-=3;
										map[it->pos.x][it->pos.y] = true;
									}else{
										it->action=7;
										it->queue_join_time = clock();		//zgloszenie ponownie wchodzi do kolejki by byc
										update_average_queue_size();
										queue_size++;
										if(it->impatient){
											it->resign_time = (clock() / CLOCKS_PER_SEC) + it->get_impatient_in;
										}
									}
								}
							}else{
								int destination_x;
								if(it->priority==1){
									destination_x = queue_end_x+2-9*(vip_queue_size+1);
								}else if(it->priority==0){
									destination_x = queue_end_x+2-9*(queue_size+1);
								}

								if(it->pos.x < destination_x){
									map[it->pos.x][it->pos.y] = false;
									it->pos.x+=3;
									map[it->pos.x][it->pos.y] = true;
								}else if(it->pos.x > destination_x){
									map[it->pos.x][it->pos.y] = false;
									it->pos.x-=3;
									map[it->pos.x][it->pos.y] = true;
								}
								if(abs(it->pos.x - destination_x) < 3){
									map[it->pos.x][it->pos.y] = false;
									it->pos.x = destination_x;
									map[it->pos.x][it->pos.y] = true;
								}
								if( abs(queue_end_x+2-it->pos.x)%9 == 0  ){
									if( it->priority==1 && can_move(it->pos.x,it->pos.y,SOUTH) ){
										map[it->pos.x][it->pos.y] = false;
										it->pos.y+=3;
										map[it->pos.x][it->pos.y] = true;
									}else if( it->priority==0 && can_move(it->pos.x,it->pos.y,NORTH)){
										map[it->pos.x][it->pos.y] = false;
										it->pos.y-=3;
										map[it->pos.x][it->pos.y] = true;
									}
								}
							}

						}
					}
				}

			}else if(it->action==9){
				if(instant_queue){
					map[it->pos.x][it->pos.y] = false;
					it->pos.x = queue_end_x-1-9*queue_size;
					it->pos.y = position.h/2;
					map[it->pos.x][it->pos.y] = true;
					it->action = 1;
				}else{
					int destination_x = position.w/14+50;
					if(it->pos.x < destination_x/* && can_move(it->pos.x,it->pos.y,EAST) */){
						map[it->pos.x][it->pos.y] = false;
						it->pos.x+=3;
						map[it->pos.x][it->pos.y] = true;
					}
					if(abs(it->pos.x - destination_x) < 3){
						map[it->pos.x][it->pos.y] = false;
						it->pos.x = destination_x;
						map[it->pos.x][it->pos.y] = true;
					}
					if(it->pos.x == destination_x){
						int destination_y = position.h/2;
						if(it->pos.y < destination_y/* && can_move(it->pos.x,it->pos.y,SOUTH) */){
							map[it->pos.x][it->pos.y] = false;
							it->pos.y+=3;
							map[it->pos.x][it->pos.y] = true;
						}else if(it->pos.y > destination_y/* && can_move(it->pos.x,it->pos.y,NORTH) */){
							map[it->pos.x][it->pos.y] = false;
							it->pos.y-=3;
							map[it->pos.x][it->pos.y] = true;
						}
						if(abs(it->pos.y - destination_y) < 3){
							map[it->pos.x][it->pos.y] = false;
							it->pos.y = destination_y;
							map[it->pos.x][it->pos.y] = true;
						}
						if(it->pos.y == destination_y){
							it->action = 1;
						}
					}
				}
			}else if(it->action==10){//droga do obrabiarki - system kolejkowy ZAMKNIETY
				map[it->pos.x][it->pos.y] = false;
				if(instant_service){
					it->pos.x = it->choice->pos.x;
					it->pos.y = it->choice->pos.y;
				}
				if(it->pos.x < 12){
					it->pos.x+=3;
				}else{
					if(it->pos.y != it->choice->pos.y){
						if(it->pos.y < it->choice->pos.y){
							it->pos.y+=3;
						}else if(it->pos.y > it->choice->pos.y){
							it->pos.y-=3;
						}
						if(abs(it->pos.y - it->choice->pos.y) < 3){
							it->pos.y = it->choice->pos.y;
						}
					}else{
						if(it->pos.x < it->choice->pos.x){
							it->pos.x+=3;
						}else if(it->pos.x > it->choice->pos.x){
							it->pos.x-=3;
						}
						if(abs(it->pos.x - it->choice->pos.x) < 3){
							it->pos.x = it->choice->pos.x;
							it->action = 8;
							it->choice->servicing = &(*it);
							if(new_job_time_distribution==1){//rozklad wykladniczy
								it->choice->service_end_time = uptime + long(Simulation::r_wykladniczy(average_new_job_time)*1000);
							}else if(new_job_time_distribution==2){//rozklad jednostajny
								it->choice->service_end_time = uptime + ((rand()%int(max_new_job_time*1000-min_new_job_time*1000) + int(min_new_job_time*1000) )/1000.0)*1000;
							}else{//wartosc nielosowa
								it->choice->service_end_time = uptime + average_new_job_time*1000;
							}
						}	
					}
				}
			}
			
			if(it->pos.x>position.w || it->pos.y>position.h || (it->pos.x<2&&it->action==6) ){
				map[it->pos.x][it->pos.y] = false;
				if(this->closed_queue_system){
					it->pos.x = 3;
					it->action = 10;
					it->choice = get_available_destroyer(it->pos.x,it->pos.y);
					it->choice->saved = true;
					map[it->pos.x][it->pos.y] = true;
				}else{
					it = jobs.erase(it);
					if(jobs.empty() || it==jobs.end() ){
						break;
					}
				}

			}

		}
	}

}

/*
Metoda zwraca najblizszy wolny server od
wspolrzednym podanych w argumencie.
*/
Server* Simulation_Box::get_available_server(int x, int y){

	int index = 0;
	int value = 10000;		//dowolna du¿a liczba

	for(unsigned int i=0;i<servers.size();i++){
		if(!servers[i].saved && abs(servers[i].pos.y - y) < value){
			index = i;
			value = abs(servers[index].pos.y - y);
		}
	}
	return &servers[index];
}

/*
Metoda zwraca najblizszy wolny destroyer od
wspolrzednym podanych w argumencie.
*/
Server* Simulation_Box::get_available_destroyer(int x, int y){

	int index = 0;
	int value = 10000;		//dowolna du¿a liczba

	for(unsigned int i=0;i<destroyers.size();i++){
		if(!destroyers[i].saved && abs(destroyers[i].pos.y - y) < value){
			index = i;
			value = abs(destroyers[index].pos.y - y);
		}
	}
	return &destroyers[index];
}

/*
Sprawdza i zwraca pozycje x nastepnego elementu na liscie zgloszen,
ktory ma takie samo action jak my. [Obecnie nieuzywana metoda]
*/
int Simulation_Box::get_next_element_x(list<Job>::iterator it){
	list<Job>::iterator it2 = it;

	while(it2!=jobs.begin()){
		if(it2 == it || it2->action != it->action){
			it2--;
		}else{
			break;
		}
	}
	if(it2==jobs.begin()){
		return 1000;
	}else{
		return it2->pos.x;
	}
}

/*
Metoda powolujac sie na tablice map[][], sprawdza czy zgloszenie
moze sie ruszyc z danego pola w danym w argumencie kierunku.
*/
bool Simulation_Box::can_move(int x, int y, int dir){
	if(dir==4){
		for(int i = 1;i<=9;i++){
			if(map[x+i][y]==true){
				return false;
			}
		}
	}
	if(dir==2){
		for(int i = 1;i<=9;i++){
			if(map[x][y+i]==true){
				return false;
			}
		}
	}
	if(dir==3){
		for(int i = 1;i<=9;i++){
			if(map[x-i][y]==true){
				return false;
			}
		}
	}
	if(dir==1){
		for(int i = 1;i<=9;i++){
			if(map[x][y-i]==true){
				return false;
			}
		}
	}
	return true;
}


//ulepszyc , zeby szybko chodzilo - niektore rzeczy mozna raz obliczyc, a nie 1000
void Simulation_Box::render_info(){
	glBegin(GL_LINE_LOOP);
	glColor3f(0.f, 0.f, 0.f );
	glVertex2f(50-1, position.y);
	glVertex2f(50, position.y+position.h);
	glVertex2f(50+position.w, position.y+position.h);
	glVertex2f(50+position.w, position.y);
	glEnd();


		////////////////////NAPIS U GORY EKRANU ODNOSNIE RODZAJU SYSTEMU KOLEJKOWEGO///////////////////////////////
	Label l00;
	l00.set(sys.SCREEN_WIDTH/2,20,sys.queue_system.str(),GLUT_BITMAP_9_BY_15,CENTER);
	l00.render();
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int y;
	int x = 60;

	y = position.y+20;
	Label l01;
	l01.set(x,y,closed_queue_system?"-- MASZYNY --":"-- ZGLOSZENIA --",GLUT_BITMAP_9_BY_15,LEFT);
	l01.render();

	y+=15;
	Label l1;
	stringstream s1;
	s1 << (closed_queue_system?"Naprawiono: ":"Obsluzono: ") << (closed_queue_system?jobs_repaired:jobs_serviced);
	l1.set(x,y,s1.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l1.render();

	if(closed_queue_system){
		y+=15;
		Label l1a;
		stringstream s1a;
		s1a << "Zepsuto: " << (closed_queue_system?jobs_destroyed:jobs_serviced);			//jobs_destroyed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		l1a.set(x,y,s1a.str(),GLUT_BITMAP_9_BY_15,LEFT);
		l1a.render();
	}else{
		y+=15;
		Label l2;
		stringstream s2;
		s2 << "Odrzucono: " << jobs_rejected;
		l2.set(x,y,s2.str(),GLUT_BITMAP_9_BY_15,LEFT);
		l2.render();

		if(impatient_probability>0 && max_queue_size > 0){
			y+=15;
			Label l3;
			stringstream s3;
			s3 << "Zrezygnowalo: " << jobs_resigned;
			l3.set(x,y,s3.str(),GLUT_BITMAP_9_BY_15,LEFT);
			l3.render();
		}
	}


	y+=20;
	Label l02;
	l02.set(x,y,"-- ZAJETOSC --",GLUT_BITMAP_9_BY_15,LEFT);
	l02.render();

	if(max_queue_size>0){
		y+=15;
		Label l4;
		stringstream s4;
		s4 << "Kolejka: " << queue_size << "/" << max_queue_size;
		l4.set(x,y,s4.str(),GLUT_BITMAP_9_BY_15,LEFT);
		l4.render();

		if(vip_probability){
			y+=15;
			Label l4b;
			stringstream s4b;
			s4b << "Kolejka VIP: " << vip_queue_size << "/" << max_queue_size;
			l4b.set(x,y,s4b.str(),GLUT_BITMAP_9_BY_15,LEFT);
			l4b.render();
		}
	}


	y+=15;
	Label l5;
	stringstream s5;
	s5 << (closed_queue_system?"Konserwatorzy: ":"Zajetosc: ") << busy_servers << "/" << servers.size();
	l5.set(x,y,s5.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l5.render();

	if(closed_queue_system){
		y+=15;
		Label l7;
		stringstream s7;
		int busy_destroyers = 0;
		for(unsigned int i =0;i<destroyers.size();i++){
			if(destroyers[i].servicing!=NULL){
				busy_destroyers++;
			}
		}
		s7 << "Obrabiarki: " << busy_destroyers << "/" << destroyers.size();
		l7.set(x,y,s7.str(),GLUT_BITMAP_9_BY_15,LEFT);
		l7.render();
	}

	y+=20;
	Label l8;
	l8.set(x,y,"-- SREDNI CZAS --",GLUT_BITMAP_9_BY_15,LEFT);
	l8.render();

	y+=15;
	Label l9;
	stringstream s9;
	s9 << (closed_queue_system?"Uszk.: ":"M. zgl.: ") << int((new_job_time_distribution==2?min_new_job_time:average_new_job_time)*1000)/1000.0;
	if(this->new_job_time_distribution==1){
		s9<<"s (M)";
	}else if(this->new_job_time_distribution==2){
		s9<<"-"<<max_new_job_time<<"s (G)";
	}else{
		s9<<"s (D)";
	}
		
	l9.set(x,y,s9.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l9.render();

	y+=15;
	Label l10;
	stringstream s10;
	s10 << (closed_queue_system?"Napr.: ":"Obsl.: ") << (service_time_distribution==2?min_service_time:average_service_time);
	if(this->service_time_distribution==1){
		s10<<"s (M)";
	}else if(this->service_time_distribution==2){
		s10<<"-"<<max_service_time<<"s (G)";
	}else{
		s10<<"s (D)";
	}
	l10.set(x,y,s10.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l10.render();

	if(impatient_probability && max_queue_size>0 && max_servers_size != numeric_limits<int>::max()){
		y+=15;
		Label l11;
		stringstream s11;
		s11 << "Zn.: " << (impatient_time_distribution==2?min_impatient_time:average_impatient_time) ;
		if(this->impatient_time_distribution==1){
			s11<<"s (M)";
		}else if(this->impatient_time_distribution==2){
			s11<<"-"<<max_impatient_time<<"s (G)";
		}else{
			s11<<"s (D)";
		}
		l11.set(x,y,s11.str(),GLUT_BITMAP_9_BY_15,LEFT);
		l11.render();
	}

	y+=20;
	Label l12;
	l12.set(x,y,"-- SZANSE ZGLOSZENIA --",GLUT_BITMAP_9_BY_15,LEFT);
	l12.render();

	y+=15;
	Label l13;
	stringstream s13;
	s13 << "Niecierpliwe: " << impatient_probability << "%";
	l13.set(x,y,s13.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l13.render();

	y+=15;
	Label l14;
	stringstream s14;
	s14 << "Priorytetowe: " << vip_probability << "%";
	l14.set(x,y,s14.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l14.render();


	y+=15;
	Label l15;
	stringstream s15;
	s15 << "Na ilosc obsluzen: ";
	l15.set(x,y,s15.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l15.render();

	y+=15;
	Label l16;
	stringstream s16;
	s16 << "1: " << services_probabilities[0] << "%, ";
	s16 << "2: " << services_probabilities[1] << "%, ";
	s16 << "3: " << services_probabilities[2] << "%, ";
	l16.set(x,y,s16.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l16.render();

	y+=15;
	Label l17;
	stringstream s17;
	s17 << "4: " << services_probabilities[3] << "%, ";
	s17 << "5: " << services_probabilities[4] << "% ";
	l17.set(x,y,s17.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l17.render();


	////////////////////////////CZAS BEDZIE WYSWIETLANY W GORNYM LEWYM ROGU////////////////////////
	Label l6;
	stringstream s6;
	s6 << "";
	clock_t uptime = clock() / CLOCKS_PER_SEC - sys.start_simulation_time;
	//czas symulacji
	//godziny
	if( (uptime / 3600) > 0){
		if( (uptime / 3600) < 10){
			s6<<"0";
		}
		s6<<(uptime/3600)<<"h";
	}
	//minuty
	if( (uptime / 60) > 0 ){
		if( ((uptime/60)%60) < 10){
			s6<<"0";
		}
		s6<<((uptime/60)%60)<<"m";
	}
	//sekundy
	if( (uptime % 60) < 10 ){
		s6<<"0";
	}
	s6<<(uptime%60)<<"s";
	l6.set(10,20,s6.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l6.render();


	x=285;//275
	y=position.y+20;


	
	Label l17b;
	stringstream s17b;
	s17b << "-- WYNIKI SYMULACJI --";
	l17b.set(x,y,s17b.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l17b.render();

	y+=15;
	Label l18;
	stringstream s18;
	s18 << "Sr. czas jedn. w kol. (ts): ";
	if(sys.jobs_that_left_queue){
		s18 << int((sys.sum_of_waiting_in_queue_times/sys.jobs_that_left_queue)/10.0)/100.0 << "s";
	}else{
		s18 << "-";
	}
	l18.set(x,y,s18.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l18.render();

	


	y+=15;
	Label l23;
	stringstream s23;
	s23 << "Sr. czas obslugi: ";
	if(sys.jobs_that_left_system){
		int result = int((sys.sum_of_waiting_in_system_times/sys.jobs_that_left_system)/10.0);
		result -= int((sys.sum_of_waiting_in_queue_times/sys.jobs_that_left_queue)/10.0);
		s23 << result/100.0 << "s";
	}else{
		s23 << "-";
	}
	l23.set(x,y,s23.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l23.render();

	

	y+=15;
	Label l24;
	stringstream s24;
	s24 << "Sr. il. obsluzonych zgl.: ";
	s24 << int(double(jobs_serviced)/double(clock() / CLOCKS_PER_SEC)*100)/100.0;
	l24.set(x,y,s24.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l24.render();

	y+=15;
	Label l21;
	stringstream s21;
	s21 << "Sr. dl. kolejki (v): ";
	s21 << int(((clock() - sys.queue_last_time)*sys.sim.queue_size+sys.average_queue_size)/clock()*100)/100.0;
	l21.set(x,y,s21.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l21.render();

	y+=15;
	Label l20;
	stringstream s20;
	s20 << "Sr. il. zaj. kanalow (m0): ";
	s20 << int(((clock() - sys.busy_last_time)*sys.sim.busy_servers+sys.average_busy_servers)/clock()*100)/100.0;
	l20.set(x,y,s20.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l20.render();

	y+=15;
	Label l22;
	stringstream s22;
	s22 << "Sr. il. zgl. w sys. (n): ";
	int result = int(((clock() - sys.queue_last_time)*sys.sim.queue_size+sys.average_queue_size)/clock()*100);
	result += int(((clock() - sys.busy_last_time)*sys.sim.busy_servers+sys.average_busy_servers)/clock()*100);
	s22 << result/100.0;
	l22.set(x,y,s22.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l22.render();

	y+=15;
	Label l19;
	stringstream s19;
	s19 << "Sr. czas jedn. w sys. (ts): ";
	if(sys.jobs_that_left_system){
		s19 << int((sys.sum_of_waiting_in_system_times/(sys.jobs_that_left_system+this->jobs_rejected))/10.0)/100.0 << "s";
	}else{
		s19 << "-";
	}
	l19.set(x,y,s19.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l19.render();


	y+=10;
	
	y+=15;
	Label l24b;
	stringstream s24b;
	s24b << "-- WYNIKI ANALIZY SYSTEMU --";
	l24b.set(x,y,s24b.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l24b.render();


	y+=15;
	Label l25;
	stringstream s25;
	s25 << "Prawd. na pelna zaj. sys. (pm): ";
	s25 << sys.pm;
	l25.set(x,y,s25.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l25.render();

	y+=15;
	Label l26;
	stringstream s26;
	s26 << "Wzgl. zdol. obsl. sys. (q): ";
	s26 << sys.q;
	l26.set(x,y,s26.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l26.render();

	y+=15;
	Label l27;
	stringstream s27;
	s27 << "Bzwzgl. zdol. obsl. sys. (A): ";
	s27 << sys.A;
	l27.set(x,y,s27.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l27.render();


	y+=15;
	Label l29;
	stringstream s29;
	s29 << "Sr. dl. kolejki (v): ";
	s29 << sys.v;
	l29.set(x,y,s29.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l29.render();

	y+=15;
	Label l28;
	stringstream s28;
	s28 << "Sr. il. zaj. kanalow (m0): ";
	s28 << sys.m0;
	l28.set(x,y,s28.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l28.render();

	y+=15;
	Label l30;
	stringstream s30;
	s30 << "Sr. il. zgl. w sys. (n): ";
	s30 << sys.n;
	l30.set(x,y,s30.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l30.render();

	y+=15;
	Label l31;
	stringstream s31;
	s31 << "Sr. czas jedn. w sys. (ts): ";
	s31 << sys.ts;
	l31.set(x,y,s31.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l31.render();


}



float Simulation_Box::impatient_level(list<Job>::iterator it){
	clock_t uptime = clock() / CLOCKS_PER_SEC;
	clock_t resign_in = abs(uptime - it->resign_time);
	if(uptime > it->resign_time){resign_in = 0;}//ta linijka nie bedzie potem potrzebna
	float red = (1.0f - float(resign_in)/float(it->get_impatient_in) );
	return red;
}