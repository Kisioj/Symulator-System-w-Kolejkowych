#include "c_Menu.h"
#include "c_System.h"

extern System sys;

Menu::Menu(void)
{
	input = NULL;
	type = 0;
}

Menu::~Menu(void)
{
}

void Menu::init(){
	//SYMULACJA NAUKOWA [0]
	Rect r;
	r.set(300,25,sys.SCREEN_WIDTH/4,sys.SCREEN_HEIGHT/10);
	Button b;
	b.set(1,r,"Symulacja Naukowa",4,1,GLUT_BITMAP_TIMES_ROMAN_24,0,CENTER);
	b.set_colors(0.8f,0.8f,0.8f,0.f,0.f,0.f,0.9f,0.9f,0.9f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f);
	buttons.push_back(b);

	//SYMULACJA NAUKOWA [1]
	Rect r2;
	r2.set(sys.SCREEN_WIDTH-(300+sys.SCREEN_WIDTH/4),25,sys.SCREEN_WIDTH/4,sys.SCREEN_HEIGHT/10);
	Button b2;
	b2.set(2,r2,"Symulacja Edukacyjna",4,1,GLUT_BITMAP_TIMES_ROMAN_24,0,CENTER);
	b2.set_colors(0.8f,0.8f,0.8f,0.f,0.f,0.f,0.9f,0.9f,0.9f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f);
	buttons.push_back(b2);


	////////////////////////////////////////////////////////////////
	/////////////////////////////BOX 1//////////////////////////////
	////////////////////////////////////////////////////////////////
	int y = 60+sys.SCREEN_HEIGHT/10;
	//LABEL ILOSC KANALOW [0]
	Label l;
	l.set(50,y+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Ilosc kanalow:",GLUT_BITMAP_9_BY_15,LEFT);
	l.show_when_menu_type = 3;
	labels.push_back(l);

	//NAUKOWA: RADIO BOX - INPUT ILOSC KANALOW [2]
	Rect r4;
	r4.set(60,y+41,14,14);
	Button b4;
	b4.set(4,r4,"",2,2,GLUT_BITMAP_9_BY_15,1,LEFT);
	b4.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	b4.selected = true;
	buttons.push_back(b4);

	//NAUKOWA: INPUT ILOSC KANALOW [3]
	Rect r3;
	r3.set(80,y+32,100,20);
	Button b3;
	b3.max_text_length = 4;
	b3.set(3,r3,"10",5,-1,GLUT_BITMAP_9_BY_15,1,LEFT);
	b3.set_colors(1.f,1.f,1.f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.95f,0.95f,0.95f,0.f,0.f,0.f,0.95f,0.95f,0.95f,0.f,0.f,0.f,0.f,0.f,0.f);
	b3.has_min_value = true;
	b3.min_value = 1;
	buttons.push_back(b3);

	//NAUKOWA: RADIO BOX - NIEOGRANICZONA ILOSC KANALOW [4]
	Rect r5;
	r5.set(60,y+73,14,14);
	Button b5;
	b5.set(5,r5,"",2,2,GLUT_BITMAP_9_BY_15,1,LEFT);
	b5.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	buttons.push_back(b5);

	//NAUKOWA: LABEL NIEOGRANICZONA ILOSC KANALOW [1]
	Label l2;
	l2.set(80,y+62+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Nieograniczona",GLUT_BITMAP_9_BY_15,LEFT);
	l2.show_when_menu_type = 1;
	labels.push_back(l2);

	//EDUKACYJNA: LICZBA ILOSC KANALOW [2]
	Label l3;
	l3.set(60,y+32+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"10",GLUT_BITMAP_9_BY_15,CENTER);
	l3.show_when_menu_type = 2;
	labels.push_back(l3);

	//EDUKACYJNA: ILOSC_KANALOW++ [5]
	Rect r6;
	r6.set(72,y+37,14,14);
	Button b6;
	b6.set(6,r6,"+",1,0,GLUT_BITMAP_9_BY_15,2,CENTER);
	b6.set_colors(0.95f,0.95f,0.95f,0.5f,0.5f,0.5f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.5f,0.5f,0.5f);
	b6.rel_index = 2;//&labels.back().s;
	buttons.push_back(b6);

	//EDUKACYJNA: ILOSC_KANALOW-- [6]
	Rect r7;
	r7.set(88,y+37,14,14);
	Button b7;
	b7.set(7,r7,"-",1,0,GLUT_BITMAP_9_BY_15,2,CENTER);
	b7.set_colors(0.95f,0.95f,0.95f,0.5f,0.5f,0.5f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.5f,0.5f,0.5f);
	b7.rel_index = 2;//&labels.back().s;
	b7.rel_min_value = 1;
	buttons.push_back(b7);
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	/////////////////////////////BOX 2//////////////////////////////
	////////////////////////////////////////////////////////////////
	y = 185+sys.SCREEN_HEIGHT/10;//175+...
	//LABEL DLUGOSC KOLEJKI [3]
	Label l4;
	l4.set(50,y+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Dlugosc kolejki:",GLUT_BITMAP_9_BY_15,LEFT);
	l4.show_when_menu_type = 3;
	labels.push_back(l4);

	//NAUKOWA: RADIO BOX - INPUT DLUGOSC KOLEJKI [7]
	Rect r8;
	r8.set(60,y+41,14,14);
	Button b8;
	b8.set(8,r8,"",2,3,GLUT_BITMAP_9_BY_15,1,LEFT);
	b8.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	b8.selected = true;
	buttons.push_back(b8);

	//NAUKOWA: INPUT DLUGOSC KOLEJKI [8]
	Rect r9;
	r9.set(80,y+32,100,20);
	Button b9;
	b9.max_text_length = 4;
	b9.set(9,r9,"10",5,-1,GLUT_BITMAP_9_BY_15,1,LEFT);
	b9.set_colors(1.f,1.f,1.f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.95f,0.95f,0.95f,0.f,0.f,0.f,0.95f,0.95f,0.95f,0.f,0.f,0.f,0.f,0.f,0.f);
	buttons.push_back(b9);

	//NAUKOWA: RADIO BOX - NIEOGRANICZONA DLUGOSC KOLEJKI [9]
	Rect r10;
	r10.set(60,y+73,14,14);
	Button b10;
	b10.set(10,r10,"",2,3,GLUT_BITMAP_9_BY_15,1,LEFT);
	b10.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	buttons.push_back(b10);

	//NAUKOWA: LABEL NIEOGRANICZONA DLUGOSC KOLEJKI [4]
	Label l5;
	l5.set(80,y+62+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Nieograniczona",GLUT_BITMAP_9_BY_15,LEFT);
	l5.show_when_menu_type = 1;
	labels.push_back(l5);

	//EDUKACYJNA: SLIDER MAXYMALNA DLUGOSC KOLEJKI [0]
	Rect r11;
	r11.set(50,35+y,200,2);
	Label l6;
	l6.set(200,y+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"",GLUT_BITMAP_9_BY_15,LEFT);
	Slider s1;
	s1.set(11,r11,l6,true,0,40,25,2);//max value u mnie na kompie to 50, ale na kompach o rozdzialce 1280x1024 powinno byc 40
	sliders.push_back(s1);
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	/////////////////////////////BOX 3//////////////////////////////
	////////////////////////////////////////////////////////////////
	y = 410+sys.SCREEN_HEIGHT/10;//290+...
	//LABEL ILOSC PRZYJSC W CIAGU 1 MIN
	Label l7;
	l7.set(50,y+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Srednia ilosc zgloszen",GLUT_BITMAP_9_BY_15,LEFT);
	l7.show_when_menu_type = 3;
	labels.push_back(l7);
	Label l8;
	l8.set(50,y+20+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"przychodzacych do systemu",GLUT_BITMAP_9_BY_15,LEFT);
	l8.show_when_menu_type = 3;
	labels.push_back(l8);
	Label l9;
	l9.set(50,y+40+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"w ciagu 1 min: ",GLUT_BITMAP_9_BY_15,LEFT);
	l9.show_when_menu_type = 3;
	labels.push_back(l9);
	Label l10;
	//cout << "WART: " << y+85+glutBitmapHeight(GLUT_BITMAP_9_BY_15) << endl;
	l10.set(134,y+85+glutBitmapHeight(GLUT_BITMAP_9_BY_15)," - ",GLUT_BITMAP_9_BY_15,CENTER);
	l10.show_when_menu_type = 1;
	labels.push_back(l10);

	//NAUKOWA: INPUT SREDNIA (MINIMALNA JEZELI ROZKLAD JEDNOSTANY) ILOSC PRZYJSC W CIAGU 1 MIN [10]
	Rect r12;
	r12.set(50,y+88,70,20);
	Button b12;
	b12.set(12,r12,"100",5,-1,GLUT_BITMAP_9_BY_15,1,LEFT);
	b12.max_text_length = 4;
	b12.set_colors(1.f,1.f,1.f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.95f,0.95f,0.95f,0.f,0.f,0.f,0.95f,0.95f,0.95f,0.f,0.f,0.f,0.f,0.f,0.f);
	b12.has_min_value = true;
	b12.min_value = 1;
	buttons.push_back(b12);

	//NAUKOWA: MAXYMALNA ILOSC PRZYJSC W CIAGU 1 MIN (ROZKLAD JEDNOSTAJNY) [11]
	Rect r13;
	r13.set(150,y+88,70,20);
	Button b13;
	b13.set(13,r13,"10",5,-1,GLUT_BITMAP_9_BY_15,1,LEFT);
	b13.max_text_length = 4;
	b13.set_colors(1.f,1.f,1.f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.95f,0.95f,0.95f,0.f,0.f,0.f,0.95f,0.95f,0.95f,0.f,0.f,0.f,0.f,0.f,0.f);
	b13.has_min_value = true;
	b13.min_value = 1;
	buttons.push_back(b13);


	//OBA: RADIO BOX - ROZKLAD WYKLADNICZY [12]
	Rect r14;
	r14.set(60,y+148,14,14);
	Button b14;
	b14.set(14,r14,"",2,4,GLUT_BITMAP_9_BY_15,3,LEFT);
	b14.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	b14.selected = true;
	buttons.push_back(b14);

	//OBA: LABEL ROZKLAD WYKLADNICZY 
	Label l11;
	l11.set(80,y+137+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Rozklad Poissona",GLUT_BITMAP_9_BY_15,LEFT);//odstepy sa z Wykladniczego
	l11.show_when_menu_type = 3;
	labels.push_back(l11);


	//OBA: RADIO BOX - ROZKLAD JEDNOSTAJNY [13]
	Rect r15;
	r15.set(60,y+183,14,14);
	Button b15;
	b15.set(15,r15,"",2,4,GLUT_BITMAP_9_BY_15,3,LEFT);
	b15.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	buttons.push_back(b15);

	//OBA: LABEL ROZKLAD JEDNOSTAJNY DYSKRETNY
	Label l12;
	l12.set(80,y+172+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Rozklad Jednostajny",GLUT_BITMAP_9_BY_15,LEFT);
	l12.show_when_menu_type = 3;
	labels.push_back(l12);

	//OBA: RADIO BOX - WARTOSC STALA (NIELOSOWA) [14]
	Rect r16;
	r16.set(60,y+218,14,14);
	Button b16;
	b16.set(16,r16,"",2,4,GLUT_BITMAP_9_BY_15,3,LEFT);
	b16.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	buttons.push_back(b16);

	//OBA: LABEL WARTOSC STALA (NIELOSOWA)
	Label l13;
	l13.set(80,y+207+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Wartosc Nielosowa",GLUT_BITMAP_9_BY_15,LEFT);
	l13.show_when_menu_type = 3;
	labels.push_back(l13);

	//EDUKACYJNA: SLIDER ILE SREDNIO OSOB MA PRZYJSC W CIAGU 1 MINUTY [1]
	Rect r17;
	r17.set(50,80+y,200,2);
	Label l14;
	l14.set(180,y+40+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"",GLUT_BITMAP_9_BY_15,LEFT);
	Slider s2;
	s2.set(17,r17,l14,true,1,200,100,2);
	sliders.push_back(s2);
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	/////////////////////////////BOX 4//////////////////////////////
	////////////////////////////////////////////////////////////////
	y = 60+sys.SCREEN_HEIGHT/10;
	//LABEL ILOSC PRZYJSC W CIAGU 1 MIN
	Label l15;
	l15.set(350,y+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Sredni czas obslugi",GLUT_BITMAP_9_BY_15,LEFT);
	l15.show_when_menu_type = 3;
	labels.push_back(l15);
	Label l16;
	l16.set(350,y+20+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"zgloszenia przez kanal, ",GLUT_BITMAP_9_BY_15,LEFT);
	l16.show_when_menu_type = 3;
	labels.push_back(l16);
	Label l17;
	l17.set(350,y+40+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"w sekundach: ",GLUT_BITMAP_9_BY_15,LEFT);
	l17.show_when_menu_type = 3;
	labels.push_back(l17);
	Label l18;
	l18.set(434,y+85+glutBitmapHeight(GLUT_BITMAP_9_BY_15)," - ",GLUT_BITMAP_9_BY_15,CENTER);
	l18.show_when_menu_type = 3;
	labels.push_back(l18);

	//OBA: INPUT SREDNIA (MINIMALNA JEZELI ROZKLAD JEDNOSTANY) ILOSC PRZYJSC W CIAGU 1 MIN [15]
	Rect r18;
	r18.set(350,y+88,70,20);
	Button b18;
	b18.set(18,r18,"001.00",5,-1,GLUT_BITMAP_9_BY_15,3,LEFT);
	b18.max_text_length = 6;
	b18.set_colors(1.f,1.f,1.f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.95f,0.95f,0.95f,0.f,0.f,0.f,0.95f,0.95f,0.95f,0.f,0.f,0.f,0.f,0.f,0.f);
	b18.carriage_type=2;
	buttons.push_back(b18);

	//OBA: MAXYMALNA ILOSC PRZYJSC W CIAGU 1 MIN (ROZKLAD JEDNOSTAJNY) [16]
	Rect r19;
	r19.set(450,y+88,70,20);
	Button b19;
	b19.set(19,r19,"003.00",5,-1,GLUT_BITMAP_9_BY_15,3,LEFT);
	b19.max_text_length = 6;
	b19.set_colors(1.f,1.f,1.f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.95f,0.95f,0.95f,0.f,0.f,0.f,0.95f,0.95f,0.95f,0.f,0.f,0.f,0.f,0.f,0.f);
	b19.carriage_type=2;
	buttons.push_back(b19);


	//OBA: RADIO BOX - ROZKLAD WYKLADNICZY [17]
	Rect r20;
	r20.set(360,y+148,14,14);
	Button b20;
	b20.set(20,r20,"",2,5,GLUT_BITMAP_9_BY_15,3,LEFT);
	b20.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	b20.selected = true;
	buttons.push_back(b20);

	//OBA: LABEL ROZKLAD WYKLADNICZY
	Label l19;
	l19.set(380,y+137+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Rozklad Wykladniczy",GLUT_BITMAP_9_BY_15,LEFT);
	l19.show_when_menu_type = 3;
	labels.push_back(l19);


	//OBA: RADIO BOX - ROZKLAD JEDNOSTAJNY [18]
	Rect r21;
	r21.set(360,y+183,14,14);
	Button b21;
	b21.set(21,r21,"",2,5,GLUT_BITMAP_9_BY_15,3,LEFT);
	b21.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	buttons.push_back(b21);

	//OBA: LABEL ROZKLAD JEDNOSTAJNY CIAGLY (ROWNOMIERNY)
	Label l20;
	l20.set(380,y+172+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Rozklad Jednostajny",GLUT_BITMAP_9_BY_15,LEFT);
	l20.show_when_menu_type = 3;
	labels.push_back(l20);

	//OBA: RADIO BOX - WARTOSC STALA (NIELOSOWA) [19]
	Rect r22;
	r22.set(360,y+218,14,14);
	Button b22;
	b22.set(22,r22,"",2,5,GLUT_BITMAP_9_BY_15,3,LEFT);
	b22.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	buttons.push_back(b22);

	//OBA: LABEL WARTOSC STALA (NIELOSOWA)
	Label l21;
	l21.set(380,y+207+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Wartosc Nielosowa",GLUT_BITMAP_9_BY_15,LEFT);
	l21.show_when_menu_type = 3;
	labels.push_back(l21);
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	/////////////////////////////BOX 5//////////////////////////////
	////////////////////////////////////////////////////////////////
	y = 370+sys.SCREEN_HEIGHT/10;
	
	//OBA: SZANSA NA NIECIERPLIWE ZGLOSZENIE
	Label l22;
	l22.set(350,y+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Prawd., ze nowe zgloszenie",GLUT_BITMAP_9_BY_15,LEFT);
	l22.show_when_menu_type = 3;
	labels.push_back(l22);
	Label l23;
	l23.set(350,y+20+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"bedzie niecierpliwe: ",GLUT_BITMAP_9_BY_15,LEFT);
	l23.show_when_menu_type = 3;
	labels.push_back(l23);

	//OBA: SLIDER PROCENT SZANS NA NIECIERPLIWE ZGLOSZENIE [2]
	Rect r23;
	r23.set(350,y+60,200,2);
	Label l24;
	l24.set(/*456*/535,y+20+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"1",GLUT_BITMAP_9_BY_15,LEFT);
	Slider s3;
	s3.set(23,r23,l24,true,0,100,50,3);
	s3.value_in_percents = true;
	sliders.push_back(s3);
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	/////////////////////////////BOX 6//////////////////////////////
	////////////////////////////////////////////////////////////////
	y = 550+sys.SCREEN_HEIGHT/10;
	
	//OBA: SZANSA NA PRIORYTETOWE ZGLOSZENIE
	Label l25;
	l25.set(350,y+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Prawd., ze nowe zgloszenie",GLUT_BITMAP_9_BY_15,LEFT);
	l25.show_when_menu_type = 3;
	labels.push_back(l25);
	Label l26;
	l26.set(350,y+20+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"bedzie priorytetowe: ",GLUT_BITMAP_9_BY_15,LEFT);
	l26.show_when_menu_type = 3;
	labels.push_back(l26);

	//OBA: SLIDER PROCENT SZANS NA PRIORYTETOWE ZGLOSZENIE [3]
	Rect r24;
	r24.set(350,y+60,200,2);
	Label l27;
	l27.set(535,y+20+glutBitmapHeight(GLUT_BITMAP_9_BY_15),/*"100%"*/"1",GLUT_BITMAP_9_BY_15,LEFT);
	Slider s4;
	s4.set(24,r24,l27,true,0,100,50,3);
	s4.value_in_percents = true;
	sliders.push_back(s4);
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	/////////////////////////////BOX 7//////////////////////////////
	////////////////////////////////////////////////////////////////
	y = 60+sys.SCREEN_HEIGHT/10;
	//LABEL CZAS OCZEKIWANIA, PO KTORYM | ZGLOSZENIE WYJDZIE Z | KOLEJKI: _ _ _ _ _ - _ _ _ _ _
	Label l28;
	l28.set(650,y+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Sredni czas oczekiwania, po ktorym",GLUT_BITMAP_9_BY_15,LEFT);
	l28.show_when_menu_type = 3;
	labels.push_back(l28);
	Label l29;
	l29.set(650,y+20+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"niecierpliwe zgloszenie zdecyduje",GLUT_BITMAP_9_BY_15,LEFT);
	l29.show_when_menu_type = 3;
	labels.push_back(l29);
	Label l30;
	l30.set(650,y+40+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"sie na wyjscie z kolejki (sek.): ",GLUT_BITMAP_9_BY_15,LEFT);
	l30.show_when_menu_type = 3;
	labels.push_back(l30);
	Label l31;
	l31.set(734,y+85+glutBitmapHeight(GLUT_BITMAP_9_BY_15)," - ",GLUT_BITMAP_9_BY_15,CENTER);
	l31.show_when_menu_type = 3;
	labels.push_back(l31);

	//OBA: INPUT SREDNIA (MINIMALNA JEZELI ROZKLAD JEDNOSTANY) CZASU OCZEKIWANIA, PO KTORYM ZGLOSZENIE WYJDZIE Z KOLEJKI [20]
	Rect r25;
	r25.set(650,y+88,70,20);
	Button b25;
	b25.set(25,r25,"010.00",5,-1,GLUT_BITMAP_9_BY_15,3,LEFT);
	b25.max_text_length = 6;
	b25.set_colors(1.f,1.f,1.f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.95f,0.95f,0.95f,0.f,0.f,0.f,0.95f,0.95f,0.95f,0.f,0.f,0.f,0.f,0.f,0.f);
	b25.carriage_type=2;
	buttons.push_back(b25);

	//OBA: INPUT MAXYMALNA WARTOSC CZASU OCZEKIWANIA, PO KTORYM ZGLOSZENIE WYJDZIE Z KOLEJKI (ROZKLAD JEDNOSTAJNY) [21]
	Rect r26;
	r26.set(750,y+88,70,20);
	Button b26;
	b26.set(26,r26,"010.00",5,-1,GLUT_BITMAP_9_BY_15,3,LEFT);
	b26.max_text_length = 6;
	b26.set_colors(1.f,1.f,1.f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.95f,0.95f,0.95f,0.f,0.f,0.f,0.95f,0.95f,0.95f,0.f,0.f,0.f,0.f,0.f,0.f);
	b26.carriage_type=2;
	buttons.push_back(b26);


	//OBA: RADIO BOX - ROZKLAD WYKLADNICZY [22]
	Rect r27;
	r27.set(660,y+148,14,14);
	Button b27;
	b27.set(27,r27,"",2,6,GLUT_BITMAP_9_BY_15,3,LEFT);
	b27.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	b27.selected = true;
	buttons.push_back(b27);

	//OBA: LABEL ROZKLAD WYKLADNICZY
	Label l32;
	l32.set(680,y+137+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Rozklad Wykladniczy",GLUT_BITMAP_9_BY_15,LEFT);
	l32.show_when_menu_type = 3;
	labels.push_back(l32);


	//OBA: RADIO BOX - ROZKLAD JEDNOSTAJNY [23]
	Rect r28;
	r28.set(660,y+183,14,14);
	Button b28;
	b28.set(28,r28,"",2,6,GLUT_BITMAP_9_BY_15,3,LEFT);
	b28.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	buttons.push_back(b28);

	//OBA: LABEL ROZKLAD JEDNOSTAJNY DYSKRETNY
	Label l33;
	l33.set(680,y+172+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Rozklad Jednostajny",GLUT_BITMAP_9_BY_15,LEFT);
	l33.show_when_menu_type = 3;
	labels.push_back(l33);

	//OBA: RADIO BOX - WARTOSC STALA (NIELOSOWA) [24]
	Rect r29;
	r29.set(660,y+218,14,14);
	Button b29;
	b29.set(29,r29,"",2,6,GLUT_BITMAP_9_BY_15,3,LEFT);
	b29.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	buttons.push_back(b29);

	//OBA: LABEL WARTOSC STALA (NIELOSOWA)
	Label l34;
	l34.set(680,y+207+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Wartosc Nielosowa",GLUT_BITMAP_9_BY_15,LEFT);
	l34.show_when_menu_type = 3;
	labels.push_back(l34);
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	/////////////////////////////BOX 8//////////////////////////////
	////////////////////////////////////////////////////////////////
	y = 320+sys.SCREEN_HEIGHT/10;
	//LABEL CZAS OCZEKIWANIA, PO KTORYM | ZGLOSZENIE WYJDZIE Z | KOLEJKI: _ _ _ _ _ - _ _ _ _ _
	Label l35;
	l35.set(650,y+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Srednio ile razy zglosenie",GLUT_BITMAP_9_BY_15,LEFT);
	l35.show_when_menu_type = 3;
	labels.push_back(l35);
	Label l36;
	l36.set(650,y+20+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"bedzie chcialo byc obsluzone:",GLUT_BITMAP_9_BY_15,LEFT);
	l36.show_when_menu_type = 3;
	labels.push_back(l36);
	

	//NAPIS SZANSA, ZE 1 OBSLUZENIE
	Label l37;
	l37.set(650,y+20+32+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"1, szansa: ",GLUT_BITMAP_9_BY_15,LEFT);
	l37.show_when_menu_type = 3;
	labels.push_back(l37);
	
	//SZANSA, ZE 1 OBSLUZENIE
	Label l38;
	l38.set(762,y+20+32+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"100%",GLUT_BITMAP_9_BY_15,CENTER);
	l38.show_when_menu_type = 3;
	labels.push_back(l38);

	//NAPIS SZANSA, ZE 2 OBSLUZENIA
	Label l39;
	l39.set(650,y+40+32+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"2, szansa: ",GLUT_BITMAP_9_BY_15,LEFT);
	l39.show_when_menu_type = 3;
	labels.push_back(l39);

	//SZANSA, ZE 2 OBSLUZENIA
	Label l40;
	l40.set(762,y+40+32+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"0%",GLUT_BITMAP_9_BY_15,CENTER);
	l40.show_when_menu_type = 3;
	labels.push_back(l40);

	//SZANSA, ZE 2 OBSLUZENIA ++ [25]
	Rect r32;
	r32.set(784,y+40+37,14,14);
	Button b32;
	b32.set(32,r32,"+",1,0,GLUT_BITMAP_9_BY_15,3,CENTER);
	b32.set_colors(0.95f,0.95f,0.95f,0.5f,0.5f,0.5f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.5f,0.5f,0.5f);
	b32.rel_index = 35;
	b32.rel_max_value = 100;
	b32.rel_value_in_percents = true;
	buttons.push_back(b32);

	//SZANSA, ZE 2 OBSLUZENIA -- [26]
	Rect r33;
	r33.set(800,y+40+37,14,14);
	Button b33;
	b33.set(33,r33,"-",1,0,GLUT_BITMAP_9_BY_15,3,CENTER);
	b33.set_colors(0.95f,0.95f,0.95f,0.5f,0.5f,0.5f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.5f,0.5f,0.5f);
	b33.rel_index = 35;
	b33.rel_min_value = 0;
	b33.rel_value_in_percents = true;
	buttons.push_back(b33);


	//NAPIS SZANSA, ZE 3 OBSLUZENIA
	Label l41;
	l41.set(650,y+60+32+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"3, szansa: ",GLUT_BITMAP_9_BY_15,LEFT);
	l41.show_when_menu_type = 3;
	labels.push_back(l41);

	//SZANSA, ZE 3 OBSLUZENIA
	Label l42;
	l42.set(762,y+60+32+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"0%",GLUT_BITMAP_9_BY_15,CENTER);
	l42.show_when_menu_type = 3;
	labels.push_back(l42);

	//SZANSA, ZE 3 OBSLUZENIA ++ [27]
	Rect r34;
	r34.set(784,y+60+37,14,14);
	Button b34;
	b34.set(34,r34,"+",1,0,GLUT_BITMAP_9_BY_15,3,CENTER);
	b34.set_colors(0.95f,0.95f,0.95f,0.5f,0.5f,0.5f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.5f,0.5f,0.5f);
	b34.rel_index = 37;
	b34.rel_max_value = 100;
	b34.rel_value_in_percents = true;
	buttons.push_back(b34);

	//SZANSA, ZE 3 OBSLUZENIA -- [28]
	Rect r35;
	r35.set(800,y+60+37,14,14);
	Button b35;
	b35.set(35,r35,"-",1,0,GLUT_BITMAP_9_BY_15,3,CENTER);
	b35.set_colors(0.95f,0.95f,0.95f,0.5f,0.5f,0.5f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.5f,0.5f,0.5f);
	b35.rel_index = 37;
	b35.rel_min_value = 0;
	b35.rel_value_in_percents = true;
	buttons.push_back(b35);


	//NAPIS SZANSA, ZE 4 OBSLUZENIA
	Label l43;
	l43.set(650,y+80+32+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"4, szansa: ",GLUT_BITMAP_9_BY_15,LEFT);
	l43.show_when_menu_type = 3;
	labels.push_back(l43);

	//SZANSA, ZE 4 OBSLUZENIA
	Label l44;
	l44.set(762,y+80+32+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"0%",GLUT_BITMAP_9_BY_15,CENTER);
	l44.show_when_menu_type = 3;
	labels.push_back(l44);

	//SZANSA, ZE 4 OBSLUZENIA ++ [29]
	Rect r36;
	r36.set(784,y+80+37,14,14);
	Button b36;
	b36.set(36,r36,"+",1,0,GLUT_BITMAP_9_BY_15,3,CENTER);
	b36.set_colors(0.95f,0.95f,0.95f,0.5f,0.5f,0.5f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.5f,0.5f,0.5f);
	b36.rel_index = 39;
	b36.rel_max_value = 100;
	b36.rel_value_in_percents = true;
	buttons.push_back(b36);

	//SZANSA, ZE 4 OBSLUZENIA -- [30]
	Rect r37;
	r37.set(800,y+80+37,14,14);
	Button b37;
	b37.set(37,r37,"-",1,0,GLUT_BITMAP_9_BY_15,3,CENTER);
	b37.set_colors(0.95f,0.95f,0.95f,0.5f,0.5f,0.5f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.5f,0.5f,0.5f);
	b37.rel_index = 39;
	b37.rel_min_value = 0;
	b37.rel_value_in_percents = true;
	buttons.push_back(b37);



	//NAPIS SZANSA, ZE 5 OBSLUZEN
	Label l45;
	l45.set(650,y+100+32+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"5, szansa: ",GLUT_BITMAP_9_BY_15,LEFT);
	l45.show_when_menu_type = 3;
	labels.push_back(l45);

	//SZANSA, ZE 5 OBSLUZEN
	Label l46;
	l46.set(762,y+100+32+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"0%",GLUT_BITMAP_9_BY_15,CENTER);
	l46.show_when_menu_type = 3;
	labels.push_back(l46);

	//SZANSA, ZE 5 OBSLUZEN ++ [31]
	Rect r38;
	r38.set(784,y+100+37,14,14);
	Button b38;
	b38.set(38,r38,"+",1,0,GLUT_BITMAP_9_BY_15,3,CENTER);
	b38.set_colors(0.95f,0.95f,0.95f,0.5f,0.5f,0.5f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.5f,0.5f,0.5f);
	b38.rel_index = 41;
	b38.rel_max_value = 100;
	b38.rel_value_in_percents = true;
	buttons.push_back(b38);

	//SZANSA, ZE 5 OBSLUZEN -- [32]
	Rect r39;
	r39.set(800,y+100+37,14,14);
	Button b39;
	b39.set(39,r39,"-",1,0,GLUT_BITMAP_9_BY_15,3,CENTER);
	b39.set_colors(0.95f,0.95f,0.95f,0.5f,0.5f,0.5f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.5f,0.5f,0.5f);
	b39.rel_index = 41;
	b39.rel_min_value = 0;
	b39.rel_value_in_percents = true;
	buttons.push_back(b39);
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////
	/////////////////////////////BOX 9//////////////////////////////
	////////////////////////////////////////////////////////////////
	//y = 570+sys.SCREEN_HEIGHT/10;
	y = 315+sys.SCREEN_HEIGHT/10;//175+...
	//OBA: BOX - SYSTEM KOLEJKOWY ZAMKNIETY [33]
	Rect r40;
	r40.set(50,y+4,18,18);
	Button b40;
	b40.set(40,r40,"",3,0,GLUT_BITMAP_9_BY_15,3,LEFT);
	b40.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,1.f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	buttons.push_back(b40);

	//NAPIS SYSTEM KOLEJKOWY ZAMKNOETY
	Label l47;
	l47.set(75,y+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"System Kolejkowy Zamkniety",GLUT_BITMAP_9_BY_15,LEFT);
	l47.show_when_menu_type = 3;
	labels.push_back(l47);

	//OBA: BOX - ZAPIS DANYCH DO PLIKU [34]
	Rect r41;
	r41.set(50,y+35+4,18,18);
	Button b41;
	b41.set(41,r41,"",3,0,GLUT_BITMAP_9_BY_15,3,LEFT);
	b41.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,1.f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	b41.selected = true;
	buttons.push_back(b41);

	//NAPIS ZAPIS DANYCH DO PLIKU
	Label l48;
	l48.set(75,y+35+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Zapis danych do pliku",GLUT_BITMAP_9_BY_15,LEFT);
	l48.show_when_menu_type = 3;
	labels.push_back(l48);
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	

	////////////////////////////////////////////////////////////////
	/////////////////////////////BOX 10/////////////////////////////
	////////////////////////////////////////////////////////////////
	y = 520+sys.SCREEN_HEIGHT/10;
	Label l49;
	l49.set(650,y+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Animacje:",GLUT_BITMAP_9_BY_15,LEFT);
	l49.show_when_menu_type = 2;
	labels.push_back(l49);

	Label l50;
	l50.set(650,y+30+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Droga do kanalu",GLUT_BITMAP_9_BY_15,LEFT);
	l50.show_when_menu_type = 2;
	labels.push_back(l50);

	Label l51;
	l51.set(650,y+50+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Wejscie do kolejki",GLUT_BITMAP_9_BY_15,LEFT);
	l51.show_when_menu_type = 2;
	labels.push_back(l51);

	Label l52;
	l52.set(650,y+70+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Odrzucenie zgloszenia",GLUT_BITMAP_9_BY_15,LEFT);
	l52.show_when_menu_type = 2;
	labels.push_back(l52);

	Label l53;
	l53.set(650,y+90+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Wyjscie z systemu",GLUT_BITMAP_9_BY_15,LEFT);
	l53.show_when_menu_type = 2;
	labels.push_back(l53);

	Label l54;
	l54.set(650,y+110+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Rezygnacja z kolejki",GLUT_BITMAP_9_BY_15,LEFT);
	l54.show_when_menu_type = 2;
	labels.push_back(l54);

	//OBA: BOX - DROGA DO KANALU [35]
	Rect r42;
	r42.set(850,y+30+7,12,12);
	Button b42;
	b42.set(42,r42,"",3,0,GLUT_BITMAP_9_BY_15,2,LEFT);
	b42.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,1.f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	b42.selected = true;
	buttons.push_back(b42);

	//OBA: BOX - WEJSCIE DO KOLEJKI [36]
	Rect r43;
	r43.set(850,y+50+7,12,12);
	Button b43;
	b43.set(43,r43,"",3,0,GLUT_BITMAP_9_BY_15,2,LEFT);
	b43.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,1.f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	b43.selected = true;
	buttons.push_back(b43);

	//OBA: BOX - ODRZUCENIE ZGLOSZENIA [37]
	Rect r44;
	r44.set(850,y+70+7,12,12);
	Button b44;
	b44.set(44,r44,"",3,0,GLUT_BITMAP_9_BY_15,2,LEFT);
	b44.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,1.f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	b44.selected = true;
	buttons.push_back(b44);

	//OBA: BOX - WYJSCIE Z SYSTEMU [38]
	Rect r45;
	r45.set(850,y+90+7,12,12);
	Button b45;
	b45.set(45,r45,"",3,0,GLUT_BITMAP_9_BY_15,2,LEFT);
	b45.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,1.f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	b45.selected = true;
	buttons.push_back(b45);

	//OBA: BOX - REZYGNACJA Z KOLEJKI [39]
	Rect r46;
	r46.set(850,y+110+7,12,12);
	Button b46;
	b46.set(46,r46,"",3,0,GLUT_BITMAP_9_BY_15,2,LEFT);
	b46.set_colors(0.95f,0.95f,0.95f,0.f,0.f,0.f,1.f,1.f,1.f,0.f,0.f,0.f,0.f,0.9f,0.f,0.f,0.4f,0.f,0.f,1.f,0.f,0.f,0.4f,0.f,0.f,0.2f,0.f);
	b46.selected = true;
	buttons.push_back(b46);
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////
	y = 30+sys.SCREEN_HEIGHT/10;
	Label l55;
	l55.set(sys.SCREEN_WIDTH/2,y+glutBitmapHeight(GLUT_BITMAP_9_BY_15),"Aby kontynuwac, nacisnij ENTER",GLUT_BITMAP_9_BY_15,CENTER);
	l55.show_when_menu_type = 3;
	labels.push_back(l55);

}

void Menu::render(){
	
	if(sys.phase==1){
		glColor3f(0.f,0.f,0.f);
		Label l;
		l.set(sys.SCREEN_WIDTH/2,sys.SCREEN_HEIGHT-30,"Aby kontynuwac, nacisnij ENTER",GLUT_BITMAP_9_BY_15,CENTER);
		//cout << sys.SCREEN_WIDTH/2 << ", ";
		l.render();

		glColor3f(1.f,1.f,1.f);
		GLfloat x = ( sys.SCREEN_WIDTH - sys.logo.textureWidth() ) / 2.f;
		GLfloat y = ( sys.SCREEN_HEIGHT - sys.logo.textureHeight() ) / 2.f;
		sys.logo.render( x, y );

		
		
	}else if (sys.phase==2){
		for(unsigned int i =0;i<buttons.size();i++){
			if(sys.menu.type==0){
				if(buttons[i].show_when_menu_type==0){
					buttons[i].render();
				}
			}else if(buttons[i].show_when_menu_type==0 || buttons[i].show_when_menu_type==3 || buttons[i].show_when_menu_type == sys.menu.type){
				buttons[i].render();
			}
		}
		glColor3f(0.f,0.f,0.f);
		for(unsigned int i=0;i<labels.size();i++){
			if(sys.menu.type==0){
				if(labels[i].show_when_menu_type==0){
					labels[i].render();
				}
			}else if(labels[i].show_when_menu_type==0 ||labels[i].show_when_menu_type==3 || labels[i].show_when_menu_type == sys.menu.type){
				labels[i].render();
			}
		}

		for(unsigned int i=0;i<sliders.size();i++){
			if(sys.menu.type==0){
				if(sliders[i].show_when_menu_type==0){
					sliders[i].render();
				}
			}else if(sliders[i].show_when_menu_type==0 ||sliders[i].show_when_menu_type==3 || sliders[i].show_when_menu_type == sys.menu.type){
				sliders[i].render();
			}
		}
	}
}
