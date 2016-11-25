#include <iostream>
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
int Palaczy_wBuforze=0;
pthread_mutex_t mutexZezwoleniaNaZapis[3];//trzy muteksy, każdy odpowiada za czekanie na odpowiednie dwa składniki
pthread_cond_t condZezwoleniaNaZapis[3];
pthread_mutex_t mutexKontroliLosowania;
pthread_cond_t condKontroliLosowania;
pthread_mutex_t KontrolZapisuDoBufora;
string ListaImion[10]={"Warren","Lowell","Elliot","Chas","Trent","Elvin","Edison","Bryce","Lonny","Alex"};
int liczbawolnych[3];//liczba wolnych elementów każdego zasobu
int liczbaiteracji=0;//ile razy losujemy w programie
vector<pthread_t> wantkiPalaczy;


struct Smoker{
	string name;
	int identyfikator; 	
	int resources;//wartość-to jakiś ze składników
   	bool Czy_wBuforze;
        Smoker(int num){
		identyfikator=num;
           	name=ListaImion[rand()%10];
                resources=rand()%3;
		cout<<identyfikator<<" "<<name<<" "<<resources<<"\n";
	        Czy_wBuforze=false;
        }
};
vector<Smoker> smokers;
vector<Smoker> readyToSmoke;
void* bufChecker(void *){
	while(true){
		if(liczbaiteracji==5) return 0;
		if(Palaczy_wBuforze<4){wpisane=false;}//ustalamy ze czyścimy bufor, kiedy palaczy jest tam >=4
		while(Palaczy_wBuforze>=4 && wpisane){
			pthread_mutex_lock(&KontrolZapisuDoBufora);
			cout<<"Usuwamy z przepelnionego buforu"<<"\n";
			for(int i=0;i<4;i++){			
				cout<<readyToSmoke[0].identyfikator<<" "<<readyToSmoke[0].name<<" "<<readyToSmoke[0].resources<<"\n";
				smokers[readyToSmoke[0].identyfikator].Czy_wBuforze=false;
				liczbawolnych[smokers[readyToSmoke[0].identyfikator].resources]++;
                        	readyToSmoke.erase(readyToSmoke.begin());
			}
			Palaczy_wBuforze=Palaczy_wBuforze-4;		
			pthread_mutex_unlock(&KontrolZapisuDoBufora);
			
		}
	}
	return 0;
}

void* barmen(void *){
	for(liczbaiteracji=0;liczbaiteracji<5;liczbaiteracji++){
		int resources=rand()%3;
	//broadcast wysyla sygnal do wszystkich kwalifikujacych sie palaczy
		switch(resources){
            		case 0:/*cout<<"Wylosowaliśmy 0\n";*/ pthread_cond_broadcast(&condZezwoleniaNaZapis[0]);
            		break;
            		case 1:/*cout<<"Wylosowaliśmy 1\n";*/ pthread_cond_broadcast(&condZezwoleniaNaZapis[1]);
            		break;
            		case 2:/*cout<<"Wylosowaliśmy 2\n";*/ pthread_cond_broadcast(&condZezwoleniaNaZapis[2]);
            		break;
      		}
		pthread_cond_wait(&condKontroliLosowania,&mutexKontroliLosowania);
	}
	return 0;
}

void* func(void *smoker){
	struct Smoker *smoker_in_func =(struct Smoker *) smoker;
	while(true){
		pthread_cond_wait(&condZezwoleniaNaZapis[smoker_in_func->resources],&mutexZezwoleniaNaZapis[smoker_in_func->resources]);	
		pthread_mutex_lock(&KontrolZapisuDoBufora);		
		
		if(smokers[smoker_in_func->identyfikator].Czy_wBuforze==false){	
			cout<<"Zapisuje "<<smoker_in_func->identyfikator<<" "<<smoker_in_func->name<<"\n";	
			readyToSmoke.push_back(*smoker_in_func);
 			smoker_in_func->Czy_wBuforze=true;
			Palaczy_wBuforze++;
			liczbawolnych[smoker_in_func->resources]--;
			cout<<"Zostalo "<<liczbawolnych[smoker_in_func->resources]<< " zasoby typu "<<smoker_in_func->resources<<"\n";
			
		}
		if(liczbawolnych[smoker_in_func->resources]==0) {pthread_cond_signal(&condKontroliLosowania);wpisane=true;}
		
		pthread_mutex_unlock(&KontrolZapisuDoBufora);
	}
	return 0;
}


int main(){
	srand(time(NULL));
        int NumOfSmokers;
	cout<<"Ile w barze jest palaczy?\n";
	cin>>NumOfSmokers;

	/*tworzenie vektoru losowych palaczy*/
	for(int i=0;i<NumOfSmokers;i++){
		smokers.push_back(Smoker(i));
                
	}
	
	for(int i=0;i<NumOfSmokers;i++)
		liczbawolnych[smokers[i].resources]++;

	/*tworzenie wątków losowych palaczy*/ 
	for(int i=0;i<NumOfSmokers;i++){
		pthread_t SmokerThread;
		wantkiPalaczy.push_back(SmokerThread);
		pthread_create(&wantkiPalaczy[i],NULL,func,(void*)&smokers[i]);

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
