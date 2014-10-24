#include "server.h"

TcpServer::TcpServer(const char *port, int keepalive){
	host_info.ai_family = AF_UNSPEC;
	host_info.ai_socktype = SOCK_STREAM;
	host_info.ai_flags = AI_PASSIVE;
	status = getaddrinfo(NULL, port, &host_info, &host_info_list);
	socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);	
	if (socketfd == -1) std::cout << "Socket error\n";

	//status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &keepalive, sizeof(keepalive));
   	status = bind(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);	
	if (status == -1) std::cout << "Bind error\n";

	status = listen(socketfd, 10);
	if (status == -1) std::cout << "Listen error\n";
	std::cout << "Port " << port << '\n';	
	server_online = true;
	cl_list_index = 0;
};

TcpServer::~TcpServer(){};

void TcpServer::Listen(int cl_index){
	std::cout << "Listening for new client at index " << cl_index << '\n';
	addr_size = sizeof(cl_addr);
	cl_socket[cl_index] = accept(socketfd,(struct sockaddr*)&cl_addr, &addr_size);
	if (cl_socket[cl_index] == -1)
		std::cout << "Listen error\n";
	else {
		std::cout << "Connection accepted from client "<<cl_index <<".\nUsing cl_socket " << cl_socket[cl_index] << "\n";
		client_online[cl_index] = true;
	}

	std::string buffer;
	char *msg;

	while(client_online[cl_index]){
		buffer = "";
		if (Read(cl_index, buffer) == 0){
			std::cout << "Message from client " << cl_index << std::endl;
			msg = new char[buffer.length()+16]();
			strcpy(msg,"<Client ");
			char int_str[8];
			sprintf(int_str,"%d",cl_index);
			strcat(msg, int_str);
			strcat(msg, "> ");
			strcat(msg, buffer.c_str());
			strcat(msg, "\3");
			for (int i=0; i<cl_list_index; i++)
				if (i != cl_index)
					this->Write(i, msg);
		}
		usleep(100);
	}
	close(cl_socket[cl_index]);
};

int TcpServer::Read(int cl_index, std::string &buffer){
	char *buf = new char[8]();
	ssize_t bytes_received;
	while (bytes_received && !(strchr(buf,'\3'))){
		bytes_received = recv(cl_socket[cl_index], buf, sizeof(buf), 0);
		if (bytes_received == 0)
			break;
		if (bytes_received == -1 || strchr(buf, '\4')){
			stopSocket(cl_index);
			std::cout << "<Client " << cl_index << "> dropped" << std::endl;
			return -1;	
		}
		buf[bytes_received] = '\0';
		buffer += buf;
	}
	if (buffer.length() < 1)
		return -1;
	return 0;
};
 
void TcpServer::Write(int cl_index, char *message){
	std::cout << "Sending message..." << std::endl;
	unsigned int len = strlen(message)+1;
	ssize_t bytes_sent;
	for (unsigned int i=0; i<len; i+=bytes_sent)
		bytes_sent = send(cl_socket[cl_index], message+i, 8, 0);
};

void TcpServer::stopSocket(int cl_index){
	this->client_online[cl_index] = false;
	close(cl_socket[cl_index]);
};

void TcpServer::Stop(){
	freeaddrinfo(host_info_list);
	close(socketfd);
};

int TcpServer::getIndex(){
	return cl_list_index++;
};

bool TcpServer::operational(){
	return server_online;
};
