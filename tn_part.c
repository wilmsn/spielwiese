//#include "tn_test.h" 
#include "tn_part.h" 


void process_tn_in(int new_tn_in_socket, char* buffer, char* client_message) {
/* Messages can llook like this:
       <word1		word2		word3		word4 				function>
		init													Reinitialization of rf24hubd (reads actual values from database)
		list		order										Lists open orders in textform
		html 		order										Lists open orders in HTML form
		set			sensor		<sensor#> 	<value>				Sets Sensor to Value (Store in Orderbuffer only)
					node		<node#>		init				Initialize this Node (Send Initdata to the Node) 
					verbose		<value>							Sets Verboselevel to new Value (1...9)
		setlast		sensor		<sensor#> 	<value>				Sets Sensor to Value and starts sending (Store in Orderbuffer and transfer all requests for this Node to Order)
			
*/
			
	char cmp_init[]="init", 
		 cmp_sensor[]="sensor",
		 cmp_set[]="set",
		 cmp_setlast[]="setlast",
		 cmp_node[]="node",
		 cmp_list[]="list",
		 cmp_html[]="html",
		 cmp_order[]="order",	 
		 cmp_verbose[]="verbose";	 
	char *wort1, *wort2, *wort3, *wort4;
	uint16_t node = 0;
	uint32_t akt_sensor = 0;
	bool tn_input_ok=false;
	char delimiter[] = " ";
	trim(buffer);
	sprintf(debug,"Incoming telnet data: %s ",buffer);
	logmsg(VERBOSETELNET, debug);
	wort1 = strtok(buffer, delimiter);
	wort2 = strtok(NULL, delimiter);
	wort3 = strtok(NULL, delimiter);
	wort4 = strtok(NULL, delimiter);
	// set/setlast sensor <sensor> <value>
	// sets a sensor to a value, setlast starts the request over air
	if ( (( strcmp(wort1,cmp_set) == 0 ) || ( strcmp(wort1,cmp_setlast) == 0 )) && (strcmp(wort2,cmp_sensor) == 0) && (wort3 != NULL) && (wort4 != NULL) ) {
		tn_input_ok = true;
		// In word3 we may have a) the number of the sensor b) the name of the sensor c) the fhem_dev of a sensor
		// for the processing we need the number of the sensor ==> find it!
		for (int i = 0; i < SENSORARRAYSIZE; i++) {
			if ( (sensor[i].sensor > 0) && ((strcmp(wort3,sensor[i].fhem_dev) == 0) || ( sensor[i].sensor == strtoul(wort3, &pEnd, 10)) ) ) {
				sprintf(debug, "Sensor found: %u Node: 0%o Channel: %u FHEM: %s", 
								sensor[i].sensor,
								sensor[i].node,
								sensor[i].channel,
								sensor[i].fhem_dev);
				logmsg(VERBOSETELNET, debug);
				akt_sensor = sensor[i].sensor;
			}
		}		
		// just add the sensor to the buffer
		node = set_sensor( akt_sensor, strtof(wort4, &pEnd));
		if ( node == 0 ) {
			sprintf(debug,"Sensor (%s) not in cache ==> running initialisation!",wort3);
			logmsg(VERBOSETELNET, debug);
			init_system();
		} else {
			if ( strcmp(wort1,cmp_setlast) == 0 ) {
				get_order(node);
				print_order_buffer();
			}
		}
	}
    // set node <node> init
	// sends the init sequence to a node
	if (( strcmp(wort1,cmp_set) == 0 ) && (strcmp(wort2,cmp_node) == 0) && (wort3 != NULL) && (strcmp(wort4,cmp_init) == 0) ) {
		tn_input_ok = true;
		init_node(getnodeadr(wort3));
	}
    // set verbose <new verboselevel>
	// sets the new verboselevel
	if (( strcmp(wort1,cmp_set) == 0 ) && (strcmp(wort2,cmp_verbose) == 0) && (wort3 != NULL) && (wort4 == 0) ) {
        if ( wort3[0] > '0' && wort3[0] < '9' + 1 ) {
			tn_input_ok = true;
			verboselevel = (wort3[0] - '0') * 1;
		}	
	}
    // list order
	// lists the current orderbuffer
	if (( strcmp(wort1,cmp_list) == 0 ) && (strcmp(wort2,cmp_order) == 0) && (wort3 == NULL) && (wort4 == NULL) ) {
		tn_input_ok = true;
		sprintf(client_message,"----- Orderbuffer(max(%d): ------\n", (int)ORDERBUFFERLENGTH); 
		write(new_tn_in_socket , client_message , strlen(client_message));
		for (int i=0; i < ORDERBUFFERLENGTH -1; i++) {
			if ( order_buffer[i].node > 0 ) {
				sprintf(client_message,"order_buffer[%d]\t Onr:\t%u,\tentry:\t%llu (%d sec.),\tnode:\t0%o,\tchannel:\t%u\tval:\t%f\n", 
					i,order_buffer[i].orderno, order_buffer[i].entrytime, (int)(order_buffer[i].entrytime - mymillis())/1000, order_buffer[i].node,
					order_buffer[i].channel, order_buffer[i].value );
				write(new_tn_in_socket , client_message , strlen(client_message));
			}				
		}
		sprintf(client_message,"----- Order(max %d): ------\n", (int)ORDERLENGTH); 
		write(new_tn_in_socket , client_message , strlen(client_message));
		for (int i=0; i < ORDERLENGTH -1; i++) {
			if ( order[i].node > 0 ) {
				sprintf(client_message,"order[%d]\t Onr:\t%u,\tentry:\t%llu (%d sec.),\tnode:\t0%o,\ttype:\t%u\tflags:\t%u\t(channel/Value)\t(%u/%f)\t(%u/%f)\t(%u/%f)\t(%u/%f)\n", 
					i,order[i].orderno, order[i].entrytime, (int)(order[i].entrytime - mymillis())/1000, order[i].node, order[i].type, order[i].flags
					,order[i].channel1, order[i].value1
					,order[i].channel2, order[i].value2
					,order[i].channel3, order[i].value3
					,order[i].channel4, order[i].value4);
				write(new_tn_in_socket , client_message , strlen(client_message));
			}				
		}
	}	
    // html order
	// lists the current order/orderbuffer for html page
	if (( strcmp(wort1,cmp_html) == 0 ) && (strcmp(wort2,cmp_order) == 0) && (wort3 == NULL) && (wort4 == NULL) ) {
		tn_input_ok = true;
		sprintf(client_message,"\n<center><big>Orderbuffer</big><table><tr><th>OrderNo</th><th>EntryTime</th><th>Node</th><th>Channel</th><th>Value</th></tr>\n"); 
		write(new_tn_in_socket , client_message , strlen(client_message));
		for (int i=0; i < ORDERBUFFERLENGTH -1; i++) {
			if ( order_buffer[i].node > 0 ) {
				sprintf(client_message,"<tr><td>%u</td><td>%llu (%d sec.)</td><td>0%o</td><td>%u</td><td>%f</td></tr>\n", 
					order_buffer[i].orderno, order_buffer[i].entrytime, (int)(order_buffer[i].entrytime - mymillis())/1000, order_buffer[i].node,
					order_buffer[i].channel, order_buffer[i].value );
				write(new_tn_in_socket , client_message , strlen(client_message));
			}				
		}
		sprintf(client_message,"</table><br><big>Order</big><br><table><tr><th>OrderNo</th><th>EntryTime</th><th>Node</th><th>Type</th><th>Flags</th><th>Channel</th><th>Value</th></tr>\n"); 
		write(new_tn_in_socket , client_message , strlen(client_message));
		for (int i=0; i < ORDERLENGTH -1; i++) {
			if ( order[i].node > 0 ) {
				sprintf(client_message,"<tr><td>%u</td><td>%llu (%d sec.)</td><td>0%o</td><td>%u</td><td>%u</td><td>%u<br>%u<br>%u<br>%u</td><td>%f<br>%f<br>%f<br>%f</td></tr>\n", 
					order[i].orderno, order[i].entrytime, (int)(order[i].entrytime - mymillis())/1000, order[i].node, order[i].type, order[i].flags
					,order[i].channel1, order[i].channel2, order[i].channel3, order[i].channel4
					,order[i].value1, order[i].value2, order[i].value3, order[i].value4);
				write(new_tn_in_socket , client_message , strlen(client_message));
			}				
		}
		sprintf(client_message,"</table></center>\n"); 
		write(new_tn_in_socket , client_message , strlen(client_message));
	}	
    // init
	// initialisation of rf24hubd: reloads data from database
	if (( strcmp(wort1,cmp_init) == 0 ) && (wort2 == NULL) && (wort3 == NULL) && (wort4 == NULL) ) {
		tn_input_ok = true;
		init_system();
	}
	if ( ! tn_input_ok) {
		sprintf(client_message,"Usage:\n");
		write(new_tn_in_socket , client_message , strlen(client_message));
		sprintf(client_message,"set[last] sensor <sensornumber> <sensorvalue>\n");
		write(new_tn_in_socket , client_message , strlen(client_message));
		sprintf(client_message,"   Sets the sensor <sensornumber> to the value <sensorvalue>\n");
		write(new_tn_in_socket , client_message , strlen(client_message));
		sprintf(client_message,"   'set' stores only; 'setlast' executes all settings for this node\n");
		write(new_tn_in_socket , client_message , strlen(client_message));
		sprintf(client_message,"set node <nodenumber> init\n");
		write(new_tn_in_socket , client_message , strlen(client_message));
		sprintf(client_message,"   Inits the Node <nodenumber>. Use <nodenumber> like '041'\n");
		write(new_tn_in_socket , client_message , strlen(client_message));
		sprintf(client_message,"init \n");
		write(new_tn_in_socket , client_message , strlen(client_message));
		sprintf(client_message,"   Reinitialisation of rf24hub (all open ordes will be deleted)\n");
		write(new_tn_in_socket , client_message , strlen(client_message));
		sprintf(client_message,"list order\n");
		write(new_tn_in_socket , client_message , strlen(client_message));
		sprintf(client_message,"   lists the content of the order queue\n");
		write(new_tn_in_socket , client_message , strlen(client_message));
		sprintf(client_message,"set verbose <verboselevel>\n");
		write(new_tn_in_socket , client_message , strlen(client_message));
		sprintf(client_message,"   sets a new verboselevel <1..9> are valid levels\n");
		write(new_tn_in_socket , client_message , strlen(client_message));
		sprintf(client_message,"\n");
		write(new_tn_in_socket , client_message , strlen(client_message));
		sprintf(client_message,"%s version %s\n", PRGNAME, PRGVERSION);
		write(new_tn_in_socket , client_message , strlen(client_message));
	} else {
		sprintf(client_message,"Command received => OK\n");
		write(new_tn_in_socket , client_message , strlen(client_message));
	}		
    
}
