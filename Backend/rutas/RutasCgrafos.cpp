#include <iostream>
#include "../CGrafos.hpp"
#include "../crow_all.h"
#include "../Grafo.hpp"

void registrarRutas(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/")([]() {
        return "Hello, Crow on Windows!";
    });
     
    CGrafos c;
    CROW_ROUTE(app, "/cgrafo").methods("POST"_method)
    ([&](const crow::request& req) {
        auto body = crow::json::load(req.body);

        if (!body) return crow::response(400);
    
        crow::json::wvalue res;
        if (!body.has("numC")) {
            res["result"] = "Falta el campo 'numC'";
            return crow::response{res};
        }
        if(!body.has("numEstudiantes")) {
            res["result"] = "Falta el campo 'numEstudiantes'";
            return crow::response{res};
        }

        int numC = body["numC"].i();
        int numEstudiantes = body["numEstudiantes"].i();
          Grafo g(numEstudiantes);
          g.agregarArista(0, 1);
          g.agregarArista(0, 4);
          g.agregarArista(1, 2);
          g.agregarArista(1, 3);
          g.agregarArista(1, 4);
          g.agregarArista(2, 3);
          g.agregarArista(3, 4);
        std::vector<int> colores(g.tamano(), -1);
        std::vector<std::vector<int>> grafo = g.obtenerAdyacencia();
        CGrafos c;
        if (c.btColor(grafo, colores, 0, numC)) {
            res["result"] = colores;
        } else {
            res["result"] = "No se pudo colorear con esa cantidad de colores";
        }

        return crow::response{res};
    });
}
