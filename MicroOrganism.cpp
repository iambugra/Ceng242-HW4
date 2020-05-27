#include "MicroOrganism.h"


/*********************************************************************
*                                    Constructor
*
* The first param is the microorganism id
* The second param is the edges of Wall type
  The given edges will have been constructed by their lengths and radius
  (without a definite location)
*/
MicroOrganism::MicroOrganism(int id, const vector<Wall*>& edges){
    this->id_no = id;
    for(int i=0; i<my_edges.size(); i++)
        my_edges.push_back(edges[i]->Clone());
    cell_cnd = NULL;
}
/*********************************************************************
*                                    Stream Operator
*
* Prints the location of the microorganism
* If it is located in a cell, print in the following format:
  "The microorganism <id1> was successfully placed into the cell <id2>."
  where id1 is the microorganism id and id2 is the cell id.
  If the microorganism is not located into any cell, then print:
  "The microorganism <id> could not be placed into any cell!"
* Do NOT add new line character "\n" to the end of the stream.
*/
ostream& operator<<(ostream& os, const MicroOrganism& morg){
    if(morg.cell_cnd == NULL)
        os << "The microorganism " << morg.id_no << " could not be placed into any cell!";
    else
        os << "The microorganism " << morg.id_no << " was successfully placed into the cell " << morg.cell_cnd->GetCellID() << ".";
    return os;
}


MicroOrganism::~MicroOrganism(){
    for(int i=0; i<my_edges.size(); i++)
        if(my_edges[i] != NULL)
            delete my_edges[i];
    
}
