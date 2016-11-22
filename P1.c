#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <thread>
#include <semaphore.h>
#include <pthread.h>

using namespace std;

pthread_mutex_t mutex[3];//trzy muteksy, każdy odpowiada za czekanie na odpowiednie dwa składniki
pthread_cond_t cond[3];
sem_t TobaccoAndPaper;
sem_t PaperAndMatches;
sem_t MatchesAndTobacco;
sem_t BarmenSem;
string listOfNames[10]={"Warren","Lowell","Elliot","Chas","Trent","Elvin","Edison","Bryce","Lonny","Alex"};

//vector<thread> smokers_threads;
vector<Smoker> readyToSmoke;

struct Smoker{
	string name; 	
	int resources;//wartość-to jakiś ze składników
   	bool inBuffer;
        Smoker(){
           	name=listOfNames[rand()%10];
                resources=rand()%3;
	        inBuffer=false;
        }
};
vector<Smoker> smokers;
void* barmen(){
	int res=rand()%3;
        switch(res){
            	case 0: pthread_cond_signal(&cond[0]);
            	break;
            	case 1: pthread_cond_signal(&cond[1]);
            	break;
            	case 2: pthread_cond_signal(&cond[2]);
            	break;
      	}

}
void* function(struct Smoker smoker){
	while(true){
                //chcę zabłokować tutaj, dopóki nie będzie sygnału z Switch{case} wyżej
		pthread_mutex_lock();
		pthread_cond_wait(&cond[smoker.resources],&mutex[smoker.resources]);
                	        
		readyToSmoke.puch_back(smoker);
 		smoker.inBuffer=true;
		

	}

}

int main(){
	srand(time(NULL));
	int n;
	cin>>n;
	for(int i=0;i<n;i++){
		smokers.push_back(Smoker());
	}
	for(int i=0;i<n;i++){
	cout<<smokers[i].name<<'\n';
	}
	return 0;
}
