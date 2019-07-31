#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <netdb.h>

#include <sys/syscall.h>

#define gettid() syscall(SYS_gettid)

#define fatal(...) do {\
		char *_ebuff = malloc(512);\
		snprintf(_ebuff , 512 , "[!!] "__VA_ARGS__) ; \
		if (errno==0){ \
			fputs(_ebuff , stderr); \
			fputs("\n" , stderr); \
		} \
		else \
			perror(_ebuff) ; \
		exit(1);\
	}while(0)

int tcp_connect(const char *addrs , const char *port);
int tcp_listen(const char *addrs , const char *port);
const char *sock_ntop(const struct sockaddr *src , char *dst , int maxsize);
void dump_data(char *buff , long len);


static inline
int Socket(int domain , int type , int proto){
	int ret = 0;
	if ((ret = socket(domain , type , proto)) < 0)
		fatal("Call to socket failed");
	return ret;
}

static inline
int Bind(int fd , const struct sockaddr *addr , socklen_t addrlen){
	int ret = 0;
	if ((ret = bind(fd , addr , addrlen)) < 0)
		fatal("Call to bind failed");
	return ret;
}

static inline
int Inet_pton(int af , const char *src , void *dst){
	int ret = 0;
	if ((ret = inet_pton(af , src , dst)) < 0)
		fatal("Call to inet_pton failed");
	return ret;
}

static inline
int Listen(int sockfd  , int backlog) {
	int ret = 0;
	if ((ret = listen(sockfd , backlog)) < 0)
		fatal("Call to listen failed");
	return ret;
}

static inline
int Connect(int sockfd , const struct sockaddr *addr , socklen_t addrlen){
	int ret = 0;
	if ((ret = connect(sockfd , addr , addrlen)) < 0){
		fatal("Call to connect failed");
	}

	return ret;
}

static inline
int Accept(int sockfd , struct sockaddr *addr , socklen_t *addrlen){
	int ret = 0;
	if ((ret = accept(sockfd , addr , addrlen)) < 0)
		fatal("Call to accept failed");

	return ret;
}

static inline
ssize_t Recvfrom(int sockfd , void *buf , size_t len , int flags,
		struct sockaddr *src_addr , socklen_t *addrlen){
	ssize_t ret = 0;
	if ((ret = recvfrom(sockfd , buf , len , flags , src_addr , addrlen)) < 0){
		fatal("Call to recvfrom failed");
	}
	return ret;

}

static inline
ssize_t Sendto(int sockfd , const void *buf , size_t len , int flags,
		const struct sockaddr *dest_addr , socklen_t addrlen){
	ssize_t ret = 0;
	if ((ret = sendto(sockfd , buf , len , flags , dest_addr , addrlen)) < 0) {
		fatal("Call to sendo failed");
	}
	return ret;
}

static inline
int Select(int nfds , fd_set *readfds , fd_set *writefds , fd_set *exceptfds,
		struct timeval *timeout){
	int ret = 0;
	if ((ret = select(nfds , readfds , writefds , exceptfds , timeout)) < 0){
		fatal("Call to select failed");
	}

	return ret;

}

static inline
void Setsockopt(int sockfd , int level , int optname , void *optval , socklen_t optlen){
	if (setsockopt(sockfd , level , optname , optval , optlen) < 0)
		fatal("Call to setsockopt failed");
}

static inline
void Getsockopt(int sockfd , int level ,int optname, void *optval , socklen_t *optlen) {
	if (getsockopt(sockfd , level , optname , optval , optlen) < 0)
		fatal("Call to getsockopt failed");
}
#endif /* ifndef COMMON_H*/
