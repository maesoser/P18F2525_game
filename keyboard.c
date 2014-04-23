//	INCLUDES
//#include <p18cxxx.h>
#include <p18f2525.h>
#include <delays.h>
#include <stdlib.h>
#include <stdio.h>

// Crystal
#define _XTAL_FREQ 8000000

//PREDEFINED FUNCTIONS
#define testbit(var, bit)       ((var) & (1 << (bit)))
#define setbit(var, bit)        ((var) |= (1 << (bit)))
#define clearbit(var, bit)      ((var) &= ~(1 << (bit)))

#pragma config OSC = HS
#pragma config WDT = OFF //Watchdog timer off to prevent undesired restarts
#pragma config LVP = OFF //LO voltage programming is off

#pragma code  //to indicate that im going to start my executable code


//FUNCTION PROTOTYPES
unsigned char ScanKeypad(void);
unsigned char ScanRow(void);
unsigned char ScanColumn(void);
unsigned char KeyDebounced(void);
void test(void);

//VARIABLES

const rom unsigned char encode[4] = {0x01,0x02,0x04,0x08};
unsigned char key;
const rom unsigned char keyboard[4][4] = {{1,2,3,12},
									     {4,5,6,13},
									     {7,8,9,14},
									     {10,0,11,15}};


void main()
{   
    ADCON1 = 0x0F; // 00001111 Make all pins digital in PORTA and PORTB. 
    
    //TRISB = 0x0F;  // 00001111 PORTB 0:3 are keypad column input
    //INTCON2 = INTCON2 & 0b01111111;  // INTCON = 0XXXXXXX Turn on the pull ups on PORTB 
    //TRISC = 0x0F; // PORTC 0:3 are keypad row outputs

    //Configure Leds
   TRISA = 0x00;  // 00000000 PORTA connected to BCD
   LATA =  0x00;    // Turn off all segments of 7 segment display

	test();
	
    /*while(1){
		key = ScanKeypad();
		if(key != 0xFF){
			LATA = key;
		}	
    }*/
    while(1){
    LATA = 	rand()%12;
    Delay10KTCYx(600);
    }
}

/*
test()
Input: Nothing
Output: Nothing, just proves that the Display works correctly by displaying a sequence of numbers.
*/
void test(){
	int i=0;
	for(i=0;i<10;i++){
   		LATA = i;
		Delay10KTCYx(60);
	}
	LATA=0;
}
/*
KeyDebounced()
Input: Nothing
Output: Return the character pressed after debouncing
The method of debouncing takes two samples and if the samples are the same, it
returns the value. If not, returns 0xFF
*/
unsigned char KeyDebounced(){
	unsigned char output = 0xFF;
	unsigned char key_before = ScanKeypad();
	unsigned char key_after = ScanKeypad();
	if( key_before == key_after) output = key_after;
	return output;
}

/*
ScanRow()
Input: Nothing
Output: Returns the Row pressed or 0xFF if there is no button pressed
*/

unsigned char ScanRow(){

	unsigned char i;
	unsigned char row = 0xFF;
	unsigned char input;

	TRISC = TRISC | 0x0F; // TRISC = XXXX1111	Rows as inputs
	TRISB = TRISB & 0xF0; // TRISB = XXXX0000	Columns as outputs

	PORTB = PORTB | 0x0F; // PORTC = XXXX1111	Turn on the columns
	input = PORTC & 0x0F; // input = 0000XXXX	Capture only the 0:3 values

	for(i=0; i<4; i++)	if(encode[i]==input) row = i;
	return row;
}

/*
ScanColumn()
Input: Nothing
Output: Returns the Column pressed or 0xFF if there is no button pressed
*/
unsigned char ScanColumn(){

	unsigned char i;
	unsigned char column = 0xFF;
	unsigned char input;

	TRISC = TRISC & 0xF0; // TRISC = XXXX0000	Rows as outputs
	TRISB = TRISB | 0x0F; // TRISB = XXXX1111	Columns as inputs

	PORTC = PORTC | 0x0F; // PORTC = XXXX1111	Turn on the rows 
	input = PORTB & 0x0F; // input = 0000XXXX   Capture the 0:3 values
	
	for(i=0; i<4; i++)	if(encode[i]==input) column = i; ///column decoding
	return column;
}

/*
ScanKeypad()
Input: Nothing
Output: Returns the character pressed or 0xFF if there is no button pressed
*/
unsigned char ScanKeypad(){

	unsigned char key = 0xFF;
	unsigned char column = ScanColumn();
	unsigned char row = ScanRow();
	if(row != 0xFF && column != 0xFF)	key = keyboard[row][column]; ///key decoding
	return key;
}