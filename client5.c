#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

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
    const char *BerichtGok = (argc > 1)? argv [1]: "guessit>gok5!>";

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
    int rp = zmq_connect(publisher, "tcp://benternet.pxl-ea-ict.be:24041");
    int rs = zmq_connect(subscriber, "tcp://benternet.pxl-ea-ict.be:24042" );

    sleep (1);

    //check if connect failed
    if (rp != 0 && rs != 0)
    {
        printf("ERROR: ZeroMQ error occurred during zmq_ctx_new(): %s\n", zmq_strerror(errno));
        return EXIT_FAILURE;
    } else{
        printf("Connection has been made! Goodluck!\n\n");
    }

    rs = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "guessit>gok5?>", 14);

    //verzend uw gok naar server
    printf("What is your guess? ");
    scanf("%s", &gok);
    strcpy(sendgok, BerichtGok);
    strcat(sendgok, gok);
    zmq_send (publisher, sendgok, strlen(sendgok), 0);


    //ontvang resultaat
    memset(buffer,0,256);
    zmq_recv(subscriber, buffer, 256,0);
    ParsedString = parse(3, buffer);
    printf("%s", ParsedString);

    zmq_close (subscriber);
    zmq_ctx_destroy (context);
    return 0;
}
