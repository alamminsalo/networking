#include "server.h"

TcpServer::TcpServer(const char *port,int max, int keepalive){
	host_info.ai_family = AF_UNSPEC;
	host_info.ai_socktype = SOCK_STREAM;
	host_info.ai_flags = AI_PASSIVE;
	status = getaddrinfo(NULL, port, &host_info, &host_info_list);
	socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);	
	if (socketfd == -1) std::cout << "Socket error\n";

	status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &keepalive, sizeof(keepalive));
   	status = bind(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);	
	if (status == -1) std::cout << "Bind error\n";

	status = listen(socketfd, 10);
	if (status == -1) std::cout << "Listen error\n";
	std::cout << "Port " << port << '\n';	

	
	online = true;
	max_cl = max;
	thread = new TcpThread[max_cl];
	for (int i=0; i<max_cl; i++)
		thread[i] = new TcpThread(i);
};
TcpServer::~TcpServer(){};

TcpThread::TcpThread(int index,int inc_sock){
	timer.tv_sec = 10;
	timer.tv_usec = 0;
	id = index;
	online = true;
	host_sock = inc_sock;
	addr_size = sizeof(cl_addr);
};
TcpThread::~TcpThread(){};

void TcpThread::Listen(){
	while(online && !(select(socketfd, &data, NULL, NULL, &timer))){
		socketfd = accept(host_sock,(struct sockaddr*)&cl_addr, &addr_size);
		if (socketfd == -1)
			std::cout << "Listen error\n";
		else {
			std::cout << "Connection accepted from client "<< id <<".\nUsing cl_socket " << socketfd << "\n";
			client_online[cl_index] = true;
		}
	}
	char *resp;
	std::string buffer;
	while (select(socketfd,&data, NULL, NULL, &timer)){
		buffer = "";
		Read();
		if (buffer != ""){
			resp = new char[buffer.length()+16]();
			buildMsg(resp);
			Write(resp);
		}
	}
	close(socketfd);
};

int TcpServer::Read(){
	msg = "";
	char *buf = new char[8]();
	ssize_t bytes_received;
	while (bytes_received && !(strchr(buf,'\3'))){
		bytes_received = recv(cl_socket[cl_index], buf, sizeof(buf), 0);
		if (bytes_received == 0)
			break;
		buf[bytes_received] = '\0';
		msg += buf;
	}
};
 
void TcpThread::Write(int cl_index, char *message){
	unsigned int len = strlen(message)+1;
	ssize_t bytes_sent;
	for (unsigned int i=0; i<len; i+=bytes_sent)
		bytes_sent = send(cl_socket[cl_index], message+i, 8, 0);
};

void TcpThread::buildMsg(char *&str){
	strcpy(str,"< ");
	strcat(str,clientname);
	strcat(str, " >");
	strcat(str, msg.c_str());
	strcat(str, "\0\3");
};

