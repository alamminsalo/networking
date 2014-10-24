#include "client.h"

TcpClient::TcpClient(){};

TcpClient::TcpClient(const char *domain, const char *port, int keepalive){
	memset(&host_info, 0, sizeof host_info);
	host_info.ai_family = AF_UNSPEC;
	host_info.ai_socktype = SOCK_STREAM;
	time.tv_sec = 0;
	time.tv_usec = 3333;

	this->domain = new char[strlen(domain) +1];
   	strcpy(this->domain, domain);	
	this->port = new char[strlen(port) +1];
	strcpy(this->port,port);
	status = getaddrinfo(this->domain, this->port, &host_info, &host_info_list);
	if (status != 0)
		std::cout << "error: " << gai_strerror(status) << "\n";
	else std::cout << "Addr status OK\n";
	this->keepalive = keepalive;	
};

TcpClient::~TcpClient(){};

void TcpClient::Connect(){
	std::cout << "Creating socket..\n";
	socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
	if (socketfd == -1)
		std::cout << "Socket error\n";
	else std::cout << "Socket status OK\n";

	socklen_t optlen = sizeof(keepalive);
	if (setsockopt(socketfd,SOL_SOCKET, SO_KEEPALIVE, &keepalive, optlen) != 0)
		std::cout << "Error setting keepalive on socket\n";
	if (keepalive == 1)
		std::cout << "Keepalive ON\n";
	else std::cout << "Keepalive OFF\n";

	status = connect(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
	if (status == -1)
		std::cout << "Connect error\n";
	else {
		std::cout << "Connect to server OK\n";
		connected = true;
	}
};

void TcpClient::Disconnect(){
	connected = false;
	std::cout << "Disconnected\n";
};

void TcpClient::Write(char *message){
	if (strcmp(message,":quit\3") == 0){
		send(socketfd,"\4", 1, 0);
		Disconnect();
		return;
	}
 	int len = strlen(message);
	ssize_t bytes_sent = send(socketfd, message, len, 0);
};

void TcpClient::Read(std::string &message){
	char *buf = new char[8]();
	ssize_t bytes_received; 
	FD_ZERO(&data);
	FD_SET(0, &data);	
	if (select(socketfd, &data, NULL, NULL, &time)) 
		while(bytes_received && !(strchr(buf,'\3'))){
			bytes_received = recv(socketfd, buf, 8, 0);
			if (bytes_received == -1){
				std::cout << "Server closed connection\n";
				return;
			}
			buf[bytes_received] = '\0';
			message += buf;
		}
};

bool TcpClient::isConnected(){
	return connected;
};

void TcpClient::closeSocket(){
	close(socketfd);
};
