/* 

mastemind.h : Header file for "mastermind.c", and "code.c"

Mastermind solver, based on Donald Knuth's method published in
the Journal of Recreational Mathematics (1976)

Written by Zack Orlaski, Grove City College for COMP 325B
09/08/20

*/


/*Game Rules*/
#define NUM_PEGS 4
#define NUM_COLORS 6
#define MAX_TURNS 5

/*Score colors*/
#define WHITE 1
#define BLACK 0

/*Board Marker*/
#define MARKED -1

/*Booleans for readability*/
#define TRUE 1
#define FALSE 0

/*Size constants*/
#define PERMUTATIONS 1296
#define MAX_DICT_SIZE 256

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*Struct representing a sequence of pegs and a score of [BLACK,WHITE]*/
typedef struct
{

    int sequence[NUM_PEGS]; //sequence of code
    int score[2];            //score [B,W]

} Code;

/*-----------------------------------------------------------*/
/* ComputerAsMastermind: Run "Baby Knuth" Algorithm          */
/* int* master: master code in the sequence to be guessed    */
/* int* initialGuess: 1st guess in the algorithm,            */
/*      reccomended to be [1,1,2,2] by Knuth                 */
/*-----------------------------------------------------------*/
int generateDictionary(int *master, int *initialGuess, Code *dictionary);


/*newCode: create a new code struct and initialize, including the score.   */
/* int* testCode: sequence being scored against, a.k. a. the correct guess */
/* int* dictCode: the sequence being added to the new Code                 */
/*-------------------------------------------------------------------------*/
Code newCode(int *testCode, int *dictCode);

/*----------------------------------------------*/
/*setSequence: Set the sequence of a Code       */
/*Code* input: Code to set the sequence of      */
/*int* sequence: sequence to assign to the code */
/*----------------------------------------------*/
void setSequence(Code *input, int *sequence);

/*---------------------------------------------*/
/* getSequence: get the Sequence of a code     */
/* Code* input: Code to take the sequence from */
/*---------------------------------------------*/
int *getSequence(Code *input);

/*-----------------------------------------------------------*/
/* toString: Assign the data of a Code to a string reference */
/* Code* input: the code being printed                       */
/* char* str: the string being written to                    */
/* WARNING!!! : User must call free() on the *str before     */
/* runtime completion, or memory leaks will occur            */
/*-----------------------------------------------------------*/
void toString(Code *input, char *str);

/*-------------------------------------------------------*/
/* scoreCode: Compute the guess and return a score array */
/*  int* master: the master code  to compare guesses to  */
/*  int* guess: the guess code to score                  */
/*-------------------------------------------------------*/
void scoreCode(int *master, int *guess, int *score);

/*------------------------------------------------------------------*/
/*  compareScore: return true if score sets are equivalent          */
/*                                                                  */
/*  int* score1: first score to compare                             */
/*  int* score2: second score to compare                            */
/*------------------------------------------------------------------*/
int compareScore(int *score1, int *score2);

/*------------------------------------------------------------------*/
/*  tickerCopy: Deep Copy a ticker over to a 2nd ticker             */
/*                                                                  */
/*  int ticker[]: original ticker                                   */
/*  int newTicker: new ticker to copy   to                          */
/*------------------------------------------------------------------*/
void tickerCopy(int ticker[], int newTicker[]);