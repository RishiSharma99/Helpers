#include "common.h"
#include "debug.h"
#include <string.h>
#include <ctype.h>


int tcp_connect(const char *addrs , const char *port) {
	debug("Connecting to %s:%s" , addrs , port);
	int sock , err;
	struct addrinfo hints = {} , *res , *p;
	hints.ai_socktype= SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	
	if ((err = getaddrinfo(addrs , port , &hints , &res))) {
		fatal("tcp_connect failed %s:%s : %s\n" , addrs , port , gai_strerror(err));
	}
	debug("Getaddrinfo successfull");
	p = res;
	do {
		debug("ptr == %p" , p );
		if ((sock = socket(p->ai_family , p->ai_socktype , p->ai_protocol)) < 0)
			continue;
		debug("socket created");
		if (!connect(sock , p->ai_addr , p->ai_addrlen))
			break;
		debug("connect failed");
		close(sock);
	} while((p=p->ai_next)!=NULL);

	if (p==NULL)
		fatal("tcp_connect failed %s:%s " , addrs , port);

	freeaddrinfo(res);
	return sock;
}

int tcp_listen(const char *addrs , const char *port){
	int sock , err;
	const int on = 1;
	struct addrinfo hints = {}, *res , *p;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;

	if ((err = getaddrinfo(addrs , port , &hints ,&res))) {
		fatal("tcp_listen failed %s:%s %s\n" , addrs , port , gai_strerror(err));
	}
	p = res;
	do {
		if ((sock = socket(p->ai_family , p->ai_socktype , p->ai_protocol)) < 0)
			continue;
		Setsockopt(sock , SOL_SOCKET , SO_REUSEADDR , (void *)&on , sizeof(on));
		if (!bind(sock , p->ai_addr , p->ai_addrlen))
			break;
		close(sock);
	} while((p=p->ai_next)!=NULL);

	if (p==NULL)
		fatal("tcp_listen failed %s:%s " , addrs , port);

	freeaddrinfo(res);
	Listen(sock , 5);

	return sock;
}

void dump_data(char *buff , long len){
	for(long i=0 ; i<len ; i+= 16){
		printf("0x%08lx | " , i);
		for(int j = 0 ; j<16 ; j++){
			if (i+j < len) printf("%02hhx " , buff[i+j]);
			else printf("   ");
		}
		printf("| ");
		for(int j = 0 ; j<16 ; j++){
			if (i+j < len && isgraph(buff[i+j])) printf("%c" , (unsigned char)buff[i+j]);
			else printf(".");
		}
		puts("");
	}
}
const char *sock_ntop(const struct sockaddr *src , char *dst , int maxsize){
	char temp[128] = {};
	switch(src->sa_family){
		case AF_INET:
			snprintf(dst , maxsize, "%s:%d" ,
					inet_ntop(AF_INET , &((struct sockaddr_in *)src)->sin_addr , temp , INET_ADDRSTRLEN) , 
					ntohs(((struct sockaddr_in *)src)->sin_port));
			return dst;
		case AF_INET6:
			snprintf(dst , maxsize, "%s:%d",
					inet_ntop(AF_INET6 , &((struct sockaddr_in6 *)src)->sin6_addr , temp , INET6_ADDRSTRLEN) ,
					ntohs(((struct sockaddr_in6 *)src)->sin6_port));
			return dst;
	}
	return NULL;
}

