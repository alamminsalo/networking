#ifndef serverh
#define serverh

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <string>
#include <stdlib.h>

class TcpServer{
protected:	
	struct timeval timer;
	int status;
	int socketfd;
	bool online;
	fd_set data;
	TcpThread *thread;
	struct addrinfo host_info;
	struct addrinfo *host_info_list;
	int max_cl;	

public:
	TcpServer(const char*, int);
	~TcpServer();
	void sendThread
};

class TcpThread:{
protected:
	struct timeval timer;
	int status;
	int socketfd;
	bool online;
	fd_set data;
	int id;
	int host_sock;
	char buf[8];
	std::string msg;
	struct sockaddr_storage cl_addr;
	socklen_t addr_size;
	void buildMsg(char*&);
	char *clientname;

public:
	TcpThread(int, int);
	~TcpThread();
	void Listen();	
	int Read();
	void Write(const char*);
};

#endif
