#include "c_Chart.h"
#include "c_System.h"

extern System sys;

Chart::Chart(void)
{
	this->title.set(100,30,"Wykres",GLUT_BITMAP_9_BY_15);
	this->position.set(0,0,200,200);
	this->horizontal.set(10, 0.0, 1.0, 4);
	this->vertical.set(10, 0.0, 1.0, 4);
}

Chart::Chart(Label title, int x, int y, int w, int h, int h_px, double h_unit, int h_skip, int v_px, double v_unit, int v_skip){
	this->title.set(x-40,y-5,title.s,title.font,LEFT);		//tytul po lewej stronie i mala czcionka - jako opis osi y
	this->position.set(x,y,w,h);
	this->horizontal.set(h_px, 0.0, h_unit, h_skip);
	this->vertical.set(v_px, 0.0, v_unit, v_skip);
	this->saved_time = NULL;
	this->simulation_time = 0;
	this->can_render = true;
	
	
	float r = 0.f;
	float g = 0.f;
	float b = 0.f;
	if(title.s=="Zajetosc Systemu"){
		r = 1.f;
	}else if(title.s=="Sr. zajetosc sys."){
		g = 1.f;
	}else if(title.s=="Sr. czas pobytu zgl. w sys. [s]"){
		b = 1.f;
	}
	Chart_Line l("generowanie losowe", 1, r, g, b);//zakomentowane, bo jednak nie bedzie legendy na wykresie
	lines.push_back(l);
}

Chart::~Chart(void)
{
}

void Chart::update(){//obecnie co 1/10 sekundy dodajemy do kazdego wykresu losowo nowy punkt
	clock_t uptime = clock() / (CLOCKS_PER_SEC / 1000);
	if(uptime>saved_time){
		double point_y = rand() % 10 + 25;
		int chart_y_max = vertical.unit * (position.h/vertical.px);
		Point p = {1.0*simulation_time,point_y};
		int jobs_that_resigned = 0;
		if(title.s=="Zajetosc Systemu"){
			if(sys.type==2){
				point_y = sys.sci.busy_servers;
				jobs_that_resigned = sys.sci.jobs_resigned;
			}else{
				point_y = sys.sim.busy_servers;
				jobs_that_resigned = sys.sim.jobs_resigned;
			}
			
		}
		if(title.s=="Sr. zajetosc sys."){
			if(sys.type==2){
				point_y = ((clock() - sys.busy_last_time)*sys.sci.busy_servers+sys.average_busy_servers)/clock();
			}else{
				point_y = ((clock() - sys.busy_last_time)*sys.sim.busy_servers+sys.average_busy_servers)/clock();
			}
		}

		if(title.s=="Sr. czas pobytu zgl. w sys. [s]"){

			point_y = int((sys.sum_of_waiting_in_system_times/(sys.jobs_that_left_system+jobs_that_resigned))/10.0)/100.0;
			if(point_y<0.0){
				point_y=0.0;
			}
		}


		p.set(1.0*simulation_time,point_y);
		if(point_y > chart_y_max){
			double v_unit = 1.0/(position.h/vertical.px) * point_y;
			vertical.unit = v_unit;
		}

		list<Chart_Line>::iterator it=lines.begin();
		(*it).line.push_back(p);
		simulation_time++;
		saved_time = uptime + 1000;
		
		if(title.s=="Zajetosc Systemu" || title.s=="Sr. zajetosc sys." || title.s=="Sr. czas pobytu zgl. w sys. [s]" ){
			Point p2 = {1.0*simulation_time,point_y};
			it=lines.begin();
			(*it).line.push_back(p2);
		}
	}	
}

void Chart::render(){
	if(can_render==false){
		return;
	}
	//horizontal labels
	int hor_labels = (position.w-10)/horizontal.px + 1;
	Label* hor = new Label[hor_labels];

	int ver_labels = (position.h-10)/vertical.px + 1;
	Label* ver = new Label[ver_labels];

	glColor3f( 0.f, 0.f, 0.f );
	glBegin(GL_LINES);
	//oœ pionowa
	glVertex2f(position.x,position.y);
	glVertex2f(position.x,position.y+position.h);
	//oœpozioma
	glVertex2f(position.x,position.y+position.h);
	glVertex2f(position.x+position.w,position.y+position.h);

	//podzialka pozioma
	for(int i=0;i<position.w-10;i+=horizontal.px){
		glVertex2f(position.x+i,position.y+position.h+4);
		glVertex2f(position.x+i,position.y+position.h-5);
		if( (i/horizontal.px) < hor_labels){
			stringstream o;
			o<< (i/horizontal.px)*horizontal.unit + horizontal.s_unit;			//+horizontal.s_unit
			hor[i/horizontal.px].set(position.x+i,position.y+position.h+15,o.str(),GLUT_BITMAP_8_BY_13);
		}
	}

	//podzialka pionowa
	for(int i=0;i<position.h-10;i+=vertical.px){
		glVertex2f(position.x-5,position.y+position.h-i);
		glVertex2f(position.x+4,position.y+position.h-i);
		if( (i/vertical.px) < ver_labels){
			stringstream o;
			if( (position.h/vertical.px)*vertical.unit < 100){//czym wieksza skala tym mniejsza dokladnosc po przecinku
				o<< int(((i/vertical.px)*vertical.unit + vertical.s_unit)*100)/100.0;					//+ vertical.s_unit
			}else if( (position.h/vertical.px)*vertical.unit < 1000){
				o<< int(((i/vertical.px)*vertical.unit + vertical.s_unit)*10)/10.0;					//+ vertical.s_unit
			}else{
				o<<int((i/vertical.px)*vertical.unit + vertical.s_unit);
			}
			ver[i/vertical.px].set(position.x-7,position.y+position.h+4-i,o.str(),GLUT_BITMAP_8_BY_13,RIGHT);
		}
	}
	glEnd();

	glColor3f( 0.f, 0.f, 0.f );
	glBegin(GL_TRIANGLES);
	//north arrow
	glVertex2f(position.x-10,position.y+10);
	glVertex2f(position.x,position.y);
	glVertex2f(position.x+11,position.y+10);
	//east arrow
	glVertex2f(position.x+position.w-10,position.y+position.h+10);
	glVertex2f(position.x+position.w,position.y+position.h);
	glVertex2f(position.x+position.w-10,position.y+position.h-11);
	glEnd();

	
	//numeracja do podzialki poziomej:
	for(int i=0;i<hor_labels;i++){
		if( !(i%(horizontal.skip+1)) ){								//miejsca stale, numery zmienne
			hor[i].render();
		}
	}

	//numeracja do podzialki pionowej:
	for(int i=0;i<ver_labels;i++){
		if( !(i%(vertical.skip+1)) ){
			ver[i].render();
		}
	}

	
	//poki na wykresie jest jedna linia, a nie kilka, mozna uzywac ponizszej linijki
	list<Chart_Line>::iterator it=lines.begin();
	it->render(*this);
	
	//tytul
	glColor3f( 0.f, 0.f, 0.f );
	title.render();

	Label l;
	l.set(position.x+position.w, position.y+position.h+20,"Czas [s]",GLUT_BITMAP_8_BY_13,CENTER);
	l.render();

	
	delete[] hor;
	delete[] ver;
	

}

