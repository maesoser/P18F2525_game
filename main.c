
#include "main.h"
 
#pragma code

const rom unsigned char encode[4] = { 0x01,0x02,0x04,0x08};   //  Type of inputs we could receive
const rom unsigned char encodeRows[4] = {0b00000001,0b00001000,0b00010000,0b00100000};
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
	#endif
	
	setSeed(); // Now we obtain the difficulty and the seed for the random generator
	createSequence(); //Fill the sequence with random numbers
	
	#ifdef DEBUG
	showNumber(1);
	showNumber(2);
	showNumber(3);
	showSequence(99);
	#endif
}

void win(){
	while(TRUE) test(2);
}

void lose(){		
	while(TRUE) test(3);
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

//Scan a Column
unsigned char ScanColumn(){
		
		unsigned char i;
		unsigned char column = 0xFF; // column = 11111111
		unsigned char input;
		//								 76543210
		TRISC = TRISC & 0b11000110; // TRISC = XX000XX0 Rows as ouputs
		TRISB = TRISB | 0x0F; // TRISB = XXXX1111 Columns as inputs
		
		PORTC = PORTC | 0b00111001; // PORTC = XXXX1111 Turn on the rows
		input = PORTB & 0x0F; // input = 0000XXXX Capture 0:3 Values
	
		for(i=0;i<4; i++) if (encode[i]==input) column = i;
		return column;
	
	}

// Scan a Row
unsigned char ScanRow(){
			
	unsigned char i;
	unsigned char row = 0xFF;
	unsigned char input;

	TRISC = TRISC | 0b00111001;	// TRISC = XX111XX1 Rows as inputs
	TRISB = TRISB & 0xF0;	// TRISB = XXXX0000 Columns as outputs

	PORTB = PORTB | 0x0F;	// PORTB = XXXX1111 Turn on the rows
	input = PORTC & 0b00111001;	// input = 00XXX00X Capture only the values connected to the rows
	
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
		while(rndnumber == sequence[index-1]){
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
	for(i=0; i<3;i++){
		LATA = number;
		Delay10KTCYx(30);
		LATA = 15;
		Delay10KTCYx(30);	
	}
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
	}	
}

// show the sequence array until index NOT INCLUDED
// Por ejemplo , showSequece(3) -> [0][1][2]
void showSequence(unsigned int index){
	LATA = 10;
	Delay10KTCYx(150);
	
	if(index == 0){
		LATA = sequence[0];
		Delay10KTCYx(100);	
	}
	
	else{	
		for(i=0 ;i < index+1 ;i++ ){
			//less or equal cause in the first iteration we do showSequence(0);
			LATA = sequence[i];
			Delay10KTCYx(100);
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
	if(index ==0){
		key = 0xFF;		
		while(key == 0xFF){
			key = KeyDebounced();
		}	
		if(key != sequence[0]){
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
			while(key == 0xFF){
				key = KeyDebounced();
			}	
			if(key != sequence[n]){
				showNumber(error +1);
				return 1;

			}	
			else {
				printNumber(sequence[n]);
			}		
				
		}
	}
	return 0;
}	
unsigned char randByTime(){
}	
/*
void beep(unsigned char note){
	//   DutyCycle = 4 * (1/8*10^6) * 16 * (scaling value + 1)
	// This is the length of time that the pulse is held high
	
	unsigned int dutyCycle=0;
	dutyCycle=note;
	OpenPWM1(DEFAULT_TIME);
	SetDCPWM1(dutyCycle);	
}
*/