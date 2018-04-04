#pragma once

struct Scale {
	int px;			//length in pixels between 
	double s_unit;	//0.0 by default, means the starting unit
	double unit;	//unit, says what is the lenght in units between 2 tics
	int skip;		//0 by default - every tics is labelled, 1 - every second tic will be labeled, 4 - every 5th
	void set(int px, double s_unit, double unit, int skip){
		this->px = px;
		this->s_unit = s_unit;
		this->unit = unit;
		this->skip = skip;
	};
};