#include <iostream>
#include <vector>
#include "rutas/RutasCgrafos.cpp"


 /* std::vector<int> reordenar(std::vector<std::vector<int>>& g) {  
        int n = g.size();  
        std::vector<int> orden(n);  
        for (int i = 0; i < n; ++i) {  
            orden[i] = i; 
        }  
         
        for (int i = 0; i < n - 1; ++i) {  
            for (int j = 0; j < n - i - 1; ++j) {  
                if (g[orden[j]].size() < g[orden[j + 1]].size()) {    
                    std::swap(orden[j], orden[j + 1]);  
                }  
            }  
        }  
        return orden;  
    }

    bool WelshPowell(std::vector<std::vector<int>>&g, int cantColores,std::vector<int> colores) {
		std::vector<int> orden = reordenar(g);
		int n = g.size();
		colores[orden[0]] = 0; // Asignar el primer color al primer nodo en el orden
        for (int i = 0; i < n-1; i++) {
            //for (int c : g[i]) {
              //  if(i==)
          //}
        }

        return false;
    }*/

int main() {
    Grafo g(5);
    CGrafos c;

    // Aristas de ejemplo
    g.agregarArista(0, 1);
    g.agregarArista(0, 4);
    g.agregarArista(1, 2);
    g.agregarArista(1, 3);
    g.agregarArista(1, 4);
    g.agregarArista(2, 3);
    g.agregarArista(3, 4);

    crow::SimpleApp app;

    // Rutas separadas
    registrarRutas(app, c, g);

    app.port(18080).multithreaded().run();

    return 0;
    std::vector<int>colores2(g.tamano(),-1);
	//std::vector<int> orden = reordenar(adj);
	
    /*std::cout << "Orden:\n";
    for (int i = 0; i < adj.size(); ++i) {
        std::cout << "Nodo " << orden[i];
     
        std::cout << std::endl;
    }*/
    
	// Ejemplo de Backtracking
   /* cout << "Backtracking:\n";

    if (btColor(adj, colores, 0, 7)) {
        cout << "Coloracion valida:\n";
        for (int i = 0; i < colores.size(); ++i)
            cout << "Nodo: " << i << "  Color: " << colores[i] << '\n';
    }
    else {
        std::cout << "La cantidad de colores no es la adecuada";
    }
    cout << "Implementacion propia:\n";
    if (Cgrafos(adj, colores2,4)) {
        cout << "Coloracion valida:\n";
        for (int i = 0; i < colores2.size(); ++i)
            cout << "Nodo: " << i << "  Color: " << colores2[i] << '\n';
    }
    else {
        std::cout << "La cantidad de colores no es la adecuada";
    }*/
    return 0;
}