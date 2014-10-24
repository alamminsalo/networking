#include "client.h"
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <pthread.h>

TcpClient *cl;

void getInput(char *&str){
	char tmp[512];
	std::cin.getline(tmp,512);
   	str = new char[strlen(tmp) +1];
	strcpy(str, tmp);
	strcat(str, "\3");	
};

void* readThread(void* i){
	std::string buf;
	while(cl->isConnected()){
		buf = "";
		cl->Read(buf);
		if (buf != "")
			std::cout << buf << std::endl;
	}
	return NULL;
};

void* writeThread(void* i){
	while(cl->isConnected()){
		char* str;
		getInput(str);
		cl->Write(str);
	}
	return NULL;
};

void startThreads(){
	pthread_t t1, t2;
	int iret1, iret2;
	iret1 = pthread_create(&t1, NULL, readThread, NULL);
	iret2 = pthread_create(&t2, NULL, writeThread, NULL);
	pthread_join(t2,NULL);
	pthread_join(t1,NULL);
};

int main(int argc, char *argv[]){
	if (argc == 3)
		cl = new TcpClient(argv[1], argv[2], 0);
	else if (argc == 4)
		cl = new TcpClient(argv[1], argv[2], atoi(argv[3]));
	else {
		std::cout << "Syntax:\% domain port\n";
		return -1;
	}
	cl->Connect();
	startThreads();
	cl->closeSocket();

	return 0;
};
