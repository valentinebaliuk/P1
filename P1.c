#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <unistd.h>

using namespace std;
bool wpisane=false;
int SmokersInBuffer=0;
pthread_mutex_t mutex[3];//trzy muteksy, każdy odpowiada za czekanie na odpowiednie dwa składniki
pthread_cond_t cond[3];
pthread_mutex_t mutexme;
pthread_cond_t condme;
pthread_mutex_t mut;
pthread_mutex_t thismut;
string listOfNames[10]={"Warren","Lowell","Elliot","Chas","Trent","Elvin","Edison","Bryce","Lonny","Alex"};
int liczbawolnych[3];
char tc[10];//tabl charow
int liczbaiteracji=0;
vector<pthread_t> smokers_threads;


struct Smoker{
	string name;
	int id; 	
	int resources;//wartość-to jakiś ze składników
   	bool inBuffer;
        Smoker(int num){
		id=num;
           	name=listOfNames[rand()%10];
                resources=rand()%3;
		printf("%d  %d \n",id,resources);
	        inBuffer=false;
        }
};
vector<Smoker> smokers;
vector<Smoker> readyToSmoke;
void* bufChecker(void *){
	while(true){
		if(liczbaiteracji==5) return 0;
		if(SmokersInBuffer<4){wpisane=false;}
		while(SmokersInBuffer>=4 && wpisane){
			pthread_mutex_lock(&mut);
			puts("pop");
			for(int i=0;i<4;i++){			
				printf("%d %d\n",readyToSmoke[0].id,readyToSmoke[0].resources);
				smokers[readyToSmoke[0].id].inBuffer=false;
				liczbawolnych[smokers[readyToSmoke[0].id].resources]++;
                        	readyToSmoke.erase(readyToSmoke.begin());
			}
			SmokersInBuffer=SmokersInBuffer-4;		
			pthread_mutex_unlock(&mut);
			
		}
	}
	return 0;
}

void* barmen(void *){
	for(liczbaiteracji=0;liczbaiteracji<5;liczbaiteracji++){
		int resources=rand()%3;
	//broadcast wysyla sygnal do wszystkich kwalifikujacych sie palaczy
		switch(resources){
            		case 0: pthread_cond_broadcast(&cond[0]);
            		break;
            		case 1: pthread_cond_broadcast(&cond[1]);
            		break;
            		case 2: pthread_cond_broadcast(&cond[2]);
            		break;
      		}
		pthread_cond_wait(&condme,&mutexme);
	}
	return 0;
}

void* func(void *smoker){
	struct Smoker *smoker_in_func =(struct Smoker *) smoker;
	while(true){
		pthread_cond_wait(&cond[smoker_in_func->resources],&mutex[smoker_in_func->resources]);
		pthread_mutex_lock(&thismut);		
		pthread_mutex_lock(&mut);		
		
		if(smokers[smoker_in_func->id].inBuffer==false){	
			printf("%d Zapisuje\n",smoker_in_func->id);	
			readyToSmoke.push_back(*smoker_in_func);
 			smoker_in_func->inBuffer=true;
			SmokersInBuffer++;
			liczbawolnych[smoker_in_func->resources]--;
			printf("%d %d\n",liczbawolnych[smoker_in_func->resources],smoker_in_func->resources);
			
		}
		if(liczbawolnych[smoker_in_func->resources]==0) {/*printf("wysylam sygnal\n")*/;pthread_cond_signal(&condme);wpisane=true;}
		
		pthread_mutex_unlock(&thismut);
		pthread_mutex_unlock(&mut);
	}
	return 0;
}


int main(){
	srand(time(NULL));
        int NumOfSmokers;
	scanf("%d", &NumOfSmokers);//cin>>NumOfSmokers;

	/*tworzenie vektoru losowych palaczy*/
	for(int i=0;i<NumOfSmokers;i++){
		smokers.push_back(Smoker(i));
                
	}
	
	for(int i=0;i<NumOfSmokers;i++)
		liczbawolnych[smokers[i].resources]++;

	/*tworzenie wątków losowych palaczy*/ 
	for(int i=0;i<NumOfSmokers;i++){
		pthread_t SmokerThread;
		smokers_threads.push_back(SmokerThread);
		pthread_create(&smokers_threads[i],NULL,func,(void*)&smokers[i]);

	}
	
	/*uruchamianie procesu, który sprawdza zawartość bufora i uruchamia wątki palenia, jeśli liczba gotowych do palenia jest >=2*/
	pthread_t bufferproc;
	pthread_create(&bufferproc,NULL,bufChecker,NULL);
	pthread_t barm;		
	pthread_create(&barm,NULL,barmen,NULL);
	pthread_join(barm,NULL);
	pthread_join(bufferproc,NULL);
	
	return 0;
}
