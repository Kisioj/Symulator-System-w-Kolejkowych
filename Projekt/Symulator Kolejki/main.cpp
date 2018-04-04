#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "c_System.h"
#include <ttmath.h>
using namespace std;

System sys;

int main(int argc, char* args[])
{
	int x = time(NULL);
	ofstream ziarno("ziarno.txt");
	ziarno << x;
	ziarno.close();
	srand(x);
	sys.Init( argc, args);
	sys.StartSimulation();

	glutMainLoop();
	return 0;
}