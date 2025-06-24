#include "api.h"
#include "SetCoverAproximado.h"
#include "SetCoverExacto.h"
#include <vector>
#include <chrono>

void registrarRutasSetCover(crow::SimpleApp &app)
{
    // Aproximado
    CROW_ROUTE(app, "/api/SetCoverAproximado").methods("POST"_method)([](const crow::request &req)
                                                                      {
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "JSON inválido");

        int numSets = body["numSets"].i();
        int minSetSize = body["minSetSize"].i();
        int maxSetSize = body["maxSetSize"].i();
        int universeSize = body["universeSize"].i();

        std::vector<element> universe;
        for (int i = 0; i < universeSize; i++) universe.push_back(element(i));
        auto sets = SetCoverAproximado::generateRandomSets(numSets, minSetSize, maxSetSize, universeSize);

        auto start = std::chrono::high_resolution_clock::now();
        auto selectedSets = SetCoverAproximado::setCover(universe, sets);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        double ms = duration.count() * 1000.0;

        crow::json::wvalue res;
        res["tiempo_ms"] = ms;
        res["num_conjuntos"] = (int)selectedSets.size();
        for (int i = 0; i < selectedSets.size(); ++i) {
            int idx = selectedSets[i];
            res["solucion"][i]["indice"] = idx;
            for (int j = 0; j < sets[idx].size(); ++j) {
                res["solucion"][i]["conjunto"][j] = sets[idx][j];
            }
}
    crow::response responseFinal{res};
    responseFinal.add_header("Access-Control-Allow-Origin", "*");
    responseFinal.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    responseFinal.add_header("Access-Control-Allow-Headers", "Content-Type");
    return responseFinal;
    });

    // Exacto
    CROW_ROUTE(app, "/api/SetCoverExacto").methods("POST"_method)([](const crow::request &req)
                                                                  {
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "JSON inválido");

        int numSets = body["numSets"].i();
        int minSetSize = body["minSetSize"].i();
        int maxSetSize = body["maxSetSize"].i();
        int universeSize = body["universeSize"].i();

        std::vector<element> universe;
        for (int i = 0; i < universeSize; i++) universe.push_back(element(i));
        auto sets = SetCoverAproximado::generateRandomSets(numSets, minSetSize, maxSetSize, universeSize);

        std::vector<std::vector<bool>> amatrix(universeSize, std::vector<bool>(sets.size(), false));
        for (int j = 0; j < sets.size(); ++j) {
            for (int idx : sets[j]) {
                amatrix[idx][j] = true;
            }
        }
        std::vector<double> cost(sets.size(), 1.0);
        SetCoverExacto solver(amatrix, cost);

        auto result = solver.SolveSCP();
        double UB = result.first;
        double time_min = result.second;
        double ms = time_min * 60.0 * 1000.0;
        auto seleccionados = solver.getSelectedIndices();


        crow::json::wvalue res;
        res["tiempo_ms"] = ms;
        res["num_conjuntos"] = (int)seleccionados.size();
        for (int i = 0; i < seleccionados.size(); ++i) {
        int idx = seleccionados[i];
        res["solucion"][i]["indice"] = idx;
        for (int j = 0; j < sets[idx].size(); ++j) {
        res["solucion"][i]["conjunto"][j] = sets[idx][j];
        }
    }   
        crow::response responseFinal{res};
        responseFinal.add_header("Access-Control-Allow-Origin", "*");
        responseFinal.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        responseFinal.add_header("Access-Control-Allow-Headers", "Content-Type");
        return responseFinal;
    });

    CROW_ROUTE(app, "/api/SetCoverAproximado").methods("OPTIONS"_method)([](const crow::request&) {
    crow::response res;
    res.code = 204; // No Content
    res.add_header("Access-Control-Allow-Origin", "*");
    res.add_header("Access-Control-Allow-Methods", "POST, OPTIONS");
    res.add_header("Access-Control-Allow-Headers", "Content-Type");
    return res;
});

// OPTIONS para Exacto (opcional si vas a usarlo también)
CROW_ROUTE(app, "/api/SetCoverExacto").methods("OPTIONS"_method)([](const crow::request&) {
    crow::response res;
    res.code = 204;
    res.add_header("Access-Control-Allow-Origin", "*");
    res.add_header("Access-Control-Allow-Methods", "POST, OPTIONS");
    res.add_header("Access-Control-Allow-Headers", "Content-Type");
    return res;
});
}
