#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <set>
#include <iterator>
#include <unordered_set>

//Valores constantes Magicos
#define STEPSIZE 0.1
#define LARGENUMBER 1e5
#define SMALLNUMBER 1e-5

class SetCoverExacto
{
public:
   SetCoverExacto(
        const std::vector<std::vector<bool>>& amatrix,
        const std::vector<double>& cost,
        int maxiters = 20,
        int subg_nsteps = 15,
        int subg_maxiters = 100
    );
    std::pair<double, double> SolveSCP();
    void greedy(const std::vector<double>* u_ext = nullptr, int niters_max = 1000);
    std::pair<std::vector<std::vector<double>>, std::vector<double>> subgradient();
    void printSelectedSets() const;
    std::vector<int> getSelectedIndices() const;


private:
// Matrices y vectores principales
    std::vector<std::vector<bool>> a;     // Matriz de cobertura
    std::vector<double> c;                // Costos
    int mrows, ncols;
    int debug = 0; // Variable de depuración
    // "Magic numbers"
    double _stepsize;
    int _subg_nadaptive;
    int _subg_nsteps;
    int _subg_maxiters;
    double _subg_maxfracchange;
    double _subg_maxabschange;
    double _max_adapt;
    double _min_adapt;
    double _u_perturb;

    int _maxiters;
    double _maxfracchange;
    double _LB_maxfracchange;

    // Variables de solución
    std::vector<bool> f_uniq, f, f_covered, s;
    std::vector<double> u;

    // Métodos privados (declaraciones)
    std::vector<bool> fix_uniq_col();
    std::vector<double> u_naught();
    double getTotalCost() const;
    double getFixedCost() const;
    void reset_all();
void reset_s();
void reset_f();
void reset_u(bool random = false);

std::vector<double> u_naught_simple();

std::pair<std::vector<std::vector<bool>>, std::vector<int>> update_core();
std::vector<bool> subgradient_solution(const std::vector<double>* u_ext = nullptr) const;

bool estanTodosCubiertosSeleccionados(int universeSize) const {
    std::unordered_set<int> cubiertos;
    for (int j = 0; j < s.size(); ++j) {
        if (s[j]) {
            for (int i = 0; i < mrows; ++i) {
                if (a[i][j])
                    cubiertos.insert(i);
            }
        }
    }
    bool completos = true;
    for (int i = 0; i < universeSize; ++i) {
        if (cubiertos.find(i) == cubiertos.end()) {
            std::cout << "Falta el elemento " << i << std::endl;
            completos = false;
        }
    }
    return completos;
}

};