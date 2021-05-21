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
    //filter
    const char *FilterGok = (argc > 1)? argv [1]: "guessit>gok!>";
    const char *FilterGok2 = (argc > 1)? argv [1]: "guessit>gok2!>";
    const char *FilterGok3 = (argc > 1)? argv [1]: "guessit>gok3!>";
    const char *FilterGok4 = (argc > 1)? argv [1]: "guessit>gok4!>";
    const char *FilterGok5 = (argc > 1)? argv [1]: "guessit>gok5!>";
    const char *FilterGok6 = (argc > 1)? argv [1]: "guessit>gok6!>";

    //variabelen
    int gok[6];
    srand(time(NULL));
    int rnd = rand() % 100 + 1;
    char buffer [256];
    char *ParsedString;

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
    }
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok, 8);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok2, 9);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok3, 9);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok4, 9);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok5, 9);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok6, 9);

    while (1) {
        //ontvang gokken
        for(int i = 0; i < 6; i++){
            memset(buffer,0,256);
            zmq_recv (subscriber, buffer, 256, 0);
            ParsedString = parse(3, buffer);
            printf("Player %d has guessed %s\n", i+1, ParsedString);
            gok[i] = atoi(ParsedString);
        }/*
        //ontvang gok1
        memset(buffer,0,256);
        zmq_recv (subscriber, buffer, 256, 0);
        ParsedString = parse(3, buffer);
        printf("Player 1 has guessed %s\n", ParsedString);
        gok[0] = atoi(ParsedString);

        //ontvang gok2
        memset(buffer,0,256);
        zmq_recv (subscriber, buffer, 256, 0);
        ParsedString = parse(3, buffer);
        printf("Player 2 has guessed %s\n", ParsedString);
        gok[1] = atoi(ParsedString);

        //ontvang gok3
        memset(buffer,0,256);
        zmq_recv (subscriber, buffer, 256, 0);
        ParsedString = parse(3, buffer);
        printf("Player 3 has guessed %s\n", ParsedString);
        gok[2] = atoi(ParsedString);

        //ontvang gok4
        memset(buffer,0,256);
        zmq_recv (subscriber, buffer, 256, 0);
        ParsedString = parse(3, buffer);
        printf("Player 4 has guessed %s\n", ParsedString);
        gok[3] = atoi(ParsedString);

        //ontvang gok5
        memset(buffer,0,256);
        zmq_recv (subscriber, buffer, 256, 0);
        ParsedString = parse(3, buffer);
        printf("Player 5 has guessed %s\n", ParsedString);
        gok[4] = atoi(ParsedString);

        //ontvang gok6
        memset(buffer,0,256);
        zmq_recv (subscriber, buffer, 256, 0);
        ParsedString = parse(3, buffer);
        printf("Player 6 has guessed %s\n", ParsedString);
        gok[5] = atoi(ParsedString);
        */

        printf("player 1 %d, player 2 %d, player3 %d, player4 %d, player 5 %d, player 6 %d\n\n", gok[0], gok[1], gok[2], gok[3], gok[4], gok[5]);

        zmq_send(publisher, "guessit>gok?>You are player one.", 32,0);
        zmq_send(publisher, "guessit>gok2?>You are player two.", 33,0);
        zmq_send(publisher, "guessit>gok3?>You are player three.", 35,0);
        zmq_send(publisher, "guessit>gok4?>You are player four.", 34,0);
        zmq_send(publisher, "guessit>gok5?>You are player five.", 35,0);
        zmq_send(publisher, "guessit>gok6?>You are player six.", 34,0);
    break;
        /*
        //stuur restultaat
        guessplayer1 = atoi(ParsedString);
        printf("gok is %d\n", guessplayer1);
        printf("nummer is %d\n", rnd);
        if(guessplayer1 < rnd){
            zmq_send(publisher, "guessit>gok?>Your guess is lower then the number", 49,0);
        } else if(guessplayer1 > rnd){
            zmq_send(publisher, "guessit>gok?>Your guess is higher then the number", 50,0);
        } else if(guessplayer1 == rnd){
            zmq_send(publisher, "guessit>gok?>U WON!", 20,0);
        } else{
            zmq_send(publisher, "guessit>gok?>something wrong!", 30,0);
        }
        break;
        */
    }

    zmq_close (publisher);
    zmq_ctx_destroy (context);
    return 0;
}
