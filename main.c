
#include "main.h"
 
#pragma code

const rom unsigned int melody[6] = { NOTE_C4, NOTE_G3, NOTE_A3, NOTE_G3, NOTE_B3, NOTE_C4};
const rom unsigned int loseSong[10] = {NOTE_B4,0,NOTE_B4,0,NOTE_B4,NOTE_E4,NOTE_D3,NOTE_B4,NOTE_E4,NOTE_D3};
const rom unsigned int winSong[10] = {NOTE_GS2,0,NOTE_GS2,0,NOTE_GS2,NOTE_GS3,NOTE_GS4,NOTE_GS3,NOTE_G3,NOTE_GS2};
const rom unsigned int keybsound[10] = {400,500,700,740,900,1000,1200,1500,2489,2794};

const rom unsigned char encode[4] = { 0x01,0x02,0x04,0x08};   //  Type of inputs we could receive
const rom unsigned char encodeRows[4] = {0b00000010,0b00001000,0b00010000,0b00100000};
const rom unsigned char keyboard[4][4] = {
{1,2,3,12},
{4,5,6,13},
{7,8,9,14},
{10,0,11,15}
};


unsigned int i;
unsigned char error;
unsigned char key;
unsigned char sequence[MAX_SEQ];
unsigned char pos;


void main(void){
	
	setup();
		
	pos=0;
	error = 0;
	//showSequence(MAX_SEQ);
	while(TRUE){
		if(pos == MAX_SEQ ) win(); // Si ha alcanzado el numero de la serie -> WIN
		if(error == MAX_ERROR) lose();		// Si ha alcanzado el numero de fallos -> LOSE
		else{		// Si no, sigue jugando
			showSequence(pos);
			error = error + checkSequence(pos);
		}
		pos++;
 	
	}
}
	
void setup(){
		
	ADCON1 = 0x0F; // 00001111 Make all pins digital in PORTA and PORTB.
	/*
	TRISB and TRISC are configured each time we run ScanKeypad.
	The pull ups of INTCON2 are not configured cause we dont want pull ups, we use pull downs
	*/
	LATA = 0x00;
	TRISA = 0x00;	
    
	#ifdef DEBUG
	test(9);
	for(i=0; i<10; i++){
		LATA = i;
		play(keybsound[i],SONG_TIME_SHORT);
	}

	#endif
	
	setSeed(); // Now we obtain the difficulty and the seed for the random generator
	createSequence(); //Fill the sequence with random numbers
	
	#ifdef DEBUG
	showSequence(MAX_SEQ-1);
	#endif

	
	for(i=0; i < 7; i++){
		play(melody[i], SONG_TIME_SHORT);
	}
	for(i=0; i < 7; i++){
		play(melody[i], SONG_TIME_SHORT);
	}
	for(i=0; i < 7; i++){
		play(melody[i], SONG_TIME_SHORT);
	}
}

void win(){
		for(i=0; i < 10; i++){
		play(winSong[i], SONG_TIME_SHORT);
	}
	main();
}

void lose(){		
		for(i=0; i < 10; i++){
		play(loseSong[i], SONG_TIME_SHORT);
	}
	main();
}	

// show a regresive sequence starting at "start"	
void test(unsigned char start){
	unsigned char i;
	
	for(i=start ;i>0; i--){
		LATA = i;
		Delay10KTCYx(TEST_TIME);
		
	}
	LATA = 0;

}

unsigned char ScanColumn(){
		
		unsigned char i;
		unsigned char column = 0xFF; // column = 11111111
		unsigned char input;
		//								 76543210
		TRISC = TRISC & 0b11000101; // TRISC = XX000X0X Rows as ouputs
		TRISB = TRISB | 0x0F; // TRISB = XXXX1111 Columns as inputs
		
		PORTC = PORTC | 0b00111010; // PORTC = XX111X1X Turn on the rows
		input = PORTB & 0x0F; // input = 0000XXXX Capture 0:3 Values
	
		for(i=0;i<4; i++) if (encode[i]==input) column = i;
		return column;
	
	}

// Scan a Row
unsigned char ScanRow(){
			
	unsigned char i;
	unsigned char row = 0xFF;
	unsigned char input;

	TRISC = TRISC | 0b00111010;	// TRISC = XX111X1X Rows as inputs
	TRISB = TRISB & 0xF0;	// TRISB = XXXX0000 Columns as outputs

	PORTB = PORTB | 0x0F;	// PORTB = XXXX1111 Turn on the rows
	input = PORTC & 0b00111010;	// input = 00XXX0X0 Capture only the values connected to the rows
	
	for(i=0; i<4; i++) if(encodeRows[i] == input) row=i;
	return row; // row is the number of the row
	
}

//Returns the character pressed
unsigned char ScanKeypad(){
		
	unsigned char key = 0xFF;
	unsigned char column = ScanColumn();
	unsigned char row = ScanRow();
	
	if(row != 0xFF && column != 0xFF) key = keyboard[row][column];
	return key;
}

// Returns the character pressed DEBOUNCED
unsigned char KeyDebounced(){
		
	unsigned char value = 0xFF;
	unsigned char key = ScanKeypad();
	Delay10KTCYx(DEBOUNCE_TIMEOUT);
	if (key==ScanKeypad()) value = key;
	
	return value;
}

//Generates a Random number which is not the same as the previous number generated		
unsigned char generateRnd(unsigned int index){
	
	unsigned char rndnumber = 0;
	
	if(index == 0) rndnumber = rand()%MAX_RANDOM_NUMBER-1;
	else{
		while(rndnumber == sequence[index-1] || rndnumber < MIN_RANDOM_NUMBER || rndnumber > MAX_RANDOM_NUMBER ){
			rndnumber = rand()%MAX_RANDOM_NUMBER-1;
		}	
	}
	return rndnumber;
}	

// Ask the user for a seed
void setSeed(){

	unsigned char seed_final = FALSE;
	unsigned int seed = 1;
	
	while(seed_final == FALSE){ // while seed is not set
		if(seed > 256) seed = 1;
			key = KeyDebounced();
			if(key != 0xFF) { //if some key is pressed
				srand(seed); // set the random seed
				seed_final = TRUE; //exit the while
			}	
		seed++; 
	}
}

// Prints a blinking number
void printNumber(unsigned char number){
	LATA = number;
	play(keybsound[number],SONG_TIME_SHORT*2);
	/*for(i=0; i<3;i++){
		LATA = number;
		play(keybsound[number],2);
		//Delay10KTCYx(30);
		LATA = 15;
		Delay10KTCYx(30);	
	}*/
	LATA = 15;
}

// Show a number with some fx
void showNumber(unsigned char number){
	for(i=0; i<3;i++){
	LATA = 10;
		Delay10KTCYx(10);
		LATA = 11;
		Delay10KTCYx(10);
		LATA = 12;
		Delay10KTCYx(10);
	}
	printNumber(number);
		for(i=0; i<3;i++){
	LATA = 10;
		Delay10KTCYx(10);
		LATA = 11;
		Delay10KTCYx(10);
		LATA = 12;
		Delay10KTCYx(10);
	}
	LATA = EMPTY_LED;
}

// fill the sequence array with random numbers	
void createSequence(){
	for(i=0; i< MAX_SEQ; i++){
		sequence[i] = generateRnd(i);
		//sequence[i] = randByTime(i);
	}	
}

// show the sequence array until index NOT INCLUDED
// Por ejemplo , showSequece(3) -> [0][1][2]
void showSequence(unsigned int index){
	LATA = 10;
	Delay10KTCYx(150);
	if(index == 0){
		LATA = sequence[0];
		play(keybsound[sequence[0]],SONG_TIME_LARGE);
		//Delay10KTCYx(100);	
	}
	
	else{	
		for(i=0 ;i < index+1 ;i++ ){
			//less or equal cause in the first iteration we do showSequence(0);
			LATA = sequence[i];
			play(keybsound[sequence[i]],SONG_TIME_LARGE);
			//Delay10KTCYx(100);
		}
	}
	LATA = 11;
	Delay10KTCYx(150);

	LATA = EMPTY_LED;	
	
}

// check the sequence array until index
unsigned char checkSequence(unsigned int index){
	unsigned char err = 0;
	unsigned char n = 0;   // variable to traverse the array
	unsigned char diff = 0;
	if(index > 3) diff =1;
	if(index > 5) diff =2;
	if(index > 3) diff =3;
	if(index ==0){
		unsigned int t = 0;
		key = 0xFF;		
		while(key == 0xFF){	 
			key = KeyDebounced();
		}	
		if(key != sequence[0]){
			play(NOTE_B3,SONG_TIME_SHORT-diff);
			showNumber(error +1);
			return 1;
		}	
		else {
			printNumber(sequence[0]);
		}
	}
	else{	
		for(n=0 ;n < index+1 ;n++ ){ // Por toda la sequencia que esta llena
			key = 0xFF;
			OpenTimer0(TIMER_INT_OFF & T0_16BIT & T0_SOURCE_INT & T0_PS_1_128);
			WriteTimer0(0x48E5);	
			while(TRUE){
	 			if( key != 0xFF){
		 			break;
		 		}
	 			if(INTCONbits.TMR0IF==1){
		 			 break;
		 		}	 
				key = KeyDebounced();
			}
				
			if(key != sequence[n]){
				INTCONbits.TMR0IF=0;
				play(NOTE_B3,SONG_TIME_SHORT);
				showNumber(error +1);
				CloseTimer0();
				return 1;

			}
			if(INTCONbits.TMR0IF==1){
				
				INTCONbits.TMR0IF=0; 
				play(NOTE_B3,SONG_TIME_SHORT);
				showNumber(error +1);
				CloseTimer0();
				return 1;

			}
				
			else {
				printNumber(sequence[n]);
			}		
				
		}
	}
	return 0;
}
	
void delayTime(unsigned char t){
	
	/*
	Tu = 4*Tclk  is 500 nsec per cycle
	we have to measure 25.000.000 nsec which is 50000 instruction
	500.000 /8 (preescaled) is 62.500 , which is inside the t1 maximum (65.536)
	but for some strange reason it measures 1/2 seconds
	*/
	
	unsigned int aux = 0;
	OpenTimer3(TIMER_INT_OFF & T3_16BIT_RW & T3_SOURCE_INT & T3_PS_1_8);
	WriteTimer3(0);
	while(aux != t){
		if((unsigned int) ReadTimer3() == 15625){
			WriteTimer3(0);
			aux ++;	
		}
	}
	CloseTimer3();
/*	unsigned int aux = 0;
	OpenTimer3(TIMER_INT_OFF & T3_16BIT_RW & T3_SOURCE_INT & T3_PS_1_4);
	WriteTimer3(65536-50000);
	PIR2bits.TMR3IF==0;
	while(aux != t*10){
		if(PIR2bits.TMR3IF==1){
			PIR2bits.TMR3IF==0;
			aux++;
			WriteTimer3(65536-50000);
		
		}
	}
	CloseTimer3();	*/
}

void StartTone(unsigned char note, unsigned char dc){
	OpenTimer2(TIMER_INT_OFF & T2_PS_1_16 & T2_POST_1_1);
		OpenPWM1(note);
		//freq = (note+1)*4*(1/8000000)*16
		SetDCPWM1(dc);
}

void StopTone(){
	ClosePWM1();
	CloseTimer2();
}
void play_music(unsigned char note, unsigned char dc,unsigned char time){
	StartTone(note,dc);
	delayTime(time);
	StopTone();
}					
void play(unsigned char note,unsigned char time){
		StartTone(note,220);
		delayTime(time);
		StopTone();
}

