#include "CGrafos.hpp" 
#include <iostream>
#include <vector>
#include <stack>

CGrafos::CGrafos(){}

bool CGrafos::Coloring(std::vector<std::vector<int>>& g, int nodo, std::vector<int>& colores, int colorIntento) {
    
    for (int vecino = 0; vecino < g.size(); ++vecino) {
        
        if (g[nodo][vecino] == 1 && colores[vecino] == colorIntento) {
            return false; 
        }
    }
    return true; 
}

bool CGrafos::btColor(std::vector<std::vector<int>>& g, std::vector<int>& colores, int nodo, int cantColores) {

    if (nodo == g.size()) {
        return true; 
    }
  
    for (int color = 0; color < cantColores; color++) { 
        
        if (Coloring(g, nodo, colores, color)) {
          
            colores[nodo] = color;
      
            if (btColor(g, colores, nodo + 1, cantColores)) {
                return true; 
            }
       
            colores[nodo] = -1; 
        }
    }

    return false; 
}

bool CGrafos::BtPropio(std::vector<std::vector<int>>& g, std::vector<int>& colores, int cantColores) {
    colores.assign(g.size(), -1);
    std::stack<int> s;
    s.push(0);
    int nodoActual = 0;
    bool colorEncontrado = false;
    while(!s.empty()){
        nodoActual = s.top();
        if(nodoActual == g.size()) {
            return true; 
        }
        for(int color = 0; color < cantColores; color++) {
            if(Coloring(g, nodoActual, colores, color)) {
                colores[nodoActual] = color;
                s.push(nodoActual + 1);
                colorEncontrado = true;
                break; 
            }
        }

        if(!colorEncontrado) {
            colores[nodoActual] = -1; 
            if(s.empty()) {
                s.pop();
                return false;
            }
           
        } else {
            colorEncontrado = false; 
        }

    }
     return false;
}