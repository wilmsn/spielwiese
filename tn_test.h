#include "tn_part.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string.h> 
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <syslog.h>
#include <signal.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <fcntl.h>


#define DEBUGSTR "DEBUG: "
#define DEBUGSTRINGSIZE 500
#define BUF 1024
// Verboselevel
#define VERBOSECRITICAL 1
#define VERBOSESTARTUP 2
#define VERBOSECONFIG 5
#define VERBOSERF24 6
#define VERBOSETELNET 7
#define VERBOSESQL 8
#define VERBOSEOTHER 9

#define PARAM_MAXLEN 80
#define PARAM_MAXLEN_CONFIGFILE 40
#define PARAM_MAXLEN_LOGFILE 40
#define PARAM_MAXLEN_PIDFILE 40
#define PARAM_MAXLEN_RF24NETWORK_CHANNEL 4
#define PARAM_MAXLEN_RF24NETWORK_SPEED 10
#define PARAM_MAXLEN_HOSTNAME 20
#define PARAM_MAXLEN_DB_SCHEMA 20
#define PARAM_MAXLEN_DB_USERNAME 20
#define PARAM_MAXLEN_DB_PASSWORD 20

int verboselevel = 2;
int sockfd;
bool start_daemon=false, tn_host_set = false, tn_port_set = false, tn_active = false, in_port_set = false, order_waiting = false;
char logfilename[300];
char tn_hostname[20], tn_portno[7];
struct sockaddr_in serv_addr;
struct hostent *server;
FILE * pidfile_ptr;
FILE * logfile_ptr;
char* pEnd;

uint64_t start_time;

	/* vars for telnet socket handling */
int tn_in_socket, new_tn_in_socket, MsgLen;
socklen_t addrlen;
struct sockaddr_in address;
long save_fd;
const int y = 1;
bool wait4message = false;

char debug[DEBUGSTRINGSIZE];
    
    
    
    
struct config_parameters {
  char logfilename[PARAM_MAXLEN_LOGFILE];
  char pidfilename[PARAM_MAXLEN_PIDFILE];
  char db_hostname[PARAM_MAXLEN_HOSTNAME];
  int db_port;
  char db_schema[PARAM_MAXLEN_DB_SCHEMA];
  char db_username[PARAM_MAXLEN_DB_USERNAME];
  char db_password[PARAM_MAXLEN_DB_PASSWORD];
  char telnet_hostname[PARAM_MAXLEN_HOSTNAME];
  int telnet_port;
  int incoming_port;
//  rf24_datarate_e rf24network_speed;
//  uint8_t rf24network_channel;
};

struct config_parameters parms;


int main(int argc, char* argv[]);

    
