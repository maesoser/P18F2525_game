#include <p18f2525.h>
#include <delays.h>
#include <stdlib.h>
#include <timers.h>

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

//#define DEBUG

#define MAX_SEQ 5 //length of the sequence
#define MAX_ERROR 3 //maximum number of errors
#define MAX_RANDOM_NUMBER 9 // max range of random number

#define TRUE 1
#define FALSE 0
#define EMPTY_LED 15

#define DEBOUNCE_TIMEOUT 10 //time of debouncing
#define GAME_TIMEOUT 30
#define TEST_TIME 40


// ------------------ PROTOTYPES ----------------------------------

void setup(void);
void test(unsigned char start);

unsigned char KeyDebounced(void);
unsigned char ScanKeypad(void);
unsigned char ScanRow(void);
unsigned char ScanColumn(void);


void win(void);
void lose(void);

void showSequence(unsigned int index);
unsigned char checkSequence(unsigned int index);
void createSequence(void);

void showNumber(unsigned char number);
void printNumber(unsigned char number);
unsigned char generateRnd(unsigned int index);
void setSeed(void);

unsigned char randByTime(void);
