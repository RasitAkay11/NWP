#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

char *strsep(char **stringp, const char *delim){
    char *rv = *stringp;
    if (rv) {
        *stringp += strcspn(*stringp, delim);
        if (**stringp)
            *(*stringp)++ = '\0';
        else
            *stringp = 0; }
    return rv;
}
char *parse(int keer, char *ParseString){
    char *String,*ParsedString;
    String = strdup(ParseString);
    for (int i = 0; i < keer; i++)
    {
        ParsedString = strsep(&String, ">");
    }
    return ParsedString;
}

int main(int argc,char * argv[]){
    //filter gok
    const char *FilterGok1 = (argc > 1)? argv [1]: "guessit>gok1!>";
    const char *FilterGok2 = (argc > 1)? argv [1]: "guessit>gok2!>";
    const char *FilterGok3 = (argc > 1)? argv [1]: "guessit>gok3!>";
    const char *FilterGok4 = (argc > 1)? argv [1]: "guessit>gok4!>";
    const char *FilterGok5 = (argc > 1)? argv [1]: "guessit>gok5!>";
    const char *FilterGok6 = (argc > 1)? argv [1]: "guessit>gok6!>";

    //filter gok
    const char *FilterJoker1 = (argc > 1)? argv [1]: "guessit>joker1?>";
    const char *FilterJoker2 = (argc > 1)? argv [1]: "guessit>joker2?>";
    const char *FilterJoker3 = (argc > 1)? argv [1]: "guessit>joker3?>";
    const char *FilterJoker4 = (argc > 1)? argv [1]: "guessit>joker4?>";
    const char *FilterJoker5 = (argc > 1)? argv [1]: "guessit>joker5?>";
    const char *FilterJoker6 = (argc > 1)? argv [1]: "guessit>joker6?>";

    //filter join
    const char *FilterJoin = (argc > 1)? argv [1]: "guessit>join!>";

    //variabelen
    srand(time(NULL));
    int gok[6], rnd = rand() % 100 + 1, h, l, r, round = 6, dichtsbijk = -100, dichtsbijg = 100; //h = highest player l = lowest player
    int spelerlaagste = -1, spelerhoogste = -1, spelerwie = -1;
    char VraagGok [6][52], StuurResultaat[6][73], StuurKick[6][31], buffer[256], stuurdichtsteja[6][43], stuurdichtstenee[6][47], *ParsedString;
    char BerichtDichts[256] = "guessit>dichts?>";

    //verzendlijst
    strcpy(VraagGok[0], "guessit>gok1?>The service wants to know your guess.");
    strcpy(VraagGok[1], "guessit>gok2?>The service wants to know your guess.");
    strcpy(VraagGok[2], "guessit>gok3?>The service wants to know your guess.");
    strcpy(VraagGok[3], "guessit>gok4?>The service wants to know your guess.");
    strcpy(VraagGok[4], "guessit>gok5?>The service wants to know your guess.");
    strcpy(VraagGok[5], "guessit>gok6?>The service wants to know your guess.");

    //ResultaatLijst
    strcpy(StuurResultaat[0], "guessit>gok1?>Congratulations! You are not kicked!");
    strcpy(StuurResultaat[1], "guessit>gok2?>Congratulations! You are not kicked!");
    strcpy(StuurResultaat[2], "guessit>gok3?>Congratulations! You are not kicked!");
    strcpy(StuurResultaat[3], "guessit>gok4?>Congratulations! You are not kicked!");
    strcpy(StuurResultaat[4], "guessit>gok5?>Congratulations! You are not kicked!");
    strcpy(StuurResultaat[5], "guessit>gok6?>Congratulations! You are not kicked!");

    //Laat player weten dat hij/zij gekickt is
    strcpy(StuurKick[0], "guessit>gok1?>Sadly, you lost.");
    strcpy(StuurKick[1], "guessit>gok2?>Sadly, you lost.");
    strcpy(StuurKick[2], "guessit>gok3?>Sadly, you lost.");
    strcpy(StuurKick[3], "guessit>gok4?>Sadly, you lost.");
    strcpy(StuurKick[4], "guessit>gok5?>Sadly, you lost.");
    strcpy(StuurKick[5], "guessit>gok6?>Sadly, you lost.");

    //Laat player weten of die het dichtste bij was
    strcpy(stuurdichtsteja[0], "guessit>dichts1?>You are the closest one.");
    strcpy(stuurdichtsteja[1], "guessit>dichts2?>You are the closest one.");
    strcpy(stuurdichtsteja[2], "guessit>dichts3?>You are the closest one.");
    strcpy(stuurdichtsteja[3], "guessit>dichts4?>You are the closest one.");
    strcpy(stuurdichtsteja[4], "guessit>dichts5?>You are the closest one.");
    strcpy(stuurdichtsteja[5], "guessit>dichts6?>You are the closest one.");

    //Laat player weten dat hij/zij niet het dichste bij is
    strcpy(stuurdichtstenee[0], "guessit>dichts1?>You are not the closest one.");
    strcpy(stuurdichtstenee[1], "guessit>dichts2?>You are not the closest one.");
    strcpy(stuurdichtstenee[2], "guessit>dichts3?>You are not the closest one.");
    strcpy(stuurdichtstenee[3], "guessit>dichts4?>You are not the closest one.");
    strcpy(stuurdichtstenee[4], "guessit>dichts5?>You are not the closest one.");
    strcpy(stuurdichtstenee[5], "guessit>dichts6?>You are not the closest one.");

    printf("Starting the service...\n");

    //connect
    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUSH);
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    int rp = zmq_connect(publisher, "tcp://benternet.pxl-ea-ict.be:24041");
    int rs = zmq_connect(subscriber, "tcp://benternet.pxl-ea-ict.be:24042");

    //check if connect failed
    if (rp != 0 && rs != 0){
        printf("ERROR: ZeroMQ error occurred during zmq_ctx_new(): %s\n", zmq_strerror(errno));
        return EXIT_FAILURE;
    } else{
        sleep(2);
        printf("The service is ready to go!\n\n");
    }

    //individueel subscriben op gokken players
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok1, strlen(FilterGok1));
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok2, strlen(FilterGok2));
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok3, strlen(FilterGok3));
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok4, strlen(FilterGok4));
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok5, strlen(FilterGok5));
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok6, strlen(FilterGok6));

    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterJoker1, strlen(FilterJoker1));
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterJoker2, strlen(FilterJoker2));
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterJoker3, strlen(FilterJoker3));
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterJoker4, strlen(FilterJoker4));
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterJoker5, strlen(FilterJoker5));
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterJoker6, strlen(FilterJoker6));

    while (1){
        for(int i = 0; i < round; i++){
            //Vraag een per een hun gok en ontvang gokken.
            if(round > 1){
                for(int i = 0; i < round; i++){
                    //stel uw vraag aan player[i]
                    zmq_send(publisher, VraagGok[i], strlen(VraagGok[i]),0);

                    //ontvang uw antwoord van player[i];
                    memset(buffer,0,256);
                    zmq_recv(subscriber, buffer, 256, 0);

                    //sla antwoord op in gok[i]
                    ParsedString = parse(3, buffer);
                    printf("%s\n", ParsedString);
                    if(strcmp(ParsedString, "I want to use my joker.") == 0){
                        gok[i] = rnd;
                    }else{
                        printf("Player %d has guessed %s\n", i+1, ParsedString);
                        gok[i] = atoi(ParsedString);
                    }
                }
            }

            printf("random nummer is %d\n", rnd);

            //De gokken gelijkstellen op de nul-as
            for(int i = 0; i < round; i++){
                gok[i] = gok[i] - rnd;
            }

            //De grootste en de kleinste detecteren
            int highest = -100, lowest = 100;
            for(int i = 0; i < round; i++){
                if(gok[i] > highest){
                    highest = gok[i];
                    h = i;
                }
                if(gok[i] < lowest){
                    lowest = gok[i];
                    l = i;
                }
            }

            //uitslag bepalen.
            if(highest > 0 && lowest > 0){
                r = h;
            }
            else if(highest < 0 && lowest < 0){
                r = l;
            }
            else if( (highest < 0 && lowest > 0) || (highest > 0 && lowest < 0)){
                int result = lowest + highest;
                if(result > 0){
                    r = h;
                }
                else{
                    r = l;
                }
            }

            //Dichtsbij detecteren
            for(int i = 0; i < round; i++){
                gok[i] = gok[i] + rnd;
            }

            for(int i = 0; i< round; i++){
                if(gok[i] <= rnd){
                    if(gok[i] > dichtsbijk){
                        dichtsbijk = gok[i];
                        spelerlaagste = i;
                    }
                }
            }

            for(int i = 0; i < round; i++){
                if(gok[i] >= rnd){
                    if(gok[i] < dichtsbijg){
                        dichtsbijg = gok[i];
                        spelerhoogste = i;
                    }
                }
            }



            printf("\nDit waren de gokken:\n");
            for(int i = 0; i < round; i++){
                printf("%d\n", gok[i]);
            }

            //Verzend de resultaten naar de spelers.
            for(int i = 0; i < round; i++){
                if(i != r && round > 1){
                    zmq_send(publisher, StuurResultaat[i], strlen(StuurResultaat[i]),0);
                }
                else{
                    zmq_send(publisher, StuurKick[i], strlen(StuurKick[i]), 0);
                }
            }

            sleep(2);

            //Verzend de gok die het dichts bij is
            if(dichtsbijk == -100){
                char dichtsbijstr[10];
                itoa(dichtsbijg, dichtsbijstr, 10);
                strcat(BerichtDichts, dichtsbijstr);
                spelerwie = spelerhoogste;
                zmq_send(publisher, BerichtDichts, strlen(BerichtDichts), 0);
            }else if(dichtsbijg == 100){
                char dichtsbijstr[10];
                itoa(dichtsbijk, dichtsbijstr, 10);
                strcat(BerichtDichts, dichtsbijstr);
                spelerwie = spelerlaagste;
                zmq_send(publisher, BerichtDichts, strlen(BerichtDichts), 0);
            }else if(dichtsbijk != -100 && dichtsbijg != 100){
                dichtsbijg = dichtsbijg - rnd;
                dichtsbijk = dichtsbijk - rnd;
                int resultaat = dichtsbijg - dichtsbijk;
                if(resultaat > 0){
                    char dichtsbijstr[10];
                    itoa(dichtsbijk, dichtsbijstr, 10);
                    strcat(BerichtDichts, dichtsbijstr);
                    spelerwie = spelerlaagste;
                    zmq_send(publisher, BerichtDichts, strlen(BerichtDichts), 0);
                } else if(resultaat < 0){
                    char dichtsbijstr[10];
                    itoa(dichtsbijg, dichtsbijstr, 10);
                    strcat(BerichtDichts, dichtsbijstr);
                    spelerwie = spelerhoogste;
                    zmq_send(publisher, BerichtDichts, strlen(BerichtDichts), 0);
                }
            }

            sleep(2);
            //Stuur naar speler of die dichste is of niet
            for(int i = 0; i< round; i++){
                if(i != spelerwie){
                    zmq_send(publisher, stuurdichtstenee[i], 45, 0);
                } else{
                    zmq_send(publisher, stuurdichtsteja[i], 41, 0);
                }
            }

            //Kick player out
            for(int i = r; i < 6; i++){
                strcpy(VraagGok[i], VraagGok[i+1]);
                strcpy(StuurKick[i], StuurKick[i+1]);
                strcpy(StuurResultaat[i], StuurResultaat[i+1]);
                strcpy(stuurdichtsteja[i], stuurdichtsteja[i+1]);
                strcpy(stuurdichtstenee[i], stuurdichtstenee[i+1]);
            }


            spelerhoogste = -1;
            spelerlaagste = -1;
            spelerwie = -1;
            strcpy(BerichtDichts, "guessit>dichts!>");
            round--;
            printf("Next round...\n\n");


           //game is gedaan, alles klaarzetten voor nieuwe game
           if(round == 1){
                zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterJoin, strlen(FilterJoin));
                sleep(3);
                zmq_send(publisher, "guessit>join?>Join back peeps", 28, 0);
                for(int i = 0; i < 6; i++){
                    memset(buffer,0,256);
                    zmq_recv(subscriber, buffer, 256,0);
                    ParsedString = parse(3, buffer);
                    printf("%d. %s\n", i+1, ParsedString);
                }
                zmq_setsockopt(subscriber, ZMQ_UNSUBSCRIBE, FilterJoin, strlen(FilterJoin));
                printf("\nStarting new game.\n");
                srand(time(NULL));
                rnd = rand() % 100 + 1;
                round = 6;

                //verzendlijst
                strcpy(VraagGok[0], "guessit>gok1?>The service wants to know your guess.");
                strcpy(VraagGok[1], "guessit>gok2?>The service wants to know your guess.");
                strcpy(VraagGok[2], "guessit>gok3?>The service wants to know your guess.");
                strcpy(VraagGok[3], "guessit>gok4?>The service wants to know your guess.");
                strcpy(VraagGok[4], "guessit>gok5?>The service wants to know your guess.");
                strcpy(VraagGok[5], "guessit>gok6?>The service wants to know your guess.");

                //ResultaatLijst
                strcpy(StuurResultaat[0], "guessit>gok1?>Congratulations! You are not kicked!");
                strcpy(StuurResultaat[1], "guessit>gok2?>Congratulations! You are not kicked!");
                strcpy(StuurResultaat[2], "guessit>gok3?>Congratulations! You are not kicked!");
                strcpy(StuurResultaat[3], "guessit>gok4?>Congratulations! You are not kicked!");
                strcpy(StuurResultaat[4], "guessit>gok5?>Congratulations! You are not kicked!");
                strcpy(StuurResultaat[5], "guessit>gok6?>Congratulations! You are not kicked!");

                //Laat player weten dat hij/zij gekickt is
                strcpy(StuurKick[0], "guessit>gok1?>Sadly, you lost.");
                strcpy(StuurKick[1], "guessit>gok2?>Sadly, you lost.");
                strcpy(StuurKick[2], "guessit>gok3?>Sadly, you lost.");
                strcpy(StuurKick[3], "guessit>gok4?>Sadly, you lost.");
                strcpy(StuurKick[4], "guessit>gok5?>Sadly, you lost.");
                strcpy(StuurKick[5], "guessit>gok6?>Sadly, you lost.");

                //Laat player weten of die het dichtste bij was
                strcpy(stuurdichtsteja[0], "guessit>dichts1?>You are the closest one.");
                strcpy(stuurdichtsteja[1], "guessit>dichts2?>You are the closest one.");
                strcpy(stuurdichtsteja[2], "guessit>dichts3?>You are the closest one.");
                strcpy(stuurdichtsteja[3], "guessit>dichts4?>You are the closest one.");
                strcpy(stuurdichtsteja[4], "guessit>dichts5?>You are the closest one.");
                strcpy(stuurdichtsteja[5], "guessit>dichts6?>You are the closest one.");

                //Laat player weten dat hij/zij niet het dichste bij is
                strcpy(stuurdichtstenee[0], "guessit>dichts1?>You are not the closest one.");
                strcpy(stuurdichtstenee[1], "guessit>dichts2?>You are not the closest one.");
                strcpy(stuurdichtstenee[2], "guessit>dichts3?>You are not the closest one.");
                strcpy(stuurdichtstenee[3], "guessit>dichts4?>You are not the closest one.");
                strcpy(stuurdichtstenee[4], "guessit>dichts5?>You are not the closest one.");
                strcpy(stuurdichtstenee[5], "guessit>dichts6?>You are not the closest one.");

                //Wacht op players op hun terug te subscriben op guessit>gok?>
                sleep(2);
            }
        }
    }

    zmq_close (publisher);
    zmq_ctx_destroy (context);
    return 0;
}
