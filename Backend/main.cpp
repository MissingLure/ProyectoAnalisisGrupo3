#include <iostream>
#include <vector>
#include "../Backend/rutas/RutasCgrafos.cpp"

int main() {

    crow::SimpleApp app;

    // Rutas separadas
    registrarRutas(app);

    app.port(18080).multithreaded().run();

    return 0;
    //std::vector<int>colores2(g.tamano(),-1);
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