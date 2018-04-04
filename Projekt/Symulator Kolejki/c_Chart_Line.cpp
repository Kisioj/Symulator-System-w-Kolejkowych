#include "c_Chart_Line.h"
#include "c_Chart.h"

Chart_Line::Chart_Line(void)
{
}

Chart_Line::~Chart_Line(void)
{
}

Chart_Line::Chart_Line(string name, int id, float r, float g, float b){
	this->name = name;
	this->id = id;
	this->color.set(r,g,b);
}

void Chart_Line::render(Chart& c){
	//linia na wykresie
	//wspolrzedne poczatkowe - w zaleznosci od tego, na ktorym wykresie rysujemy
	int start_x = c.position.x;
	int start_y = c.position.y+c.position.h;

	//wspolrzedne punktu w pixelach obliczone na podstawie px * (unit)x / unit
	int new_x;
	int new_y;

	//dlugosc w jednostkach (nie pikselach) wykresu
	double horizontal_units; 
	double vertical_units;
	if(!line.empty()){
		horizontal_units = (c.position.w-10) / c.horizontal.px * c.horizontal.unit;
		vertical_units = (c.position.h-10) / c.vertical.px * c.vertical.unit;
		if(line.back().x > horizontal_units + c.horizontal.s_unit){
			c.horizontal.s_unit = line.back().x - horizontal_units;
		}
	}

	glColor3f(color.r,color.g,color.b);
	glBegin(GL_LINE_STRIP);
	for(list<Point>::iterator it=line.begin(); it!=line.end(); ++it){
		new_x = c.horizontal.px*((*it).x-c.horizontal.s_unit)/c.horizontal.unit;
		new_y = c.vertical.px*((*it).y-c.vertical.s_unit)/c.vertical.unit;


		bool draw = true;

		//wyœwietlamy tylko ostatnie dane na wykresie, reszty nie
		if(new_x<0){
			draw = false;
		}

		//jezeli punkt lub poprzedni punkt wychodzi poza górn¹ granicê wykresu, to obecnie kolorujemy ten odcinek
		//na inny kolor, ale w przyszlosci chcemy, zeby po prostu on sie nie pokazywal
		else if(it!=line.begin() ){
			list<Point>::iterator prev = it;
			--prev;
			int prev_new_x = c.horizontal.px*(*prev).x/c.horizontal.unit - c.horizontal.s_unit*c.horizontal.px;//BUG
			int prev_new_y = c.vertical.px*(*prev).y/c.vertical.unit;

			if(prev_new_y>c.position.h && new_y>c.position.h){
				glEnd();
				glBegin(GL_LINE_STRIP);
				glColor3f(0.f, 1.f, 1.f );
				glVertex2f(start_x+prev_new_x,start_y-prev_new_y);

			}else if(new_y>c.position.h){
				int move_y = new_y - c.position.h;
				int move_x = (new_x-prev_new_x)/(new_y-prev_new_y)*move_y;

				glVertex2f(start_x+new_x-move_x, start_y-(new_y-move_y) );
				glEnd();
				glBegin(GL_LINE_STRIP);
				glColor3f(0.f, 1.f, 1.f );
				glVertex2f(start_x+new_x-move_x, start_y-(new_y-move_y) );

			}else if(prev_new_y>c.position.h){
				int move_y = c.position.h - new_y;
				int move_x = (new_x-prev_new_x)/(prev_new_y-new_y)*move_y;

				glColor3f(0.f, 1.f, 1.f );
				glVertex2f(start_x+new_x-move_x, start_y-(new_y+move_y) );
				glEnd();
				glBegin(GL_LINE_STRIP);
				glColor3f(color.r,color.g,color.b);
				glVertex2f(start_x+new_x-move_x, start_y-(new_y+move_y) );

			}else{
				glColor3f(color.r,color.g,color.b);
			}
		}	
		if(draw)
			glVertex2f(start_x+new_x,start_y-new_y);
		else{
			it = line.erase(it);
			if(line.empty()){
				break;
			}
		}

	}
	glEnd();
	

}