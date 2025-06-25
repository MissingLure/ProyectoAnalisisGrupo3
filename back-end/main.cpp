#include <iostream>
#include <vector>
#include <chrono>
#include "TSP/rutasTSP.h"
#include "SetCover/api.h"
#include "Coloracion/RutasCGrafos.h"

using namespace std;
using namespace chrono;
using json = nlohmann::json;

int main()
{
    crow::SimpleApp app;
    registrarRutas(app);
    registrarRutasSetCover(app);
    registrarRutasCGrafos(app);
    cout << "Crow server starting on port 18080..." << endl;
    app.port(18080).multithreaded().run();

    return 0;
}