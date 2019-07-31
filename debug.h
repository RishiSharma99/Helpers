#ifndef DEBUG__H
#define DEBUG__H

#include <stdio.h>

#ifdef DEBUG

#define BT_BUF_SIZE 15

#include <execinfo.h>
#define BUG(expr) \
    do{ \
        if(!(expr)) \
            ; \
        else {\
            fprintf(stderr , "[**] BUG(%s) in %s:%d :%s()\n\n",#expr, __FILE__,\
					__LINE__ ,__func__); \
            int __n; \
            void *buffer[BT_BUF_SIZE]; \
            __n = backtrace(buffer , BT_BUF_SIZE); \
            backtrace_symbols_fd(buffer , __n , 2); \
            exit(EXIT_FAILURE); \
        } \
    }while(0)

#define debug(fmt,...) do{ \
        printf("[%s:%s():%d] "fmt , __FILE__ , __func__ , __LINE__ , ##__VA_ARGS__); \
        puts(""); \
    }while(0)
#else
#define debug(...)
#define BUG(...)
#endif /*DEBUG*/
static inline
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
#endif
