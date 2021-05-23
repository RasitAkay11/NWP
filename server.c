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
    const char *FilterGok = (argc > 1)? argv [1]: "guessit>gok!>";
    const char *FilterGok2 = (argc > 1)? argv [1]: "guessit>gok2!>";
    const char *FilterGok3 = (argc > 1)? argv [1]: "guessit>gok3!>";
    const char *FilterGok4 = (argc > 1)? argv [1]: "guessit>gok4!>";
    const char *FilterGok5 = (argc > 1)? argv [1]: "guessit>gok5!>";
    const char *FilterGok6 = (argc > 1)? argv [1]: "guessit>gok6!>";

    //variabelen
    srand(time(NULL));
    int gok[6], rnd = rand() % 100 + 1,highest = -100, charlentgh, lowest = 100, h, l; //h = highest player l = lowest player
    char *VraagGok [6][36], buffer[256], *ParsedString;

    //verzendlijst
    VraagGok[0][34] = "guessit>gok?>What is your guess?: ";
    VraagGok[1][35] = "guessit>gok2?>What is your guess?: ";
    VraagGok[2][35] = "guessit>gok3?>What is your guess?: ";
    VraagGok[3][35] = "guessit>gok4?>What is your guess?: ";
    VraagGok[4][35] = "guessit>gok5?>What is your guess?: ";
    VraagGok[5][35] = "guessit>gok6?>What is your guess?: ";

    printf("Starting the service...\n");

    //connect
    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUSH);
    void *subscriber = zmq_socket (context, ZMQ_SUB);
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
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok, 8);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok2, 9);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok3, 9);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok4, 9);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok5, 9);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, FilterGok6, 9);

    //Momenteel is het de bedoeling dat de server en client1 alleen communiceren. Als client1 werkt komen de rest, dat is hoe ik werk.
    //server stuurt vraag. client ontvangt. client stuurt antw. server ontvangt zijn eige vraag, MAAR dat is niet wat door de broker gaat. Wat er door de broker gaat klopt.
    //Server ontvangt bij lijn 99, lijn 94. Die receive werkt precies niet.
    //Om code te runnen -> eerst client.c runnen dan server.c
    while (1) {
        sleep(2);
        //Vraag een per een hun gok en ontvang gokken. Nummer ze
        for(int i = 0; i < 1; i++){
            if( i < 0){
                charlentgh = 35;
            } else{
                charlentgh = 34;
            }
            //stel uw vraag aan player[i]
            zmq_send(publisher, VraagGok[i][charlentgh], charlentgh,0);
            //sleep(4);

            //ontvang uw antwoord van player[i];
            memset(buffer,0,256);
            zmq_recv(subscriber, buffer, 256, 0);
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
            //r = h;
        }
        else if(highest < 0 && lowest < 0){
            printf("\nplayer %d guessed %d, but it is the farest, bye bye!\n", l, lowest+rnd);
            //r = l;
        }
        else if( (highest < 0 && lowest > 0) || (highest > 0 && lowest < 0)){
            int result = lowest + highest;
            if(result > 0){
                printf("\nplayer %d guessed %d, but it is the farest, bye bye!\n", h, highest+rnd);
                //r = h;
            }
            else{
                printf("\nplayer %d guessed %d, but it is the farest, bye bye!\n", l, lowest+rnd);
                //r = l;
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
