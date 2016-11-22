#include <pthread.h>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>

 
using namespace std;

string listOfNames[10]={"Warren","Lowell","Elliot","Chas","Trent","Elvin","Edison","Bryce","Lonny","Alex"};
vector<Smoker> smokers;
//vector<thread> smokers_threads;
//vector<Smoker> readyToSmoke;

struct Smoker{
	string name; 	
	bool resouses[3];
   	bool inBuffer;
        Smoker(){
           	srand(time(NULL));
           	name=listOfNames[rand()%11];
                resourses[0]=rand()%2;
                resourses[1]=rand()%2; 
	        resourses[2]=rand()%2;
 		inBuffer=false;
        }
};


/*void* function{


}*/

int main(){
int n;
cin>>n;
for(int i=0;i<n;i++){
	smokers.push_back(new Smoker());
}
Smoker s=smokers.pop_back();
cout<<s.name<<'\n';
return 0;
}

