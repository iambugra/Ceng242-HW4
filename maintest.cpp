#include "maintest.h"
#ifdef _WIN32
#include <Windows.h>
#include <windows.h>
#else
#include <unistd.h>
#endif
#define yello "\033[93m"
#define normalo "\033[0m"

vector<vector<MicroOrganism*> > moV;
vector<Particle*> particles;
vector<Wall*> Walls;
vector<Tissue*> tis;
int readsofar=0;
// How to compile:
// by Makefile : make all , make run , make valgrind

void prStrength(int tisgroup,int cellid){
	static int testidStr=1;
	cout <<"StrengthenCellWall test #"<<testidStr++<<endl;
	cout << "\tcell id: "<< cellid <<endl;
	cout << "\t"<<tis[tisgroup]->GetCell(cellid) << endl;
	Cell* totest= new Cell(tis[tisgroup]->GetCell(cellid));
	totest->StrengthenCellWall();
	cout << "\tafter Strengthen():"<<endl;
	cout << "\t"<<*totest << endl;
	cout << "-----s-----s-----s-----" <<endl;
	delete totest;
}
int moid=0;
void prFit(vector<MicroOrganism*> v,int tisgroup,int cellid){
	static int testidFit=1;
	cout <<"DoesFitInto test #"<<testidFit++<<endl;
	cout << "\tcell id: "<< cellid <<endl;
	cout << "\t"<<tis[tisgroup]->GetCell(cellid) << endl;
	Cell* totest= new Cell(tis[tisgroup]->GetCell(cellid));
	
	for(int i=0;i<v.size();i++){

		cout<<"\t"<<"MicroOrganism #"<<moid++;
		cout << "\t"<< (v[i]->DoesFitInto(*totest)?"True":"False")<< endl;
	}
	cout << "-----d-----f-----i-----" <<endl;
	delete totest;
}

void tests(const char* cellFile, const char* MicroOrganismFile) {
	vector<int> tisSize;
	int continueReading=1;
	int cur=0;
	while(continueReading){
		tis.push_back(new Tissue());
		continueReading=ReadCells(cellFile,*(tis.back()));
		tisSize.push_back(readsofar-cur);
		cur=readsofar;
	}
	/* Add tests here*/

	bool strtest=true;
	bool dfitest=true;
	bool intrctest=true;


	int id=0;
	int prev=id;
	for(int i=0;i<tis.size();i++){
		prev=id;
		cout <<"-->"<<endl;
		cout <<"tissue "<<"#"<<i+1<<" has "<<tisSize[i]<<" elements."<<endl;
		cout <<"-->"<<endl;
		if(strtest){
			for(int j=0;j<tisSize[i];j++){
				prStrength(i,id);
				id++;
			}
			cout<<endl<<endl;
		}
		if(dfitest){
			id=prev;
			for(int j=0;j<tisSize[i];j++){
				for(int k=0;k<moV.size();k++){
					prFit(moV[k],i,id);
				}
				moid=0;
				id++;
			}
		}
	}
	if(intrctest){
		for(int i=0;i<min(tis.size(),moV.size());i++){
			cout<<"Interact Test #"<<i+1<<endl;
			tis[i]->Interact(moV[i]);
			for(int j=0;j<moV[i].size();j++){
				cout<< "\t" << *moV[i][j]<<endl;
			}
		}
	}





	/* don't forget to delete your allocations above*/

	/* delete tissues */
	for(int i=0;i<tis.size();i++){
		delete tis[i];
	}
}

/******************************************************************************/
/*                                    MAIN()                                  */
/******************************************************************************/
int main(int argc, char* argv[]) {

	#ifdef _WIN32
		HANDLE hConsole;
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleOutputCP( CP_UTF8 );
		SetConsoleTextAttribute(hConsole,14);
		cout<<"A tester by Alp Yılmaz .feat CurvyWall3369"<<endl;
		SetConsoleTextAttribute( hConsole , 15);
		Sleep(1.5*1000);
	#else
		cout<<yello<<"A tester by Alp Yılmaz .feat CurvyWall3369"<<normalo<<endl;
		sleep(1.5);
	#endif

	ReadMicroOrganisms(argv[2]);
	tests(argv[1],argv[2]);

	for (int i=0; i<particles.size(); i++)
		delete particles[i];

	for (int j=0; j<Walls.size(); j++)
		delete Walls[j];
	//cout<<moV.size()<<endl;
	for(int i=0; i<moV.size(); i++){
		//cout<<moV[i].size()<<endl;
		for(int j=0; j<moV[i].size();j++){
			delete moV[i][j];
		}
	}
return 0;

}
