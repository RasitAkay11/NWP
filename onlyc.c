#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
    char player[10] = "";
    srand (time(NULL));
    int rnd = rand() % 100 + 1, gok;

    printf("What is your name? ");
    scanf("%s", &player);
    printf("%s joined the party!\n", player);
    printf("What is your guess? ");
    scanf("%d", &gok);

    printf("The number was %d\n", rnd);

    if(gok == rnd){
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

    printf("The number was %d\n", rnd);

    return 0;
}
