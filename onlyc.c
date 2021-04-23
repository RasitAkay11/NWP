#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

int main()
{
    srand (time(NULL));
    int rnd = rand() % 100 + 1, highest = -100, lowest = 100,round = 6, h, l, r;
    char player[6][10];
    int gok[6];

    printf("Fill in the 6 player names:\n");
    for(int i = 0; i < 6; i++){
        scanf("%s", &player[i][10]);
    }
    printf("\nWelcome to the party %s, %s, %s, %s, %s, %s!\n\n", player[1], player[2], player[3], player[4], player[5], player[6]);


    for(int i = 0; i < round; i++){
        for(int i = 0; i < round; i++){
            printf("%s, what is your guess? ", player[i+1]);
            scanf("%d", &gok[i]);
        }

        for(int i = 0; i < round; i++){
            gok[i] = gok[i] - rnd;
        }

        for(int i = 0; i < round; i++)
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
            printf("\n%s guessed %d, but it is the farest, bye bye!\n", player[h], highest+rnd);
            r = h;
        }
        else if(highest < 0 && lowest < 0){
            printf("\n%s guessed %d, but it is the farest, bye bye!\n", player[l], lowest+rnd);
            r = l;
        }
        else if( (highest < 0 && lowest > 0) || (highest > 0 && lowest < 0)){
            int result = lowest + highest;
            if(result > 0){
                printf("\n%s guessed %d, but it is the farest, bye bye!\n", player[h], highest+rnd);
                r = h;
            }
            else{
                printf("\n%s guessed %d, but it is the farest, bye bye!\n", player[l], lowest+rnd);
                r = l;
            }
        }

        printf("The number was %d\n", rnd);

        for(int i = r; i < round; i++){
            strcpy(player[i], player[i+1]);
        }
        printf("The remaining players are: ");
        for(int i = 0; i < round; i++){
            printf("%s ", player[i]);
        }
        printf("\nNEXT ROUND LOADING....\n");
    }
    return 0;
}
