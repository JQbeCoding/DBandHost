/**
 * This file is an http server written in C to host the contents of this
 * directory for me. This will also respond to requests from the html
 * pages, along with queries from the database. All in all this will be
 * the backend of this directory. For the sake of data not getting lost
 * we will be using TCP
 *
 * @author Ja'Quis Franklin, Developer at MayDayz Smokn BBQ
 * @date 10/07/2025
 */

/*First notable thing is the introduction of Unix system APIs like Posix
instead of a library to handle these request.
*/

#include <stdio.h>
// This library is used to create a socket
#include <sys/socket.h>
// This library is included for AF_INET which defines addresses like IPv4 or IPv6
#include <arpa/inet.h>

// Macro to define the port number that will be used throughout the file.
#define PORT 8080


/**This struct is used to define the structure of an AF_INET structure.
 struct sockaddr_in
 {
     short sin_family;
     in_port_t sin_port;
     struct in_addr sin_addr;
     char sin_zero[8];
};*/

int main(void)
{
     int socketdf;
    socketdf = socket(AF_INET, SOCK_STREAM, 0);
    if (socketdf < 0)
    {
        printf("There was an error creating the socket.\n");
    }

    struct sockaddr_in socket_address;
    //Allows requests to be handled from any address
    socket_address.sin_addr.s_addr = INADDR_ANY;
    //Uses my port declared from my macro
    socket_address.sin_port = PORT;
    

    return 0;
}