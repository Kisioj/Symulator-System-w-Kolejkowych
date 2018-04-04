#pragma once
#include "h_Includes.h"
#include "s_Rect.h"
#include "s_Label.h"
#include "s_Color.h"
#include <string>
#include <vector>
using namespace std;

class Button
{
public:
	int id;
	Rect pos;
	Label text;
	int type;		//1 - normal, 2 - radio, 3 - checkbox, 4 - selectable, 5 - input, 6 - guzik od suwaka(slidera)
	int group_id;	//mainly for radio

	Color color;
	Color outline;

	Color c_me;			//mouse enter color
	Color o_me;			//outline mouse enter color

	Color c_selected;	//color jak jest wybrany button
	Color o_selected;	//kolor obwodki jak button jest wybrany

	Color c_selected_me;	//kolor jak button jest wybrany a myszka jest nad nim
	Color o_selected_me;	//kolor jak button jest wybrany, a myszka jest nad nim

	Color t_color;	//text color

	bool selected;	//jezeli jest radiem , lub checkboxem etc.
	bool mouse_over;	//true, jezeli myszka znajduje sie nad tym buttonem

	bool show_carriage;				//domyslnie false - pokazywanie karetki, tylko do input
	clock_t toogle_carriage_time;	//czas kiedy zmienimy czas karetki, tylko do input
	unsigned int max_text_length;	//maxymalna dlugosc inputu
	int carriage_type;				//domyslnie 1, jezeli 2 to bedzie efekt jakby byl wlaczony insert
	unsigned int carriage_index;				//domyslnie 0, jest to index chara w stringu, na ktory wskazuje karetka
									//(tylko jezeli carriage_type=2 ma to jakies znaczenie)

	//0 - zawsze widzialne, 1 - widziane gdy symulacja naukowa, 2 - gdy edukacyjna, 3 - widzialne, gdy naukowa lub edukacyjna
	int show_when_menu_type;

	//string * rel;	//relation to some string, needed for + and - buttons
	int rel_index;		//labels are in vector so rel_index will be index to that vector
	int rel_max_value;	//for relation
	int rel_min_value;	//for relation
	bool rel_value_in_percents;	//domyslnie false, jezeli true to wartosci beda w procentach, potrzebne do butonow + i -

	float radius;		//for checkboxes which are circles

	bool has_min_value;	//domyslnie false, true np. dla ilosci kanalow gdzie min = 1;
	int min_value;		//domyslnie 0, 1 dla np. ilosci kanalow

public:
	Button(void);
	~Button(void);
	void set(int id, Rect pos, string text, int type, int group_id, void* font,int show_when_menu_type, int tpos = CENTER);
	void set_colors(float,float,float,float,float,float,float,float,float,
					float,float,float,float,float,float,float,float,float,
					float,float,float,float,float,float,float,float,float);
	void render();
};
