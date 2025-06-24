#include <iostream>
#include <vector>
#include <chrono>
#include "grafos.h"
#include "crow_all.h"
#include <nlohmann/json.hpp>
#include "../back-end/rutas/rutasTSP.cpp"

using namespace std;
using namespace chrono;
using json = nlohmann::json;

int main()
{
    crow::SimpleApp app;
    registrarRutas(app);

    cout << "Crow server starting on port 18080..." << endl;
    app.port(18080).multithreaded().run();

    return 0;
}