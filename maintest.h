#ifndef MAIN_H
#define MAIN_H

#include "Particle.h"
#include "Wall.h"
#include "CurvyWall.h"
#include "Cell.h"
#include "MicroOrganism.h"
#include "Triangular.h"
#include "Circular.h"
#include "Squadratic.h"
#include "Tissue.h"

#include <fstream>
#include <string>
#define sep <<" "<<
extern vector<vector<MicroOrganism*> > moV;
void ReadMicroOrganisms(const char* fileName) {

  ifstream moFile(fileName);
	if (moFile.is_open())
  {
    int numberOfMicroOrganisms, id, noGroups, group;
    string type;                      // microorganism type
    moFile >> numberOfMicroOrganisms >> noGroups;
    moV.resize(noGroups,vector<MicroOrganism*>());
    for (int i=0; i<numberOfMicroOrganisms; i++) {
      moFile >> id >> group;                   // read microorganism id,group
      moFile >> type;                 // read microorganism type
      if (type == "Circular") {       // Circular
        float radius;
        moFile >> radius;
        moV[group].push_back(new Circular(id, radius));
      }
      else if (type == "Squadratic") { // Squadratic
        float edgeLength;
        moFile >> edgeLength;
        moV[group].push_back(new Squadratic(id, edgeLength));
      }
      else if (type == "Triangular") { // Triangular
        vector<Wall*> edges;
        float edgeLength[3];
        moFile >> edgeLength[0] >> edgeLength[1] >> edgeLength[2];
        for (int j=0; j<3; j++)
          edges.push_back(new Wall(edgeLength[j]));
        string RNAsequence;
        moFile >> RNAsequence;
        NucleoBase* RNA = new NucleoBase(RNAsequence, NULL);
        moV[group].push_back(new Triangular(id, edges, *RNA));
        for (int j=0; j<3; j++)
          delete edges[j];
        delete RNA;
      }
      else;                           // Unknown type does not exist yet
    }
    moFile.close();
  }
  return;
}

/*********************************************************************
*				ReadCells
*
* Takes the file name which holds the cell data as first @param
* Creates the corresponding cell objects into the Tissue object given
  in the second @param.and holds them in vector
/********************************************************************/
extern vector<Particle*> particles;
extern vector<Wall*> Walls;
int ReadCells(const char* fileName, Tissue& tissue) {
  static int runbefore;
  int interrupt=0;
	ifstream cellsFile(fileName);
	if (cellsFile.is_open())
  {
		int numberOfParticles, numberOfWalls, numberOfCells, id;
    cellsFile >> numberOfParticles >> numberOfWalls >> numberOfCells;
    if(!runbefore){

		// READ PARTICLES
		for (int i=0; i<numberOfParticles; i++) {
			float x, y;	                              // particle coordinates
			cellsFile >> id >> x >> y;
			particles.push_back(new Particle(x, y));	// it will locate in particles[id]
		}


		// READ WALLS
		for (int i=0; i<numberOfWalls; i++) {
				int p1, p2, center;      // start point id, end point id, center point id
        cellsFile >> id;       // read the wall id
        // read walls
          char wallType;
					cellsFile >> wallType;
					if (wallType == 's') {	              // straight wall
						cellsFile >> p1 >> p2;
            Wall* wall = new Wall(*particles[p1], *particles[p2]);
						Walls.push_back(wall);
					}
					else {                               // curvy wall
            cellsFile >> p1 >> p2 >> center;
            Wall* wall = new CurvyWall(*particles[p1], *particles[p2], *particles[center]);
						Walls.push_back(wall);
					}
  		}
    }
		// READ CELLS
    if(runbefore){
      //readprev
		for (int i=0; i<numberOfParticles; i++) {
			float x, y;	                              // particle coordinates
      cellsFile >> id >> x >> y;
    }

		for (int i=0; i<numberOfWalls; i++) {
      int p1, p2, center;      // start point id, end point id, center point id
      char wallType;
      cellsFile >> id >> wallType;
      if (wallType == 's') {	              // straight wall
        cellsFile >> p1 >> p2;
      }
      else {                               // curvy wall
        cellsFile >> p1 >> p2 >> center;
      }
      
    }
    }
    for(int i=0;i<runbefore;i++){
      while(1){
        int temp;
        cellsFile >> temp;
        if(temp== -1){
          break;
        }
      }
    }
    extern int readsofar;
    int looops=numberOfCells-readsofar;
		for (int i=0; i<looops; i++) {
      int wallcount;
        cellsFile >> id;
        if(id== -1){
          interrupt=1;
          //cout<<"they broke my heart"<<endl;
          break;
        }
      readsofar++;
        cellsFile >> wallcount;       // read the cell id and the number of walls in the cell

				vector<Wall*> cellWall;
        cellWall.clear();
				for (int j=0; j<wallcount; j++) {
          int id;
					cellsFile >> id;
          cellWall.push_back(Walls[id]->Clone());
				}
        Cell* cell = new Cell(id, cellWall, &tissue);
				tissue.AddCell(*cell);  // do not delete cell since it is added directly;
        for (int j=0; j<cellWall.size(); j++)
          delete cellWall[j];
		}

    runbefore++;
    cellsFile.close();

  }
  return interrupt;
}

#endif
