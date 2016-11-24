#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include<cstdio>

using namespace std;
int us=0;
int SmokersInBuffer=0;
pthread_mutex_t mutex[3];//trzy muteksy, każdy odpowiada za czekanie na odpowiednie dwa składniki
pthread_cond_t cond[3];
pthread_mutex_t barmenmut;
pthread_cond_t barmencond;
pthread_mutex_t mutexme;
pthread_mutex_t mut;
string listOfNames[10]={"Warren","Lowell","Elliot","Chas","Trent","Elvin","Edison","Bryce","Lonny","Alex"};
int tab[3],liczba_niewypisanych_palaczy;

vector<pthread_t> smokers_threads;


struct Smoker{
	string name;
	int id; 	
	int resources;//wartość-to jakiś ze składników
   	bool inBuffer;
        /*Smoker(int num){
		id=num;
           	name=listOfNames[rand()%10];
                resources=rand()%3;
		cout<<name<<" "<<resources<<"\n";
	        inBuffer=false;
        }*/
	Smoker(int i,string nam, int resour){
		id=i;
		name=nam;
		resources=resour;
		inBuffer=false;
	}
};
vector<Smoker> smokers;
vector<Smoker> readyToSmoke;
void* bufChecker(void *){
	while(true){
		while(SmokersInBuffer>=4){
			cout<<"pop"<<'\n';
			pthread_mutex_lock(&mut);
			for(int i=0;i<4;i++){
								
				cout<<readyToSmoke[0].id<<" "<<readyToSmoke[0].name<<" ";
				cout<<readyToSmoke[0].resources<<'\n';
				smokers[readyToSmoke[0].id].inBuffer=false;
                        	readyToSmoke.erase(readyToSmoke.begin());
			}
			SmokersInBuffer=SmokersInBuffer-4;		
			pthread_mutex_unlock(&mut);
			
		}
	}
	return 0;
}

//robi to co bufChecker ale tylko raz
/*void* bufChecker2(void *){
	while(SmokersInBuffer>=4){
		cout<<"pop"<<'\n';
		pthread_mutex_lock(&mut);
		for(int i=0;i<4;i++){
			cout<<readyToSmoke[0].name<<"\n";
			cout<<readyToSmoke[0].resources<<'\n';
			smokers[readyToSmoke[0].id].inBuffer=false;
                       	readyToSmoke.erase(readyToSmoke.begin());
		}
		SmokersInBuffer=SmokersInBuffer-4;		
		pthread_mutex_unlock(&mut);
		
	}

}*/

void* barmen(void *){
	//zakladam ze liczba_niewypisanych_palaczy==0 tutaj, inaczej tej funkcji nie wywolujemy	
	
	//tutaj bede czekac az pthread==0
	puts("barman");printf("%d \n",liczba_niewypisanych_palaczy);

	//int resources=rand()%3;
	//cout<<resources<<"\n";
	if(us==1){ pthread_cond_broadcast(&cond[2]);us=0;liczba_niewypisanych_palaczy=tab[2];}
	if(us==0){ pthread_cond_broadcast(&cond[1]);us=1;liczba_niewypisanych_palaczy=tab[1];}
	
       /*switch(resources){
            	case 0: pthread_cond_signal(&cond[0]);
            	break;
            	case 1: pthread_cond_signal(&cond[1]);
            	break;
            	case 2: pthread_cond_signal(&cond[2]);
            	break;
      	}*/
	
	//liczba_niewypisanych_palaczy=tab[resources];
	//broadcast wysyla sygnal do wszystkich kwalifikujacych sie palaczy
	/*switch(resources){
            	case 0: pthread_cond_broadcast(&cond[0]);
            	break;
            	case 1: pthread_cond_broadcast(&cond[1]);
            	break;
            	case 2: pthread_cond_broadcast(&cond[2]);
            	break;
      	}*/
	
	return 0;
}

void* func(void *smoker){
	struct Smoker *smoker_in_func =(struct Smoker *) smoker;
	while(true){
		pthread_cond_wait(&cond[smoker_in_func->resources],&mutex[smoker_in_func->resources]);
		//pthread_mutex_lock(&mutexme);
		pthread_mutex_lock(&mut);
		
							
		
		if(smokers[smoker_in_func->id].inBuffer==false){	
			cout<<smoker_in_func->name<<" Zapisuje"<<'\n';	
			readyToSmoke.push_back(*smoker_in_func);
 			smoker_in_func->inBuffer=true;
			SmokersInBuffer++;
			liczba_niewypisanych_palaczy--;
			if(!liczba_niewypisanych_palaczy) pthread_cond_signal(&barmencond);
			////////
		}
		
		//pthread_mutex_unlock(&mutexme);
		pthread_mutex_unlock(&mut);
	}
	return 0;
}


int main(){
	srand(time(NULL));
        int NumOfSmokers;
	cin>>NumOfSmokers;
	/*tworzenie vektoru losowych palaczy*/
	/*for(int i=0;i<NumOfSmokers;i++){
		smokers.push_back(Smoker(i));
                
	}*/
	
        
	smokers.push_back(Smoker(0,"Jack",1));
	smokers.push_back(Smoker(1,"Alex",1));
	smokers.push_back(Smoker(2,"Dima",1));
	smokers.push_back(Smoker(3,"Mike",1));
	smokers.push_back(Smoker(4,"Jack",2));
	smokers.push_back(Smoker(5,"Alex",2));
	smokers.push_back(Smoker(6,"Dima",2));
	smokers.push_back(Smoker(7,"Mike",2));
	




	/*for(int i=0;i<3;i++)cout<<"xxxxx "<<tab[i]<<"\n";

	for(int i=0;i<NumOfSmokers;i++)
		cout<<"Smoker "<<i<<" "<<smokers[i].resources<<"\n";
	*/
	for(int i=0;i<NumOfSmokers;i++)
		tab[smokers[i].resources]++;

	//for(int i=0;i<3;i++)cout<<"xxxxx "<<tab[i]<<"\n";
	



	/*tworzenie wątków losowych palaczy*/ 
	for(int i=0;i<NumOfSmokers;i++){
		pthread_t SmokerThread;
		smokers_threads.push_back(SmokerThread);
		pthread_create(&smokers_threads[i],NULL,func,(void*)&smokers[i]);

	}
	
	/*uruchamianie procesu, który sprawdza zawartość bufora i uruchamia wątki palenia, jeśli liczba gotowych do palenia jest >=2*/
	      
		pthread_t bufferproc;
		pthread_create(&bufferproc,NULL,bufChecker,NULL);
	/*jeśli palaczy w buforze <2, losujemy jeszcze zasoby*/
		for(int i=0;i<5;i++){
			cout<<"kok"<<" SmokersInBuffer = "<<SmokersInBuffer<<"    i = "<<i<<'\n';
			if(SmokersInBuffer<4){
				if(liczba_niewypisanych_palaczy>0)
					pthread_cond_wait(&barmencond,&barmenmut);
				pthread_t barm;
				pthread_create(&barm,NULL,barmen,NULL);
				pthread_join(barm,NULL);
			}
				

		}
	
	/*jeśli bufferproc się skończy tzn, że 2 palaczy wypaliło swoje papierosy*/
	pthread_join(bufferproc,NULL);
	
	return 0;
}
