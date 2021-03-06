#include "c_Scientific.h"
#include "c_System.h"

extern System sys;

Scientific::Scientific(void)
{
	this->end_time = 600;		//seconds
	this->jobs_serviced = 0;
	this->jobs_rejected = 0;
	this->jobs_resigned = 0;
	this->jobs_repaired = 0;
	this->jobs_destroyed = 0;
	this->busy_servers = 0;
	
}

Scientific::~Scientific(void)
{
}

void Scientific::set(){
	if(this->closed_queue_system){
		clock_t uptime = clock() / (CLOCKS_PER_SEC / 1000);
		for(int i =0;i<this->max_queue_size;i++){
			Job* j = new Job;
			j->set(i+1, false, 1, false, numeric_limits<long>::max(), 0, 8,0);
			
			clock_t job_destroy_time;
			if(new_job_time_distribution==1){
				job_destroy_time = uptime + long(Simulation::r_wykladniczy(average_new_job_time)*1000);
			}else if(new_job_time_distribution==2){
				job_destroy_time = uptime + ((rand()%int(max_new_job_time*100-min_new_job_time*100) + int(min_new_job_time*100))/100.0)*1000;
			}else{
				job_destroy_time = uptime + average_new_job_time*1000;
			}
			j->service_end_time = job_destroy_time;

			if(destroyers.empty()){
				destroyers.push_back(j);
			}else{
				bool job_added = false;
				for(list<Job*>::iterator it=destroyers.begin() ; it != destroyers.end(); ++it){
					if( (*it)->service_end_time < j->service_end_time){
						destroyers.insert(it,j);
						job_added = true;
						break;
					}
				}
				if(job_added==false){
					destroyers.push_back(j);
				}
			}
		}
	}
}
void Scientific::update(){
	/*
	DODAWANIE ZGLOSZEN DO KOLEJKI
	*/
	clock_t uptime = clock() / (CLOCKS_PER_SEC / 1000);
	static clock_t new_job_time = uptime 
		+ long(Simulation::r_wykladniczy(average_new_job_time)*1000);
	static int new_id = 1;

	//cout << "(upt:" << uptime/1000.0 << "s, newt: " << new_job_time/1000.0 << "s), ";

	//JEZELI SYSTEM KOLEJKOWY NIE JEST ZAMKNIETY
	if(!this->closed_queue_system){
		Job* j = NULL;
		if(uptime>new_job_time ){
			int priority = 0;
			if(prob(vip_probability)){
				priority = 1;
			}
			
			//JEZELI JEST JAKIŚ WOLNY KANAŁ, LUB MIEJSCE W KOLEJCE, LUB ZGŁOSZENIE
			//JEST PRIORYTETOWE TO WCHODZI DO SYSTEMU
			if(queue.size() < unsigned int(max_queue_size) || priority == 1 || 
				(max_queue_size<=0 && servers.size() < unsigned int(max_servers_size) ) ){
				bool impatient = 0;
				int get_impatient_in = 0;
				if(prob(impatient_probability)){
					impatient = 1;
					if(impatient_time_distribution==1){
						get_impatient_in = Simulation::r_wykladniczy(average_impatient_time);
					}if(impatient_time_distribution==2){
						get_impatient_in = (rand()%int(max_impatient_time*100-min_impatient_time*100) 
							+ int(min_impatient_time*100))/100.0;
					}else{
						get_impatient_in = average_impatient_time;
					}
				}
				int services = get_services_number(services_probabilities,5) + 1;

				j = new Job;

				j->system_join_time = clock();		//zapisujemy czas wejscia do systemu
				j->queue_join_time = clock();		//zapisujemy czas wejscia do kolejki
				clock_t resign_time = uptime + (get_impatient_in*1000);
				j->set(new_id++, priority, services, impatient, resign_time, get_impatient_in, 7 ,0);
				
				/*
				DODAJEMY DO KOLEJKI ZGLOSZENIE: BEZ PRIORYTETU
				NA KONIEC, Z PRIORYTETEM NA POCZATEK ZARAZ ZA
				OSTATNIM INNYM ZGŁOSZENIEM Z PRIORYTETEM
				*/
				update_average_queue_size();
				if(max_queue_size>0){//JEZELI ISTNIEJE KOLEJKA W SYSTEMIE,
					if(j->priority == 0){
						queue.push_back(j);
					}else{
						bool job_added = false;
						for(list<Job*>::iterator it=queue.begin() ; it != queue.end(); ++it){
							if( (*it)->priority == 0 ){
								queue.insert(it,j);
								job_added = true;
								break;
							}
						}
						if(job_added==false && queue.size() < unsigned int(max_queue_size) ){
							queue.push_back(j);
						}
					}
				}else{//JEZELI W SYSTEMIE NIE MA KOLEJKI, TO ZGLOSZENIA PROBOJA ODRAZU ISC DO KANALOW
					if(servers.size() < unsigned int(max_servers_size)){
						
						if(service_time_distribution==1){//rozklad wykladniczy
							j->service_end_time = uptime 
								+ long(Simulation::r_wykladniczy(average_service_time)*1000);
						}else if(service_time_distribution==2){//rozklad jednostajny
							j->service_end_time = uptime 
								+ ((rand()%int(max_service_time*100-min_service_time*100) 
								+ int(min_service_time*100))/100.0)*1000;
						}else{//wartosc nielosowa
							j->service_end_time = uptime + average_service_time*1000;
						}
						j->action = 3;
						if(servers.empty()){
							servers.push_back(j);
						}else{
							bool job_added = false;
							for(list<Job*>::iterator it=servers.begin() ; it != servers.end(); ++it){
								if( (*it)->service_end_time < j->service_end_time){
									servers.insert(it,j);
									job_added = true;
									break;
								}
							}
							if(job_added==false){
								servers.push_back(j);
							}
						}
						update_average_business();
						busy_servers++;
						
					}else{
						queue.push_back(j);
					}
					
				}
				/*
				JEŻELI PO DODANIU ZGŁOSZENIA DO KOLEJKI 
				OKAZAŁO SIĘ, ŻE KOLEJKA JEST ZA
				DŁUGA TO USUWAMY OSTATNI JEJ ELEMENT
				*/
				if(queue.size() > unsigned int(max_queue_size) ){
					if(queue.back()->impatient){
						for(list<Job*>::iterator it=impatients.begin() ; it != impatients.end(); ++it){
							if( (*it)->id == queue.back()->id ){
								impatients.erase(it);
								break;
							}
						}
					}
					delete queue.back();
					queue.pop_back();
					jobs_rejected++;
				}

			}else{//JEZELI WSZYSTKIE KANALY I KOLEJKA BYLY ZAJETE, A ZGL. NIE MIALO PRIORYTETU
				jobs_rejected++;
			}

			if(new_job_time_distribution==1){
				new_job_time = uptime + long(Simulation::r_wykladniczy(average_new_job_time)*1000);
			}else if(new_job_time_distribution==2){
				new_job_time = uptime + ((rand()%int(max_new_job_time*100-min_new_job_time*100) 
					+ int(min_new_job_time*100))/100.0)*1000;
			}else{
				new_job_time = uptime + average_new_job_time*1000;
			}
		}
		/*
		DODAWANIE NOWYCH NIECIERPLIWYCH ZGLOSZEN
		DO LISTY NIECIERPLIWYCH ZGLOSZEN
		*/
		if(j!=NULL && j->impatient){
			if(impatients.empty()){
				impatients.push_back(j);
			}else{
				bool job_added = false;
				for(list<Job*>::iterator it=impatients.begin() ; it != impatients.end(); ++it){
					if( (*it)->resign_time < j->resign_time){
						impatients.insert(it,j);
						job_added = true;
						break;
					}
				}
				if(job_added==false){
					impatients.push_back(j);
				}
			}
		}

	}else{//ZAMKNIETY SYSTEM KOLEJKOWY - ZGLOSZENIE JEST W OBRABIARCE
		while(!destroyers.empty() && destroyers.back()->service_end_time < uptime){
			destroyers.back()->action = 7;

			//ZGŁOSZENIE BĘDĄC W OBRABIARCE JEST POZA SYSTEMEM
			destroyers.back()->system_join_time = clock();		//zapisujemy czas wejscia do systemu
			destroyers.back()->queue_join_time = clock();		//zapisujemy czas wejscia do kolejki

			update_average_queue_size();
			queue.push_back(destroyers.back());

			destroyers.pop_back();
			jobs_destroyed++;
		}
	}
		
	/*
	USUWANIE NIECIERPLIWYCH ZGŁOSZEŃ
	Z KOLEJKI
	*/
	while(!impatients.empty() && impatients.back()->resign_time < uptime){
		if(impatients.back()->action != 3){
			update_average_queue_size();
			for(list<Job*>::iterator it=queue.begin() ; it != queue.end(); ++it){
				if( (*it)->id == impatients.back()->id ){
					queue.erase(it);
					break;
				}
			}
			delete impatients.back();
			jobs_resigned++;
		}
		impatients.pop_back();
	}
	/////////////////////////////////

	/*
	PRZECHODZENIE Z KOLEJKI
	DO KANAŁÓW
	*/
	while(!queue.empty() && servers.size() < unsigned int(max_servers_size) ){
		if(service_time_distribution==1){//rozklad wykladniczy
			queue.front()->service_end_time = uptime 
				+ long(Simulation::r_wykladniczy(average_service_time)*1000);
		}else if(service_time_distribution==2){//rozklad jednostajny
			queue.front()->service_end_time = uptime 
				+ ((rand()%int(max_service_time*100-min_service_time*100) 
				+ int(min_service_time*100))/100.0)*1000;
		}else{//wartosc nielosowa
			queue.front()->service_end_time = uptime + average_service_time*1000;
		}
		queue.front()->action = 3;
		if(servers.empty()){
			servers.push_back(queue.front());
		}else{
			bool job_added = false;
			for(list<Job*>::iterator it=servers.begin() ; it != servers.end(); ++it){
				if( (*it)->service_end_time < queue.front()->service_end_time){
					servers.insert(it,queue.front());
					job_added = true;
					break;
				}
			}
			if(job_added==false){
				servers.push_back(queue.front());
			}
		}
		update_average_business();
		busy_servers++;

		//DO WYKRESÓW I SYMULACYJNEJ ANALIZY
		sys.sum_of_waiting_in_queue_times+=clock() - queue.front()->queue_join_time;
		sys.jobs_that_left_queue++;			
		////////////////////////////////////

		update_average_queue_size();
		queue.pop_front();
	}
	
	/*
	USUWAMY KLIENTÓW Z KANAŁÓW
	*/
	while(!servers.empty() && servers.back()->service_end_time < uptime){
		if(!this->closed_queue_system){
			servers.back()->services--;
			if(servers.back()->services > 0){
				if(servers.back()->impatient){
					servers.back()->resign_time = uptime + (servers.back()->get_impatient_in*1000);
				}
				/*
				DODAJEMY DO KOLEJKI ZGLOSZENIE: BEZ PRIORYTETU
				NA KONIEC, Z PRIORYTETEM NA POCZATEK ZARAZ ZA
				OSTATNIM INNYM ZGŁOSZENIEM Z PRIORYTETEM
				*/
				servers.back()->queue_join_time = clock();	//zapisujemy czas wejscia do kolejki
				update_average_queue_size();
				if(servers.back()->priority == 0){
					queue.push_back(servers.back());
				}else{
					bool job_added = false;
					for(list<Job*>::iterator it=queue.begin() ; it != queue.end(); ++it){
						if( (*it)->priority == 0 ){
							queue.insert(it,servers.back());
							job_added = true;
							break;
						}
					}
					if(job_added==false){
						queue.push_back(servers.back());
					}
				}
				
				/*
					JEŻELI PO DODANIU ZGŁOSZENIA DO KOLEJKI OKAZAŁO SIĘ,
					ŻE KOLEJKA JEST ZA DŁUGA TO USUWAMY OSTATNI JEJ ELEMENT
				*/
				if(queue.size() > unsigned int(max_queue_size) ){
					if(queue.back()->impatient){
						for(list<Job*>::iterator it=impatients.begin() ; it != impatients.end(); ++it){
							if( (*it)->id == queue.back()->id ){
								queue.erase(it);
								break;
							}
						}
					}
					/*
					JEŻELI USUWANE ZGŁOSZENIE BYŁO NIECIERPLIWE TO USUWAMY JE TEŻ
					Z LISTY NIECIERPLIWYCH ZGŁOSZEŃ
					*/
					if(queue.back()->impatient){
						for(list<Job*>::iterator it=impatients.begin() ; it != impatients.end(); ++it){
							if(queue.back()->id == (*it)->id){
								impatients.erase(it);
								break;
							}
						}
					}

					delete queue.back();
					queue.pop_back();
				}

			}else{
				/*
				JEŻELI USUWANE ZGŁOSZENIE BYŁO NIECIERPLIWE TO USUWAMY JE TEŻ
				Z LISTY NIECIERPLIWYCH ZGŁOSZEŃ
				*/
				if(servers.back()->impatient){
					for(list<Job*>::iterator it=impatients.begin() ; it != impatients.end(); ++it){
						if(servers.back()->id == (*it)->id){
							impatients.erase(it);
							break;
						}
					}
				}

				//DO WYKRESÓW I SYMULACYJNEJ ANALIZY
				sys.sum_of_waiting_in_system_times+=clock() - servers.back()->system_join_time;
				sys.jobs_that_left_system++;
				////////////////////////////////////
				delete servers.back();
			}
			servers.pop_back();
			jobs_serviced++;

			update_average_business();
			busy_servers--;
		}else{//ZAMKNIETY SYSTEM KOLEJKOWY - ZGLOSZENIA Z KANALOW WRACAJA WPROST DO OBRABIAREK
			
			//DO WYKRESÓW I SYMULACYJNEJ ANALIZY
			servers.back()->action = 8;
			sys.sum_of_waiting_in_system_times+=clock() - servers.back()->system_join_time;
			sys.jobs_that_left_system++;
			////////////////////////////////////

			clock_t job_destroy_time;
			if(new_job_time_distribution==1){
				job_destroy_time = uptime 
					+ long(Simulation::r_wykladniczy(average_new_job_time)*1000);
			}else if(new_job_time_distribution==2){
				job_destroy_time = uptime 
					+ ((rand()%int(max_new_job_time*100-min_new_job_time*100)  
					+ int(min_new_job_time*100))/100.0)*1000;
			}else{
				job_destroy_time = uptime + average_new_job_time*1000;
			}
			servers.back()->service_end_time = job_destroy_time;
						
			if(destroyers.empty()){
				destroyers.push_back(servers.back());
			}else{
				bool job_added = false;
				for(list<Job*>::iterator it=destroyers.begin() ; it != destroyers.end(); ++it){
					if( (*it)->service_end_time < servers.back()->service_end_time){//HMM?
						destroyers.insert(it,servers.back());
						job_added = true;
						break;
					}
				}
				if(job_added == false){
					destroyers.push_back(servers.back());
				}
			}
			servers.pop_back();
			jobs_repaired++;

			update_average_business();
			busy_servers--;
			
		}
	}
}

void Scientific::render_info(){
	glBegin(GL_LINE_LOOP);
	glColor3f(0.f, 0.f, 0.f );
	glVertex2f(50-1, sys.sim.position.y);
	glVertex2f(50, sys.sim.position.y+sys.sim.position.h);
	glVertex2f(50+sys.sim.position.w, sys.sim.position.y+sys.sim.position.h);
	glVertex2f(50+sys.sim.position.w, sys.sim.position.y);
	glEnd();


	////////////////////NAPIS U GORY EKRANU ODNOSNIE RODZAJU SYSTEMU KOLEJKOWEGO///////////////////////////////
	Label l00;
	l00.set(sys.SCREEN_WIDTH/2,20,sys.queue_system.str(),GLUT_BITMAP_9_BY_15,CENTER);
	l00.render();
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int y;
	int x = 60;


	y = sys.sim.position.y+20;
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
		if(max_servers_size!=numeric_limits<int>::max()){
			y+=15;
			Label l2;
			stringstream s2;
			s2 << "Odrzucono: " << jobs_rejected;
			l2.set(x,y,s2.str(),GLUT_BITMAP_9_BY_15,LEFT);
			l2.render();
		}

		if(impatient_probability>0 && max_queue_size > 0){
			y+=15;
			Label l3;
			stringstream s3;
			s3 << "Zrezygnowalo: " << jobs_resigned;
			l3.set(x,y,s3.str(),GLUT_BITMAP_9_BY_15,LEFT);
			l3.render();
		}
	}


	y+=25;
	Label l02;
	l02.set(x,y,"-- ZAJETOSC --",GLUT_BITMAP_9_BY_15,LEFT);
	l02.render();

	if(max_servers_size!=numeric_limits<int>::max() && max_queue_size>0){
		y+=15;
		Label l4;
		stringstream s4;
		s4 << "Kolejka: " << queue.size() << "/";
		if(max_queue_size!=numeric_limits<int>::max()){
			s4 << max_queue_size;
		}else{
			s4 << "INF";
		}
		l4.set(x,y,s4.str(),GLUT_BITMAP_9_BY_15,LEFT);
		l4.render();
	}


	y+=15;
	Label l5;
	stringstream s5;
	s5 << (closed_queue_system?"Konserwatorzy: ":"Zajetosc: ") << busy_servers << "/";
	if(max_servers_size!=numeric_limits<int>::max()){
		s5 << max_servers_size;
	}else{
		s5 << "INF";
	}
	l5.set(x,y,s5.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l5.render();

	if(closed_queue_system){
		y+=15;
		Label l7;
		stringstream s7;
		s7 << "Obrabiarki: " << destroyers.size() << "/" << max_queue_size;
		l7.set(x,y,s7.str(),GLUT_BITMAP_9_BY_15,LEFT);
		l7.render();
	}

	y+=25;
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

	y+=25;
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
	clock_t uptime = (clock() / CLOCKS_PER_SEC) - sys.start_simulation_time;
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
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	x=285;
	y=sys.sim.position.y+20;

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
	s21 << int(((clock() - sys.queue_last_time)*sys.sci.queue.size()+sys.average_queue_size)/clock()*100)/100.0;
	l21.set(x,y,s21.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l21.render();

	y+=15;
	Label l20;
	stringstream s20;
	s20 << "Sr. il. zaj. kanalow (m0): ";
	s20 << int(((clock() - sys.busy_last_time)*sys.sci.busy_servers+sys.average_busy_servers)/clock()*100)/100.0;
	l20.set(x,y,s20.str(),GLUT_BITMAP_9_BY_15,LEFT);
	l20.render();

	y+=15;
	Label l22;
	stringstream s22;
	s22 << "Sr. il. zgl. w sys. (n): ";
	int result = int(((clock() - sys.queue_last_time)*sys.sci.queue.size()+sys.average_queue_size)/clock()*100);
	result += int(((clock() - sys.busy_last_time)*sys.sci.busy_servers+sys.average_busy_servers)/clock()*100);
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

void Scientific::render(){

	render_info();
}