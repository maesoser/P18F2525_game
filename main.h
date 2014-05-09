#include <p18f2525.h>
#include <delays.h>
#include <stdlib.h>
#include <timers.h>
#include <pwm.h>

#pragma config OSC = HS
#pragma config WDT = OFF
#pragma config LVP = OFF
//#define DEBUG

#define MAX_SEQ 60 //length of the sequence
#define MAX_ERROR 3 //maximum number of errors
#define MAX_RANDOM_NUMBER 9 // max range of random number
#define MIN_RANDOM_NUMBER 1

#define TRUE 1
#define FALSE 0
#define EMPTY_LED 15

#define DEBOUNCE_TIMEOUT 10 //time of debouncing
#define GAME_TIMEOUT 30
#define TEST_TIME 40

#define SONG_TIME_LARGE 3
#define SONG_TIME_SHORT 2

#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

// ------------------ PROTOTYPES ----------------------------------

void setup(void);
void test(unsigned char start);

unsigned char KeyDebounced(void);
unsigned char ScanKeypad(void);
unsigned char ScanRow(void);
unsigned char ScanColumn(void);

unsigned char randByTime(void);
void play(unsigned char note,unsigned char time);
void delayTime(unsigned char t);
void StartTone(unsigned char note, unsigned char dc);
void StopTone();


void win(void);
void lose(void);

void showSequence(unsigned int index);
unsigned char checkSequence(unsigned int index);
void createSequence(void);

void showNumber(unsigned char number);
void printNumber(unsigned char number);
unsigned char generateRnd(unsigned int index);
void setSeed(void);



