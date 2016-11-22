#include <iostream>
#include <pthread.h>



pthread_mutex_t mutex;
pthread_cond_t cond;
void* to_write(void *args){
pthread_cond_signal(&cond);
	
return 0;
}

void* write(void *args){
	while(true){
		pthread_cond_wait(&cond, &mutex); 
		std::cout<<"LOL"<<'\n';
	}
return 0;
}

int main(){
	pthread_t thread1,thread2;
        std::cout<<("LOL")<<'\n';
	
        int p=pthread_create(&thread1,NULL,write,NULL);
	pthread_create(&thread2,NULL,to_write,NULL);

        
	return p;
}
