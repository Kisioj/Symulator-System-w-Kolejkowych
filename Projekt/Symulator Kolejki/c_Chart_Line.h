#pragma once
#include "s_Color.h"
#include "s_Point.h"
#include <string>
#include <list>
using namespace std;

class Chart;

class Chart_Line
{
public:
	string name;		//do legendy
	int id;				//do legendy, zeby bylo wiadomo, na ktorym miejscu w legencie ma byc ta linia opisana
	Color color;
	list<Point> line;	//punkty potrzebne do narysowania lini na wykresie
public:
	Chart_Line(void);
	Chart_Line(string name, int id, float r, float g, float b);
	~Chart_Line(void);
	void render(Chart& c);
};
