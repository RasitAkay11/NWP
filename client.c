#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

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
    //bericht
    const char *BerichtGok = (argc > 1)? argv [1]: "guessit>gok1!>";
    bool playing = true;
    int goki = 0;
    char gok[10];
    char sendgok[100];
    char buffer[256];
    char *ParsedString;
    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUSH);
    void *subscriber = zmq_socket (context, ZMQ_SUB);

    //intro
    printf("The rules are easy.. You will race against 5 other people!\nThe person who will be the closest to the random number every 6 rounds wins! Except if someone already guesses it!\n\nMaking connection with the server...\n");

    //connect
    int rp = zmq_connect(publisher, "tcp://benternet.backup.pxl-ea-ict.be:24041");
    int rs = zmq_connect(subscriber, "tcp://benternet.backup.pxl-ea-ict.be:24042" );

    sleep (1);

    //check if connect failed
    if (rp != 0 && rs != 0)
    {
        printf("ERROR: ZeroMQ error occurred during zmq_ctx_new(): %s\n", zmq_strerror(errno));
        return EXIT_FAILURE;
    } else{
        printf("Connection has been made! Goodluck!\n\n");
    }

    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "guessit>gok1?>", 14);

    while(playing == true){
    //Ontvang vraag en verstuur antwoord
    memset(buffer,0,256);
    zmq_recv(subscriber, buffer, 256,0);
    ParsedString = parse(3, buffer);
    printf("%s\n", ParsedString);
    printf("Fill in your guess between 0 and 100: ");
    scanf("%s", gok);
    goki = atoi(gok);
    while(((goki < 0) || (goki > 100))){
        printf("\n");
        printf("The service said between 0 and 100 u stupid fuck try again: ");
        scanf("%s", gok);
        goki = atoi(gok);
    }
    strcpy(sendgok, BerichtGok);
    strcat(sendgok, gok);
    zmq_send(publisher, sendgok, strlen(sendgok), 0);

    //ontvang resultaat
    memset(buffer,0,256);
    zmq_recv(subscriber, buffer, 256,0);
    ParsedString = parse(3, buffer);
    printf("%s\n", ParsedString);
    if((strcmp(ParsedString, "Sadly, you lost.")) == 1){
        zmq_setsockopt(subscriber, ZMQ_UNSUBSCRIBE, "guessit>gok1?>", 14);
    }
    printf("\n");
    }
    zmq_close (subscriber);
    zmq_ctx_destroy (context);
    return 0;
}
