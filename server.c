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

    //variabelen
    srand(time(NULL));
    int rnd = rand() % 100 + 1, guessplayer1;
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

    while (1) {
        //ontvang gok
        memset(buffer,0,256);
        zmq_recv (subscriber, buffer, 256, 0);
        ParsedString = parse(3, buffer);
        printf("The player has guessed %s\n", ParsedString);

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
    }

    zmq_close (publisher);
    zmq_ctx_destroy (context);
    return 0;
}
