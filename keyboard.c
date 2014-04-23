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


/********************* PINOUT **************************************/
/*
		|1 MCLR	PGC 28|	->		       	A1 |1 B   16| 5V
 BCD 	|2 A0	PGD 27|	->			    A2 |2 C	  15| F			G F X A B
 BCD	|3 A1	 B5 26|			       	   |3	  14| G			|		|	
 BCD	|4 A2	 B4 25|	       			   |4	  13| A			|		|
 BCD	|5 A3	 B3 24|	Col3			   |5 RBI 12| B			|		|
		|6 A4	 B2 23| Col2			A3 |6 D	  11| C   		E D X C 0
		|7 A5	 B1 22| Col1			A0 |7 A	  10| D
		|8 GND	 B0 21| Col0		    GND|8 GND  9| E			* 0 means BD			
OSC		|9 		 5V 20|	5v										* X means GND
OSC		|10		    19| GND
Row0	|11 C0	 RX 18|
Row1	|12 C1	 TX 17|
Row2	|13 C2	 C5 16|
Row3	|14 C3	 C4 15|
*/

/* REMEMBER!!
Z & 1100 = XX00
Z | 1100 = 11XX
*/

/* 
 TRISA -> BCD
 TRISC = ROWS
 TRISB = COLUMNS

	C1-C2-C3-C4-R1-R2-R3-R4
	21-22-23-24-11-12-13-14
*/
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