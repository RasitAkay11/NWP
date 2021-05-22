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
    //filter naam
    const char *FilterNaam = (argc > 1)? argv [1]: "guessit>naam!>";
    const char *FilterNaam2 = (argc > 1)? argv [1]: "guessit>naam2!>";
    const char *FilterNaam3 = (argc > 1)? argv [1]: "guessit>naam3!>";
    const char *FilterNaam4 = (argc > 1)? argv [1]: "guessit>naam4!>";
    const char *FilterNaam5 = (argc > 1)? argv [1]: "guessit>naam5!>";
    const char *FilterNaam6 = (argc > 1)? argv [1]: "guessit>naam6!>";

    //filter gok
    const char *FilterGok = (argc > 1)? argv [1]: "guessit>gok!>";
    const char *FilterGok2 = (argc > 1)? argv [1]: "guessit>gok2!>";
    const char *FilterGok3 = (argc > 1)? argv [1]: "guessit>gok3!>";
    const char *FilterGok4 = (argc > 1)? argv [1]: "guessit>gok4!>";
    const char *FilterGok5 = (argc > 1)? argv [1]: "guessit>gok5!>";
    const char *FilterGok6 = (argc > 1)? argv [1]: "guessit>gok6!>";

    //variabelen
    int gok[6], highest = -100, lowest = 100, h, l, r; //h = highest player l = lowest player
    char *player[6][10];
    srand(time(NULL));
    int rnd = rand() % 100 + 1;
    char buffer [256];
    char *ParsedString;

    printf("Starting the service...\n");

    //connect
    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUSH);
    void *subscriber = zmq_socket (context, ZMQ_SUB);
    int rp = zmq_connect(publisher, "tcp://benternet.pxl-ea-ict.be:24041");
    int rs = zmq_connect(subscriber, "tcp://benternet.pxl-ea-ict.be:24042");

    //check if connect failed
    if (rp != 0 && rs != 0)
    {
        printf("ERROR: ZeroMQ error occurred during zmq_ctx_new(): %s\n", zmq_strerror(errno));

        return EXIT_FAILURE;
    } else{
        sleep(2);
        printf("The service is ready to go!\n\n");
    }

    while (1) {

        //individueel subscriben op namen players
        zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterNaam, 10);
        zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterNaam2, 11);
        zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterNaam3, 11);
        zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterNaam4, 11);
        zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterNaam5, 11);
        zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterNaam6, 11);

        //ontvang namen
        for(int i = 0; i < 6; i++){
            memset(buffer,0,256);
            zmq_recv(subscriber, buffer, 256, 0);
            player[i][9] = parse(3, buffer);
            printf("Player %d's name is %s\n", i+1, player[i][9]);
        }

        //individueel unsubscriben op namen players
        zmq_setsockopt(subscriber, ZMQ_UNSUBSCRIBE, FilterNaam, 10);
        zmq_setsockopt(subscriber, ZMQ_UNSUBSCRIBE, FilterNaam2, 11);
        zmq_setsockopt(subscriber, ZMQ_UNSUBSCRIBE, FilterNaam3, 11);
        zmq_setsockopt(subscriber, ZMQ_UNSUBSCRIBE, FilterNaam4, 11);
        zmq_setsockopt(subscriber, ZMQ_UNSUBSCRIBE, FilterNaam5, 11);
        zmq_setsockopt(subscriber, ZMQ_UNSUBSCRIBE, FilterNaam6, 11);


        //individueel subscriben op gokken players
        zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok, 8);
        zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok2, 9);
        zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok3, 9);
        zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok4, 9);
        zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok5, 9);
        zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok6, 9);

        //ontvang gokken
        for(int i = 0; i < 6; i++){
            memset(buffer,0,256);
            zmq_recv (subscriber, buffer, 256, 0);
            ParsedString = parse(3, buffer);
            printf("Player %d has guessed %s\n", i+1, ParsedString);
            gok[i] = atoi(ParsedString);
        }

        printf("\nplayer 1 %d\nplayer 2 %d\nplayer3 %d\nplayer4 %d\nplayer 5 %d\nplayer 6 %d\nThe number %d\n\n", gok[0], gok[1], gok[2], gok[3], gok[4], gok[5], rnd);

        //De gokken gelijkstellen op de nul-as
        for(int i = 0; i < 6; i++){
            gok[i] = gok[i] - rnd;
        }

        //De grootste en de kleinste detecteren
        for(int i = 0; i < 6; i++){
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
        zmq_send(publisher, "guessit>gok?>You are player one.", 32,0);
        zmq_send(publisher, "guessit>gok2?>You are player two.", 33,0);
        zmq_send(publisher, "guessit>gok3?>You are player three.", 35,0);
        zmq_send(publisher, "guessit>gok4?>You are player four.", 34,0);
        zmq_send(publisher, "guessit>gok5?>You are player five.", 35,0);
        zmq_send(publisher, "guessit>gok6?>You are player six.", 34,0);
    break;

    }

    zmq_close (publisher);
    zmq_ctx_destroy (context);
    return 0;
}
