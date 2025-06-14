
#include <iostream>
#include "../CGrafos.hpp"
#include "../crow_all.h"

 

void registrarRutas(crow::SimpleApp& app, CGrafos& c, Grafo& g) {
    CROW_ROUTE(app, "/")([]() {
        return "Hello, Crow on Windows!";
    });

    CROW_ROUTE(app, "/cgrafo").methods("POST"_method)
    ([&c, &g](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400);

        crow::json::wvalue res;
        if (!body.has("numC")) {
            res["result"] = "Falta el campo 'numC'";
            return crow::response{res};
        }

        int numC = body["numC"].i();
        std::vector<int> colores(g.tamano(), -1);
        std::vector<std::vector<int>> grafo = g.obtenerAdyacencia();

        if (c.btColor(grafo, colores, 0, numC)) {
            res["result"] = colores;
        } else {
            res["result"] = "No se pudo colorear con esa cantidad de colores";
        }

        return crow::response{res};
    });
}
