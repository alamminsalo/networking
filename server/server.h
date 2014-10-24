#ifndef serverh
#define serverh

#include <iostream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

class TcpServer{
protected:
	struct addrinfo host_info;
	struct addrinfo *host_info_list;
	struct sockaddr_storage cl_addr;
	socklen_t addr_size;
	int status;
	int cl_socket[10];
	int socketfd;
	int cl_list_index;
	bool client_online[10];
	bool server_online;

public:
	TcpServer(const char*, int);
	~TcpServer();
	void Listen(int);	
	int Read(int, std::string&);
	void Write(int, char*);
	void Stop();
	void stopSocket(int);
	int getIndex();
	bool operational();
};

#endif
