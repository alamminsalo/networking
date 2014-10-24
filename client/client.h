#ifndef clienth
#define clienth

#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <string>
#include <unistd.h>

class TcpClient{
protected:
	char *domain;
   	char *port;
	int status, keepalive;
	struct addrinfo host_info;
	struct addrinfo *host_info_list;
	int socketfd;
	bool connected;
	fd_set data;
	struct timeval time;

public:
	TcpClient();
	TcpClient(const char*, const char*, int);
	~TcpClient();
	void Connect();
	void Disconnect();
	void Write(char*);
	void Read(std::string&);
	bool isConnected();
	void closeSocket();
};

#endif
