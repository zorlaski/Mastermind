/*

mastermind.c : the main driver file for the mastermind program.

Mastermind solver, based on Donald Knuth's method published in
the Journal of Recreational Mathematics (1976)

Written by Zack Orlaski, Grove City College for COMP 325B
09/02/20

*/

/*Headers*/
#include "mastermind.h"

/*-----------------------------------------------------------*/
/* ComputerAsMastermind: Run "Baby Knuth" Algorithm          */
/* int* master: master code in the sequence to be guessed    */
/* int* initialGuess: 1st guess in the algorithm,            */
/*      reccomended to be [1,1,2,2] by Knuth                 */
/*-----------------------------------------------------------*/
int computerAsMastermind(int *master, int *initialGuess);

int main()
{

    /*~~~~~~ Array Dictionary ~~~~~~*/
    int master[NUM_PEGS];                      //Master code, all permutations will be checked as master
    int initialGuess[NUM_PEGS] = {1, 1, 2, 2}; //Knuth's suggested first code
    int ticker[NUM_PEGS] = {1, 1, 1, 1};       //"odometer" like ticker that creates the permutations

    int tickIndex,  //Position within the ticker
        itCount,    //number of sequences generated so far
        maxGuesses, //highest number of guesses required to get master code using "Baby Knuth"
        guessCount, //number of guesses an individual master code took using "Baby-Knuth"
        guessSum;   //sum of all guessCounts using "Baby Knuth"

    int stdDevData[PERMUTATIONS]; //array of guessCounts, used for calculating Standard Deviation
    double guessAverage,          //average number of guesses required to get master code using "Baby Knuth"
        standardDeviation = 0;    //standard deviation of number of guesses required to get master code using "Baby Knuth"

    tickIndex = itCount = maxGuesses = guessSum = 0;
    printf("Written by Zack Orlaski, Grove City College for COMP 325B\n");

    /*STARTING ALGORITHM*/

    tickerCopy(ticker, master); //Copy over ticker

    guessCount = computerAsMastermind(master, initialGuess); //Run "Baby Knuth" with current master

    /*update values*/
    maxGuesses = guessCount;
    guessSum = guessCount;
    stdDevData[itCount] = guessCount;
    itCount++;

    //Run until all codes generated
    while (itCount < PERMUTATIONS)
    {
        //Increment current ticker number if less than max num
        if (ticker[tickIndex] < NUM_COLORS)
        {

            ++ticker[tickIndex]; //increment

            tickerCopy(ticker, master); //copy ticker

            //!Begin Main action on sequence
            guessCount = computerAsMastermind(master, initialGuess); //Run "Baby Knuth" with current master

            //update maxGuesses
            if (guessCount > maxGuesses)
                maxGuesses = guessCount;

            //update guessSum
            guessSum += guessCount;

            //update stdDevData
            stdDevData[itCount] = guessCount;

            //update itCount
            itCount++;

            //!End Main Action

            tickIndex = 0; //back to the beginning
        }

        else
        { //shift right and increment
            ticker[tickIndex] = 1;
            tickIndex++;
        }
    }

    /*Calculate Standard Deviation*/
    guessAverage = (double)guessSum / PERMUTATIONS;
    for (int data = 0; data < PERMUTATIONS; data++)
    {
        double diff = stdDevData[data] - guessAverage; //difference
        standardDeviation += pow(diff, 2);
    }
    standardDeviation /= PERMUTATIONS;
    standardDeviation = sqrt(standardDeviation);

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("~~~~~~~~~~~~~FINAL STATS~~~~~~~~~~~\n");
    printf("Max guesses required:\t%6d\n", maxGuesses);
    printf("Avg. guesses required:\t%6.5lf\n", guessAverage);
    printf("Guess Std. Deviation:\t%6.5lf\n", standardDeviation);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

/*-----------------------------------------------------------*/
/* ComputerAsMastermind: Run "Baby Knuth" Algorithm          */
/* int* master: master code in the sequence to be guessed    */
/* int* initialGuess: 1st guess in the algorithm,            */
/*      reccomended to be [1,1,2,2] by Knuth                 */
/*-----------------------------------------------------------*/
int computerAsMastermind(int *master, int *initialGuess)
{
    //Dictionary of Sequences
    Code dictionary[MAX_DICT_SIZE];
    //current guess Code
    Code sample;

    //create a dictionary from current master
    int dictCount = generateDictionary(master, initialGuess, dictionary);

    //status indicator, false if code found
    int notFound;

    //running tally of guesses, including initial guess
    int guesses = 1;

    if (dictCount == 0) //initial guess found code
        notFound = FALSE;
    else //not found
        notFound = TRUE;

    sample = dictionary[0]; //grab first code in dict

    while (notFound) //run until code found
    {

        int arySize = dictCount; //number of array spots with useful Codes

        scoreCode(master, sample.sequence, sample.score); //check code

        guesses++; //update guesses

        if (sample.score[0] == NUM_PEGS) //code found
            notFound = FALSE;
        else //score other codes against sample code
        {
            dictCount = 0;

            //score all codes against the first
            for (int c = 1; c < arySize; c++)
            {

                scoreCode(sample.sequence,
                          dictionary[c].sequence,
                          dictionary[c].score);

                //add codes with matching scores to first code to updated dictionary
                if (compareScore(sample.score, dictionary[c].score))
                    dictionary[dictCount++] = dictionary[c];
            }
        }
        sample = dictionary[0];
    }
    return guesses; //total guesses taken
}