#pragma once
#include "s_Rect.h"
#include "s_Label.h"
#include "c_Button.h"
#include <sstream>
#include <string>
using namespace std;

class Slider
{
public:
	int id;
	Rect pos;
	Label text;

	Button b;		//element ruchomy
	Button b2;		//drugi element ruchomy

	bool vertical;	//true - pionowy slider, false - poziomy (horizontal)
	int click_x;	//x elementu ruchomego, pixela od ktorego zaczal przeciaganie uzytkownik
	int click_y;	//y elementu ruchomego, pixela od ktorego zaczal przeciaganie uzytkownik

	int min_value;
	int max_value;
	int value;		//wartosc pierwszego elementu ruchomego
	int value2;		//wartosc drugiego elementu ruchomowego

	int show_when_menu_type;

	bool value_in_percents;		//domyslnie false, true jezeli wartosc pokazana na sliderze jest w procentach
public:
	Slider(void);
	~Slider(void);
	void set(int id, Rect pos, Label text, bool vertical, int min_value, int max_value, int value, int show_when_menu_type);
	void render();
};
