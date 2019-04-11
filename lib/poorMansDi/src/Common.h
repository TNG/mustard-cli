#ifndef COMMON_H
#define COMMON_H

#define errmsg(message) {fprintf(stderr, "\033[31m[    ERROR ] \033[0m%s\n\t     \033[37m%s:%d\n\t     %s\033[0m\n", message, __FILE__,__LINE__,__PRETTY_FUNCTION__); fflush(stderr);}
#define infomsg(message) {fprintf(stderr,"\033[92m[     INFO ] \033[0m%s\n\t     \033[37m%s:%d\n\t     %s\033[0m\n", message, __FILE__,__LINE__,__PRETTY_FUNCTION__); fflush(stderr);}
#define warnmsg(message) {fprintf(stderr,"\033[33m[     WARN ] \033[0m%s\n\t     \033[37m%s:%d\n\t     %s\033[0m\n", message, __FILE__,__LINE__,__PRETTY_FUNCTION__); fflush(stderr);}

#define errmsgf(format,...) {char tmp[1024];snprintf(tmp,1024,format,__VA_ARGS__);errmsg(tmp);}
#define infomsgf(format,...) {char tmp[1024];snprintf(tmp,1024,format,__VA_ARGS__);infomsg(tmp);}
#define warnmsgf(format,...) {char tmp[1024];snprintf(tmp,1024,format,__VA_ARGS__);warnmsg(tmp);}

#define SINGLE_ARG(...) __VA_ARGS__

#endif
