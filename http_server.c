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

// For malloc
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

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

const char *get_mime_type(const char *file_path);

int main(void)
{
    int socketdf;
    socketdf = socket(AF_INET, SOCK_STREAM, 0);

    if (socketdf < 0)
    {
        perror("Error creating the socket");
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
        perror("Error binding the port and address to the socket");
        return 1;
    }
    if (listen(socketdf, BACKLOG) < 0)
    {
        perror("Error listening for connections");
        return 1;
    }
    printf("Server listening on port %d...\n", PORT);
    // Starting the listening process
    while (1)
    {
        int client_socket;
        struct sockaddr_in client_addr;
        socklen_t addr_size = sizeof(client_addr);

        // Accepts the connection to the address
        client_socket = accept(socketdf, (struct sockaddr *)&client_addr, &addr_size);
        if (client_socket < 0)
        {
            perror("Error: Failed to accept connection");
            continue;
        }

        char buffer_arr[BUFFER] = {0};
        char method[10];
        char path[256];
        char http_version[20];

        ssize_t bytes_received = recv(client_socket, buffer_arr, BUFFER - 1, 0);

        if (bytes_received <= 0)
        {
            close(client_socket);
            continue;
        }

        buffer_arr[bytes_received] = '\0';

        if (sscanf(buffer_arr, "%s %s %s", method, path, http_version) < 2)
        {
            const char *bad_request = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
            send(client_socket, bad_request, strlen(bad_request), 0);
            close(client_socket);
            continue;
        }

        if (strcmp(method, "GET") == 0)
        {
            const char *file_to_open = (strcmp(path, "/") == 0) ? "index.html" : path + 1;

            struct stat st;

            if (stat(file_to_open, &st) == -1)
            {
                const char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nFile Not Found!\n";
                send(client_socket, not_found, strlen(not_found), 0);
                close(client_socket);
                printf("Error: File not found: %s\n", file_to_open);
                continue;
            }

            long file_size = st.st_size;

            FILE *file = fopen(file_to_open, "rb");
            if (file == NULL)
            {
                perror("Error opening file");
                close(client_socket);
                continue;
            }

            char *file_buffer = (char *)malloc(file_size);
            if (file_buffer == NULL)
            {
                const char *internal_error = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\nContent-Length: 22\r\n\r\nMemory allocation failed\n";
                send(client_socket, internal_error, strlen(internal_error), 0);
                fclose(file);
                close(client_socket);
                perror("Error allocating memory for file");
                continue;
            }

            size_t actual_bytes_read = fread(file_buffer, 1, file_size, file);
            fclose(file);

            const char *mime_type = get_mime_type(file_to_open);

            char response_header[BUFFER];
            snprintf(response_header, BUFFER,
                     "HTTP/1.1 200 OK\r\n"
                     "Content-Type: %s\r\n"
                     "Content-Length: %zu\r\n"
                     "Connection: close\r\n"
                     "\r\n",
                     mime_type, actual_bytes_read);

            send(client_socket, response_header, strlen(response_header), 0);
            send(client_socket, file_buffer, actual_bytes_read, 0);

            free(file_buffer);

            printf("Served %s (%s) to client.\n", file_to_open, mime_type);
        }
        else
        {
            const char *not_allowed = "HTTP/1.1 405 Method Not Allowed\r\nContent-Length: 0\r\n\r\n";
            send(client_socket, not_allowed, strlen(not_allowed), 0);
        }

        close(client_socket);
    }
    return 0;
}

const char *get_mime_type(const char *file_path)
{
    const char *dot = strrchr(file_path, '.');
    if (!dot || dot == file_path)
        return "application/octet-stream";

    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
        return "text/html";
    if (strcmp(dot, ".css") == 0)
        return "text/css";
    if (strcmp(dot, ".js") == 0)
        return "application/javascript";
    if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
        return "image/jpeg";
    if (strcmp(dot, ".png") == 0)
        return "image/png";
    if (strcmp(dot, ".gif") == 0)
        return "image/gif";

    return "application/octet-stream";
}