#include "c_Slider.h"
#include "c_System.h"

extern System sys;

Slider::Slider(void)
{
	vertical = false;
	click_x=0;
	click_y=0;

	min_value=0;
	max_value=50;
	value=25;

	show_when_menu_type=2;
	value_in_percents = false;
}

Slider::~Slider(void)
{
}

void Slider::set(int id, Rect pos, Label text, bool vertical, int min_value, int max_value, int value, int show_when_menu_type){
	this->id = id;
	this->pos.set(pos.x,pos.y,pos.w,pos.h);
	this->text = text;
	this->vertical = vertical;
	this->min_value = min_value;
	this->max_value = max_value;
	this->value = value;
	this->show_when_menu_type = show_when_menu_type;
	pos.set(pos.x+double(value)/double(max_value)*double(pos.w),pos.y-8,11,15); 
	b.set(id,pos,"",6,0,GLUT_BITMAP_9_BY_15,show_when_menu_type,LEFT);
	b2.set(id,pos,"",50,0,GLUT_BITMAP_9_BY_15,show_when_menu_type,LEFT);
}
void Slider::render(){
	if( (sys.mouse_x < b.pos.x+5) && (sys.mouse_x > b.pos.x-6) && (sys.mouse_y < b.pos.y+b.pos.h) && (sys.mouse_y > b.pos.y) ){
		b.mouse_over = true;
	}else{
		b.mouse_over = false;
		if(b2.type==6){
			if( (sys.mouse_x < b2.pos.x+5) && (sys.mouse_x > b2.pos.x-6) && (sys.mouse_y < b2.pos.y+b2.pos.h) && (sys.mouse_y > b2.pos.y) ){
				b2.mouse_over = true;
			}else{
				b2.mouse_over = false;
			}
		}
	}

	float x = pos.x;//50
	float y = pos.y;//35+175+sys.SCREEN_HEIGHT/10;
	float w = pos.w;//200
	float h = pos.h;//2


	glColor3f(0.51f,0.51f,0.51f);
	glBegin(GL_LINE_STRIP);
	glVertex2d(x,		y+h);
	glVertex2d(x-1,		y);
	glVertex2d(x+w,	y);
	glEnd();
	glColor3f(0.77f,0.77f,0.77f);
	glBegin(GL_LINE_STRIP);
	glVertex2d(x,		y+h);
	glVertex2d(x+w,	y+h);
	glVertex2d(x+w,	y);
	glEnd();

	//przyjmujemy, ze w = 200
	int line_length = 7;
	glColor3f(0.f,0.f,0.f);
	for(int i = 0;i<=w;i+=10){
		if(!(i%int(w/5))){
			line_length=7;
		}else{
			line_length=4;
		}
		glBegin(GL_LINES);
		glVertex2d(x+i,	y+12);
		glVertex2d(x+i,	y+12+line_length);
		glEnd();
		if(line_length==7){
			stringstream ss;
			/*
			ss << int(double(i)/double(w)*double(max_value-min_value)+min_value);
			if(value_in_percents){
				ss<<"%";
			}*/
			if(value_in_percents){
				ss << double(i)/double(w)*double(max_value/100.0-min_value/100.0)+min_value/100.0;
			}else{
				ss << int(double(i)/double(w)*double(max_value-min_value)+min_value);
			}
			Label l;
			l.set(x+i,y+35,ss.str(),GLUT_BITMAP_8_BY_13,CENTER);
			l.render();
		}
	}


	///////////////RYSOWANIE PRZESUWNIKA//////////////////////
	for(int i=0;i<2;i++){
		int x2;
		int y2;


		if(i==0){
			x2 = b.pos.x;
			if(b.selected){
				x2 = b.pos.x + sys.mouse_x-click_x;
				if(x2<pos.x){
					x2 = pos.x;
				}else if(x2>pos.x+pos.w){
					x2 = pos.x + pos.w;
				}
			}
			y2 = b.pos.y;
			value = double(x2-pos.x)/double(pos.w)*double(max_value-min_value)+min_value;
		}else if(i==1 && b2.type==6){
			x2 = b2.pos.x;
			if(b2.selected){
				x2 = b2.pos.x + sys.mouse_x-click_x;
				if(x2<pos.x){
					x2 = pos.x;
				}else if(x2>pos.x+pos.w){
					x2 = pos.x + pos.w;
				}
			}
			y2 = b2.pos.y;
			value2 = double(x2-pos.x)/double(pos.w)*double(max_value-min_value)+min_value;
		}

		if( (i==0 && b.type==6) || (i==1 && b2.type==6) ){
			glColor3f(0.77f,0.77f,0.77f);
			glBegin(GL_QUADS);//11px szerowkosci,  20px wysokosci
			glVertex2d(x2-6,	y2);
			glVertex2d(x2+5,	y2);
			glVertex2d(x2+5,	y2+15);
			glVertex2d(x2-6,	y2+15);
			glEnd();
			glBegin(GL_TRIANGLES);
			glVertex2d(x2-6,	y2+15);
			glVertex2d(x2+5,	y2+15);
			glVertex2d(x2,		y2+20);
			glEnd();

			glColor3f(0.43f,0.43f,0.43f);
			glBegin(GL_LINE_STRIP);
			glVertex2d(x2+5,	y2);
			glVertex2d(x2+5,	y2+14);
			glVertex2d(x2+5-6,	y2+20);
			glEnd();

			glColor3f(0.88f,0.88f,0.88f);
			glBegin(GL_LINE_STRIP);
			glVertex2d(x2+5-1,	y2+1);
			glVertex2d(x2-5-1,	y2+1);
			glVertex2d(x2-5,	y2+16);
			glVertex2d(x2+5-6,	y2+20);
			glEnd();

			glColor3f(0.51f,0.51f,0.51f);
			glBegin(GL_LINE_STRIP);
			glVertex2d(x2+4,	y2+1);
			glVertex2d(x2+4,	y2+14);
			glVertex2d(x2+4-5,	y2+19);
			glEnd();
		}
		
	}
	glColor3f(0.f,0.f,0.f);
	stringstream txt_value;
	
	if(value_in_percents){
		if(b2.type==6){
			if(value2 > value){
				txt_value << value/100.0 << " - " << value2/100.0;
			}else{
				txt_value << value2/100.0 << " - " << value/100.0;
			}
		}else{
			txt_value << value/100.0;
		}
	}else{
		if(b2.type==6){
			if(value2 > value){
				txt_value << value << " - " << value2;
			}else{
				txt_value << value2 << " - " << value;
			}
		}else{
			txt_value << value;
		}
	}

	text.s = txt_value.str();
	text.render();

}