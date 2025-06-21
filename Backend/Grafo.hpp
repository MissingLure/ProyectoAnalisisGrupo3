#include <iostream>
#include <vector>
#include <iostream>
#include <vector>
#include <iomanip> // Para std::setw, para una salida más bonita de la matriz

class Grafo {
private:
    int numV; 
    std::vector<std::vector<int>> adjMatrix; 

public:
    
    Grafo(int V) {
        this->numV = V;
        adjMatrix.resize(V, std::vector<int>(V, 0));
    }

    
    void agregarArista(int u, int v) {
        // Validaciones básicas para evitar accesos fuera de rango
        if (u >= 0 && u < numV && v >= 0 && v < numV) {
            adjMatrix[u][v] = 1; // Marca la conexión de u a v
            adjMatrix[v][u] = 1; // Marca la conexión de v a u (para grafo no dirigido)
            std::cout << "Arista agregada: (" << u << ", " << v << ")" << std::endl;
        } else {
            std::cerr << "Error: Vértices (" << u << ", " << v << ") fuera de rango." << std::endl;
        }
    }

    // Método para mostrar el grafo usando la matriz de adyacencia
    void mostrarGrafo() {
        std::cout << "Matriz de Adyacencia (" << numV << "x" << numV << "):" << std::endl;
        
        // Imprimir cabecera de columnas
        std::cout << "  "; // Espacio para el número de fila
        for (int i = 0; i < numV; ++i) {
            std::cout << std::setw(3) << i;
        }
        std::cout << std::endl;
        std::cout << "  "; // Línea separadora
        for (int i = 0; i < numV; ++i) {
            std::cout << "---";
        }
        std::cout << std::endl;

        // Imprimir filas y contenido de la matriz
        for (int i = 0; i < numV; ++i) {
            std::cout << std::setw(2) << i << "|"; // Número de fila
            for (int j = 0; j < numV; ++j) {
                std::cout << std::setw(3) << adjMatrix[i][j];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    
   
    bool existeArista(int u, int v) const {
        if (u >= 0 && u < numV && v >= 0 && v < numV) {
            return adjMatrix[u][v] == 1;
        }
        return false; 
    }

  
    int tamano() const {
        return numV;
    }

    
    const std::vector<std::vector<int>>& obtenerAdyacencia() const {
        return adjMatrix;
    }
};
