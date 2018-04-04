#include "c_Button.h"
#include "c_System.h"

extern System sys;

Button::Button(void)
{
	selected = false;
	mouse_over = false;
	show_carriage = false;
	max_text_length = 10;
	show_when_menu_type = 3;
	rel_index=-1;
	rel_max_value = 10;
	rel_min_value = 0;
	
	radius = 10.0;
	carriage_type = 1;
	carriage_index = 0;

	has_min_value = false;
	min_value = 0;
	rel_value_in_percents = false;
}



Button::~Button(void)
{
}

void Button::set(int id, Rect pos, string text, int type, int group_id, void* font, int show_when_menu_type, int tpos){
	this->id = id;
	this->pos.set(pos.x,pos.y,pos.w,pos.h);

	int text_x=pos.x+(pos.w/2);
	int text_y=pos.y+(pos.h/2)+(glutBitmapHeight(font)/4);
	if(tpos==LEFT){
		text_x = pos.x + 5;
	}

	this->text.set(text_x,text_y,text,font,tpos);
	this->type = type;
	this->group_id = group_id;

	this->show_when_menu_type = show_when_menu_type;
}

void Button::render(){
	if(sys.phase==2){
		//Jezeli wybrano rozklad jednostajny to pokaz input do wpisania max wartosci, a jak nie to nie
		if( id == 13 && !sys.menu.buttons[13].selected ){
			return;
		}else if(id == 19 && !sys.menu.buttons[18].selected ){
			return;
		}else if(id == 26 && !sys.menu.buttons[23].selected ){
			return;
		}else if(id == 15){//obsluga sliderow
			if(selected){
				sys.menu.sliders[1].b2.type=6;
			}else{
				sys.menu.sliders[1].b2.type=7;
			}
		}
	}


	bool logic_condition;
	if(this->type==2){//inaczej mierzymy czy myszka jest nad buttonem jezeli button jest okragly
		logic_condition = sqrt(pow(sys.mouse_x-pos.x,2.0)+pow(sys.mouse_y-pos.y,2.0)) <= radius;
	}else{
		logic_condition = sys.mouse_x < pos.x+pos.w && sys.mouse_x > pos.x && sys.mouse_y < pos.y+pos.h && sys.mouse_y > pos.y;
	}

	int color_id = 0;
	if(logic_condition){
		mouse_over = true;
		if(selected)
			color_id = 4;
		else
			color_id = 2;
	}else{
		mouse_over = false;
		if(selected)
			color_id = 1;
		else
			color_id = 3;
	}

		
	switch(color_id){
		case 1:
			glColor3f(c_selected.r,c_selected.g,c_selected.b);
			break;
		case 2:
			glColor3f(c_me.r,c_me.g,c_me.b);
			break;
		case 3:
			glColor3f(color.r,color.g,color.b);
			break;
		case 4:
			glColor3f(c_selected_me.r,c_selected_me.g,c_selected_me.b);
			break;
	}


	if(this->type==2){
		glBegin(GL_POLYGON);
		float radius = 10.0;
		for (int i=0; i <360; i+=30)
		{
			float degInRad = i*0.0174532925;
			glVertex2f(pos.x+cos(degInRad)*radius,pos.y+sin(degInRad)*radius);
		}
		glEnd();
	}else{
		glBegin(GL_QUADS);
		glVertex2d(pos.x,			pos.y);
		glVertex2d(pos.x+pos.w,		pos.y);
		glVertex2d(pos.x+pos.w,		pos.y+pos.h);
		glVertex2d(pos.x,			pos.y+pos.h);
		glEnd();
	}


	switch(color_id){
		case 1:
			glColor3f(o_selected.r,o_selected.g,o_selected.b);
			break;
		case 2:
			glColor3f(o_me.r,o_me.g,o_me.b);
			break;
		case 3:
			glColor3f(outline.r,outline.g,outline.b);
			break;
		case 4:
			glColor3f(o_selected_me.r,o_selected_me.g,o_selected_me.b);
			break;
	}

	if(this->type==2){
		glBegin(GL_LINE_LOOP);
		radius = 10.0;
		for (int i=0; i <360; i+=30)
		{
			float degInRad = i*0.0174532925;
			glVertex2f(pos.x+cos(degInRad)*radius,pos.y+sin(degInRad)*radius);
		}
		glEnd();
	}else{
		glBegin(GL_LINE_LOOP);
		glVertex2d(pos.x-1,			pos.y);
		glVertex2d(pos.x+pos.w,		pos.y);
		glVertex2d(pos.x+pos.w,		pos.y+pos.h);
		glVertex2d(pos.x,			pos.y+pos.h);
		glEnd();
	}


	glColor3f(t_color.r,t_color.g,t_color.b);
	text.render();
	if(type == 5 && selected){
		if(clock() / (CLOCKS_PER_SEC / 1000) > toogle_carriage_time){
			show_carriage = !show_carriage;
			toogle_carriage_time = clock() / (CLOCKS_PER_SEC / 1000)+500;
		}
		if(show_carriage){
			if(carriage_type==1){
				int text_px_length = 0;
				for(unsigned int i = 0; i < text.s.size(); i++){
					text_px_length += glutBitmapWidth(text.font,text.s[i]);
				}
				glBegin(GL_LINES);
				glVertex2d(pos.x+6+text_px_length,pos.y+1);
				glVertex2d(pos.x+6+text_px_length,pos.y+1+glutBitmapHeight(text.font));
				//cout << "x,y:" << pos.x+5+text_px_length << "," <<glutBitmapHeight(text.font) << endl;
				glEnd();
			}else{//karetka typu 'insert'
					
				if(carriage_index<0){
					carriage_index=0;
				}else if(carriage_index>text.s.size()){
					carriage_index=text.s.size();
				}

				int text_px_length = 5;		//5 since all the text in input starts 5px from left
				for(unsigned int i = 0; i < carriage_index; i++){
					text_px_length += glutBitmapWidth(text.font,text.s[i]);
				}
				int el_px_length = glutBitmapWidth(text.font,text.s[carriage_index]);
				glBegin(GL_QUADS);
				glVertex2d(pos.x+text_px_length,					pos.y+glutBitmapHeight(text.font));
				glVertex2d(pos.x+text_px_length+el_px_length,		pos.y+glutBitmapHeight(text.font));
				glVertex2d(pos.x+text_px_length+el_px_length,		pos.y+glutBitmapHeight(text.font)+2);
				glVertex2d(pos.x+text_px_length,					pos.y+glutBitmapHeight(text.font)+2);
				glEnd();

			}
		}
	}
}

void Button::set_colors(float r,float g,float b,float ro,float go,float bo,
				float rme ,float gme,float bme, float rome, float gome, float bome,
				float rmd, float gmd, float bmd, float romd, float gomd, float bomd,
				float rsme, float gsme, float bsme, float orsme, float ogsme, float obsme,
				float cr, float cg, float cb){

	color.set(r,g,b);
	outline.set(ro,go,bo);

	c_me.set(rme,gme,bme);
	o_me.set(rome,gome,bome);
	
	c_selected.set(rmd,gmd,bmd);
	o_selected.set(romd,gomd,bomd);

	c_selected_me.set(rsme,gsme,bsme);
	o_selected_me.set(orsme,ogsme,obsme);

	t_color.set(cr,cg,cb);

}
				