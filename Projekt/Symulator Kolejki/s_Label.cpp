#include "s_Label.h"
#include "c_System.h"

extern System sys;

Label::Label(void){show_when_menu_type=3;}
Label::Label(string s, void* font, int pos){
	this->s = s;
	this->font = font;
	this->pos = pos;
}
void Label::set(int x, int y, string s, void* font, int pos){
	this->x = x;
	this->y = y;
	this->s = s;
	this->font = font;
	this->pos = pos;
	/*
	if(sys.phase==2){//tylko do MENU
		if(s=="-"){//tylko do buttona sluzanego do dekrementacji
			this->x = this->x-3;
		}
	}
	*/
}
void Label::render(){
	//cout << "[" << s << "]";
	if(sys.phase==2){
		//Jezeli wybrano rozklad jednostajny to pokaz myslnik miedzy buttonami z przedzialami, a jak nie to nie
		if( x==134 && y == int(410+(sys.SCREEN_HEIGHT/10)+85+glutBitmapHeight(GLUT_BITMAP_9_BY_15)) && s==" - "){
			if(!sys.menu.buttons[13].selected){
				return;
			}
		}else if(x==434 && y==int(60+(sys.SCREEN_HEIGHT/10)+85+glutBitmapHeight(GLUT_BITMAP_9_BY_15))){
			if(!sys.menu.buttons[18].selected){
				return;
			}
		}else if(x==734 && y==int(60+(sys.SCREEN_HEIGHT/10)+85+glutBitmapHeight(GLUT_BITMAP_9_BY_15))){
			if(!sys.menu.buttons[23].selected){
				return;
			}
		}
	}
	int px_length = 0;
	for(unsigned int i = 0; i < s.size(); i++){
		px_length += glutBitmapWidth(font,s[i]);
	}
	switch(pos){
			case CENTER:
				glRasterPos2f(x-(px_length/2), y);
				break;
			case LEFT:
				glRasterPos2f(x, y);
				break;
			case RIGHT:
				glRasterPos2f(x-px_length, y);
				break;
	}
	for (unsigned int i = 0; i < s.size(); i++){
		glutBitmapCharacter(font, s[i]);
	}
}