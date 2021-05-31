#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

char *strsep(char **stringp, const char *delim) {
    char *rv = *stringp;
    if (rv) {
        *stringp += strcspn(*stringp, delim);
        if (**stringp)
            *(*stringp)++ = '\0';
        else
            *stringp = 0; }
    return rv;
}
char *parse(int keer, char *ParseString)
{
    char *String,*ParsedString;
    String = strdup(ParseString);
    for (int i = 0; i < keer; i++)
    {
        ParsedString = strsep(&String, ">");
    }
    return ParsedString;
}

int main ( int argc, char * argv[] )
{
    //filter gok
    const char *FilterGok1 = (argc > 1)? argv [1]: "guessit>gok1!>";
    const char *FilterGok2 = (argc > 1)? argv [1]: "guessit>gok2!>";
    const char *FilterGok3 = (argc > 1)? argv [1]: "guessit>gok3!>";
    const char *FilterGok4 = (argc > 1)? argv [1]: "guessit>gok4!>";
    const char *FilterGok5 = (argc > 1)? argv [1]: "guessit>gok5!>";
    const char *FilterGok6 = (argc > 1)? argv [1]: "guessit>gok6!>";

    //filter join
    const char *FilterJoin = (argc > 1)? argv [1]: "guessit>join!>";

    //variabelen
    srand(time(NULL));
    int gok[6], rnd = rand() % 100 + 1, h, l, r, round = 6; //h = highest player l = lowest player
    char VraagGok [6][52], StuurResultaat[6][73], StuurKick[6][31], buffer[256], *ParsedString;

    //verzendlijst
    strcpy(VraagGok[0], "guessit>gok1?>The service wants to know your guess.");
    strcpy(VraagGok[1], "guessit>gok2?>The service wants to know your guess.");
    strcpy(VraagGok[2], "guessit>gok3?>The service wants to know your guess.");
    strcpy(VraagGok[3], "guessit>gok4?>The service wants to know your guess.");
    strcpy(VraagGok[4], "guessit>gok5?>The service wants to know your guess.");
    strcpy(VraagGok[5], "guessit>gok6?>The service wants to know your guess.");

    //ResultaatLijst
    strcpy(StuurResultaat[0], "guessit>gok1?>Congratulations! You are not kicked! Next round loading...");
    strcpy(StuurResultaat[1], "guessit>gok2?>Congratulations! You are not kicked! Next round loading...");
    strcpy(StuurResultaat[2], "guessit>gok3?>Congratulations! You are not kicked! Next round loading...");
    strcpy(StuurResultaat[3], "guessit>gok4?>Congratulations! You are not kicked! Next round loading...");
    strcpy(StuurResultaat[4], "guessit>gok5?>Congratulations! You are not kicked! Next round loading...");
    strcpy(StuurResultaat[5], "guessit>gok6?>Congratulations! You are not kicked! Next round loading...");

    //Laat player weten dat hij/zij gekickt is
    strcpy(StuurKick[0], "guessit>gok1?>Sadly, you lost.");
    strcpy(StuurKick[1], "guessit>gok2?>Sadly, you lost.");
    strcpy(StuurKick[2], "guessit>gok3?>Sadly, you lost.");
    strcpy(StuurKick[3], "guessit>gok4?>Sadly, you lost.");
    strcpy(StuurKick[4], "guessit>gok5?>Sadly, you lost.");
    strcpy(StuurKick[5], "guessit>gok6?>Sadly, you lost.");

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

    while (1) {
        for(int i = 0; i < 6; i++){
            //Vraag een per een hun gok en ontvang gokken. Nummer ze
            for(int i = 0; i < round; i++){
                //stel uw vraag aan player[i]
                zmq_send(publisher, VraagGok[i], strlen(VraagGok[i]),0);

                //ontvang uw antwoord van player[i];
                memset(buffer,0,256);
                zmq_recv(subscriber, buffer, 256, 0);

                //sla antwoord op in gok[i]
                ParsedString = parse(3, buffer);
                printf("Player %d has guessed %s\n", i+1, ParsedString);
                gok[i] = atoi(ParsedString);
            }

            printf("Getal is %d\n", rnd);

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
                    printf("\nGrootste gedetecteerd.\n");
                }
                if(gok[i] < lowest){
                    lowest = gok[i];
                    l = i;
                    printf("Kleinste gedetecteerd.\n");
                }
            }

            //uitslag bepalen.
            if(highest > 0 && lowest > 0){
                r = h;
                printf("\nUistlag bepalen eerste if.\n");
            }
            else if(highest < 0 && lowest < 0){
                r = l;
                printf("\nUistlag bepalen tweede if.\n");
            }
            else if( (highest < 0 && lowest > 0) || (highest > 0 && lowest < 0)){
                int result = lowest + highest;
                if(result > 0){
                    r = h;
                    printf("\nUistlag bepalen derde if.\n");
                }
                else{
                    r = l;
                    printf("\nUistlag bepalen vierde if.\n");
                }
            }

            printf("\nWaarde van r: %d\n\n", r);

            //Verzend de resultaten naar de spelers.
            if(round > 1){
                for(int i = 0; i < round; i++){
                    if(i != r){
                        zmq_send(publisher, StuurResultaat[i], strlen(StuurResultaat[i]),0);
                        printf("%s\n", StuurResultaat[i]);
                    }
                    else{
                        zmq_send(publisher, StuurKick[i], strlen(StuurKick[i]), 0);
                        printf("%s\n", StuurKick[i]);
                    }
                }
            }else{
                zmq_send(publisher, StuurKick[r], strlen(StuurKick[r]), 0);
                printf("%s\n", StuurResultaat[i]);
            }

            //Kick player out
            for(int i = r; i < 6; i++){
                strcpy(VraagGok[i], VraagGok[i+1]);
                strcpy(StuurKick[i], StuurKick[i+1]);
                strcpy(StuurResultaat[i], StuurResultaat[i+1]);
            }


            printf("\n");

                round--;
                printf("Next round...\n\n");

           if(round == 1){
                zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterJoin, strlen(FilterJoin));
                sleep(5);
                zmq_send(publisher, "guessit>join?>Join back peeps", 28, 0);
                zmq_setsockopt(subscriber, ZMQ_UNSUBSCRIBE, FilterJoin, strlen(FilterJoin));
                round = 6;
            }
        }
    }

    zmq_close (publisher);
    zmq_ctx_destroy (context);
    return 0;
}
