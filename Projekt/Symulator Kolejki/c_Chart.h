#pragma once
#pragma warning (disable : 4244)
#pragma warning (disable : 4305)
#include <string>
#include <sstream>
#include <list>
#include <iostream>
#include "s_Rect.h"
#include "s_Scale.h"
#include "s_Label.h"
#include "c_Chart_Line.h"
#include "h_Includes.h"
using namespace std;

class Chart
{
public:
	Label title;		//title of the chart
	Rect position;		//position - defines x,y,width,height
	Scale horizontal;	//pozioma podzialka
	Scale vertical;		//pionowa podzialka
	list<Chart_Line> lines;
	clock_t saved_time;
	int simulation_time;
	bool can_render;
public:
	void render();
	void update();
	Chart(void);
	Chart(Label title, int x, int y, int w, int h, int h_px, double h_unit, int h_skip, int v_px, double v_unit, int v_skip);
	~Chart(void);
};
