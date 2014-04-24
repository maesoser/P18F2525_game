#include <p18f2525.h>
#include <delays.h>
#include <stdlib.h>

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF

//#define DEBUG

#define MAX_SEQ 100
#define MAX_ERRORS 3
#define MAX_RANDOM_NUMBER 9
#define TRUE 1
#define FALSE 0
#define EMPTY_LED 15

#define DEBOUNCE_TIMEOUT 10
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

