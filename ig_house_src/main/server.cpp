
#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdkconfig.h"

// #include "esp_misc.h"
// #include "esp_sta.h"
// #include "esp_system.h"

#include "freertos/task.h"
#include <sys/socket.h>

#include "cJSON.h"



/* TODO
1. Construct API:
	* Server(virt, impl logic) <- (derived, impl phy): tcp, udp, can bus, LoRa 
		> threads:
		accept conn, receive data to queue
		transmit data to queue, close connection
		close sock by  keep-alive rule
		____
		> data:
		list of open connections
			descr
			last acces time
			keep-alive rule: zero, some time, unlim
			possible: statistic(heap)
		rx/tx queue
		in derived: addr, port, etc..
		____
		> logic:
		api for close conn from timeout list
	____
	* ModuleRouter
		> data:
		list of module's
		system module
		 * system log
		 * settings
		 * os system data
		 * error notifier
		queues acces from modules
		rx/tx queue pointers
		____
		> logic:
		json parsing
		client/server message routing
		furure: auth
	____
	* Module(virt, impl routines) <- (derived, impl module logic)
		> logic
		json processing
		notifying
		____
		> data
		Notifier's list
		Calc
		____
		> derived example:
		hw: button, switch, step motor, led, lcd, etc...
		sw: from timer tooooo.. pyton module =)
	____
	* Client:
		> threads:
		opening conn
		____
		> logic
		response waiting
		retrying
		____
		> data
		list of open connections
			descr
			last acces time
			keep-alive rule: zero, some time, unlim
			possible: statistic(heap)
		rx/tx queue
		in derived: addr, port, etc..
	____
	* Calc
		> logic:
		subscribtions: request notifing
		recalc on new value
		____
		> data
		list of all last values






*/



JsonServer::JsonServer(bool is_bcast, QueueHandle_t out_queue)
{

}

JsonServer::~JsonServer()
{

}

int JsonServer::start(uint16_t port)
{
	struct sockaddr_in serv_addr;
    int nport = htons((u_short)port);

	if((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket() error");
		return -1;
	}

    bzero(&serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = nport;

	if(bind(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr))==-1) {
		perror("bind() error");
		return -1;
	}

	fprintf(stderr, "Server is ready: %s\n", inet_ntoa(serv_addr.sin_addr));

	if (listen(s,5) == -1) {
		perror("listen() error");
		return -1;
	}

    return 0;
}

int JsonServer::run()
{
    int addrlen, ns;
    struct sockaddr_in clnt_addr;
    char buf[80];

   	while(1){

		bzero(&clnt_addr, sizeof(clnt_addr));
		addrlen = sizeof(clnt_addr);

		if ((ns = accept(s, (struct sockaddr *)&clnt_addr, (socklen_t*)&addrlen))==-1) {
			perror("accept() error");
            return -1;
		}

		fprintf(stderr, "Client = %s\n", inet_ntoa(clnt_addr.sin_addr));

	//////	if((pid=fork())==-1) {
	///		perror("fork() error");
		//	exit(1);
		//}

	//	if(pid==0) {
        int nbytes;

    //	close(s);
        while((nbytes = recv(ns, buf, sizeof(buf), 0))!=0) 
        {

            cJSON *json = cJSON_Parse(buf);

            if(!json){
                perror("JSON parse error!\n");
                continue;
            }

            char *string = cJSON_Print(json);

            send(ns, string, strlen(string), 0);

            cJSON_Delete(json);
            free(string);			
        }
		
    printf("client disconnected!\n");

		close(ns);
	}

    return -1;

}


void JsonServer::stop()
{

}