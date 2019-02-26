#include "tn_test.h" 



#include "tn_part.h" 

pid_t pid;

void sighandler(int signal) {

    kill(pid, SIGKILL);
    exit (0);
}

int main(int argc, char* argv[]) {

    printf("Programmstart \n");

    // init SIGTERM and SIGINT handling
    signal(SIGTERM, sighandler);
    signal(SIGINT, sighandler);
    
    signal(SIGCHLD,SIG_IGN); 

        tn_in_socket = socket( AF_INET, SOCK_STREAM, 0);
        if (tn_in_socket == -1) { printf ("Error Socket \n"); }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons (7001);
        setsockopt( tn_in_socket, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int) );
        if (bind( tn_in_socket, (struct sockaddr *) &address, sizeof (address)) == 0 ) {
            printf ("Binding Socket OK");
            //logmsg(VERBOSESTARTUP, debug);
        }
		listen (tn_in_socket, 5);
		addrlen = sizeof (struct sockaddr_in);
		save_fd = fcntl( tn_in_socket, F_GETFL );
		save_fd |= O_NONBLOCK;
		fcntl( tn_in_socket, F_SETFL, save_fd );
        while(1) {
//		if ( ! wait4message ) {
			new_tn_in_socket = accept ( tn_in_socket, (struct sockaddr *) &address, &addrlen );
			if (new_tn_in_socket > 0) {
                pid = fork();
                if (pid == 0) {
                    char *buffer =  (char*) malloc (BUF);
                    char client_message[30];
                    //wait4message = true;
                    // send something like a prompt. perl telnet is waiting for it otherwise we get error
                    // use this in perl: my $t = new Net::Telnet (Timeout => 2, Port => 7001, Prompt => '/rf24hub>/');
                    sprintf(client_message,"rf24hub> ");
                    write(new_tn_in_socket , client_message , strlen(client_message));
                    printf ("Client %s ist connected ...\n", inet_ntoa (address.sin_addr));
                    //logmsg(VERBOSECONFIG, debug);
                    //save_fd = fcntl( new_tn_in_socket, F_GETFL );
                    //save_fd |= O_NONBLOCK;
                    //fcntl( new_tn_in_socket, F_SETFL, save_fd );
                    sprintf(buffer,"                                                            ");
                    //while ( recv(new_tn_in_socket, buffer, BUF, 0) < 1 ) {
                    MsgLen = recv(new_tn_in_socket, buffer, BUF, 0);
                    sprintf(client_message,"%s",buffer);
                    write(new_tn_in_socket , client_message , strlen(client_message));
                    printf("Buffer: %s Msglen: %d\n",buffer,MsgLen);
                    //if (MsgLen>0) {
                    //    process_tn_in(new_tn_in_socket, buffer, client_message);
                    close (new_tn_in_socket);
                    exit(0);
                } else {
                    close (new_tn_in_socket);
                }
            }
            printf(".");
            sleep(1);
        }
}

