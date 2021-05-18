#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

int main (void)
{
    void *context = zmq_ctx_new();
    void *publisher = zmq_socket(context, ZMQ_PUSH);
    void *subscriber = zmq_socket (context, ZMQ_SUB);

    int rc = zmq_setsockopt(publisher, ZMQ_SUBSCRIBE, "rasit?>",6);

    //connect
    int rp = zmq_connect(publisher, "tcp://benternet.pxl-ea-ict.be:24041");
    int rs = zmq_connect(subscriber, "tcp://benternet.pxl-ea-ict.be:24042");
    //check if connect failed
    if (rp != 0 && rs != 0)
    {
        printf("ERROR: ZeroMQ error occurred during zmq_ctx_new(): %s\n", zmq_strerror(errno));

        return EXIT_FAILURE;
    }

    while (1) {
        char buffer [10];
        zmq_recv (publisher, buffer, 5, 0);
        printf ("Received Hello\n");
        sleep (1);          //  Do some 'work'
        zmq_send (publisher, "World", 5, 0);
    }
    return 0;
}
