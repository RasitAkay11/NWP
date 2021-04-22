#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
    char player[6][10];
    int gok[6];
    srand (time(NULL));
    int rnd = rand() % 100 + 1, highest = -100, lowest = 100, round = 6, h, l;

    printf("Fill in the 6 player names:\n");
    for(int i = 0; i < 6; i++){
        scanf("%s", &player[i][10]);
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
            h = i+1;
        }
        if(gok[i] < lowest)
        {
            lowest = gok[i];
            l = i+1;
        }
    }

    if(highest > 0 && lowest > 0){
        printf("%s guessed %d  but it is the farest, bye bye!\n", player[h], highest+rnd);
    }
    else if(highest < 0 && lowest < 0){
        printf("%s guessed %d but it is the farest, bye bye!\n", player[l], lowest+rnd);
    }
    else if( (highest < 0 && lowest > 0) || (highest > 0 && lowest < 0)){
        int result = lowest + highest;
        if(result > 0){
            printf("%s guessed %d but it is the farest, bye bye!\n", player[h], highest+rnd);
        }
        else{
            printf("%s guessed %d but it is the farest, bye bye!\n", player[l], lowest+rnd);
        }
    }

    printf("The number was %d\n", rnd);

    return 0;
}
