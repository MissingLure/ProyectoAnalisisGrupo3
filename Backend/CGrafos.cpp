#include "CGrafos.hpp"
CGrafos::CGrafos()
{
}
bool CGrafos::btColor(std::vector<std::vector<int>> &g, std::vector<int> &colores, int nodo, int cantColores)
{
    if (nodo == g.size()) return true;

    for (int color = 1; color <= cantColores; color++) {
        if (Coloring(g, nodo, colores, color)) {
            colores[nodo] = color;
            if (btColor(g, colores, nodo + 1, cantColores)) {
                return true;
            }
            colores[nodo] = -1; // backtrack
        }
    }
    return false; // no color válido
}

bool CGrafos::Coloring(std::vector<std::vector<int>>&g,int nodo, std::vector<int>& colores, int color) {
    for (int vecino : g[nodo]) {        
            if (colores[vecino] == color) {
                return false; 
            }	
    }
    return true;
}




bool CGrafos::BtPropio(std::vector<std::vector<int>>& g, std::vector<int>& colores2, int cantColores)  {
    for(int v=0; v<g.size(); v++){
        for(int color = 0; color<cantColores;color++){
            if(Coloring(g,v,colores2,color)){
                colores2[v]=color;
            }
        }
    }
    std::cout<<"Validando colores2 \n"<<std::endl;
    for(int c: colores2){
        //std::cout<<"Color: "<<c<<std::endl;
        if(c==-1){
            return false;
        }
    }
    return true;
} 