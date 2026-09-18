#include <asm-generic/socket.h>
#include <locale.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <unistd.h>

#define DEFAULT_SERVER_PORT 8000
#define REDIS_SERVER_HOST "127.0.0.1"

char redis_host_ip[32];

int setup_listening_socket(int port)
{
    int sock;
    struct sockaddr_in server_addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // this enables reuse of the socket address
    // otherwise it will go to TIME_WAIT state
    int enable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsocketopt(SO_REUSEADDR)");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // assign socket to an ip address and port - which is done by bind function
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind() failed");
        exit(EXIT_FAILURE);
    }

    // listens request to the socket on the assigned address
    if (listen(sock, 10) < 0)
    {
        perror("Listen failed");
    }

    return sock;
}

void handle_client(int client_socket) {}

void enter_server_loop(int server_socket)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    while (1)
    {
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr,
                                   &client_addr_len);
        if (client_addr < 0)
        {
            perror("accept() failed");
        }
        handle_client(client_socket);
        close(client_socket);
    }
}

void print_stats(int signo)
{
    struct rusage rusagebuf;
    getrusage(RUSAGE_SELF, &rusagebuf);
    printf("\nUser time: %lds %ldms, System time: %lds %ldms\n",
           rusagebuf.ru_utime.tv_sec, rusagebuf.ru_utime.tv_usec / 1000,
           rusagebuf.ru_stime.tv_sec, rusagebuf.ru_stime.tv_usec / 1000);
    exit(0);
}

int main(int argc, char *argv[])
{

    int server_port;

    // gets the server port
    if (argc > 1)
    {
        server_port = atoi(argv[1]);
    }
    else
    {
        server_port = DEFAULT_SERVER_PORT;
    }

    // gets the redis host ip
    if (argc > 2)
    {
        strcpy(redis_host_ip, argv[2]);
    }
    else
    {
        strcpy(redis_host_ip, REDIS_SERVER_HOST);
    }

    // setup socket
    int server_socket = setup_listening_socket(server_port);

    // connect to redis
    connect_to_redis_server(redis_host_ip);

    setlocale(LC_NUMERIC, "");
    printf("Server is listening on port %d\n", server_port);

    signal(SIGINT, print_stats);
    enter_server_loop(server_socket);
    return 0;
}
