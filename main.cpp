#include "calc.hpp"

#ifndef PC
	APP_NAME("CPsudoku")
	APP_DESCRIPTION("Sudoku game & solver. To start a new game: Random > 75\% > Set (Use 'Keyboard' to go to the menu)")
	APP_AUTHOR("SnailMath")
	APP_VERSION("1.0.0")
#endif

void square(int x, int y, int w, int h, uint16_t c){
  for (int ix = 0; ix<w ; ix++)
    for (int iy = 0; iy<h ; iy++)
      //setPixel takes care to check if the dot is on the screen.
      setPixel(x+ix,y+iy,c);
}

//draw a 7-segment display with the segments in n lit up
void segment(uint8_t n, int x, int y, int w, int h, uint16_t c){
	/*A*/ if(n&0b10000000) square(x,   y,     w+h, h,   c);
	/*B*/ if(n&0b01000000) square(x+w, y,     h,   w+h, c);
	/*C*/ if(n&0b00100000) square(x+w, y+w,   h,   w+h, c);
	/*D*/ if(n&0b00010000) square(x,   y+2*w, w+h, h,   c);
	/*E*/ if(n&0b00001000) square(x,   y+w,   h,   w+h, c);
	/*F*/ if(n&0b00000100) square(x,   y,     h,   w+h, c);
	/*G*/ if(n&0b00000010) square(x,   y+w,   w+h, h,   c);
}

//draw a 1 digit number on the screen
void digit(uint8_t n, int x, int y, int w, int h, uint16_t c){
	if(n>='a') n-=0x20;
	switch (n) {
		case '0': case 0:		segment(0b11111100, x, y, w, h, c); break;
		case '1': case 1:		segment(0b01100000, x, y, w, h, c); break;
		case '2': case 2:		segment(0b11011010, x, y, w, h, c); break;
		case '3': case 3:		segment(0b11110010, x, y, w, h, c); break;
		case '4': case 4:		segment(0b01100110, x, y, w, h, c); break;
		case '5': case 5:  case 'S':	segment(0b10110110, x, y, w, h, c); break;
		case '6': case 6:		segment(0b10111110, x, y, w, h, c); break;
		case '7': case 7:		segment(0b11100100, x, y, w, h, c); break;
		case '8': case 8:		segment(0b11111110, x, y, w, h, c); break;
		case '9': case 9:		segment(0b11110110, x, y, w, h, c); break;
		case 'A': case 10: 		segment(0b11101110, x, y, w, h, c); break;
		case 'B': case 11: 		segment(0b00111110, x, y, w, h, c); break;
		case 'C': case 12: 		segment(0b10011100, x, y, w, h, c); break;
		case 'D': case 13: 		segment(0b01111010, x, y, w, h, c); break;
		case 'E': case 14: 		segment(0b10011110, x, y, w, h, c); break;
		case 'F': case 15: 		segment(0b10001110, x, y, w, h, c); break;
		case 'G': 			segment(0b10111100, x, y, w, h, c); break;
		case 'H': 			segment(0b01101110, x, y, w, h, c); break;
		case 'I': 			segment(0b00001100, x, y, w, h, c); break;
		case 'J': 			segment(0b01110000, x, y, w, h, c); break;
		case 'K': 			segment(0b00101100, x, y, w, h, c); break;
		case 'L': 			segment(0b00011100, x, y, w, h, c); break;
		case 'M': 			segment(0b11101100, x, y, w, h, c); break;
		case 'N': 			segment(0b00101010, x, y, w, h, c); break;
		case 'O': 			segment(0b00111010, x, y, w, h, c); break;
		case 'P': 			segment(0b11001110, x, y, w, h, c); break;
		case 'Q': 			segment(0b11100110, x, y, w, h, c); break;
		case 'R': 			segment(0b00001010, x, y, w, h, c); break;
		case 'T': 			segment(0b00011110, x, y, w, h, c); break;
		case 'U': 			segment(0b01111100, x, y, w, h, c); break;
		case 'V': 			segment(0b00111000, x, y, w, h, c); break;
		case 'W': 			segment(0b01111110, x, y, w, h, c); break;
		case 'X': 			segment(0b00110010, x, y, w, h, c); break;
		case 'Y': 			segment(0b01110110, x, y, w, h, c); break;
		case 'Z': 			segment(0b11011010, x, y, w, h, c); break;
	}
}

void digitprint(const char* s, int x, int y, int w, int h, uint16_t c){
	int i = 0;
	while(s[i]!=0){ digit(s[i],x+(i*2*w),y,w,h,c); i++;}
}

uint16_t sudoku[9*9];
bool given[9*9];
uint16_t error[3];
bool showerror;
int selected;
uint8_t sudoku2[9*9];
int mode;
#define MODE_EDIT	0
#define MODE_MENU	1
#define MODES		2
int menuselected;
#define MENU_CLEAR	0
#define	MENU_DEFAULT	1
#define	MENU_RANDOM	2
#define MENU_75		3
#define	MENU_SET 	4
#define	MENU_CHECK  	5
#define	MENU_SOLVE_1 	6
#define MENUITEMS	7

uint8_t su_val(uint16_t sudoku){
	for(int i=0;i<9;i++) if(sudoku==(1<<i)) return i+1;
	return 0;
}

void sudoku_draw(){
	fillScreen(color(255,255,255));
	if(showerror){ //draw the errors
		square(6,6,308,308,color(128,255,128));
		for(int iy=0;iy<3;iy++)for(int jy=0;jy<3;jy++) if(error[0]&1<<(iy*3+jy)) square(6,6+(iy*104)+(jy*34),308,32,color(255,128,128));
		for(int ix=0;ix<3;ix++)for(int jx=0;jx<3;jx++) if(error[1]&1<<(ix*3+jx)) square(6+(ix*104)+(jx*34),6,32,308,color(255,128,128));
		for(int  x=0; x<3; x++)for(int  y=0; y<3; y++) if(error[2]&1<<( y*3+ x)) square(6+(x*104),6+(y*104),100,100,color(255,128,128));
	} //the thick lines (4px) //each field is 104 wide (100 white and the 4px border) //the thin lines (2px) (32px white)
	square(  2,  2,316,  4, 0);square(  2, 38,316,  2, 0);square(  2, 72,316,  2, 0);square(  2,106,316,  4, 0);square(  2,142,316,  2, 0);
	square(  2,176,316,  2, 0);square(  2,210,316,  4, 0);square(  2,246,316,  2, 0);square(  2,280,316,  2, 0);square(  2,314,316,  4, 0);
	square(  2,  2,  4,316, 0);square( 38,  2,  2,316, 0);square( 72,  2,  2,316, 0);square(106,  2,  4,316, 0);square(142,  2,  2,316, 0);
	square(176,  2,  2,316, 0);square(210,  2,  4,316, 0);square(246,  2,  2,316, 0);square(280,  2,  2,316, 0);square(314,  2,  4,316, 0);
	for(int iy=0; iy<3; iy++)for(int ix=0; ix<3; ix++)
		for(int jy=0; jy<3; jy++)for(int jx=0; jx<3; jx++){
			int i=jx+(ix*3)+(jy*9)+(iy*27);
			if(i==selected&&mode==MODE_EDIT)
				square(6+(ix*104)+(jx*34),6+(iy*104)+(jy*34),32,32,color(255,128,128));
			uint8_t result = su_val(sudoku[i]);
			if(result){
				digit(result,6+8+(ix*104)+(jx*34),6+4+(iy*104)+(jy*34),10,5,given[i]?color(0,0,128):color(0,0,0));
			}else{
				int j=1;
				for(int ky=0; ky<3; ky++)for(int kx=0; kx<3; kx++){
					if(sudoku[i]&j)
						digit(kx+ky*3+1,6+2+(ix*104)+(jx*34)+(kx*11),6+2+(iy*104)+(jy*34)+(ky*11),3,1,color(0,0,0));
					j=j<<1;
				}
			}
			i++;
		}
	int line=0;
	digitprint("clear"	, 30, 330+(line++*18), 6, 3, 0);
	digitprint("default"	, 30, 330+(line++*18), 6, 3, 0);
	digitprint("random"	, 30, 330+(line++*18), 6, 3, 0);
	digitprint("75 prozent"	, 30, 330+(line++*18), 6, 3, 0);
	digitprint("set"	, 30, 330+(line++*18), 6, 3, 0);
	digitprint("check"	, 30, 330+(line++*18), 6, 3, 0);
	digitprint("solve 1"	, 30, 330+(line++*18), 6, 3, 0);
	if(mode==MODE_MENU) triangle(5,330+2+(menuselected*18), 5+20,330+9+(menuselected*18), 5,330+16+(menuselected*18), color(255,0,0),color(255,0,0));
	LCD_Refresh();
}


//RNG inspired by https://github.com/Qqwy/SimpleRNG/blob/master/c/simple_rng.c
uint32_t randomInt = 1;
uint32_t nextRand(){
	randomInt^=randomInt<<13;
	randomInt^=randomInt>>17;
	randomInt^=randomInt<< 5;
	return randomInt;
}

uint32_t randInt(uint32_t max){
	//create a mask to create the lowest power of 2 that is bigger or equal than max
	uint32_t mask = 0xffffffff;
	uint32_t shift = max-1;
	while(shift){
		shift = shift>>1;
		mask = mask<<1;
	}
	mask = ~mask;
	//use this mask to generate random numbers until we get one in the desired region.
	uint32_t number;
	do{
		number = mask&nextRand(); //get a random number
	}while(number>=max);		//repeat if it is not smaller than max (e.g max=3 gives you 0, 1 or 2)
	return number;
}

//su_subs
void su_clear(){
	for(int i=0; i<9*9 ; sudoku[i++]=0);
	for(int i=0; i<9*9 ; given[i++]=0);
	for(int i=0; i<3 ; error[i++]=0);
}



void su_default(){
	//for(int i=0; i<9*9 ; given[i++]=1);
	for(int k=0;k<3;k++)for(int j=0;j<3;j++)
	for(int i=0;i<9;i++)sudoku[i+9*j+k*9*3]=1<<(k+i+(3*j))%9;
}
void su_random(){
	//create conversion table
	uint8_t map[9];
	for(int i=0;i<9;i++)map[i]=i;
	//for(int i=0;i<9;i++){printf("%d ",map[i]);}printf("\n");
	//move around items of the table
	for(int i=8;i>=0;i--){ //from 8 to 0
		int r = randInt(i+1); //generate a random number to change the current number with (this needs 1-9, not 0-8)
		uint8_t x = map[i]; map[i]=map[r]; map[r]=x;
	}
	//for(int i=0;i<9;i++){printf("%d ",map[i]);}printf("\n");

	//create default field with the conversion table
	for(int k=0;k<3;k++)for(int j=0;j<3;j++)
	for(int i=0;i<9;i++)sudoku[i+9*j+k*9*3]=1<<map[(k+i+(3*j))%9];
	
	//switch rows
	for(int j=0;j<3;j++){
		//switch 1-2 or 1-3 or none
		int r = 3;
		while(r>2) r = nextRand() >> 5 & 0b11; //random number 0-2
		if(r)for(int i=0;i<9;i++){
			int x = sudoku[i+((j*3)*9)];
			sudoku[i+((j*3)*9)] = sudoku[i+((j*3+r)*9)];
			sudoku[i+((j*3+r)*9)] = x;
		}
		//switch 2-3 or none
		r = nextRand() >> 5 & 0b1; //random number 0-1
		if(r)for(int i=0;i<9;i++){
			int x = sudoku[i+((j*3+1)*9)];
			sudoku[i+((j*3+1)*9)] = sudoku[i+((j*3+2)*9)];
			sudoku[i+((j*3+2)*9)] = x;
		}
	}
	//switch columns
	for(int j=0;j<3;j++){
		//switch 1-2 or 1-3 or none
		int r = 3;
		while(r>2) r = nextRand() >> 5 & 0b11; //random number 0-2
		if(r)for(int i=0;i<9;i++){
			int x                 = sudoku[(9*i)+(j*3)+0];
			sudoku[(9*i)+(j*3)+0] = sudoku[(9*i)+(j*3)+r];
			sudoku[(9*i)+(j*3)+r] = x;
		}
		//switch 2-3 or none
		r = nextRand() >> 5 & 0b1; //random number 0-1
		if(r)for(int i=0;i<9;i++){
			int x                 = sudoku[(9*i)+(j*3)+1];
			sudoku[(9*i)+(j*3)+1] = sudoku[(9*i)+(j*3)+2];
			sudoku[(9*i)+(j*3)+2] = x;
		}
	}
	//switch blocks rows
	{
		//switch 1-2 or 1-3 or none
		int r = 3;
		while(r>2) r = nextRand() >> 5 & 0b11; //random number 0-2
		if(r)for(int i=0;i<9*3;i++){
			int x = sudoku[i];
			sudoku[i] = sudoku[i+(9*3*r)];
			sudoku[i+(9*3*r)] = x;
		}
		//switch 2-3 or none
		r = nextRand() >> 5 & 0b1; //random number 0-1
		if(r)for(int i=0;i<9*3;i++){
			int x = sudoku[i+(9*3*1)];
			sudoku[i+(9*3*1)] = sudoku[i+(9*3*2)];
			sudoku[i+(9*3*2)] = x;
		}
	}
	//switch blocks columns
	{
		//switch 1-2 or 1-3 or none
		int r = 3;
		while(r>2) r = nextRand() >> 5 & 0b11; //random number 0-2
		if(r)for(int i=0;i<9;i++)for(int j=0;j<3;j++){
			int x = sudoku[j+i*9+3*0];
			sudoku[j+i*9+3*0] = sudoku[j+i*9+3*r];
			sudoku[j+i*9+3*r] = x;
		}
		//switch 2-3 or none
		r = nextRand() >> 5 & 0b1; //random number 0-1
		if(r)for(int i=0;i<9;i++)for(int j=0;j<3;j++){
			int x = sudoku[j+i*9+3*1];
			sudoku[j+i*9+3*1] = sudoku[j+i*9+3*2];
			sudoku[j+i*9+3*2] = x;
		}
	}
}

void su_75(){
	for(int i=0;i<9*9;i++)
		if((nextRand() >> 5 & 0b11 ) == 0) sudoku[i]=0,given[i]=0;
}

void su_check(){
	if(showerror){
		showerror=false;
	}else{
		for(int i=0; i<3 ; error[i++]=0);
		for(int i=0;i<9;i++){
			int sum = 0;
			for(int j=0;j<9;j++) sum+=sudoku[9*i+j];
			if(sum!=0b111111111) error[0]|=1<<i;
		}
		for(int i=0;i<9;i++){
			int sum = 0;
			for(int j=0;j<9;j++) sum+=sudoku[9*j+i];
			if(sum!=0b111111111) error[1]|=1<<i;
		}
		for(int iy=0;iy<3;iy++)for(int ix=0;ix<3;ix++){
			int sum = 0;
			for(int jy=0;jy<3;jy++)for(int jx=0;jx<3;jx++) sum+=sudoku[(ix*3+jx)+(iy*3+jy)*9];
			if(sum!=0b111111111) error[2]|=1<<(ix+3*iy);
		}
		showerror=true;
	}
}

void su_set(){
	for(int i=0; i<9*9 ; given[i++]=0);
	for(int i=0; i<9*9;i++) if(su_val(sudoku[i]))given[i]=1;
}

void su_solve_pre(){
	//replace unknown fields with all selected (0b111111111)
	for(int i=0;i<9*9;i++){
		uint16_t su = su_val(sudoku[i]);
		if(0==su)sudoku[i]=0b111111111;
		sudoku2[i]=su;
	}
}
void su_solve1(){
	su_solve_pre();
	//ehm... now I have to write code that solves sudokus... I didn't expect to get this far...
	//for every square check if it is not solved first
	int i=0;
	for(int iy=0;iy<9;iy++)for(int ix=0;ix<9;i++,ix++)if(su_val(sudoku[i])==0){
		//Check for every unsolved field if a number is already in another field
		int bx  = ix/3;
		int by  = iy/3;
		//waagerecht
		for(int x=0;x<9;x++)
			sudoku[i]&=~(1<<(sudoku2[iy*9+x]-1));
		//senkrecht
		for(int y=0;y<9;y++)
			sudoku[i]&=~(1<<(sudoku2[y*9+ix]-1));
		//feld
		for(int x=0;x<3;x++)for(int y=0;y<3;y++)
			sudoku[i]&=~(1<<(sudoku2[bx*3+x+(by*3+y)*9]-1));
		sudoku2[i] = su_val(sudoku[i]);

	}
}

//The acutal main
void main2(){

	for(int i=0; i<9*9 ; sudoku[i++]=0);
	for(int i=0; i<3 ; error[i++]=0);
	showerror=false;
	//sudoku[6+(1*9)]=0b111111111;
	selected = 0;
	mode = 0;
	menuselected = 0;


	uint32_t key1, key2; 
	bool pressed = false;
	while(true){
		sudoku_draw();
		getKey(&key1, &key2);
		if(pressed){
			if(key1==0&&key2==0)pressed=false;
		}else{
			if(key1||key2)pressed=true;
			if(testKey(key1, key2, KEY_CLEAR))break;
			//if(testKey(key1, key2, KEY_EXE  ))break;
			if(testKey(key1, key2, KEY_KEYBOARD  )){mode++;if(mode>=MODES)mode=0;showerror=false;}
			if(mode==MODE_EDIT){
				if(testKey(key1, key2, KEY_RIGHT))	selected++;
				if(testKey(key1, key2, KEY_LEFT))	selected--;
				if(testKey(key1, key2, KEY_DOWN))	selected+=9;
				if(testKey(key1, key2, KEY_UP))		selected-=9;
				if(selected<0)				selected+=81;
				if(selected>=81)			selected-=81;
				if(given[selected]==0){
					if(testKey(key1, key2, KEY_1))sudoku[selected]^=1<<0;
					if(testKey(key1, key2, KEY_2))sudoku[selected]^=1<<1;
					if(testKey(key1, key2, KEY_3))sudoku[selected]^=1<<2;
					if(testKey(key1, key2, KEY_4))sudoku[selected]^=1<<3;
					if(testKey(key1, key2, KEY_5))sudoku[selected]^=1<<4;
					if(testKey(key1, key2, KEY_6))sudoku[selected]^=1<<5;
					if(testKey(key1, key2, KEY_7))sudoku[selected]^=1<<6;
					if(testKey(key1, key2, KEY_8))sudoku[selected]^=1<<7;
					if(testKey(key1, key2, KEY_9))sudoku[selected]^=1<<8;}
			}else if(mode==MODE_MENU){
				if(testKey(key1, key2, KEY_DOWN)) if(menuselected<MENUITEMS-1)	menuselected++;
				if(testKey(key1, key2, KEY_UP))   if(menuselected>0)		menuselected--;
				if(testKey(key1, key2, KEY_EXE  )){
					if(menuselected==MENU_CLEAR){su_clear();
					}else if(menuselected==MENU_DEFAULT){su_default();
					}else if(menuselected==MENU_RANDOM){su_random();
					}else if(menuselected==MENU_75){su_75();
					}else if(menuselected==MENU_SET){su_set();
					}else if(menuselected==MENU_SOLVE_1){su_solve1();
					}else if(menuselected==MENU_CHECK){su_check();
					}
				}
			}
		}
	}
}
