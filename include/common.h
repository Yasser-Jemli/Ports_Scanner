#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <stdbool.h>

#define STATUS_SUCCESS 0
#define STATUS_ERROR 1
#define SYS_CALL_ERROR -1
#define LOG_FILE "port_scan_results.log"
#define VERSBOSE_ACTIVE 1 
#define VERBOSE_INACTIVE 0
#define LOCALHOST_SEARCH_SELECTED 1 
#define LOCALHOST_SEARCH_NOT_SELECTED 0
#define IPADDRESS_SEARCH_SELECTED 1
#define IPADDRESS_SEARCH_NOT_SELECTED 0
#define RANGEPORTS_SEARCH_SELECTED 1
#define RANGEPORTS_SEARCH_NOT_SELECTED 0

void print_help_menu(int argc , char *argv[]);
void log_scan_result(const char *ip_address, int port, int status);
void get_timestamp(char *buffer, int size);
void log_message(const char *level, bool verbose_mode, const char *format, ...);

#endif                         