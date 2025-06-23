#include <iostream>
#include <vector>
#include <chrono>

#include "grafos.h"
#include "crow_all.h"
#include <nlohmann/json.hpp>

using namespace std;
using namespace chrono;
using json = nlohmann::json;

void registrarRutas(crow::SimpleApp &app)
{
    CROW_ROUTE(app, "/")([]()
                         { return "Hello, world!"; });

    CROW_ROUTE(app, "/solve-tsp").methods("POST"_method)([](const crow::request &req)
                                                         {
    json req_body;
    try {
        req_body = json::parse(req.body);
    } catch (const json::parse_error& e) {
        return crow::response(400, "Invalid JSON: " + string(e.what()));
    }

    json points = req_body["points"];
    json adj_matriz = req_body["matriz"];
    int N = points.size();
    Grafos g(N);
    vector<vector<int>> matriz(N, vector<int>(N));

    try {
        for (int i = 0; i < N; ++i) {
            if (!adj_matriz[i].is_array() || adj_matriz[i].size() != N)
                return crow::response(400, "Invalid dimensions.");
            for (int j = 0; j < N; ++j)
                matriz[i][j] = adj_matriz[i][j].get<int>();
        }
    } catch (const json::exception& e) {
        return crow::response(400, "Error parsing values: " + string(e.what()));
    }

    auto start = high_resolution_clock::now();
    vector<int> ruta = g.TSPComunidad(matriz);
    auto end = high_resolution_clock::now();
    double totalCost = 0;
    if (ruta.size() > 1) {
        for (int i = 0; i < ruta.size() - 1; ++i) {
            int cost = matriz[ruta[i]][ruta[i + 1]];
            totalCost += cost;
        }
    }

    json res;
    res["path_indices"] = ruta;
    res["distance"] = totalCost;
    res["time"] = duration<double>(end - start).count();

    json path_coords = json::array();
    for (int index : ruta) {
        if (index >= 0 && index < N) {
            path_coords.push_back({
                {"lat", points[index]["lat"].get<double>()},
                {"lng", points[index]["lng"].get<double>()}
            });
        }
    }
    res["path"] = path_coords;

    crow::response response(res.dump());
    response.code = 200;
    response.set_header("Content-Type", "application/json");
    return response; });
