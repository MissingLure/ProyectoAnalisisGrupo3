#include <iostream>
#include <vector>
#include "../back-end/rutas/RutasCgrafos.cpp"

int main() {

    crow::SimpleApp app;

    // Rutas separadas
    registrarRutas(app);

    app.port(18080).multithreaded().run();

    return 0;
}