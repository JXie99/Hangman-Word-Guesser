/*
* File: proj1.cpp
* Project: CMSC 202 Project 1, Spring 2021
* Author: Johnson Xie
*
* Description: This program will run a simple hangman game where the user
*             has to guess a random word pulled from words2.txt file. 
*             user will have 6 attempts to guess.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

//Name: loadWords
//PRE-CON: There will be a text file of words
//POST-CON: Returns the num of words in text file
int loadWords();
//Name: fillArray
//PRE-CON: There will be an empty array to fill with strings of words
//POST-CON: Fills array with strings of words
void fillArray(string theArray[]);
//Name: randWord
//PRE-CON: There will be a populated array of words and size of that array.
//POST-CON: Returns a random word in string
string randWord(string theArray[], int arraySize);
//Name: displayLetter
//PRE-CON: Assumes that we will have a word to use 
//POST-CON: creates a string that replaces all letters in word with '_'
string displayLetter(string mysteryWord);
//Name: guessLetter()
//PRE-CON: Assumes that user will enter lowercase letter
//POST-CON: plays hangman game 
void guessLetter(string guessWord, string dashesWord);

int main() 
{
    bool anotherGame = false; 
    char tryAgain;
    const char GAME_OVER = 'n';
    const char REMATCH = 'y';

    cout << "Welcome to UMBC Word Guess" << endl;

    // generates random seed for time
    srand(time(NULL));

    int sizeOfArray = loadWords();
    string wordList[sizeOfArray] = {};
    fillArray(wordList); //filling array through pass by reference 

    while(anotherGame != true)
    {
        string wordToGuess = randWord(wordList, sizeOfArray);
        string wordInDashes = displayLetter(wordToGuess);
        guessLetter(wordToGuess, wordInDashes);

        cout << "Would you like to play another game y/n: " << endl;
        cin >> tryAgain;

        // if user choices are not y or n
        while(tryAgain != REMATCH && tryAgain != GAME_OVER)
        {
            cout << "Please enter y or n: " << endl;
            cin >> tryAgain;
        }

        if(tryAgain == GAME_OVER)
            // breaks out of loop if user does not want to play another game.
            anotherGame = true;
    }
    return 0;
}

//finds out the num of words in the file to be used to create array 
int loadWords() 
{
    int currSize = 0;
    string theWord;

    ifstream myfile("words2.txt");
    if(myfile.is_open()) 
    {
        cout << "Your file was imported!" << endl;

        while(getline(myfile, theWord))
        {
            currSize ++;
        }

        cout << currSize << " words imported." << endl;
    }

    else
    {
        cout << "unable to open file" << endl;
    }

    myfile.close();
    return currSize;
}

//fill array with words from file
void fillArray(string theArray[])
{
    string thatWord;
    int counter = 0;
    ifstream myfile("words2.txt");
    while (getline(myfile, thatWord))
    {
        theArray[counter] = thatWord;
        counter ++;
    }
    myfile.close();
}

//selects a random word 
string randWord(string theArray[], int arraySize)
{
    // selects random word from array and returns the word to use
    int randNum = rand() % arraySize; // random number between 0 and (array size - 1)
    string theWord = theArray[randNum];
    //cout << theWord << endl;
    return theWord;
}

//displays the word in dashes 
string displayLetter(string mysteryWord)
{
    string displayWord = mysteryWord;
    for (int i = 0; i < displayWord.length(); i++)
    {
        displayWord[i] = '_';
    }
    return displayWord;
}

//the actual hangman game
void guessLetter(string guessWord, string dashesWord)
{
    char usedLetters[50] = {};
    int lettersLeft = dashesWord.length();
    char theLetter;
    int tries = 6;
    int usedLetterIndex = 0;
    bool validChoice = false;
    bool usedLetter = false;

    cout << "ok I am thinking of a word with " << guessWord.length() << " letters." << endl;
    cout << endl;
    
    do {
        int counter = 0;
        while(tries != 0 && validChoice != true)
        {
            cout << "You have " << tries << " tries left." << endl;
            cout << lettersLeft << " letters remain" << endl;
            cout << dashesWord << endl;
            cout << "What letter would you like to guess? " << endl;
            cin >> theLetter;
            cout << endl;

            // checks if the letter is a lower case using ASCII
            while (int(theLetter) < 97 || int(theLetter) > 122) 
            {
                cout << endl;
                cout << "Invalid choice, What letter would you like to guess?" << endl;
                cin >> theLetter;
            }

            // check if letter has been used or not
            // if first guess, no need to check 
            if (usedLetterIndex == 0) 
            {
                usedLetters[usedLetterIndex] = theLetter;
                usedLetterIndex++;
                validChoice = true;
            }

            //not the first guess, so check if letter has been used
            else
            {
                for (int i = 0; i < usedLetterIndex; i++)
                {
                    if (theLetter == usedLetters[i]) 
                    {
                        usedLetter = true;
                    }
                }

                if(usedLetter == true)
                {
                    cout << theLetter << " has already been guessed." << endl;
                    cout << endl;
                    tries--;
                }

                else
                {
                    usedLetters[usedLetterIndex] = theLetter;
                    usedLetterIndex++;
                    validChoice = true;
                }
            }
        }

        if(tries != 0)
        {
            //if letter is found in mystery word, replace dash with letter.
            for (int i = 0; i < guessWord.length(); i++) 
            {
                if (guessWord[i] == theLetter) 
                {
                    cout << "Good guess! " << theLetter << " was in the mystery word." << endl;
                    dashesWord[i] = theLetter;
                    lettersLeft--;
                }

                //if letter not in word
                else
                {
                    counter++;
                    
                    //end of word is reach and letter is not found 
                    if(counter == guessWord.length())
                    {
                        tries--;
                        cout << "There is no " << theLetter << " in puzzle" << endl;
                        cout << endl;
                    }
                }
            }
        }

        validChoice = false;

    }while(tries != 0 && guessWord != dashesWord); // loops should end if word is guessed or out of tries

    if(guessWord == dashesWord)
    {
        cout << "Congrats you win! the word was " << guessWord << endl;
    }
    if(tries == 0)
    {
        cout << "You lose, the word was " << guessWord << endl;
    }
}
