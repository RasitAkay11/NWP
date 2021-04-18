#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
    char player[6][10];
    int gok[6];
    srand (time(NULL));
    int rnd = rand() % 100 + 1, highest = -100, lowest = 100;

    printf("Fill in the 6 player names:\n");
    for(int i = 0; i < 6; i++){
        //printf("What is your name? ");
        scanf("%s", &player[i][10]);
        //printf("%s joined the party!\n", player[i+1]);
    }
    printf("Welcome to the party %s, %s, %s, %s, %s, %s!\n", player[1], player[2], player[3], player[4], player[5], player[6]);
    for(int i = 0; i < 6; i++){
        printf("%s, what is your guess? ", player[i+1]);
        scanf("%d", &gok[i]);
    }



    for(int i = 0; i < 6; i++){
        gok[i] = gok[i] - rnd;
    }

    for(int i = 0; i < 6; i++)
    {
        if(gok[i] > highest)
        {
            highest = gok[i];
        }
        if(gok[i] < lowest)
        {
            lowest = gok[i]; //Lowest werkt niet zonder lijn 25 en 26, gaat nooit in de if en neemt een radnom nummer.
        }
    }


    printf("Dit is de hoogste %d, dit is de laagste %d\n", highest+rnd, lowest+rnd);

    if(lowest < 0){
        lowest = abs(lowest);
        highest = abs(highest);
    }
    if(highest < 0){
        lowest = abs(lowest);
        highest = abs(highest);
    }

    if(highest > lowest){
        printf("%d is de verste, er uit kut!\n", highest);
    }
    else if(lowest > highest){
        printf("%d is de verste, er uit kut\n", lowest);
    }

    printf("The number was %d\n", rnd);

    return 0;
}
