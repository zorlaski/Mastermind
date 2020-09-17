/* 

code.c : Contains methods for Code, an ADT defined in header file "mastermind.h", 

Mastermind solver, based on Donald Knuth's method published in
the Journal of Recreational Mathematics (1976)

Written by Zack Orlaski, Grove City College for COMP 325B
09/08/20

*/

#include "mastermind.h"

/*-------------------------------------------------------------------------*/
/*newCode: create a new code struct and initialize, including the score.   */
/* int* testCode: sequence being scored against, a.k. a. the correct guess */
/* int* dictCode: the sequence being added to the new Code                 */
/*-------------------------------------------------------------------------*/
Code newCode(int *testCode, int *dictCode)
{
    Code tempCode;

    //initialize array
    setSequence(&tempCode, dictCode);

    //copy over score
    scoreCode(testCode, dictCode, tempCode.score);

    return tempCode;
}

/*----------------------------------------------*/
/*setSequence: Set the sequence of a Code       */
/*Code* input: Code to set the sequence of      */
/*int* sequence: sequence to assign to the code */
/*----------------------------------------------*/
void setSequence(Code *input, int *sequence)
{

    for (int tile = 0; tile < NUM_PEGS; tile++)
        input->sequence[tile] = sequence[tile];
}

/*---------------------------------------------*/
/* getSequence: get the Sequence of a code     */
/* Code* input: Code to take the sequence from */
/*---------------------------------------------*/
int *getSequence(Code *input)
{
    return input->sequence;
}

/*-----------------------------------------------------------*/
/* toString: Assign the data of a Code to a string reference */
/* Code* input: the code being printed                       */
/* char* str: the string being written to                    */
/* WARNING!!! : User must call free() on the *str before     */
/* runtime completion, or memory leaks will occur            */
/*-----------------------------------------------------------*/
void toString(Code *input, char *str)
{

    //Start new string
    sprintf(str, "Code: -");

    for (int i = 0; i < NUM_PEGS; i++)
    {

        char *tempString = (char *)malloc(sizeof(char));

        //turn int to string
        sprintf(tempString, "%d-", input->sequence[i]);

        //add int-string to sequence
        strcat(str, tempString);
        free(tempString);
    }

    char *tempString = (char *)malloc(sizeof(char));

    //Add score to string
    sprintf(tempString, "\nBlack:\t%2d\nWhite:\t%2d\n", input->score[0], input->score[1]);
    strcat(str, tempString);

    free(tempString);

    //done
}

/*-------------------------------------------------------*/
/* scoreCode: Compute the guess and return a score array */
/*  int* master: the master code  to compare guesses to  */
/*  int* guess: the guess code to score                  */
/*-------------------------------------------------------*/
void scoreCode(int *master, int *guess, int *score)
{

    //Scores are [Black,White]
    score[BLACK] = 0;
    score[WHITE] = 0;

    int tile;               //tile index
    int whiteFound = FALSE; //white found indicator

    /*copy master and guess into locally scoped arrays so a tile isnt counted twice*/

    int masterCopy[NUM_PEGS];
    int guessCopy[NUM_PEGS];
    //deep copy
    for (tile = 0; tile < NUM_PEGS; tile++)
    {
        masterCopy[tile] = master[tile];
        guessCopy[tile] = guess[tile];
    }

    /*Black Scoring*/
    for (tile = 0; tile < NUM_PEGS; tile++)
    {

        //Look for Black
        if (guessCopy[tile] == masterCopy[tile])
        {
            ++score[BLACK];            //Black Awarded
            masterCopy[tile] = MARKED; //master Board Marked
            guessCopy[tile] = MARKED;  //master Board Marked

        } //if (looking for black)
    }     //black

    tile = 0;

    /*White Scoring*/
    for (tile = 0; tile < NUM_PEGS; tile++)
    {

        int masterTile = 0; //For checking each code num for white points

        while ((guessCopy[tile] != MARKED) && masterTile < NUM_PEGS)
        {

            if (guessCopy[tile] == masterCopy[masterTile])
            {
                ++score[WHITE];                  //White awarded
                masterCopy[masterTile] = MARKED; //master Board Marked
                guessCopy[tile] = MARKED;        //Guess Board Marked
            }

            masterTile++;
        } //while(masterTile < NUM_PEGS)

    } //white
}

/*------------------------------------------------------------------*/
/*  generateDictionary: Create a code dictionary, minimized using   */
/*  the first guess ([1,1,2,2] is the most efficient.)              */
/*                                                                  */
/*  int* master: the master code to compare guesses to              */
/*  int* initialGuess: the initial guess code to score              */
/*------------------------------------------------------------------*/
int generateDictionary(int *master, int *initialGuess, Code *dictionary)
{
    Code tickerCode;                         //current guess Code
    int ticker[NUM_PEGS] = {1, 1, 1, 1};    //"odometer" like ticker that creates the permutations
    int newTicker[NUM_PEGS] = {1, 1, 1, 1}; //copy of ticker
    int tickIndex,                           //Position within the ticker
        itCount,                             //number of sequences generated so far
        dictCount;                           //number of words containted within dictionary

    tickIndex = itCount = dictCount = 0;

    /***************************************************/

    //!MAKE FIRST GUESS
    Code initial = newCode(master, initialGuess);

    if (initial.score[0] == NUM_PEGS) //Check if found
    {
        dictionary[0] = initial; //only 1st Code needed then
    }

    else //not found
    {

        tickerCopy(ticker, newTicker); //copy ticker

        //Generate codes
        tickerCode = newCode(initialGuess, newTicker);

        //if same score, add to dictionary
        if (compareScore(tickerCode.score, initial.score))
            dictionary[dictCount++] = tickerCode;

        itCount++; //update itCount

        //run for all permutations
        while (itCount < PERMUTATIONS)
        {

            //Increment current ticker number if less than max num
            if (ticker[tickIndex] < NUM_COLORS)
            {

                ++ticker[tickIndex];           //increment
                tickerCopy(ticker, newTicker); //copy ticker

                //create new code from sequence
                tickerCode = newCode(initialGuess, newTicker);

                //add codes with matching scores to first code to dictionary
                if (compareScore(tickerCode.score, initial.score))
                    dictionary[dictCount++] = tickerCode;

                itCount++; //update itCount

                tickIndex = 0;
            }

            else
            { //shift right and increment
                ticker[tickIndex] = 1;
                tickIndex++;
            }
        }
    }
    return dictCount;
}

/*------------------------------------------------------------------*/
/*  compareScore: return true if score sets are equivalent          */
/*                                                                  */
/*  int* score1: first score to compare                             */
/*  int* score2: second score to compare                            */
/*------------------------------------------------------------------*/
int compareScore(int *score1, int *score2)
{
    return (
        score1[0] == score2[0] &&
        score1[1] == score2[1]
        );
}

/*------------------------------------------------------------------*/
/*  tickerCopy: Deep Copy a ticker over to a 2nd ticker             */
/*                                                                  */
/*  int ticker[]: original ticker                                   */
/*  int newTicker: new ticker to copy   to                          */
/*------------------------------------------------------------------*/
void tickerCopy(int ticker[], int newTicker[])
{
    for (int tile = 0; tile < NUM_PEGS; tile++)
        newTicker[tile] = ticker[tile];
}
