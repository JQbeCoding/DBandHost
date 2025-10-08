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

/* This library is included for AF_INET which defines addresses like IPv4 or IPv6,
and includes the sockaddr_in library*/
#include <arpa/inet.h>

// For File reading
#include <unistd.h>

// For string functions
#include <string.h>

// Macro to define the port number that will be used throughout the file.
#define PORT 8080
// Macro for the backlog variable. (Should be changed to 1024 for production)
#define BACKLOG 10
// Macro for buffer size
#define BUFFER 1024

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
        return 1;
    }

    struct sockaddr_in socket_address;
    // Defines the type of IP address needed
    socket_address.sin_family = AF_INET;
    // Allows requests to be handled from any address
    socket_address.sin_addr.s_addr = INADDR_ANY;
    // Uses my port declared from my macro and htons to
    // ensure correct byte order for reading
    socket_address.sin_port = htons(PORT);

    // Binding the socket to the address and port
    int binded_socket = bind(socketdf, (struct sockaddr *)&socket_address, sizeof(socket_address));
    if (binded_socket < 0)
    {
        printf("There was an error with the binding the port and adress to the socket.\n");
        return 1;
    }
    if (listen(socketdf, BACKLOG) < 0)
    {
        printf("There is an error listening for connections\n");
    }
    printf("Server listening on port %d...\n", PORT);
    while (1)
    {

        int client_socket;
        socklen_t addr_size = sizeof(socket_address);
        client_socket = accept(socketdf, (struct sockaddr *)&socket_address, &addr_size);
        if (client_socket < 0)
        {
            printf("Error: Failed to accept connection\n");
            continue;
        }
        char buffer_arr[BUFFER];
        recv(client_socket, buffer_arr, BUFFER, 0);
        if (strncmp(buffer_arr, "GET ", 4) == 0)
        {
            // Using file to open the index page of the directory
            FILE *html_file = fopen("index.html", "r");
            if (html_file == NULL)
            {
                printf("Error reading file: File doesn't exist\n");
                close(client_socket);
                continue;
            }


            // Defining a larger file buffer for the actual content of the file
            char file_content_buffer[8192];

            // Reads up to the size of the file but no more bytes than the actual buffer
            size_t bytes_read = fread(file_content_buffer, 1, sizeof(file_content_buffer) - 1, html_file);

            // The size of content sent is in bytes and not the actual buffer size
            // It ends with a null terminator
            file_content_buffer[bytes_read] = '\0';
            fclose(html_file);

            char response_header[512];
            snprintf(response_header, sizeof(response_header),
                     "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %zu\r\n\r\n",
                     bytes_read);

            send(client_socket, response_header, strlen(response_header), 0);
            send(client_socket, file_content_buffer, bytes_read, 0);

            printf("Served index.html to client.\n");
        }
        close(client_socket);
    }
    return 0;
}