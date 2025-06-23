
using namespace std;
#include "SetCoverAproximado.h"
#include "SetCoverExacto.h"
#include <unordered_set>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <cassert>
#include "crow_all.h"
#include "api.h"

void Aproximado(std::vector<std::vector<int>> sets, std::vector<element> &universe ){
/* std::vector<element> universe;
    for (int i = 0; i < 100; i++)
    {
        universe.push_back(element(i));
    }

    auto sets = SetCoverAproximado::generateRandomSets(100, 5, 20, universe.size());
    std::cout << "Generated sets:" << std::endl;
    SetCoverAproximado::printSets(sets);
*/

auto start = std::chrono::high_resolution_clock::now();
auto selectedSets = SetCoverAproximado::setCover(universe, sets);
auto end = std::chrono::high_resolution_clock::now();

std::chrono::duration<double> duration = end - start;
double time_seconds = duration.count();
int minutos = static_cast<int>(time_seconds) / 60;
int segundos = static_cast<int>(time_seconds) % 60;
int milisegundos = static_cast<int>((time_seconds - minutos * 60 - segundos) * 1000);

SetCoverAproximado::printSets(sets, selectedSets, universe.size());
std::cout << "Tiempo: " << time_seconds * 1000 << " ms ("
          << minutos << " min " << segundos << " s " << milisegundos << " ms)" << std::endl;


    
}

void ExactoUniverse(std::vector<std::vector<int>> sets, std::vector<element> &universe){

     std::vector<std::vector<bool>> amatrix(universe.size(), std::vector<bool>(sets.size(), false));
    for (int j = 0; j < sets.size(); ++j) {
        for (int idx : sets[j]) {
            amatrix[idx][j] = true;
        }
    }

    std::vector<double> cost(sets.size(), 1.0);
    
    SetCoverExacto solver(amatrix, cost);
    auto [UB, time_used] = solver.SolveSCP();


double total_seconds = time_used * 60.0;
int minutos = static_cast<int>(total_seconds) / 60;
int segundos = static_cast<int>(total_seconds) % 60;
int milisegundos = static_cast<int>((total_seconds - minutos * 60 - segundos) * 1000);

std::cout << "Tiempo: " << total_seconds * 1000 << " ms ("
          << minutos << " min " << segundos << " s " << milisegundos << " ms)" << std::endl;

    
}

int main()
{
    /*
    std::vector<element> universe;
    for (int i = 0; i < 1000; i++)
    {
        universe.push_back(element(i));
    }
      auto sets = SetCoverAproximado::generateRandomSets(1000, 1000, 1000, universe.size());

      */


    //return 0;

    
    // Inicializa la aplicación Crow
    crow::SimpleApp app;
    registrarRutas(app);
    app.port(18080).multithreaded().run();

}
