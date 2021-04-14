#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
    char player[6][10];
    int gok[6];
    srand (time(NULL));
    int rnd = rand() % 100 + 1;

    printf("Fill in the 6 player names:\n");
    for(int i = 0; i < 6; i++){
        //printf("What is your name? ");
        scanf("%s", &player[i][10]);
        printf("%s joined the party!\n", player[i+1]);
    }

    for(int i = 0; i < 6; i++){
        printf("%s, what is your guess? ", player[i+1]);
        scanf("%d", &gok[i]);
    }


    /*if(gok == rnd){
        printf("Congratulations, you guessed it right!\n");
    }
    else if(gok > rnd){
        printf("Sad! Your guess is bigger then the number! You lost.\n");
    }
    else if(gok < rnd){
        printf("Sad! Your guess is less then the number! You lost.\n");
    }
    else{
        printf("Oops, something went terrible wrong.\n");
    }
    */
    printf("The number was %d\n", rnd);

    return 0;
}
