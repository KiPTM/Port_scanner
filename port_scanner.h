#ifndef PORT_SCANNER_H
#define PORT_SCANNER_H

// Function to parse command line arguments
int parse_arguments(int argc, char *argv[], char *target_ip, int *start_port, int *end_port);

// Function to scan ports
void scan_ports(char *target_ip, int start_port, int end_port);

#endif // PORT_SCANNER_H
