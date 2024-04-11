#include <stdio.h>
#include <stdlib.h>
#include "port_scanner.h"

int main(int argc, char *argv[]) {
    char target_ip[16];
    int start_port, end_port, num_threads;

    // Parse arguments
    if (parse_arguments(argc, argv, target_ip, &start_port, &end_port) < 0) {
        return EXIT_FAILURE;
    }

    // Set the default number of threads
    num_threads = 4; // You can adjust this value as needed

    // Scan ports concurrently
    scan_ports_concurrent(target_ip, start_port, end_port, num_threads);

    return EXIT_SUCCESS;
}
