#pragma once

struct Rect {
	int x;		//x coordinate
	int y;		//y coordinate
	int w;		//width
	int h;		//height
	void set(int x, int y, int w, int h){
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	};
};