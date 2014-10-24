#include "server.h"
#include <pthread.h>

TcpServer *serv;

void* runThread(void* i){
	int index = serv->getIndex();
	while(serv->operational()){
		serv->Listen(index);	
	}
	return NULL;
};

void startThreads(int thread_num){
	pthread_t thread[thread_num];
	for (int i=0; i<thread_num; i++){
		pthread_create(
			&thread[i],
			NULL,
			runThread, 
			NULL
		);
		usleep(100);
	}
	for (int i=0; i<thread_num; i++)
		pthread_join(thread[i],NULL);
};

int main(){
	serv = new TcpServer("3333", 0);
	startThreads(10);
	serv->Stop();
	return 0;
};
