#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "port_scanner.h"

// Structure to pass arguments to thread function
struct ThreadArgs {
    char *target_ip;
    int start_port;
    int end_port;
};

int parse_arguments(int argc, char *argv[], char *target_ip, int *start_port, int *end_port) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <target_ip> <start_port> <end_port>\n", argv[0]);
        return -1;
    }

    strncpy(target_ip, argv[1], 16);
    *start_port = atoi(argv[2]);
    *end_port = atoi(argv[3]);

    return 0;
}

// Function to scan ports in a range
void *scan_ports_range(void *args) {
    struct ThreadArgs *thread_args = (struct ThreadArgs *)args;

    int sock;
    struct sockaddr_in target_addr;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set target address
    target_addr.sin_family = AF_INET;
    target_addr.sin_addr.s_addr = inet_addr(thread_args->target_ip);

    // Scan ports
    for (int port = thread_args->start_port; port <= thread_args->end_port; ++port) {
        target_addr.sin_port = htons(port);

        // Connect to port
        if (connect(sock, (struct sockaddr *)&target_addr, sizeof(target_addr)) < 0) {
            printf("Port %d is closed\n", port);
        } else {
            printf("Port %d is open\n", port);
        }
    }

    // Close socket
    close(sock);

    return NULL;
}

// Function to scan ports using multiple threads
void scan_ports_concurrent(char *target_ip, int start_port, int end_port, int num_threads) {
    // Calculate ports per thread
    int ports_per_thread = (end_port - start_port + 1) / num_threads;

    // Create threads
    pthread_t threads[num_threads];
    struct ThreadArgs thread_args[num_threads];

    for (int i = 0; i < num_threads; ++i) {
        // Calculate port range for thread
        int thread_start_port = start_port + i * ports_per_thread;
        int thread_end_port = (i == num_threads - 1) ? end_port : thread_start_port + ports_per_thread - 1;

        // Set thread arguments
        thread_args[i].target_ip = target_ip;
        thread_args[i].start_port = thread_start_port;
        thread_args[i].end_port = thread_end_port;

        // Create thread
        if (pthread_create(&threads[i], NULL, scan_ports_range, (void *)&thread_args[i]) != 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }

    // Join threads
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
}
