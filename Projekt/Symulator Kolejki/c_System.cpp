#include "c_System.h"


System::System(void)
{
	type = 1;
	phase = 1;

	sum_of_waiting_in_queue_times = 0.0;
	sum_of_waiting_in_system_times = 0.0;
	jobs_that_left_queue = 0;
	jobs_that_left_system = 0;

	average_busy_servers = 0.0;
	average_queue_size = 0.0;

	for(int i = 0;i<7;i++){
		options[i] = false;
	}

}

System::~System(void)
{
}

void System::StartSimulation(){
	Simulation::start();

	
}
void System::CreateSimulationBox(){
	int w = (SCREEN_WIDTH/2)-75;
	int h = (SCREEN_HEIGHT/2)-75;
	cout << "w: " << w << ", h:" << h << endl;
	
	
	Label l("Simulation", GLUT_BITMAP_9_BY_15);
	sim.set(l,50+w+50,50+h+50,w,h);
	//symulacje edukacyjna i tak ustawiamy bo korzystamy z jej polozenia
	//przy renderowaniu informacji do symulacji naukowej
	if(type==2){
		sci.set();
	}
}
void System::CreateCharts(){
	int w = (SCREEN_WIDTH/2)-75;
	int h = (SCREEN_HEIGHT/2)-75;
	cout << "w: " << w << ", h:" << h << endl;

	Label l1("Sr. czas pobytu zgl. w sys. [s]", GLUT_BITMAP_8_BY_13);

	int max_servers_size;
	if(this->type==2){
		max_servers_size = this->sci.max_servers_size;
	}else{
		max_servers_size = this->sim.max_servers_size;
	}
	if(max_servers_size>10){//tylko pomocne przy rysowaniu wykresow, aby wykres nie byl zbyt wielki od poczatku
		max_servers_size = 10;
	}
	double c1_v_unit = 1.0/(h/10) * max_servers_size;		//obliczamy ile jednostek ma byc na podzialce pionowej
															//10 to liczba pixeli, co ktora na osi znajduje sie podzialka
	int c1_v_skip = (max_servers_size/10)+1;				//pokazujemy wartosci co 10 procent
	Chart c1( l1, 50, 50, w, h, 10, 1.0, 4, 10, c1_v_unit, 2);
	charts.push_back(c1);
	

	
	Label l2("Sr. zajetosc sys.", GLUT_BITMAP_8_BY_13);
	Chart c2(l2, 50+w+50, 50, w, h, 10, 1.0, 4, 10, c1_v_unit, 2);
	charts.push_back(c2);
	
	if(type==2){
		Label l3("Zajetosc Systemu", GLUT_BITMAP_8_BY_13);
		Chart c3(l3, 50+w+50, 50+h+50, w, h, 10, 1.0, 4, 10, c1_v_unit, 2);
		charts.push_back(c3);
	}
	
}
void System::Init( int argc, char* args[] ){
	glutInit( &argc, args );
	glutInitDisplayMode( GLUT_DOUBLE );


	SCREEN_WIDTH = glutGet(GLUT_SCREEN_WIDTH);
	SCREEN_HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);
	SCREEN_COLOR_DEPTH = 32;
	SCREEN_FPS = 60;


	stringstream mode;
	mode << SCREEN_WIDTH << "x" << SCREEN_HEIGHT << ":" << SCREEN_COLOR_DEPTH << "@" << SCREEN_FPS;
	glutGameModeString( mode.str().c_str() );
	//glutCreateWindow( "Symulator K" );
	glutEnterGameMode();

	glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, -1.0, 1.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();


	glEnable( GL_TEXTURE_2D );
	ilInit();
    ilClearColour( 255, 255, 255, 000 );

	if( !logo.loadTextureFromFile( "logo.png" ) )
    {
        cout <<  "Unable to load file texture!" << endl;
	}else{
		cout << "Loaded file texture!" << endl;
	}

	menu.init();
}

typedef ttmath::Big<TTMATH_BITS(64), TTMATH_BITS(128)> MyBig;
/*
Metoda potrzebna, abysmy wiedzieli, ktorych wzorow uzywac
do analizy systemu kolejkowego, oraz do wygengenerowania
wyœwietlanej póŸniej nazwy systemu kolejkowego na ekranie
*/
void System::InitSystemQueueType(){
	ttmath::Parser<MyBig> parser;


	if(type==2){//naukowy
		stringstream s0;
		s0 << "";
		if(sci.closed_queue_system){
			options[0] = true;
			s0<<"M/M/m/FIFO/N/F";
		}else{
			if(sci.new_job_time_distribution==1){
				s0<<"M/";
			}else if(sci.new_job_time_distribution==2){
				s0<<"G/";
			}else{
				s0<<"D/";
			}
			if(sci.service_time_distribution==1){
				s0<<"M/";
			}else if(sci.service_time_distribution==2){
				s0<<"G/";
			}else{
				s0<<"D/";
			}
			if(sci.max_servers_size==numeric_limits<int>::max()){
				options[4] = true;
				s0<<"INF";
			}else{
				s0<<"m/";
				if(sci.max_queue_size==0){
					options[1] = true;
					s0 << "-/m";
				}else{
					s0 << "FIFO/";
					if(sci.max_queue_size==numeric_limits<int>::max()){
						options[2] = true;
						s0<<"INF";
					}else{
						options[3] = true;
						s0<<"m+N";
					}
				}
			}
		}
		if(sci.impatient_probability>0){
			options[5] = true;
		}
		if(sci.vip_probability>0){
			options[6] = true;
		}

		queue_system<<"System kolejkowy ";
		if(options[0]){
			queue_system<<"zamkniety ";
		}else if(options[1]){
			queue_system<<"ze stratami ";
		}else if(options[2]){
			queue_system<<"z oczekiwaniem ";
		}else if(options[3]){
			queue_system<<"mieszany ";
		}else if(options[4]){
			queue_system<<"o nieograniczonej ilosci kanalow obslugi ";
		}
		queue_system<<s0.str();
		
		//dla naukowej jeszcze sprawdzenie czy jest nieskonczonosc kanalow
		if(sci.max_queue_size>0 && sci.max_servers_size < numeric_limits<int>::max() ){
			if(options[5] && options[6]){
				queue_system<<" z niecierpliwymi i priorytetowymi klientami";
			}else if(options[5]){
				queue_system<<" z niecierpliwymi klientami";
			}else if(options[6]){
				queue_system<<" z priorytetowymi klientami";
			}
		}
	}else if (type==1){//edukacyjny
		stringstream s0;
		s0 << "";
		if(sim.closed_queue_system){
			options[0] = true;
			s0<<"M/M/m/FIFO/N/F";
		}else{
			if(sim.new_job_time_distribution==1){
				s0<<"M/";
			}else if(sim.new_job_time_distribution==2){
				s0<<"G/";
			}else{
				s0<<"D/";
			}
			if(sim.service_time_distribution==1){
				s0<<"M/";
			}else if(sim.service_time_distribution==2){
				s0<<"G/";
			}else{
				s0<<"D/";
			}

			s0<<"m/";
			if(sim.max_queue_size==0){
				options[1] = true;
				s0 << "-/m";
			}else{
				s0 << "FIFO/";
				options[3] = true;
				s0<<"m+N";
			}
			
		}
		if(sim.impatient_probability>0){
			options[5] = true;
		}
		if(sim.vip_probability>0){
			options[6] = true;
		}

		queue_system<<"System kolejkowy ";
		if(options[0]){
			queue_system<<"zamkniety ";
		}else if(options[1]){
			queue_system<<"ze stratami ";
		}else if(options[2]){
			queue_system<<"z oczekiwaniem ";
		}else if(options[3]){
			queue_system<<"mieszany ";
		}else if(options[4]){
			queue_system<<"o nieograniczonej ilosci kanalow obslugi ";
		}
		queue_system<<s0.str();
		if(sim.max_queue_size>0){//dla naukowej jeszcze sprawdzenie czy jest nieskonczonosc kanalow
			if(options[5] && options[6]){
				queue_system<<" z niecierpliwymi i priorytetowymi klientami";
			}else if(options[5]){
				queue_system<<" z niecierpliwymi klientami";
			}else if(options[6]){
				queue_system<<" z priorytetowymi klientami";
			}
		}
	}


	double lambda;	//intensywnosc przychodzenia nowych zgloszen, czyli
					//liczba zgloszen na jednostke czasu
	double miu;		//intensywnosc obslugi, czyli
					//liczba jednostek obsluzonych w ciagu jednostki czasu
	double ro;		//wzgledna intensywnosc obslugi


	double q;		//wzgledna zdolnosc obslugi systemu (2.83) strona 67
	double A;		//bezwzgledna zdolnosc obslugi systemu (2.84) strona 67

	double m0;		//srednia liczba zajetych kanalow obslugi - strona 68

	double v;		//srednia ilosc klientow w kolejce

	int m;			//ilosc kanalow w systemie
	int N;			//dlugosc kolejki w systemie

	double n;		//srednia liczba zgloszen w systemie (w trakcie obslugi oraz w kolejce)

	double ts;		//sredni czas przebywania zgloszenia w systemie
	double to;		//sredni czas obslugi
	double tf;		//sredni czas oczekiwania zgloszenia w kolejce

	//prawdopodobienstwo, ze wszystkie (m z m) kanaly sa zajete, bedziemy obliczali:
	double pm = 0;

	//prawdopodobienstwo, ze nowe zgloszenie nie bedzie moglo wejsc do systemu ani poczekalni:
	double podm = 0;	

	//lambda wzor 1.1
	if(type==2){//naukowy
		if(sci.new_job_time_distribution==2){
			lambda = 1/((sci.max_new_job_time - sci.min_new_job_time)/2.0);
		}else{
			lambda = 1/(sci.average_new_job_time);
		}

		if(sci.service_time_distribution==2){
			miu = 1/((sci.max_service_time - sci.min_service_time)/2.0);
		}else{
			miu = 1/(sci.average_service_time);
		}

		m = sci.max_servers_size;
		N = sci.max_queue_size;


	}else if(type==1){//edukacyjny
		if(sim.new_job_time_distribution==2){
			lambda = 1/((sim.max_new_job_time - sim.min_new_job_time)/2.0);
		}else{
			lambda = 1/(sim.average_new_job_time);
		}

		if(sim.service_time_distribution==2){
			miu = 1/((sim.max_service_time - sim.min_service_time)/2.0);
		}else{
			miu = 1/(sim.average_service_time);
		}

		m = sim.servers.size();
		N = sim.max_queue_size;
	}
	miu;// *= 60;
	ro = lambda / miu; //(strona 65)

	cout << "lambda: " << lambda << endl;
	cout << "miu: " << miu << endl;
	cout << "ro: " << ro << endl;

	MyBig licznik;
	MyBig mianownik;
	stringstream l1;


	////////////////////////////////////////////////
	//System kolejkowy ze stratami M/M/m/-/m////////
	////////////////////////////////////////////////
	//////////DZIWNE WYNIKI TU WYCHODZ¥
	if(options[1]){
		v = 0;		//nie ma kolejki, wiec srednia ilosc klientow w kolejce jest rowna 0
		tf = 0;		//nie ma kolejki, wiec sredni czas oczekiwania w kolejce jest rowny 0
		
		///////////////////////////////////////////////////////////////////////////////
		//wzor (2.82) strona 66 - na prawdopodobienstwo, ze wszystkie kanaly sa zajete
		///////////////////////////////////////////////////////////////////////////////
		
		l1.str(""); l1.clear();
		l1 << ro << "^" << m << " / " << "factorial(" << m << ")";
		parser.Parse(l1.str().c_str());
		licznik = parser.stack[0].value;

		mianownik = 1;	//dla i = 0
		for(int i = 1; i<=m; i++){
			l1.str(""); l1.clear();
			l1 << ro << "^" << i << " / " << "factorial(" << i << ")";
			parser.Parse(l1.str().c_str());
			mianownik += parser.stack[0].value;
		}

		l1.str(""); l1.clear();
		l1 << licznik/mianownik;
		l1 >> pm;

		////////////////////////////////////////////////////////////////////////////////////

		podm = pm;			// w tym systemie kolejkowym zawsze podm = pm
		q = 1 - pm;			//(2.83)
		A = lambda * q;		//(2.84)

		n = A/miu;			//(2.86) strona 67
		m0 = n;				//(2.85) strona 67

		ts = n/lambda;		//(2.122) strona 79 - przeksztalcenie II formuly Little'a
		
	}

	///////////////////////////////////////////////////
	//System kolejkowy z oczekiwaniem M/M/m/FIFO/INF///
	///////////////////////////////////////////////////
	if(options[2]){
		//////////////////////////////////////////////////////////////////////////////
		//wzor (2.98) strona 72 - na prawdopodobienstwo, ze wszystkie kanaly sa zajete
		///////////////////////////////////////////////////////////////////////////////
		assert(ro < m);	// ASSERT
		l1.str(""); l1.clear();
		l1 << ro << "^" << m << " / " << "factorial(" << m << ")";
		parser.Parse(l1.str().c_str());
		licznik = parser.stack[0].value;

		mianownik = 1;	//dla i = 0
		for(int i = 1; i<=(m-1); i++){
			l1.str(""); l1.clear();
			l1 << ro << "^" << i << " / " << "factorial(" << i << ")";
			parser.Parse(l1.str().c_str());
			mianownik += parser.stack[0].value;
		}
		l1.str(""); l1.clear();
		l1 << ro << "^" << m << " / (factorial(" << m-1 << ")" << " * " << m-ro << ")";
		parser.Parse(l1.str().c_str());
		mianownik += parser.stack[0].value;

		l1.str(""); l1.clear();
		l1 << licznik/mianownik;
		l1 >> pm;
		//////////////////////////////////////////////////////////////////////////////////

		podm = 0;				// w tym systemie kolejkowym zawsze podm = 0
		q = 1 - podm;			//strona 76, zawsze bedzie rowne 1 w tym systemie
		A = lambda * q;			//(2.113)
		m0 = A/miu;				//(2.112)

		//////////////////////////////////////////////////////////////
		//wzor (2.101) strona 73 - na srednia ilosc klientow w kolejce v
		//////////////////////////////////////////////////////////////
		assert(ro/m < 1);
		l1.str(""); l1.clear();
		l1 << ro << "^" << m+1 << " / (" << m-ro << "^2 * factorial(" << m-1 << ")" <<")";
		parser.Parse(l1.str().c_str());
		licznik = parser.stack[0].value;

		mianownik = 1;	//dla i = 0
		for(int i = 1; i<=(m-1); i++){
			l1.str(""); l1.clear();
			l1 << ro << "^" << i << " / " << "factorial(" << i << ")";
			parser.Parse(l1.str().c_str());
			mianownik += parser.stack[0].value;
		}
		l1.str(""); l1.clear();
		l1 << ro << "^" << m << " / (factorial(" << m-1 << ")" << " * " << m-ro << ")";
		parser.Parse(l1.str().c_str());
		mianownik += parser.stack[0].value;

		l1.str(""); l1.clear();
		l1 << licznik/mianownik;
		l1 >> v;
		////////////////////////////////////////////////////////////////////

		tf = v/lambda;		//(2.118) I formula Little'a dla stanu ustalonego
		ts = tf + q/miu;	//(2.120)
		n = lambda * ts;	//(2.123) II formula Little'a w stanie ustalonym

	}
	///////////////////////////////////////////////////
	//System kolejkowy mieszany M/M/m/FIFO/m+N/////////
	///////////////////////////////////////////////////
	if(options[3]){
		double p0;
		double q1 = ro/m;
		if(equals(q1,1.0)){//przypadek a) strona 86, kiedy q1 = 1
			/////////////////////////////////
			//wzor (2.148) strona 86 - na p0
			/////////////////////////////////
			//prawa strona rownania (2.148)
			l1.str(""); l1.clear();
			l1 << ro << "^" << m << " / factorial(" << m << ") * " << N+1;
			parser.Parse(l1.str().c_str());
			licznik = parser.stack[0].value;

			//lewa strona rownania (2.148)
			for(int k = 0;k<=(m-1);k++){
				l1.str(""); l1.clear();
				l1 << ro << "^" << k << " / factorial(" << k << ")";
				parser.Parse(l1.str().c_str());
				licznik += parser.stack[0].value;
			}
			l1.str(""); l1.clear();
			l1 << licznik << "^(-1)";
			parser.Parse(l1.str().c_str());
			licznik = parser.stack[0].value;

			l1.str(""); l1.clear();
			l1 << licznik;
			l1 >> p0;

		}else{//przypadek b) strona 86, kiedy q1 != 1
			/////////////////////////////////
			//wzor (2.149) strona 86 - na p0
			/////////////////////////////////
			//prawa strona rownania (2.149)
			l1.str(""); l1.clear();
			l1 << ro << "^" << m << " / factorial(" << m << ") * ";
			l1 << "(1-" << (ro/m) << "^" << N+1 << ")/(1-" << (ro/m) << ")";
			parser.Parse(l1.str().c_str());
			licznik = parser.stack[0].value;


			//lewa strona rownania (2.149)
			for(int k = 0;k<=(m-1);k++){
				l1.str(""); l1.clear();
				l1 << ro << "^" << k << " / factorial(" << k << ")";
				parser.Parse(l1.str().c_str());
				licznik += parser.stack[0].value;
			}
			l1.str(""); l1.clear();
			l1 << licznik << "^(-1)";
			parser.Parse(l1.str().c_str());
			licznik = parser.stack[0].value;

			l1.str(""); l1.clear();
			l1 << licznik;
			l1 >> p0;
		}
		//////////////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////////////
		//wzor 2 z (2.145) strona 85 - prawd. ze wszystkie kanaly sa zajete pm
		///////////////////////////////////////////////////////////////////
		licznik = 0;
		for(int j = m; j<= m+N;j++){
			l1.str(""); l1.clear();
			l1 << "(1/" << m << "^" << j-m << ") * (1/factorial(" << m << ") ) * " << ro << "^" << j << " * " << p0;
			parser.Parse(l1.str().c_str());
			licznik += parser.stack[0].value;
		}
		l1.str(""); l1.clear();
		l1 << licznik;
		l1 >> pm;
		///////////////////////////////////////////////////////////////////

	
		//////////////////////////////////////////////////////////////
		//wzor (2.150) strona 86 - podm, tzn. prawdopodobienstwo odmowy podm
		//////////////////////////////////////////////////////////////
		l1.str(""); l1.clear();
		l1 << ro << "^" << m+N << " / (" << m << "^" << N << "* factorial(" << m <<") ) * " << p0;
		parser.Parse(l1.str().c_str());
		licznik = parser.stack[0].value;

		l1.str(""); l1.clear();
		l1 << licznik;
		l1 >> podm;
		/////////////////////////////////////////////////////////////////

		q = 1 - podm;		//(2.151);
		A = lambda * q;		//(2.152);
		m0 = A/miu;			//(2.153);


		//////////////////////////////////////////////////////////////
		//wzory (2.159) i (2.160) strona 88 - na srednia ilosc klientow w kolejce v
		//////////////////////////////////////////////////////////////
		if(equals(ro,m)){
			l1.str(""); l1.clear();
			l1 << m<<"^"<<m<<"/( factorial(" << m << ")) * ";
			l1 << N*(N+1)/2 << " * ";
			l1 << p0;
			parser.Parse(l1.str().c_str());
			licznik = parser.stack[0].value;
			
			l1.str(""); l1.clear();
			l1 << licznik;
			l1 >> v;
		}else{
			l1.str(""); l1.clear();
			l1 << ro<<"^"<<m+1<<"/( factorial(" << m-1 << ")) * " << p0 << " * ";
			l1 << "(" << "1-("<<ro<<"/"<<m<<")^"<<N<< " * ("<< N << " * (1 - "<<ro<<"/"<<m<< ") + 1) )";
			l1 << "/ ( " << m << " - " << ro << ")^2";
			cout <<"l1: " << l1.str() << endl;
			parser.Parse(l1.str().c_str());
			licznik = parser.stack[0].value;
			
			l1.str(""); l1.clear();
			l1 << licznik;
			l1 >> v;
		}
		//////////////////////////////////////////////////////////////
		/*
		//////////////////////////////////////////////////////////////
		//wzor (2.155) strona 87 - na srednia ilosc klientow w kolejce v
		//////////////////////////////////////////////////////////////
		l1.str(""); l1.clear();
		double a = ro/m;
		
		licznik = 0;
		for(int i=1;i<=N;i++){//prawa strona (nawias) rownania 2.155, pozniej trzeba bedzie pomnozyc razy lewa strone
			l1.str(""); l1.clear();
			l1 << i << " * " << a << "^" << (i-1);
			parser.Parse(l1.str().c_str());
			licznik += parser.stack[0].value;
		}

		l1.str(""); l1.clear();
		l1 << ro<<"^"<<m+1<<"/("<<m<<" * factorial(" << m << ")) * "<<p0<<" * "<<licznik;
		parser.Parse(l1.str().c_str());
		licznik = parser.stack[0].value;

		l1.str(""); l1.clear();
		l1 << licznik;
		l1 >> v;
		///////////////////////////////////////////////////////////////
		*/

		tf = v/lambda;		//strona 89, sredni czas oczekiwania zglosenia w kolejce,
							//stosujac I formule Little'a
		to = 1/miu;			//strona 89
		ts = tf + q*to;
		n = v + m0;			//(2.161)
	}


	////////////////////////////////////////////////////////////////////////////
	//System kolejkowy o nieograniczonej ilosci kanalow obslugi M/M/INF/////////
	////////////////////////////////////////////////////////////////////////////
	if(options[4]){
		
		pm = 0;					//w tym systemie kolejkowym pm zawsze = 0
		podm = 0;				// w tym systemie kolejkowym zawsze podm = 0
		q = 1 - podm;
		A = lambda * q;
		//m0 = A/miu;
		m0 = ro;				// (1.186)

		v = 0;					// (2.190), w tym systemie kolejkowym zawsze v = 0

		tf = 0;					// (2.190), w tym systemie kolejkowym zawsze tf = 0
		ts = 1/miu;				// (2.189) strona 95

		n = ro;					//(2.188) strona 94
		
	}


	///////////////////////////////////////////////////
	//System kolejkowy zamkniêty M/M/m/FIFO/N/F////////
	///////////////////////////////////////////////////
	if(options[0]){
		podm = 0; //w tym systemie kolejkowym nie ma odmów
		q = 1 - podm;
		////////////////////////////////////////////////////////////////////////////////////
		//wzor (2.214) strona 104 - na prawdopodobienstwo, ze wszystkie kanaly sa wolne - p0
		////////////////////////////////////////////////////////////////////////////////////
		double p0;
		
		licznik = 0;

		for(int i = 0;i<=m;i++){
			l1.str(""); l1.clear();
			l1 << "factorial("<<N<<")/(factorial("<<i<<") * factorial("<<N-i<<")) * " << ro << "^" << i;
			parser.Parse(l1.str().c_str());
			licznik += parser.stack[0].value;
		}
		for(int j = m+1; j<=N; j++){
			l1.str(""); l1.clear();
			l1 << "factorial("<<N<<")/(factorial("<<m<<") * factorial("<<N-j<<") * " << m << "^"<<j-m<<") * " << ro << "^" << j;
			parser.Parse(l1.str().c_str());
			licznik += parser.stack[0].value;			
		}
		l1.str(""); l1.clear();
		l1 << licznik << "^(-1)";
		parser.Parse(l1.str().c_str());
		licznik = parser.stack[0].value;

		l1.str(""); l1.clear();
		l1 << licznik;
		l1 >> p0;
		//////////////////////////////////////////////////////////////////////////////////////



		/////////////////////////////////////////////////////////////////////////////////////////
		//w oparciu o wzory (2.213) na stronie 104, liczymy prawd., ze wszystkie kanaly sa zajete pm
		/////////////////////////////////////////////////////////////////////////////////////////
		pm = 0.0;	//jezeli dlugosc kolejki jest mniejsza niz ilosc kanalow,
					//to nigdy wszystkie kanaly nie beda zajete

		if(N>=m){	//jezeli kolejka jest dluzsza lub rowna ilosci kanalow
			l1.str(""); l1.clear();
			l1 << "factorial("<<N<<")/(factorial("<<m<<") * factorial("<<N-m<<")) * " << ro << "^" << m << " * " << p0;
			parser.Parse(l1.str().c_str());
			licznik = parser.stack[0].value;
			for(int j = m+1; j<=N; j++){
				l1.str(""); l1.clear();
				l1 << j << "* factorial("<<N<<")/(factorial("<<m<<") * "<<m<<"^"<<j-m<<" * factorial("<<N-j<<")) * " << ro << "^" << j << " * " << p0;
				parser.Parse(l1.str().c_str());
				licznik += parser.stack[0].value;
			}
			l1.str(""); l1.clear();
			l1 << licznik;
			l1 >> pm;
		}
		/////////////////////////////////////////////////////////////////////////////////////////



		//////////////////////////////////////////////////////////////////////////////
		//wzor (2.216) strona 104 - srednia liczba zgloszen oczekujacych w kolejce, v
		//////////////////////////////////////////////////////////////////////////////
		v = 0.0;	//jezeli kolejka (ilosc obrabiarek) jest krotsza niz ilosc kanalow,
					//to w kolejce srednio nie bedzie nikogo, bo kazde nowe zgloszenie
					//bedzie natychmiast obslugiwane

		if(N>m){	//jezeli kolejka jest dluzsza niz ilosc kanalow	
			licznik = 0;
			for(int r = 1;r<=(N-m);r++){
				l1.str(""); l1.clear();
				l1 << r << "* factorial("<<N<<")/(factorial("<<m<<") * "<<m<<"^"<<(m+r)-m<<" * factorial("<<N-(m+r)<<")) * " << ro << "^" << m+r << " * " << p0;
				parser.Parse(l1.str().c_str());
				licznik += parser.stack[0].value;
			}
			l1.str(""); l1.clear();
			l1 << licznik;
			l1 >> v;
		}
		//////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////
		//wzor (2.217) strona 104 - srednia liczba zajetych kanalow obslugi, m0
		////////////////////////////////////////////////////////////////////////
		licznik = 0.0;
		for(int i = 1; i <= (m-1); i++){ //lewa czesc rownania
			l1.str(""); l1.clear();
			l1 << i << "* factorial("<<N<<")/(factorial("<<i<<") * factorial("<<N-i<<")) * " << ro << "^" << i << " * " << p0;
			parser.Parse(l1.str().c_str());
			licznik += parser.stack[0].value;
		}

		l1.str(""); l1.clear();
		l1 << m << "* factorial("<<N<<")/(factorial("<<m<<") * factorial("<<N-m<<")) * " << ro << "^" << m << " * " << p0;
		parser.Parse(l1.str().c_str());
		licznik += parser.stack[0].value;

		for(int k = m+1; k<= N; k++){
			l1.str(""); l1.clear();
			l1 << m << "* factorial("<<N<<")/(factorial("<<m<<") * "<<m<<"^"<<k-m<<" * factorial("<<N-k<<")) * " << ro << "^" << k << " * " << p0;
			parser.Parse(l1.str().c_str());
			licznik += parser.stack[0].value;
		}

		l1.str(""); l1.clear();
		l1 << licznik;
		l1 >> m0;
		////////////////////////////////////////////////////////////////////////
		

		A = m0 * miu;				//(2.218) strona 105
		n = N - (m0/ro);			//(2.220) strona 105
		ts = n /(lambda*(N-n) );	//(2.221) strona 105 - stosujac II formule Little'a
		tf = ts - (1/miu);			//(2.222) strona 105 - stosujac I formule Little'a

	}


	cout << "Prawdopodobienstwo, ze wszystkie kanaly sa zajete: " << pm << endl;
	cout << "Wzgledna zdolnosc obslugi systemu: " << q << endl;
	cout << "Bezwzgledna zdolnosc obslugi systemu: " << A << endl;
	cout << "Srednia liczba zajetych kanalow: " << m0 << endl;
	cout << "Srednia liczba klientow w kolejce: " << v << endl;
	cout << "Srednia liczba zgloszen w systemie: " << n << endl;
	cout << "Sredni czas przebywania zgloszenia w systemie: " << ts << endl;
	cout << "Sredni czas przebywania zgloszenia w kolejce: " << tf << endl;
	cout << "Sredni czas przebywania zgloszenia w kanale: " << to << endl;

	this->pm =	int(pm*100)	/100.0;
	this->q =	int(q*100)	/100.0;
	this->A =	int(A*100)	/100.0;
	this->m0 =	int(m0*100)	/100.0;
	this->v =	int(v*100)	/100.0;
	this->n =	int(n*100)	/100.0;
	this->ts =	int(ts*100)	/100.0;
}