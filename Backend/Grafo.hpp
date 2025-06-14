#include <iostream>
#include <vector>
class Grafo {
private:
    int numV; 
    std::vector<std::vector<int>> adj; 
	
public:
    Grafo(int V) {
        this->numV = V;
        adj.resize(V);
    }

    
    void agregarArista(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); 
    }

    
    void mostrarGrafo() {
        for (int i = 0; i < numV; ++i) {
            std::cout << "Nodo " << i << ": ";
            for (int vecino : adj[i]) {
                std::cout << vecino << " ";
            }
            std::cout << std::endl;
        }
    }
    const std::vector<std::vector<int>>& obtenerAdyacencia() const {
        return adj;
    }
    int tamano() const {
        return numV;
	}
};
