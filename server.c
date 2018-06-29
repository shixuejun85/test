#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

// ユーザ登録機能
#define USERS_CREATE_URL        	 "/api/lib/v1/users/create"

// ビーコンデータPOST機能
#define LOCATIONS_CREATE_URL      "/api/lib/v1/locations/create"

// 動的パラメータ取得機能URL
#define CLIENT_SETTING_URL    	 	"/api/lib/v1/client_settings"

// 一定間隔でKeepAliveを送信する機能
#define PING_URL    	 						"/api/lib/v1/ping"

// IPアドレス等のデータを送信する機能
#define IP_PUSH_URL    	 					"/api/lib/v1/receiver_statuses/create"

int parseApi(char* buf); 
int parseApi(char* buf) {
	if(0 == strncmp(buf + 4, USERS_CREATE_URL, 24))
		return 0;
	return -1;
}

int main(void) {
 
    int rsock, wsock;
    struct sockaddr_in addr, client;
    int len;
    int ret;
    
    /* make socket */
    rsock = socket(AF_INET, SOCK_STREAM, 0);
 
    if (rsock < 0) {
        fprintf(stderr, "Error. Cannot make socket\n");
        return -1;
    }
    
    /* socket setting */
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;
 
    /* binding socket */    
    ret = bind(rsock, (struct sockaddr *)&addr, sizeof(addr));
    
    if (ret < 0) {
        fprintf(stderr, "Error. Cannot bind socket\n");
        return -1;
    }
 
    /* listen socket */
    listen(rsock, 5);
	char buf[1024];
	while(1){ 
    	/* accept TCP connection from client */
    	len = sizeof(client);
   		 wsock = accept(rsock, (struct sockaddr *)&client, &len);
		
		// 送られてきたメッセージ(COMMAND)を受け取ります
        memset(buf,0,1024);
        recv(wsock,buf,1024,0);
        if (buf[0]=='\0')   strcpy(buf,"NULL");
        puts(buf);
		int ret = parseApi(buf);
		if(0 == ret)
   		 /* send message */
    	write(wsock, "USERS_CREATE_URL\n", sizeof(USERS_CREATE_URL));
		else
    	write(wsock, "test HTTP1.1 200 OK", 14);
 
    	/* close TCP session */
    	close(wsock);
	}
    close(rsock);
 
    return 0;
}
