#include "Tissue.h"


float get_min_x(vector<Wall*> wall){
    float sonuc = 999999;
    for(int i=0; i<wall.size(); i++){
        if(wall[i]->GetInitialParticle().X() < sonuc)
            sonuc = wall[i]->GetInitialParticle().X();
        if(wall[i]->GetFinalParticle().X() < sonuc)
            sonuc = wall[i]->GetFinalParticle().X();
    }
    
    return sonuc;
}
float get_max_x(vector<Wall*> wall){
    float sonuc = -999999;
    for(int i=0; i<wall.size(); i++){
        if(wall[i]->GetInitialParticle().X() > sonuc)
            sonuc = wall[i]->GetInitialParticle().X();
        if(wall[i]->GetFinalParticle().X() > sonuc)
            sonuc = wall[i]->GetFinalParticle().X();
    }
    return sonuc;
}
float get_min_y(vector<Wall*> wall){
    float sonuc = 999999;
    for(int i=0; i<wall.size(); i++){
        if(wall[i]->GetInitialParticle().Y() < sonuc)
            sonuc = wall[i]->GetInitialParticle().Y();
        if(wall[i]->GetFinalParticle().Y() < sonuc)
            sonuc = wall[i]->GetFinalParticle().Y();
    }
    return sonuc;
}
float get_max_y(vector<Wall*> wall){
    float sonuc = -999999;
    for(int i=0; i<wall.size(); i++){
        if(wall[i]->GetInitialParticle().Y() > sonuc)
            sonuc = wall[i]->GetInitialParticle().Y();
        if(wall[i]->GetFinalParticle().Y() > sonuc)
            sonuc = wall[i]->GetFinalParticle().Y();
    }
    return sonuc;
}

/*********************************************************************
*                                    Constructor
*
*/
Tissue::Tissue(){
    min_x = min_y = 999999;
    max_x = max_y = -999999;
    coords[0] = min_x; coords[1] = max_x; coords[2] = min_y; coords[3] = max_y;
}
/*********************************************************************
*                                    Constructor
*
* The first param is the minimum x border
* The second param is the maximum x border
* The third param is the minimum y border
* The fourth param is the maximum y border
* The tissue is the region residing among those coordinates.
*/
Tissue::Tissue(float minX, float maxX, float minY, float maxY){
    this->min_x = minX;
    this->max_x = maxX;
    this->min_y = minY;
    this->max_y = maxY;
    coords[0] = min_x; coords[1] = max_x; coords[2] = min_y; coords[3] = max_y;
    
}
/*********************************************************************
*                                Copy Constructor
*
* Shallow copy
*/
Tissue::Tissue(const Tissue& rhs){
    this->min_x = rhs.min_x;
    this->max_x = rhs.max_x;
    this->min_y = rhs.min_y;
    this->max_y = rhs.max_y;
    coords[0] = rhs.coords[0]; coords[1] = rhs.coords[1]; coords[2] = rhs.coords[2]; coords[3] = rhs.coords[3];
    for(int i=0; i<rhs.cells.size(); i++)
        this->cells.push_back(rhs.cells[i]);
//    DEEP GEREKTIREBILIR, belki de shallow
}
/*********************************************************************
*                                    Destructor
*
*/
Tissue::~Tissue(){
    min_x = max_x = min_y = max_y = coords[0] = coords[1] = coords[2] = coords[3] = 0;
    for(vector<Cell*>::iterator iter = cells.begin(); iter != cells.end(); ++iter){
        delete (*iter);
    }
}

/*********************************************************************
*                                    GetCoordinates
*
* @return the coordinates of minimum x border, maximum x border,
  minimum y border and maximum y border in an array with the stated
    order.
*/
float* Tissue::GetCoordinates(){
    return coords;
}


/*********************************************************************
*                                    AddCell
*
* Adds the actual reference of the cell given in the param to storage
  of cells belonging to this Tissue object.
*/
void Tissue::AddCell(Cell& cell){

    if(get_max_x(cell.GetCellWall()) > max_x) max_x = get_max_x(cell.GetCellWall());
    if(get_max_y(cell.GetCellWall()) > max_y) max_y = get_max_y(cell.GetCellWall());
    if(get_min_x(cell.GetCellWall()) < min_x) min_x = get_min_x(cell.GetCellWall());
    if(get_min_y(cell.GetCellWall()) < min_y) min_y = get_min_y(cell.GetCellWall());
    cells.push_back(&cell);
}
/*********************************************************************
*                                    RemoveCell
*
* Deletes the cell given in the param and removes it from the storage
  of cells belonging to this Tissue object.
* Do the necessary operation that your own implementation requires.
    (Whatever it is: Like decreasing the number of cells, or erasing from
    storage that you used to hold the cells, or anything else...)
*/
void Tissue::RemoveCell(Cell& cell){
    for(vector<Cell*>::iterator iter = cells.begin(); iter != cells.end(); ++iter){
        if(*iter == &cell){
            cells.erase(iter);
        }
    }
    min_x = min_y = 999999;
    max_x = max_y = -999999;
    for(int i=0; i<cells.size(); i++){
        if(get_max_x(cells[i]->GetCellWall()) > max_x){
            max_x = get_max_x(cells[i]->GetCellWall());
            coords[1] = max_x;
        }
        if(get_max_y(cells[i]->GetCellWall()) > max_y){
            max_y = get_max_y(cells[i]->GetCellWall());
            coords[3] = max_y;
        }
        if(get_min_y(cells[i]->GetCellWall()) < min_y){
            min_y = get_min_y(cells[i]->GetCellWall());
            coords[2] = min_y;
        }
        if(get_min_x(cells[i]->GetCellWall()) < min_x){
            min_x = get_min_x(cells[i]->GetCellWall());
            coords[0] = min_x;
        }
    }
    
}
/*********************************************************************
*                                    GetCell
*
* @return the cell whose id is given in the param
* If the cell whose id is given does not exit, in other words it is a
  dead cell, then the method should throw DeadCellException which was
    defined in Exception.h.
*/
const Cell& Tissue::GetCell(int id) const{
    for(int i=0; i<cells.size(); i++){
        if(cells[i]->GetCellID() == id)
            return *cells[i];
    }
    throw DeadCellException();
    
}
/*********************************************************************
*                                    Interact
*
* This method makes the cells interacting with the microorganisms in
  the param. For each microorganism a compatible cell is found.
  Input will be given satisfying that each microorganism fits into a cell.
*/
void Tissue::Interact(const vector<MicroOrganism*>& micorgs){
//    cout << "mic: " << *micorgs[1] << endl;
//    cout << "cell: " << *cells[6] << endl;
//    cells[6]->StrengthenCellWall();
//    cout << "cell: " << *cells[6] << endl;
    for(int i=0; i<micorgs.size(); i++){
        for(int j=0; j<cells.size(); j++){
            if(micorgs[i]->DoesFitInto(*cells[j])){ 
//                cells[j]->StrengthenCellWall();
//                cout << *cells[j]<<"\n";
                micorgs[i]->ConnectToCell(cells[j]);
                break;
            }
            
        }
        
    }
}
/*********************************************************************
*          THIS METHOD WILL NOT BE GRADED. YOU CAN LEAVE IT EMPTY!
*
*                                    CreatePartitions
*
* Creates an R-Tree hierarchy as follows:
    - Each microorganism given in the param is put into minimum
      bounding rectangle.
    - At the bottom level of tree the individual bounding rectangles
      of the microorganisms will be placed into the leaves.
    - On one level up, the overlapping bounding rectangles at the lower
      level are bounded with a minimum bounding rectangle and these
      larger rectangles are put into the nodes. Note that there may be
      more than two rectangles overlapping at the lower level and these
      may not be overlapped all each other. Namely, bounding rectangle
      A may be overlap with rectangle B, and B may overlap with rectangle
      C, but A may not overlap with C. Still, A, B and C all will be
      bounded with the same minimum rectangle, say D, and this embracing
      rectangle D will be put into one level up of A, B, C. Hierarchically,
      D will be parent, and A, B and C will be children of D.
    - At the higher levels the same procedure is applied. Finally, in
      the root, there will be the the largest Tissue object embracing all.
    - Attention to the following!: Let A, B, C, D, E, F and G be in the
      same level of the tree. Let A, B, and C overlap somehow, call the
      minimum bounding rectangle of those as X. Similarly, let D, E, F
      and G overlap and call the minimum bounding rectangle of those as Y.
      Assume that none of A, B and C overlaps with any of D, E, F and G.
      However, the resulting bounding rectangles X and Y may overlap. In
      this case, the overlap of X and Y are handled in one level up!
      (Namely, we do NOT take the minimum bounding rectangle of A, B, C, D,
      E, F and G directly. We first take those of A, B, C (which is X)
      and those of D, E, F and G  (which is Y) separately while going up
      to one level up. Then, we take the minimum bounding rectangle of X
      and Y in the next level while going up to one more level up.)
    - It may happen that a rectangle may not overlap/intersect with any
      other rectangle at some level. In this case, you should put it into
      a minimum bounding rectangle of the same size with itself in one
      level up. Only, in the highest level, if X, Y, ... and Z are the
      rectangle nodes which are never intersecting with each other, you
      should put them all in a single bounding rectangle R, and terminate
      by making R root.
*/
void Tissue::CreatePartitions(const vector<MicroOrganism*>&){
    
}
/*********************************************************************
*          THIS METHOD WILL NOT BE GRADED. YOU CAN LEAVE IT EMPTY!
*
*                                    Cure By Plasma
*
* This method takes an other Tissue object as param ( a tissue taken
  from a person getting well after Covid-19) and replaces the corresponding
  part of this Tissue object with that tissue. Hence, all the harmful
  microorganisms, including the Covid-19, will have been removed through
  this plasma treatment! :)
* The tissue given in the param will always be a subset of the current
  tissue. Namely, the pattern of tissue taken will comply with the
  tissue of the sick person.
*/
void Tissue::TreatByPlasma(Tissue){
    
}
