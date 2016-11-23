#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

using namespace std;
int SmokersInBuffer=0;
pthread_mutex_t mutex[3];//trzy muteksy, każdy odpowiada za czekanie na odpowiednie dwa składniki
pthread_cond_t cond[3];
pthread_mutex_t mutexme;
pthread_mutex_t mut;
bool TobaccoAndPaper;
bool PaperAndMatches;
bool MatchesAndTobacco;
string listOfNames[10]={"Warren","Lowell","Elliot","Chas","Trent","Elvin","Edison","Bryce","Lonny","Alex"};

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
	        inBuffer=false;
        }
};
vector<Smoker> smokers;
vector<Smoker> readyToSmoke;
void* bufChecker(void *){
	while(true){
		if(SmokersInBuffer>=2){
			cout<<"pop"<<'\n';
			pthread_mutex_lock(&mut);
			for(int i=0;i<2;i++){
				cout<<readyToSmoke[0].name<<"\n";
				smokers[readyToSmoke[0].id].inBuffer=false;
                        	readyToSmoke.erase(readyToSmoke.begin());
			}
			SmokersInBuffer=SmokersInBuffer-2;
			pthread_mutex_unlock(&mut);
			return 0;
		}
	}
	return 0;
}
void* barmen(void *){
	int res=rand()%3;
        switch(res){
            	case 0: pthread_cond_signal(&cond[0]);
            	break;
            	case 1: pthread_cond_signal(&cond[1]);
            	break;
            	case 2: pthread_cond_signal(&cond[2]);
            	break;
      	}
	return 0;
}

void* function(void *smoker){
	struct Smoker *smok =(struct Smoker *) smoker;
	while(true){
		pthread_cond_wait(&cond[smok->resources],&mutex[smok->resources]);

		pthread_mutex_lock(&mut);	
						cout<<"sos"<<'\n';	
		pthread_mutex_lock(&mutexme);
		if(smokers[smok->id].inBuffer==false){		
			readyToSmoke.push_back(*smok);
 			smok->inBuffer=true;
			SmokersInBuffer++;
		}
		pthread_mutex_unlock(&mutexme);
		pthread_mutex_unlock(&mut);
	}
	return 0;
}


int main(){
	srand(time(NULL));
        int n;
	cin>>n;
	/*tworzenie vektoru losowych palaczy*/
	for(int i=0;i<n;i++){
		smokers.push_back(Smoker(i));
                
	}
	
	/*tworzenie wątków losowych palaczy*/ 
	for(int i=0;i<n;i++){
		pthread_t thread1;
		smokers_threads.push_back(thread1);
		pthread_create(&smokers_threads[i],NULL,function,(void*)&smokers[i]);

	}
	
	/*uruchamianie procesu, który sprawdza zawartość bufora i uruchamia wątki palenia, jeśli liczba gotowych do palenia jest >=2*/
        pthread_t bufferproc;
	pthread_create(&bufferproc,NULL,bufChecker,NULL);
	/*jeśli palaczy w buforze <2, losujemy jeszcze zasoby*/
	while(SmokersInBuffer<2){
		pthread_t barm;		
		pthread_create(&barm,NULL,barmen,NULL);
				
	}
	/*jeśli bufferproc się skończy tzn, że 2 palaczy wypaliło swoje papierosy*/
	pthread_join(bufferproc,NULL);
	
	return 0;
}
