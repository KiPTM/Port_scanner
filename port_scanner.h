#ifndef PORT_SCANNER_H
#define PORT_SCANNER_H

// Function to parse command line arguments
int parse_arguments(int argc, char *argv[], char *target_ip, int *start_port, int *end_port);

// Function to scan ports
void scan_ports(char *target_ip, int start_port, int end_port);

void scan_ports_concurrent(char *target_ip, int start_port, int end_port, int num_threads);

#endif // PORT_SCANNER_H
