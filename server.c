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

    //variabelen
    srand(time(NULL));
    int gok[6], rnd = rand() % 100 + 1,highest = -100, lowest = 100, h, l, r, round = 6; //h = highest player l = lowest player
    char *VraagGok [6][52], buffer[256], *ParsedString;

    //verzendlijst
    VraagGok[0][51] = "guessit>gok1?>The service wants to know your guess.";
    VraagGok[1][51] = "guessit>gok2?>The service wants to know your guess.";
    VraagGok[2][51] = "guessit>gok3?>The service wants to know your guess.";
    VraagGok[3][51] = "guessit>gok4?>The service wants to know your guess.";
    VraagGok[4][51] = "guessit>gok5?>The service wants to know your guess.";
    VraagGok[5][51] = "guessit>gok6?>The service wants to know your guess.";

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
        for(int i = 0; i < 5; i++){

            //Vraag een per een hun gok en ontvang gokken. Nummer ze
            for(int i = 0; i < round; i++){
                //stel uw vraag aan player[i]
                zmq_send(publisher, VraagGok[i][51], 51,0);

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
            for(int i = 0; i < round; i++){
                if(gok[i] > highest){
                    highest = gok[i];
                    h = i+1;
                }
                if(gok[i] < lowest){
                    lowest = gok[i];
                l = i+1;
                }
            }

            printf("The lowest is %d and the highest is %d", lowest+rnd, highest+rnd);

            //uitslag bepalen.
            if(highest > 0 && lowest > 0){
                printf("\nplayer %d guessed %d, but it is the farest, bye bye!\n", h, highest+rnd);
                r = h;
            }
            else if(highest < 0 && lowest < 0){
                printf("\nplayer %d guessed %d, but it is the farest, bye bye!\n", l, lowest+rnd);
                r = l;
            }
            else if( (highest < 0 && lowest > 0) || (highest > 0 && lowest < 0)){
                int result = lowest + highest;
                if(result > 0){
                    printf("\nplayer %d guessed %d, but it is the farest, bye bye!\n", h, highest+rnd);
                    r = h;
                }
                else{
                    printf("\nplayer %d guessed %d, but it is the farest, bye bye!\n", l, lowest+rnd);
                    r = l;
                }
            }

            for(int i = r; i < round; i++){
                strcpy(*VraagGok[i], *VraagGok[i+1]);
            }


            //for(int i = r; i < round; i++){
              //  printf("The remaining players are: %s\n", *VraagGok[i]);
            //}
            zmq_send(publisher, "guessit>gok1?>You are player one.", 33,0);
            zmq_send(publisher, "guessit>gok2?>You are player two.", 33,0);
            zmq_send(publisher, "guessit>gok3?>You are player three.", 35,0);
            zmq_send(publisher, "guessit>gok4?>You are player four.", 34,0);
            zmq_send(publisher, "guessit>gok5?>You are player five.", 35,0);
            zmq_send(publisher, "guessit>gok6?>You are player six.", 34,0);
            round--;
        }
    }

    zmq_close (publisher);
    zmq_ctx_destroy (context);
    return 0;
}
