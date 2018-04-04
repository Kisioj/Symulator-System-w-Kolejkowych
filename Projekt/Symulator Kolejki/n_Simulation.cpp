#include "n_Simulation.h"
#include "c_System.h"

extern System sys;

namespace Simulation {
	void start(){
		glutPassiveMotionFunc( Simulation::mouseMovement );		//mouse movement when not pressed
		glutMotionFunc( Simulation::mousePressedMovement );					//mouse movement when pressed
		glutMouseFunc( Simulation::mousePress );					//press
		glutKeyboardFunc( Simulation::keysPress );
		glutSpecialFunc( Simulation::keysSpecialPress);
		glutTimerFunc( 1000 / sys.SCREEN_FPS, Simulation::mainLoop, 0 );
	}

	void mainLoop( int val )
	{
		if(sys.phase!=4){//exit phase, so there's no error when leaving the game
			update();
			
			static int p = 0;
			if(p==0 || sys.type==1){
				render();
			}
			p++;
			p%=10;
			glutTimerFunc( 1000 / sys.SCREEN_FPS, mainLoop, val );
		}
	}


	void update()
	{
		if(sys.phase==3){
			if(sys.type==1){
				sys.sim.update();
			}else{
				sys.sci.update();
			}
			for(unsigned int i = 0; i < sys.charts.size(); i++){
				sys.charts[i].update();
			}
		}

	}


	void render()
	{
		glClearColor (1.0,1.0,1.0,1.0);
		glClear( GL_COLOR_BUFFER_BIT );
		glLoadIdentity();
		
		if(sys.phase==3){
			for(unsigned int i = 0; i < sys.charts.size(); i++){
				sys.charts[i].render();
			}
			if(sys.type==1){
				sys.sim.render();
			}else{
				sys.sci.render();
			}
		}else{ // if(sys.phase==1 || sys.phase==2)
			sys.menu.render();
		}
		glutSwapBuffers();

	}
	void keysSpecialPress(int key,int x, int y){
		if(sys.phase==2 && sys.menu.input != NULL ){
			string * result = &sys.menu.input->text.s;
			unsigned int max_length = sys.menu.input->max_text_length;
			int carriage_type = sys.menu.input->carriage_type;
			bool changed = false;
			if(carriage_type==2){
				if(key == GLUT_KEY_RIGHT && sys.menu.input->carriage_index<(result->size()-1)){
					sys.menu.input->carriage_index++;
					//omijamy kropke
					if( (*result)[sys.menu.input->carriage_index] == '.' && sys.menu.input->carriage_index<(result->size()-1)){
						sys.menu.input->carriage_index++;
					}
					changed = true;
				}else if(key==GLUT_KEY_LEFT && sys.menu.input->carriage_index>0){
					sys.menu.input->carriage_index--;
					//omijamy kropke
					if( (*result)[sys.menu.input->carriage_index] == '.' && sys.menu.input->carriage_index>0 ){
						sys.menu.input->carriage_index--;
					}
					changed = true;
				}
			}
			if(changed){
				sys.menu.input->show_carriage = true;
				sys.menu.input->toogle_carriage_time = clock() / (CLOCKS_PER_SEC / 1000)+500;
			}
		}

	}
	void keysPress( unsigned char key, int x, int y )
	{

		//If the user presses q
		if(sys.phase==2 && sys.menu.input != NULL ){
			string * result = &sys.menu.input->text.s;
			unsigned int max_length = sys.menu.input->max_text_length;
			int carriage_type = sys.menu.input->carriage_type;
			bool changed = false;

			if(carriage_type==1){
				if(key == 8){
					if( result->size() > 0){
						result->erase( result->end()-1 );
						changed = true;
					}
				}else if(key >= '0' && key <= '9'){
					if( ((result->size() && (*result)[0]!='0')|| !result->size()) && result->size() < max_length){
						(*result)+=key;
						changed = true;
					}
				}

			}else if(carriage_type==2){
				if(key >= '0' && key <= '9'){
					(*result)[sys.menu.input->carriage_index] = char(key);
					if( sys.menu.input->carriage_index<(result->size()-1) ){
						//przechodzimy do nastepnej literki, ale omijamy kropke
						sys.menu.input->carriage_index++;
						//omijamy kropke
						if( (*result)[sys.menu.input->carriage_index] == '.' && sys.menu.input->carriage_index<(result->size()-1)){
							sys.menu.input->carriage_index++;
						}
						changed = true;
					}
					
				}
			}
			if(changed){
				sys.menu.input->show_carriage = true;
				sys.menu.input->toogle_carriage_time = clock() / (CLOCKS_PER_SEC / 1000)+500;
			}
		}
		
		if( key == 13 )//wypisze koordynaty myszki
		{
			if(sys.phase==1){
				sys.phase=2;
				sys.logo.freeTexture();
			}else if(sys.phase==2 && (sys.menu.buttons[0].selected || sys.menu.buttons[1].selected) ){
				
				//KOMENT
				for(unsigned int i =0; i<sys.menu.buttons.size();i++){ // jezeli nie ma inputu gdzies to daj input 0
					if(sys.menu.buttons[i].selected && sys.menu.buttons[i].type==5){
						if(sys.menu.buttons[i].text.s.size() == 0){
							if(sys.menu.buttons[i].has_min_value){
								stringstream ss;
								ss << sys.menu.buttons[i].min_value;
								sys.menu.buttons[i].text.s=ss.str();
							}else{
								sys.menu.buttons[i].text.s="0";
							}
							//jezeli input ma minimalna wartosc, a w polu tekstowym jest 0:
						}else if(sys.menu.buttons[i].has_min_value && sys.menu.buttons[i].text.s == "0"){
							stringstream ss;
							ss << sys.menu.buttons[i].min_value;
							sys.menu.buttons[i].text.s=ss.str();
						}
					}
				}

				//END OF KOMENT
			
				if(sys.menu.buttons[0].selected){
					sys.type=2;
				}else if(sys.menu.buttons[1].selected){
					sys.type=1;
				}


				int * max_servers_size = &sys.sci.max_servers_size;
				int * max_queue_size = &sys.sci.max_queue_size;

				int * new_job_time_distribution = &sys.sci.new_job_time_distribution;
				double * average_new_job_time = &sys.sci.average_new_job_time;
				double * min_new_job_time = &sys.sci.min_new_job_time;
				double * max_new_job_time = &sys.sci.max_new_job_time;

				int * service_time_distribution = &sys.sci.service_time_distribution;
				double * average_service_time = &sys.sci.average_service_time;
				double * min_service_time = &sys.sci.min_service_time;
				double * max_service_time = &sys.sci.max_service_time;

				int * impatient_probability = &sys.sci.impatient_probability;
				int * vip_probability = &sys.sci.vip_probability;

				int * impatient_time_distribution = &sys.sci.impatient_time_distribution;
				double * average_impatient_time = &sys.sci.average_impatient_time;
				double * min_impatient_time = &sys.sci.min_impatient_time;
				double * max_impatient_time = &sys.sci.max_impatient_time;

				int * services_probabilities = sys.sci.services_probabilities;
				
				bool * closed_queue_system = &sys.sci.closed_queue_system;
				bool * write_data_to_file = &sys.sci.write_data_to_file;

				bool * instant_service = &sys.sim.instant_service;
				bool * instant_queue = &sys.sim.instant_queue;
				bool * instant_reject = &sys.sim.instant_reject;
				bool * instant_leave = &sys.sim.instant_leave;
				bool * instant_resign = &sys.sim.instant_resign;
				
				

				if(sys.menu.buttons[0].selected){//SYMULACJA NAUKOWA
					if(sys.menu.buttons[2].selected){//ograniczona ilosc kanalow
						string * s = &sys.menu.buttons[3].text.s;
						int value = 0;
						for(unsigned int j =0;j< s->size();j++){
							value += double((*s)[j]-'0') * pow(10.0,double(s->size()-(j+1)));
						}
						sys.sci.max_servers_size = value;
					}else{//nieograniczona ilosc kanalow
						sys.sci.max_servers_size = numeric_limits<int>::max();
					}

					if(sys.menu.buttons[7].selected){//ograniczona dlugosc kolejki
						string * s = &sys.menu.buttons[8].text.s;
						int value = 0;
						for(unsigned int j =0;j< s->size();j++){
							value += double((*s)[j]-'0') * pow(10.0,double(s->size()-(j+1)));
						}
						sys.sci.max_queue_size = value;
					}else{//nieograniczona dlugosc kolejki
						sys.sci.max_queue_size = numeric_limits<int>::max();
					}


					string * s1 = &sys.menu.buttons[10].text.s;
					int value1 = 0;
					for(unsigned int j =0;j< s1->size();j++){
						value1 += double((*s1)[j]-'0') * pow(10.0,double(s1->size()-(j+1)));
					}
					if(sys.menu.buttons[12].selected){//rozklad wyklandiczy
						sys.sci.new_job_time_distribution = 1;
						sys.sci.average_new_job_time = 60.0/value1;
					}else if(sys.menu.buttons[13].selected/*&& (sys.menu.buttons[10].text.s!=sys.menu.buttons[11].text.s)*/){//rozklad jednostajny

						
						string * s2 = &sys.menu.buttons[11].text.s;
						int value2 = 0;
						for(unsigned int j =0;j< s2->size();j++){
							value2 += double((*s2)[j]-'0') * pow(10.0,double(s2->size()-(j+1)));
						}

						  
						if( abs( (60.0/value1-60.0/value2)*100 ) >= 1 ){
							sys.sci.new_job_time_distribution = 2;
							if( (60.0/value1) <= (60.0/value2) ){
								sys.sci.min_new_job_time = 60.0/value1;
								sys.sci.max_new_job_time = 60.0/value2;
							}else{
								sys.sci.min_new_job_time = 60.0/value2;
								sys.sci.max_new_job_time = 60.0/value1;
							}
						}else{
							sys.sci.new_job_time_distribution = 3;
							sys.sci.average_new_job_time = 60.0/value1;
						}
					}else{//wartosc nielosowa
						sys.sci.new_job_time_distribution = 3;
						sys.sci.average_new_job_time = 60.0/value1;
					}
				}else if(sys.menu.buttons[1].selected){//SYMULACJA EDUKACYJNA
					max_servers_size = &sys.sim.max_servers_size;
					max_queue_size = &sys.sim.max_queue_size;

					new_job_time_distribution = &sys.sim.new_job_time_distribution;
					average_new_job_time = &sys.sim.average_new_job_time;
					min_new_job_time = &sys.sim.min_new_job_time;
					max_new_job_time = &sys.sim.max_new_job_time;

					service_time_distribution = &sys.sim.service_time_distribution;
					average_service_time = &sys.sim.average_service_time;
					min_service_time = &sys.sim.min_service_time;
					max_service_time = &sys.sim.max_service_time;

					impatient_probability = &sys.sim.impatient_probability;
					vip_probability = &sys.sim.vip_probability;

					impatient_time_distribution = &sys.sim.impatient_time_distribution;
					average_impatient_time = &sys.sim.average_impatient_time;
					min_impatient_time = &sys.sim.min_impatient_time;
					max_impatient_time = &sys.sim.max_impatient_time;

					services_probabilities = sys.sim.services_probabilities;

					closed_queue_system = &sys.sim.closed_queue_system;
					write_data_to_file = &sys.sim.write_data_to_file;

					/*
					"Ile bajtów pamiêci zajmie tablica adresów 16 bitowych, której pierwszy adres to
					(szesnastkowo) FF20, a ostatni FFFF. Ile adresów pomieœci tablica? Wyniki podaj w
					postaci dziesiêtnej."
					*/
					//animacje ON i OFF

					string * s = &sys.menu.labels[2].s;
					int value = 0;
					for(unsigned int i =0; i<s->size(); i++){
						value += double( (*s)[i]-'0') * pow(10.0,double( s->size()-(i+1) ) );
					}
					(*max_servers_size) = value;

					
					int x1 = sys.menu.sliders[0].pos.x;
					int w1 = sys.menu.sliders[0].pos.w;
					int x2 = sys.menu.sliders[0].b.pos.x;
					int max_value = sys.menu.sliders[0].max_value;
					int min_value = sys.menu.sliders[0].min_value;
					(*max_queue_size) = double(x2-x1)/double(w1)*double(max_value-min_value)+min_value;

					x1 = sys.menu.sliders[1].pos.x;
					w1 = sys.menu.sliders[1].pos.w;
					x2 = sys.menu.sliders[1].b.pos.x;
					int x3 = sys.menu.sliders[1].b2.pos.x;
					max_value = sys.menu.sliders[1].max_value;
					min_value = sys.menu.sliders[1].min_value;

					int value1 = double(x2-x1)/double(w1)*double(max_value-min_value)+min_value;
					int value2 = double(x3-x1)/double(w1)*double(max_value-min_value)+min_value;

					if(sys.menu.buttons[12].selected){//rozklad wyklandiczy
						(*new_job_time_distribution) = 1;
						(*average_new_job_time) = 60.0/value1;
					}else if(sys.menu.buttons[13].selected && abs( (60.0/value1-60.0/value2)*100 ) >= 1){//rozklad jednostajny
						(*new_job_time_distribution) = 2;
						if( (60.0/value1) <= (60.0/value2) ){
							(*min_new_job_time) = 60.0/value1;
							(*max_new_job_time) = 60.0/value2;
						}else{
							(*min_new_job_time) = 60.0/value2;
							(*max_new_job_time) = 60.0/value1;
						}
					}else{//wartosc nielosowa
						(*new_job_time_distribution) = 3;
						(*average_new_job_time) = 60.0/value1;
					}
					

					(*instant_service) = !sys.menu.buttons[35].selected;
					(*instant_queue) = !sys.menu.buttons[36].selected;
					(*instant_reject) = !sys.menu.buttons[37].selected;
					(*instant_leave) = !sys.menu.buttons[38].selected;
					(*instant_resign) = !sys.menu.buttons[39].selected;



				}

				string * s1 = &sys.menu.buttons[15].text.s;
				double d_value1 = 0;
				for(unsigned int j =0;j<3;j++){//czesc dziesietna
					d_value1 += double((*s1)[j]-'0') * pow(10.0,double(3-(j+1)));
				}
				for(unsigned int j=4;j<6;j++){//czesc ulamkowa
					d_value1 += double((*s1)[j]-'0') / pow(10.0,double((j+1)-4));
				}
				//double
				if(sys.menu.buttons[17].selected){//rozklad wykladniczy
					(*service_time_distribution) = 1;
					(*average_service_time) = d_value1;
				}else if(sys.menu.buttons[18].selected && (sys.menu.buttons[15].text.s!=sys.menu.buttons[16].text.s) ){//rozklad jednostajny
					(*service_time_distribution) = 2;
					string * s2 = &sys.menu.buttons[16].text.s;
					double d_value2 = 0;
					for(unsigned int j =0;j<3;j++){//czesc dziesietna
						d_value2 += double((*s2)[j]-'0') * pow(10.0,double(3-(j+1)));
					}
					for(unsigned int j=4;j<6;j++){//czesc ulamkowa
						d_value2 += double((*s2)[j]-'0') / pow(10.0,double((j+1)-4));
					}
					if(d_value1 <= d_value2){
						(*min_service_time) = d_value1;
						(*max_service_time) = d_value2;
					}else{
						(*min_service_time) = d_value2;
						(*max_service_time) = d_value1;
					}
				}else{//wartosc nielosowa
					(*service_time_distribution) = 3;
					(*average_service_time) = d_value1;
				}
				
				int x1 = sys.menu.sliders[2].pos.x;
				int w1 = sys.menu.sliders[2].pos.w;
				int x2 = sys.menu.sliders[2].b.pos.x;
				int max_value = sys.menu.sliders[2].max_value;
				int min_value = sys.menu.sliders[2].min_value;
				(*impatient_probability) = double(x2-x1)/double(w1)*double(max_value-min_value)+min_value;

				x1 = sys.menu.sliders[3].pos.x;
				w1 = sys.menu.sliders[3].pos.w;
				x2 = sys.menu.sliders[3].b.pos.x;
				max_value = sys.menu.sliders[3].max_value;
				min_value = sys.menu.sliders[3].min_value;
				(*vip_probability) = double(x2-x1)/double(w1)*double(max_value-min_value)+min_value;
				



				s1 = &sys.menu.buttons[20].text.s;
				d_value1 = 0.0;
				for(unsigned int j =0;j<3;j++){//czesc dziesietna
					d_value1 += double((*s1)[j]-'0') * pow(10.0,double(3-(j+1)));
				}
				for(unsigned int j=4;j<6;j++){//czesc ulamkowa
					d_value1 += double((*s1)[j]-'0') / pow(10.0,double((j+1)-4));
				}
				//double
				if(sys.menu.buttons[22].selected){//rozklad wykladniczy
					(*impatient_time_distribution) = 1;
					(*average_impatient_time) = d_value1;
				}else if(sys.menu.buttons[23].selected && (sys.menu.buttons[20].text.s!=sys.menu.buttons[21].text.s) ){//rozklad jednostajny
					(*impatient_time_distribution) = 2;
					string * s2 = &sys.menu.buttons[21].text.s;
					double d_value2 = 0;
					for(unsigned int j =0;j<3;j++){//czesc dziesietna
						d_value2 += double((*s2)[j]-'0') * pow(10.0,double(3-(j+1)));
					}
					for(unsigned int j=4;j<6;j++){//czesc ulamkowa
						d_value2 += double((*s2)[j]-'0') / pow(10.0,double((j+1)-4));
					}
					if(d_value1 <= d_value2){
						(*min_impatient_time) = d_value1;
						(*max_impatient_time) = d_value2;
					}else{
						(*min_impatient_time) = d_value2;
						(*max_impatient_time) = d_value1;
					}
				}else{//wartosc nielosowa
					(*impatient_time_distribution) = 3;
					(*average_impatient_time) = d_value1;
				}

				s1 = &sys.menu.labels[33].s;
				int value = 0;
				for(unsigned int i =0; i<s1->size()-1; i++){
					value += double( (*s1)[i]-'0') * pow(10.0,double( (s1->size()-1)-(i+1) ) );
				}
				services_probabilities[0] = value;

				s1 = &sys.menu.labels[35].s;
				value = 0;
				for(unsigned int i =0; i<s1->size()-1; i++){
					value += double( (*s1)[i]-'0') * pow(10.0,double( (s1->size()-1)-(i+1) ) );
				}
				services_probabilities[1] = value;

				s1 = &sys.menu.labels[37].s;
				value = 0;
				for(unsigned int i =0; i<s1->size()-1; i++){
					value += double( (*s1)[i]-'0') * pow(10.0,double( (s1->size()-1)-(i+1) ) );
				}
				services_probabilities[2] = value;

				s1 = &sys.menu.labels[39].s;
				value = 0;
				for(unsigned int i =0; i<s1->size()-1; i++){
					value += double( (*s1)[i]-'0') * pow(10.0,double( (s1->size()-1)-(i+1) ) );
				}
				services_probabilities[3] = value;

				s1 = &sys.menu.labels[41].s;
				value = 0;
				for(unsigned int i =0; i<s1->size()-1; i++){
					value += double( (*s1)[i]-'0') * pow(10.0,double( (s1->size()-1)-(i+1) ) );
				}
				services_probabilities[4] = value;


				(*closed_queue_system) = sys.menu.buttons[33].selected;
				(*write_data_to_file) = sys.menu.buttons[34].selected;
				
				
				
				
				cout << endl;
				cout << "Servers size: " << (*max_servers_size) << endl;
				cout << "Queue size: " << (*max_queue_size) << endl;
				cout << "Sredni czas pomiedzy zgloszeniami: ";
				switch( (*new_job_time_distribution) ){
					case 1:
						cout << "Wykladniczy"<<endl;
						cout << "Sredni: " << (*average_new_job_time) << endl;
						break;
					case 2:
						cout << "Jednostajny"<<endl;
						cout << "Przedzial: " << (*min_new_job_time) << " - " << (*max_new_job_time) << endl;
						break;
					case 3:
						cout << "Nielosowy"<<endl;
						cout << "Sredni: " << (*average_new_job_time) << endl;
						break;
				}
				cout << "Sredni czas obslugi: ";
				switch( (*service_time_distribution) ){
					case 1:
						cout << "Wykladniczy"<<endl;
						cout << "Sredni: " << (*average_service_time) << endl;
						break;
					case 2:
						cout << "Jednostajny"<<endl;
						cout << "Przedzial: " << (*min_service_time) << " - " << (*max_service_time) << endl;
						break;
					case 3:
						cout << "Nielosowy"<<endl;
						cout << "Sredni: " << (*average_service_time) << endl;
						break;
				}
				cout << "Impatient probability: " << (*impatient_probability) << "%" << endl;
				cout << "VIP probability: " << (*vip_probability) << "%" << endl;
				cout << "Sredni czas zniecierpliwienia: ";
				switch( (*impatient_time_distribution) ){
					case 1:
						cout << "Wykladniczy"<<endl;
						cout << "Sredni: " << (*average_impatient_time) << endl;
						break;
					case 2:
						cout << "Jednostajny"<<endl;
						cout << "Przedzial: " << (*min_impatient_time) << " - " << (*max_impatient_time) << endl;
						break;
					case 3:
						cout << "Nielosowy"<<endl;
						cout << "Sredni: " << (*average_impatient_time) << endl;
						break;
				}
				
				cout << "Srednio ile razy zgloszenie bedzie chcialo byc obsluzone: " << endl;
				cout << "1, szansa: " << services_probabilities[0] << endl;
				cout << "2, szansa: " << services_probabilities[1] << endl;
				cout << "3, szansa: " << services_probabilities[2] << endl;
				cout << "4, szansa: " << services_probabilities[3] << endl;
				cout << "5, szansa: " << services_probabilities[4] << endl;
				
				if(sys.menu.buttons[1].selected){
					cout << "instant_service " << (*instant_service) << endl;
					cout << "instant_queue " << (*instant_queue) << endl;
					cout << "instant_reject " << (*instant_reject) << endl;
					cout << "instant_leave " << (*instant_leave) << endl;
					cout << "instant_resign " << (*instant_resign) << endl;
				}
				cout << "closed_queue_system: " << (*closed_queue_system) << endl;
				cout << "write_data_to_file: " << (*write_data_to_file) << endl;
				cout << endl;



				sys.menu.buttons.clear();
				sys.menu.labels.clear();
				sys.menu.sliders.clear();
				sys.phase=3;
				sys.CreateCharts();
				sys.CreateSimulationBox();
				sys.start_simulation_time = clock() / CLOCKS_PER_SEC;

				sys.InitSystemQueueType();


				//ZEROWANIE NIEKTORYCH DANYCH DO WYKRESOW
				sys.busy_last_time = clock();
				sys.queue_last_time = clock();
				

			}
		}

		if( key == char(27) )
		{
			sys.phase=4;//exit phase
			glutLeaveGameMode();
			glutLeaveMainLoop();
			glutExit();
		}
	}


	void mousePress(int button, int state, int x, int y) 
	{
		if(sys.phase==2){
			switch (button) {
			case GLUT_LEFT_BUTTON:
				if(state == GLUT_DOWN){
					int group_id = 0;
					int button_id = 0;

					if(sys.menu.input!=NULL){
						sys.menu.input->selected=false;
					}
					sys.menu.input = NULL;  //resetujemy wskaznik na input, jezeli uzytkownik gdzies odkliknie


					for(unsigned int i =0;i<sys.menu.sliders.size();i++){//sprawdzamy tez slidery,a w zasadzie ich ruchoma czesc
						if(sys.menu.sliders[i].b.mouse_over){
							sys.menu.sliders[i].b.selected = true;
							sys.menu.sliders[i].click_x = x;
							sys.menu.sliders[i].click_y = y;
							if(sys.menu.sliders[i].b.id==23 || sys.menu.sliders[i].b.id==24 || sys.menu.sliders[i].b.id==11){
								sys.menu.buttons[33].selected = false; //33 - system kolejkowy zamkniety
							}
						}else if(sys.menu.sliders[i].b2.mouse_over){
							sys.menu.sliders[i].b2.selected = true;
							sys.menu.sliders[i].click_x = x;
							sys.menu.sliders[i].click_y = y;
						}
					}

					for(unsigned int i = 0; i<sys.menu.buttons.size(); i++){
						if(sys.menu.buttons[i].mouse_over){
							if(sys.menu.buttons[i].id == 10 || sys.menu.buttons[i].id==5
								|| (sys.menu.buttons[i].id >= 32 && sys.menu.buttons[i].id <= 39) ){
								sys.menu.buttons[33].selected = false; //33 - system kolejkowy zamkniety

							}
							if(sys.menu.buttons[i].type==2){//radio boxa nie da sie odznaczyc
								sys.menu.buttons[i].selected = true;
							}else{
								if(sys.menu.buttons[i].type!=1){
									sys.menu.buttons[i].selected = !sys.menu.buttons[i].selected;
									if(sys.menu.buttons[i].id == 40){//wybor system kolejkowy zamkniety
										
										static bool   t00;
										static bool   t01;
										
										static bool   t1;
										static bool   t2;
										static int    t3;
										static int    t4;
										static string t5;
										static string t6;
										static string t7;
										static string t8;
										static string t9;

										static int t10;
										static string t11;
										if(sys.menu.buttons[i].selected){
											
											t00 = sys.menu.buttons[2].selected;
											t01 = sys.menu.buttons[4].selected;
											t1 = sys.menu.buttons[7].selected;
											t2 = sys.menu.buttons[9].selected;
											t3 = sys.menu.sliders[2].b.pos.x;
											t4 = sys.menu.sliders[3].b.pos.x;
											t5 = sys.menu.labels[33].s;
											t6 = sys.menu.labels[35].s;
											t7 = sys.menu.labels[37].s;
											t8 = sys.menu.labels[39].s;
											t9 = sys.menu.labels[41].s;
											t10 = sys.menu.sliders[0].b.pos.x;
											t11 = sys.menu.buttons[8].text.s;

											sys.menu.buttons[2].selected = true;
											sys.menu.buttons[4].selected = false;
											sys.menu.buttons[7].selected = true;
											sys.menu.buttons[9].selected = false;
											sys.menu.sliders[2].b.pos.x = sys.menu.sliders[2].pos.x;
											sys.menu.sliders[3].b.pos.x = sys.menu.sliders[3].pos.x;
											sys.menu.labels[33].s = "100%";
											sys.menu.labels[35].s = "0%";
											sys.menu.labels[37].s = "0%";
											sys.menu.labels[39].s = "0%";
											sys.menu.labels[41].s = "0%";

											if(sys.menu.buttons[8].text.s=="0" || sys.menu.buttons[8].text.s==""){
												sys.menu.buttons[8].text.s="1";
											}


											//dlugosc kolejki w systemie zamknietym moze byc max 10
											//bo max 10 obrabiarek zmiesci sie na ekranie
											double min_val	= sys.menu.sliders[0].min_value;
											double max_val	= sys.menu.sliders[0].max_value;
											double b_pos_x	= sys.menu.sliders[0].b.pos.x;
											double pos_x	= sys.menu.sliders[0].pos.x;
											double pos_w	= sys.menu.sliders[0].pos.w;
											int value = (b_pos_x - pos_x)/ pos_w * (max_val - min_val) + min_val; 
											if(value > 10){
												int unit_to_pixel = pos_w / (max_val - min_val);
												int new_b_pos_x = pos_x + (10 - min_val)*unit_to_pixel;
												sys.menu.sliders[0].b.pos.x = new_b_pos_x;
											}else if(value < 1){
												int unit_to_pixel = pos_w / (max_val - min_val);
												int new_b_pos_x = pos_x + (1 - min_val)*unit_to_pixel;
												sys.menu.sliders[0].b.pos.x = new_b_pos_x;
											}
										}else{
											sys.menu.buttons[2].selected = t00;
											sys.menu.buttons[4].selected = t01;
											sys.menu.buttons[7].selected = t1;
											sys.menu.buttons[9].selected = t2;
											sys.menu.sliders[2].b.pos.x = t3;
											sys.menu.sliders[3].b.pos.x = t4;
											sys.menu.labels[33].s = t5;
											sys.menu.labels[35].s = t6;
											sys.menu.labels[37].s = t7;
											sys.menu.labels[39].s = t8;
											sys.menu.labels[41].s = t9;
											sys.menu.sliders[0].b.pos.x = t10;
											sys.menu.buttons[8].text.s = t11;

										}
									}
								}else{
									if(sys.menu.buttons[i].rel_index != -1){
										string * s = &sys.menu.labels[sys.menu.buttons[i].rel_index].s;
										int value = 0;
										unsigned int s_size = s->size();
										if(sys.menu.buttons[i].rel_value_in_percents){
											s_size--;
										}
										for(unsigned int j =0;j< s_size;j++){
											value += double((*s)[j]-'0') * pow(10.0,double(s_size-(j+1)));
										}
										//cout << "value: " << value;
										//cout << "s: " << sys.menu.buttons[i].text.s;
										int id = sys.menu.buttons[i].id;
										if(sys.menu.buttons[i].text.s=="+"){
											if (id==32 || id==34 || id==36 || id==38){//tylko do szans na ponowna obsluge
												if(sys.menu.labels[33].s != "0%"){
													string * parent_s = &sys.menu.labels[33].s;
													int parent_value = 0;
													unsigned int parent_s_size = parent_s->size();
													if(sys.menu.buttons[i].rel_value_in_percents){
														parent_s_size--;
													}
													for(unsigned int j =0;j< parent_s_size;j++){
														parent_value += double((*parent_s)[j]-'0') * pow(10.0,double(parent_s_size-(j+1)));
													}

													parent_value--;
													value++;
													stringstream parent_ss;
													parent_ss << parent_value;
													if(sys.menu.buttons[i].rel_value_in_percents){
														parent_ss << "%";
													}
													(*parent_s) = parent_ss.str();

												}
											}else{
												if(value < sys.menu.buttons[i].rel_max_value){
													value++;
												}
											}
										}else if(sys.menu.buttons[i].text.s=="-"){
											if(value > sys.menu.buttons[i].rel_min_value){
												value--;
												if(id==33 || id==35 || id==37 || id==39){//tylko do szans na ponowna obsluge
													string * parent_s = &sys.menu.labels[33].s;
													int parent_value = 0;
													unsigned int parent_s_size = parent_s->size();
													if(sys.menu.buttons[i].rel_value_in_percents){
														parent_s_size--;
													}
													for(unsigned int j =0;j< parent_s_size;j++){
														parent_value += double((*parent_s)[j]-'0') * pow(10.0,double(parent_s_size-(j+1)));
													}

													parent_value++;
													stringstream parent_ss;
													parent_ss << parent_value;
													if(sys.menu.buttons[i].rel_value_in_percents){
														parent_ss << "%";
													}
													(*parent_s) = parent_ss.str();
												}
											}
										}
										stringstream ss;
										ss << value;
										if(sys.menu.buttons[i].rel_value_in_percents){
											ss << "%";
										}
										(*s) = ss.str();
									}
								}
							}
							group_id = sys.menu.buttons[i].group_id;
							button_id = sys.menu.buttons[i].id;
							if(sys.menu.buttons[i].type==5){
								sys.menu.buttons[i].show_carriage = true;
								sys.menu.buttons[i].toogle_carriage_time = clock() / (CLOCKS_PER_SEC / 1000) + 500;
								sys.menu.input = &sys.menu.buttons[i];
							}
						}
					}
					if(group_id && button_id){	//odznaczenie buttonow z grup
						for(unsigned int i =0; i<sys.menu.buttons.size();i++){
							if(sys.menu.buttons[i].group_id == group_id && sys.menu.buttons[i].id != button_id){
								sys.menu.buttons[i].selected=false;
							}
						}
					}

					for(unsigned int i =0; i<sys.menu.buttons.size();i++){ // jezeli nie ma inputu gdzies to daj input 0
						if(!sys.menu.buttons[i].selected && sys.menu.buttons[i].type==5){
							if(sys.menu.buttons[i].text.s.size() == 0){
								if(sys.menu.buttons[i].has_min_value){
									stringstream ss;
									ss << sys.menu.buttons[i].min_value;
									sys.menu.buttons[i].text.s=ss.str();
								}else{
									sys.menu.buttons[i].text.s="0";
								}
								//jezeli uzytkownik ma minimalna wartosc, a w polu tekstowym jest 0:
							}else if(sys.menu.buttons[i].has_min_value && sys.menu.buttons[i].text.s == "0"){
								stringstream ss;
								ss << sys.menu.buttons[i].min_value;
								sys.menu.buttons[i].text.s=ss.str();
							}
						}
					}

					
					

					//sprawdzenie typu symulacji: naukowa lub edukacyjna
					if(sys.menu.buttons.size()){
						if(sys.menu.buttons[0].selected){
							sys.menu.type = 1;
						}else if(sys.menu.buttons[1].selected){
							sys.menu.type = 2;
						}else{
							sys.menu.type = 0;
						}
					}

					//cout<<"-[end of state 1]-"<<endl;
				}
				if (state == GLUT_UP){
					
					for(unsigned int i =0;i<sys.menu.sliders.size();i++){
						if(sys.menu.sliders[i].b.selected){
							sys.menu.sliders[i].b.selected = false;
							sys.menu.sliders[i].b.pos.x = sys.menu.sliders[i].b.pos.x + sys.mouse_x-sys.menu.sliders[i].click_x;
							if(sys.menu.sliders[i].b.pos.x<sys.menu.sliders[i].pos.x){
								sys.menu.sliders[i].b.pos.x = sys.menu.sliders[i].pos.x;
							}else if(sys.menu.sliders[i].b.pos.x>sys.menu.sliders[i].pos.x+sys.menu.sliders[i].pos.w){
								sys.menu.sliders[i].b.pos.x = sys.menu.sliders[i].pos.x+sys.menu.sliders[i].pos.w;
							}
						}
						if(sys.menu.sliders[i].b2.selected){
							sys.menu.sliders[i].b2.selected = false;
							sys.menu.sliders[i].b2.pos.x = sys.menu.sliders[i].b2.pos.x + sys.mouse_x-sys.menu.sliders[i].click_x;
							if(sys.menu.sliders[i].b2.pos.x<sys.menu.sliders[i].pos.x){
								sys.menu.sliders[i].b2.pos.x = sys.menu.sliders[i].pos.x;
							}else if(sys.menu.sliders[i].b2.pos.x>sys.menu.sliders[i].pos.x+sys.menu.sliders[i].pos.w){
								sys.menu.sliders[i].b2.pos.x = sys.menu.sliders[i].pos.x+sys.menu.sliders[i].pos.w;
							}
						}
					}
					
				}
				break;
			case GLUT_RIGHT_BUTTON:
				if (state == GLUT_DOWN){
					//cout<<"state 4"<<endl;
				}
				if (state == GLUT_UP){
					//cout<<"state 5"<<endl;
				}
				break;
			default:
				break;
			}
		}
	}


	void mouseMovement(int x, int y){
		sys.mouse_x = x;
		sys.mouse_y = y;

	}

	void mousePressedMovement(int x, int y){
		sys.mouse_x = x;
		sys.mouse_y = y;
	}


	double r_wykladniczy(double EX){
		double lambda = 1.0/EX;		//lambda = 1.0 / EX
		double u = (rand() % 10000)/10000.0;
		return log(1 - u)/(-lambda);
	}



}

/*
dodajemy ta funkcje przed kazda zmiana zajetosci w systemie
last_time to wartosc w milisekundach
*/
void update_average_business(){
	int busy_servers;
	if(sys.type==2){//naukowy
		busy_servers = sys.sci.busy_servers;
	}else{//edukacyjny
		busy_servers = sys.sim.busy_servers;
	}
	sys.average_busy_servers += (clock() - sys.busy_last_time)*busy_servers;
	sys.busy_last_time = clock();
}


/*
dodajemy ta funkcje przed kazda zmiana dlugosci kolejki w systemie
last_time to wartosc w milisekundach
*/
void update_average_queue_size(){
	int queue_size;
	if(sys.type==2){//naukowy
		queue_size = sys.sci.queue.size();
	}else{//edukacyjny
		queue_size = sys.sim.queue_size;
	}
	sys.average_queue_size += (clock() - sys.queue_last_time)*queue_size;
	sys.queue_last_time = clock();
}


bool prob(int chance){
	if(chance > 100) chance = 100;
	if(chance < 0) chance = 0;

	int random = rand() % 100 + 1;
	if(random <= chance){
		return true;
	}else{
		return false;
	}
}
//funkcja losuje jeden element z tablicy na podstawie prawdopodobienstw i 
//zwraca indeks elementu, ktory wylosowano
int get_services_number(int* tab, int size){
	int random = rand() % 100 + 1;
	int interval_start = 0;

	for(int i = 0; i < size; i++){
		if(random > interval_start && random <= (interval_start+tab[i]) ){
			return i;
		}else{
			interval_start = tab[i];
		}
	}
	return -1;
}

bool equals(double d1, double d2){
	double epsilon = 0.0000001;
	if(d1 == d2){	
		return true;
	}else if(d1 > d2){
		if(d1 - d2 < epsilon){
			return true;
		}
	}else if(d1 < d2){
		if(d2 - d1 < epsilon){
			return true;
		}
	}
	return false;
}