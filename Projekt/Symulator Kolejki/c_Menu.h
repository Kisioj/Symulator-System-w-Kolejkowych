#pragma once
#include "h_Includes.h"
#include "LTexture.h"
#include "c_Button.h"
#include "c_Slider.h"
#include "s_Label.h"
#include <vector>
using std::vector;

class Menu
{
public:
	vector<Button> buttons;
	vector<Slider> sliders;
	vector<Label> labels;

	Button * input;		//wskaznik do obecnie edytowanego inputu
	int type;		//1 - wybrana symulacja naukowa, 2 - edukacyjna, domyslnie 0 - zadna
public:
	void render();
	void init();
	Menu(void);
	~Menu(void);
};
