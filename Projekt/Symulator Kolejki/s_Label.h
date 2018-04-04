#pragma once
#pragma warning (disable : 4244)
#pragma warning (disable : 4305)
#include <string>
#include "h_Includes.h"
using namespace std;

/*
	Scale label, some xtics and ytics can be labelled 
*/
struct Label {
	int x;
	int y;
	string s;
	void* font;
	int pos;
	int show_when_menu_type;	//tylko uzyteczne do menu
	Label(void);
	Label(string s, void* font, int pos = CENTER);
	void set(int x, int y, string s, void* font, int pos = CENTER);
	void render();
};