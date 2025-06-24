#include <cmath>
#include "../CGrafos.hpp"
#include "../Grafo.hpp"
#include "../crow_all.h"

void registrarRutas(crow::SimpleApp& app) {
     CROW_ROUTE(app, "/")([]() {
        return "Hello, Crow on Windows!";
    });
CROW_ROUTE(app, "/cgrafo").methods("POST"_method)
([](const crow::request& req) {
    auto body = crow::json::load(req.body);
    crow::json::wvalue res;

    if (!body || !body.has("numC") || !body.has("numEstudiantes") || !body.has("algoritmo")) {
        res["result"] = "Faltan campos 'numC', 'numEstudiantes' o 'algoritmo'";
        return crow::response(400, res);
    }

    int numC = body["numC"].i();
    int numEstudiantes = body["numEstudiantes"].i();
    std::string algoritmo = body["algoritmo"].s();

    const int sillasPorMesa = 5;
    const int mesasPorFila = 2;
    const int estudiantesPorFila = sillasPorMesa * mesasPorFila;

    int filas = static_cast<int>(std::ceil((double)numEstudiantes / estudiantesPorFila));

    Grafo g(numEstudiantes);

    int totalMesas = static_cast<int>(std::ceil(numEstudiantes / 5.0));
    int inicio = 0;
    int fin = 0;
    int number = 0;

    for (int m = 0; m < totalMesas; ++m) {
        inicio = m * 5;
        fin = std::min(inicio + 5, numEstudiantes);
        for (int i = inicio; i < fin; ++i) {
            number = (i + 1) % 20;
            if (number != 0 && i + 1 < numEstudiantes) {
                g.agregarArista(i, i + 1);
            }
            if (i >= 20) {
                g.agregarArista(i, i - 20);
            }
        }
    }

    std::vector<int> colores(numEstudiantes, -1);
    std::vector<std::vector<int>> grafo = g.obtenerAdyacencia();
    CGrafos c;

    double tiempo;

    if (algoritmo == "btColor") {
        auto btColorToMeasure = [&](std::vector<int>& cls) {
            return c.btColor(grafo, cls, 0, numC);
        };
        tiempo = c.medirTiempo("btColor (Backtracking)", btColorToMeasure, colores);
    } else if (algoritmo == "btPropio") {
        auto btPropioToMeasure = [&](std::vector<int>& cls) {
            return c.BtPropio(grafo, cls, numC);
        };
        tiempo = c.medirTiempo("btPropio (Backtracking propio)", btPropioToMeasure, colores);
    } else {
        res["result"] = "Algoritmo no válido";
        return crow::response(400, res);
    }

    if (std::find(colores.begin(), colores.end(), -1) == colores.end()) {
        res["result"] = colores;
        res["tiempo"] = tiempo;
    } else {
        res["result"] = "No se pudo colorear con esa cantidad de colores";
        res["tiempo"] = tiempo;
    }

    return crow::response(res);
});


}
