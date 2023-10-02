// simple hangman game

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_WORDS 22
#define MAX_LENGTH 45

void printBoard(char[], char[], int *);
char *pickWord(char *[MAX_WORDS]);
bool checkContains(char *, char);
bool gameOverCheck(char[], char[], int *);

int main()
{
    char *words[MAX_WORDS];
    char word[MAX_LENGTH];
    char guesses[26] = {}; // 26 letters in the alphabet, i think
    int mistakes = 0;      // google says you are only allowed 7 guesses
    int *pMistakes = &mistakes;

    // open the words.txt file and put all the words into the words array
    FILE *pF = fopen("words.txt", "r");
    char buffer[45];

    // check if file was opened successfully
    if (pF == NULL)
    {
        printf("\ncouldn't find the file dude :(");
        return 1;
    }
    // file opened no problemo

    // get the words from the file
    int counter = 0;

    while (fgets(buffer, MAX_LENGTH, pF) != NULL)
    {
        words[counter] = buffer;
        // get rid of that yucky newline character
        if (words[counter][strlen(words[counter]) - 1] == '\n')
        {
            words[counter][strlen(words[counter]) - 1] = 0;
        }

        // Allocate memory for the word and copy it
        words[counter] = strdup(buffer);
        // THIS WAS THE PROBLEM I HAD GRAAHHHHHHHH. all of the pointers inside words were pointing towards the same
        // memory address so when buffer became something new all of the array elements changed too >:(
        // top 10 pointer moments

        if (words[counter] == NULL)
        {
            printf("no memory eks dee (skill issue)");
            return 1;
        }

        counter++;
    }

    // pick a random word
    strcpy(word, pickWord(words));

    // printf("picked word: %s", word);

    // start the game!!!
    while (!gameOverCheck(word, guesses, pMistakes))
    {
        char guess = ' ';

        printBoard(word, guesses, pMistakes);

        // get user input
        printf("your guess: ");
        scanf(" %c", &guess); // that space is important!!! without it adds the newline character as well
        fflush(stdin);

        // add that guess to the guesses array and repeat (if it is a unique guess)
        bool flag = false;
        for (int i = 0; i < strlen(guesses); i++)
        {
            if (guesses[i] == guess)
            {
                flag = true;
                printf("dude, you already tried that");
            }
        }

        if (!flag)
            guesses[strlen(guesses)] = tolower(guess);
    }

    fclose(pF);

    return 0;
}

void printBoard(char word[], char guesses[], int *pMistakes)
{
    // printf("\e[H\e[2J\e[3J"); // clears the screen to make it cool and clean
    printf("\n######## HANGMAN ########\n");
    printf("\n");

    printf("WORD:\n");
    for (int i = 0; i < strlen(word); i++)
    {
        bool flag = false;
        for (int j = 0; j < strlen(guesses); j++)
        {
            if (word[i] == guesses[j])
            {
                printf("%c", word[i]);
                flag = true;
                break;
            }
        }

        if (isspace(word[i]))
        {
            printf(" ");
            flag = true;
        }

        if (!flag)
            printf("-");
    }
    printf("\n");
    printf("\n");
    printf("GUESSES: ");

    for (int i = 0; i < strlen(guesses); i++)
    {
        printf("%c ", guesses[i]);
    }

    printf("\n");
    printf("MISTAKES: %d", *pMistakes);
    printf("\n");
    printf("\n");
}

char *pickWord(char *words[])
{
    srand(time(0)); // set the random seed to the current time

    int randNum = rand() % (MAX_WORDS - 1); // choose a random number, this will be the index

    return words[randNum];
}

bool checkContains(char *str, char ch)
{
}

bool gameOverCheck(char word[], char guesses[], int *pMistakes)
{
    printf("\e[H\e[2J\e[3J"); // clears the screen to make it cool and clean

    // check the number of mistakes
    int count = 0;
    for (int i = 0; i < strlen(guesses); i++)
    {
        for (int j = 0; j < strlen(word); j++)
        {
            if (word[j] == guesses[i])
            {
                count++;
            }
        }
    }

    *pMistakes = strlen(guesses) - count; // mistakes = total guesses - correct guesses

    // check if they are out of turns
    if (*pMistakes == 7)
    {
        printf("\n you ran out of tries dude");
        return true;
    }

    return false;
}