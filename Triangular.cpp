#include "Triangular.h"


/*********************************************************************
*                                    Constructor
*
* The first param is the id of the cell
* The second param is the vector of edges
* The third parameter is a NucleoBase object that defining its RNA.
*/
Triangular::Triangular(int id, const vector<Wall*>& p_edges, const NucleoBase& base) : MicroOrganism(id, p_edges){
    this->id_no = id;
    for(int i=0; i<p_edges.size(); i++)
        this->edges.push_back(p_edges[i]->Clone());
    this->nucleo_base = new NucleoBase(base);
        
}
/*********************************************************************
*                                    Destructor
*
*/
Triangular::~Triangular(){
    for(int i=0; i<edges.size(); i++)
        if(edges[i] != NULL)
            delete edges[i];
        
    delete nucleo_base;
}
/*********************************************************************
*                                    ConnectToCell
*
* See the base class explanation.
*/
void Triangular::ConnectToCell(Cell* const cell){
//    if(cell_cnd == NULL){
        Cell* temp_cell = new Cell(*cell);
        temp_cell->StrengthenCellWall();
        if(DoesFitInto(*temp_cell)){
            cell_cnd = cell;
            cell_cnd->StrengthenCellWall();
        }
        delete temp_cell;
//    }
}
/*********************************************************************
*                                    DoesFitInto
*
* See the base class explanation.
*/
bool Triangular::DoesFitInto(const Cell& cell) const{
//    cout << "girdi\n";
    Cell* temp_cell = new Cell(cell);
    Wall *cell_side0, *cell_side1, *cell_side2;
    temp_cell->StrengthenCellWall();
    if(temp_cell->GetCellWall().size() != 3){
        delete temp_cell;
        return false;
    }
    cell_side0 = new Wall(*(temp_cell->GetCellWall()[0]));
    cell_side1 = new Wall(*(temp_cell->GetCellWall()[1]));
    cell_side2 = new Wall(*(temp_cell->GetCellWall()[2]));
    
    if(cell_side0->GetLength() == edges[0]->GetLength() && cell_side1->GetLength() == edges[1]->GetLength() && cell_side2->GetLength() == edges[2]->GetLength()){
        delete temp_cell;
        delete cell_side0;
        delete cell_side1;
        delete cell_side2;
        return true;
        
    }else if(cell_side0->GetLength() == edges[0]->GetLength() && cell_side1->GetLength() == edges[2]->GetLength() && cell_side2->GetLength() == edges[1]->GetLength()){
        delete temp_cell;
        delete cell_side0;
        delete cell_side1;
        delete cell_side2;
        return true;
        
    }else if(cell_side0->GetLength() == edges[1]->GetLength() && cell_side1->GetLength() == edges[0]->GetLength() && cell_side2->GetLength() == edges[2]->GetLength()){
        delete temp_cell;
        delete cell_side0;
        delete cell_side1;
        delete cell_side2;
        return true;;
        
    }else if(cell_side0->GetLength() == edges[1]->GetLength() && cell_side1->GetLength() == edges[2]->GetLength() && cell_side2->GetLength() == edges[0]->GetLength()){
        delete temp_cell;
        delete cell_side0;
        delete cell_side1;
        delete cell_side2;
        return true;
        
    }else if(cell_side0->GetLength() == edges[2]->GetLength() && cell_side1->GetLength() == edges[0]->GetLength() && cell_side2->GetLength() == edges[1]->GetLength()){
        delete temp_cell;
        delete cell_side0;
        delete cell_side1;
        delete cell_side2;
        return true;
        
    }else if(cell_side0->GetLength() == edges[2]->GetLength() && cell_side1->GetLength() == edges[1]->GetLength() && cell_side2->GetLength() == edges[0]->GetLength()){
        delete temp_cell;
        delete cell_side0;
        delete cell_side1;
        delete cell_side2;
        return true;
    }else{
        delete temp_cell;
        delete cell_side0;
        delete cell_side1;
        delete cell_side2;
        return false;
    }
    
}
/*********************************************************************
*                                        React
*
* This type of microorganism kills the cell that it connects to.
* Note that while you are deleting the cell, you should also make the
* Tissue noticed that the corresponding cell was killed.
*/
void Triangular::React(){
    cell_cnd->GetTissue()->RemoveCell(*cell_cnd);
    delete cell_cnd;
}
/*********************************************************************
*                                    GetRNA
*
* Special to this type of microorganism
* @return the start node of the NucleoBase sequence which holds the RNA
  of the microorganism
*/
const NucleoBase& Triangular::GetRNA() const{
    NucleoBase* cur = this->nucleo_base;
    while(cur->prev != NULL)
        cur = cur->prev;
    return *cur;
}
/*********************************************************************
*                                    Mutate
*
* Special to this type of microorganism
* Mutate the RNA of this Triangular microorganism by crossing over with
  RNA of the Triangular microorganism given in the param.
* Crossing over is done as follows:
  Both RNA is compared in both ways: from the beginning to end and from
  end to beginning. When they start to differ, the corresponding G-S
  and A-U nucleobases in the differing part of both RNA are interchanged.
  For example:
  - Let RNA-1 be ASGAGGAUUGSUAGSSSAAG
    (make readable: ASGAG GAUUGS UAGSSSAAG)
  - Let RNA-2 be ASGAGSSGAUUUAGSSSAAG
    (make readable: ASGAG SSGAUU UAGSSSAAG)
  Since ASGAG part at the beginning and UAGSSSAAG part at the end are
  the same in both RNA, those parts are held. For the parts remaining
  in the middle, the corresponding G-S nucleobases and the corresponding
  A-U nucleobases are interchanged. Other type of correspondences are
    removed from the chain (Deleted). At the end:
  - RNA-1 becomes ASGAGSAUAGSSSAAG
    (make readable: ASGAG SA UAGSSSAAG)
  - RNA-2 becomes ASGAGGUUAGSSSAAG
    (make readable: ASGAG GU UAGSSSAAG)
* You can assume that at least 1 character from the beginning and the end
    will be the same for both RNA sequence (Just for you not to struggle
    with the so detailed implementation). Also, you can assume that at least
    1 character of RNAs will differ.
*/
void Triangular::Mutate(Triangular& rhs){
    NucleoBase *myinit, *rhsinit;
    string my_rna, rhs_rna;
    unsigned long first_differ, last_differ;
    unsigned long size;
    
    myinit = nucleo_base->GetInitial();
    rhsinit = rhs.nucleo_base->GetInitial();
    
    while(myinit->next != NULL)
        my_rna.push_back(myinit->base);
    while(rhsinit->next != NULL)
        rhs_rna.push_back(rhsinit->base);
    
    size = my_rna.size();
    
    for(unsigned long i=0; i<size; i++){
        if(my_rna[i] != rhs_rna[i]){
            first_differ = i;
            break;
        }
    }
    for(unsigned long i=size-1; i>=0; i--){
        if(my_rna[i] != rhs_rna[i]){
            last_differ = i;
            break;
        }
        
    }
    
    for(unsigned long i=first_differ; i<=last_differ; i++){
        if(my_rna[i] == 'S' && rhs_rna[i] == 'G'){
            my_rna[i] = 'G';
            rhs_rna[i] = 'S';
        }else if(my_rna[i] == 'G' && rhs_rna[i] == 'S'){
            my_rna[i] = 'S';
            rhs_rna[i] = 'G';
        }else if(my_rna[i] == 'A' && rhs_rna[i] == 'U'){
            my_rna[i] = 'U';
            rhs_rna[i] = 'A';
        }else if(my_rna[i] == 'U' && rhs_rna[i] == 'A'){
            my_rna[i] = 'A';
            rhs_rna[i] = 'U';
        }else{
            my_rna[i] = rhs_rna[i] = 'X';
        }
    }
    
    for(int i=0; i<my_rna.size(); i++){
        if(my_rna[i] == 'X'){
            my_rna.erase(i,1);
            i--;
        }
        if(rhs_rna[i] == 'X'){
            rhs_rna.erase(i,1);
            i--;
        }
    }
    
    delete nucleo_base;
    delete rhs.nucleo_base;
    
    nucleo_base = new NucleoBase(my_rna, NULL);
    rhs.nucleo_base = new NucleoBase(rhs_rna, NULL);
    
}
/*********************************************************************/
