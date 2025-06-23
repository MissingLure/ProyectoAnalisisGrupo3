#include "SetCoverExacto.h"

#include <algorithm> // Para std::fill, etc.
#include <cassert>

SetCoverExacto::SetCoverExacto(
    const std::vector<std::vector<bool>>& amatrix,
    const std::vector<double>& cost,
    int maxiters,
    int subg_nsteps,
    int subg_maxiters
)
    : a(amatrix), c(cost), mrows(amatrix.size()), ncols(amatrix[0].size()),
      _stepsize(0.1),                   // Equivalente a _stepsize = 0.1
      _subg_nadaptive(20),              // hardcoded en Python
      _subg_nsteps(20 * subg_nsteps),   // 20 * subg_nsteps como en Python
      _subg_maxiters(subg_maxiters),
      _subg_maxfracchange(0.000020),
      _subg_maxabschange(0.010),
      _max_adapt(0.06),
      _min_adapt(0.002),
      _u_perturb(0.06),
      _maxiters(maxiters),
      _maxfracchange(0.001),
      _LB_maxfracchange(0.050)
{
    // f_uniq = _fix_uniq_col()
    f_uniq = fix_uniq_col();

    // f = f_uniq (copia)
    f = f_uniq;

    // f_covered = np.any(self.a[:,self.f], axis=1) -- traduce: cada fila, ¿hay al menos un 'true' en columnas f?
    f_covered.resize(mrows, false);
    for (int i = 0; i < mrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            if (f[j] && a[i][j]) {
                f_covered[i] = true;
                break;
            }
        }
    }

    // s = f_uniq (copia)
    s = f_uniq;

    // u = _u_naught()
    u = u_naught();
}

void SetCoverExacto::printSelectedSets() const {
    std::cout << "\nConjuntos seleccionados por el algoritmo exacto:\n";
    for (int j = 0; j < s.size(); ++j) {
        if (s[j]) {
            std::cout << "Conjunto " << j << ": { ";
            for (int i = 0; i < mrows; ++i) {
                if (a[i][j]) std::cout << i << " ";
            }
            std::cout << "}\n";
        }
    }

     // Validación de cobertura
    if (estanTodosCubiertosSeleccionados(mrows)) {
        std::cout << "¡Todos los elementos del universo están cubiertos!\n";
    } else {
        std::cout << "Advertencia: NO todos los elementos del universo están cubiertos.\n";
    }

      std::cout << "Seleccionados: " << std::count(s.begin(), s.end(), true) << "; ";
}

std::vector<int> SetCoverExacto::getSelectedIndices() const {
    std::vector<int> indices;
    for (int j = 0; j < s.size(); ++j) {
        if (s[j]) {
            indices.push_back(j);
        }
    }
    return indices;
}


double SetCoverExacto::getTotalCost() const {
    double sum = 0.0;
    for (int j = 0; j < ncols; ++j)
        if (s[j]) sum += c[j];
    return sum;
}

double SetCoverExacto::getFixedCost() const {
    double sum = 0.0;
    for (int j = 0; j < ncols; ++j)
        if (f[j]) sum += c[j];
    return sum;
}

void SetCoverExacto::reset_all() {
    _stepsize = 0.1;
    reset_f();
    reset_s();
    reset_u();
}

void SetCoverExacto::reset_s() {
    s = f_uniq; // Copia directa
}

void SetCoverExacto::reset_f() {
    f = f_uniq;
    f_covered.assign(mrows, false);
    for (int i = 0; i < mrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            if (f[j] && a[i][j]) {
                f_covered[i] = true;
                break;
            }
        }
    }
}

void SetCoverExacto::reset_u(bool random) {
    if (random)
        u = u_naught_simple();
    else
        u = u_naught();
}

std::vector<double> SetCoverExacto::u_naught_simple() {
    std::vector<double> vec(mrows);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int i = 0; i < mrows; ++i)
        vec[i] = dis(gen);
    return vec;
}

std::vector<double> SetCoverExacto::u_naught() {
    // Paso 1: Calcular adjusted_cost para cada columna
    std::vector<double> covered_count(ncols, 0.0);
    for (int j = 0; j < ncols; ++j)
        for (int i = 0; i < mrows; ++i)
            if (a[i][j]) covered_count[j] += 1.0;
    
    std::vector<double> adjusted_cost(ncols, 1e12); // Usa un valor muy grande para columnas que no cubren nada
    for (int j = 0; j < ncols; ++j)
        if (covered_count[j] > 0)
            adjusted_cost[j] = c[j] / covered_count[j];
    
    // Paso 2: Para cada fila, buscar el mínimo adjusted_cost de las columnas que la cubren
    std::vector<double> result(mrows, 1e12);
    for (int i = 0; i < mrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            if (a[i][j] && adjusted_cost[j] < result[i]) {
                result[i] = adjusted_cost[j];
            }
        }
        // Si ninguna columna cubre la fila, podría quedarse en 1e12 (no debería pasar si los datos están bien formados)
    }
    return result;
}

std::vector<bool> SetCoverExacto::fix_uniq_col() {
    std::vector<int> n_covered_col(mrows, 0);
    // Para cada fila, contar cuántas columnas la cubren
    for (int i = 0; i < mrows; ++i)
        for (int j = 0; j < ncols; ++j)
            if (a[i][j]) n_covered_col[i]++;
    
    std::vector<bool> ifix(ncols, false);
    // Verifica que todas las filas estén cubiertas por al menos una columna
    for (int i = 0; i < mrows; ++i)
        if (n_covered_col[i] == 0)
            throw std::runtime_error("There are uncovered rows! Please check your input!");

    // Para cada fila cubierta solo por UNA columna, esa columna es única y debe ir en la solución
    for (int i = 0; i < mrows; ++i) {
        if (n_covered_col[i] == 1) {
            for (int j = 0; j < ncols; ++j)
                if (a[i][j]) ifix[j] = true;
        }
    }
    return ifix;
}


void SetCoverExacto::greedy(const std::vector<double>* u_ext, int niters_max) {
    int niters = 1;
    const std::vector<double>& u = (u_ext != nullptr) ? *u_ext : this->u;
    std::vector<double> utmp = u;
    // 1. iuncovered: filas no cubiertas
    std::vector<bool> iuncovered(mrows, false);
    for (int i = 0; i < mrows; ++i) {
        bool covered = false;
        for (int j = 0; j < ncols; ++j)
            if (s[j] && a[i][j]) { covered = true; break; }
        iuncovered[i] = !covered;
    }

    std::vector<double> score(ncols, 0.0);

    while (std::count(iuncovered.begin(), iuncovered.end(), true) > 0 && niters <= niters_max) {
        // mu[j]: cantidad de filas no cubiertas que cubre cada columna
        std::vector<double> mu(ncols, 0.0);
        for (int j = 0; j < ncols; ++j)
            for (int i = 0; i < mrows; ++i)
                if (a[i][j] && iuncovered[i])
                    mu[j] += 1.0;
        for (int j = 0; j < ncols; ++j)
            if (mu[j] <= SMALLNUMBER) mu[j] = SMALLNUMBER;

        // utmp[i] = 0 para las filas cubiertas
        for (int i = 0; i < mrows; ++i)
            if (!iuncovered[i]) utmp[i] = 0.0;

        // gamma[j] = c[j] - suma( a[i][j] * utmp[i] ) para todas las filas i
        std::vector<double> gamma(ncols, 0.0);
        for (int j = 0; j < ncols; ++j) {
            double sum = 0.0;
            for (int i = 0; i < mrows; ++i)
                if (a[i][j])
                    sum += utmp[i];
            gamma[j] = c[j] - sum;
        }

        // select_gamma: true donde gamma[j]>=0
        std::vector<bool> select_gamma(ncols, false);
        for (int j = 0; j < ncols; ++j)
            select_gamma[j] = (gamma[j] >= 0);

        // score[j] = (gamma[j] / mu[j]) si select_gamma, si no: gamma[j] * mu[j]
        for (int j = 0; j < ncols; ++j) {
            if (select_gamma[j])
                score[j] = gamma[j] / mu[j];
            else
                score[j] = gamma[j] * mu[j];
        }

        // Buscar la columna que NO está seleccionada y tiene menor score
        double min_score = 1e20;
        int inewcolumn = -1;
        for (int j = 0; j < ncols; ++j) {
            if (!s[j] && score[j] < min_score) {
                min_score = score[j];
                inewcolumn = j;
            }
        }

        if (inewcolumn == -1) break; // por seguridad, debería no ocurrir

        // Selecciona la columna y actualiza iuncovered
        s[inewcolumn] = true;
        for (int i = 0; i < mrows; ++i)
            if (a[i][inewcolumn])
                iuncovered[i] = false;

        ++niters;
    }

}


std::pair<std::vector<std::vector<bool>>, std::vector<int>> SetCoverExacto::update_core() {
    // Encuentra los índices de filas no cubiertas
    std::vector<int> core_rows;
    for (int i = 0; i < mrows; ++i)
        if (!f_covered[i])
            core_rows.push_back(i);

    // Encuentra los índices de columnas no fijas
    std::vector<int> core_cols;
    for (int j = 0; j < ncols; ++j)
        if (!f[j])
            core_cols.push_back(j);

    // Construye la nueva submatriz core
    std::vector<std::vector<bool>> a_core(core_rows.size(), std::vector<bool>(core_cols.size()));
    for (size_t i = 0; i < core_rows.size(); ++i)
        for (size_t j = 0; j < core_cols.size(); ++j)
            a_core[i][j] = a[core_rows[i]][core_cols[j]];

    // Devuelve la submatriz y los índices de columnas originales
    return {a_core, core_cols};
}

std::pair<std::vector<std::vector<double>>, std::vector<double>> SetCoverExacto::subgradient() {
    double UB_full = getTotalCost();
    std::vector<double> ufull = u;

    // Update core
    auto core_result = update_core();
    auto a_core = core_result.first;
    auto core_cols = core_result.second;

    int mcore = a_core.size();
    int ncore = (mcore > 0) ? a_core[0].size() : 0;

    // u_this: solo los multiplicadores para filas no cubiertas
    std::vector<double> u_this;
    for (int i = 0; i < mrows; ++i)
        if (!f_covered[i]) u_this.push_back(u[i]);

    // cost: solo las columnas no fijas
    std::vector<double> cost;
    for (int j = 0; j < ncols; ++j)
        if (!f[j]) cost.push_back(c[j]);

    double UB_fixed = getFixedCost();
    double UB = UB_full - UB_fixed;

    // Vectores de historial para guardar pasos
    std::vector<std::vector<double>> u_sequence(mcore, std::vector<double>(_subg_nsteps, 0.0));
    std::vector<double> Lu_sequence(_subg_nsteps, 0.0);

    std::vector<bool> x(ncore, false);

    int niters_max = _subg_maxiters;
    double maxfracchange = _subg_maxfracchange;
    double maxabschange = _subg_maxabschange;

    double f_change = LARGENUMBER;
    double a_change = LARGENUMBER;
    int niters = 0;
    double Lu_max0 = 0;

    while ((f_change > maxfracchange || a_change > maxabschange) && (niters < niters_max)) {
        // Perturbar u_this
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-1.0, 1.0);
        for (int i = 0; i < mcore; ++i)
            u_this[i] *= (1.0 + dis(gen) * _u_perturb);

        for (int i = 0; i < mcore; ++i)
            u_sequence[i][0] = u_this[i];

        // cost_u = cost - a_csc.dot(u_sequence[:,0])
        // Traducimos el producto matriz/vec
        std::vector<double> cost_u(ncore, 0.0);
        for (int j = 0; j < ncore; ++j) {
            double sum = 0.0;
            for (int i = 0; i < mcore; ++i)
                if (a_core[i][j])
                    sum += u_sequence[i][0];
            cost_u[j] = cost[j] - sum;
        }

        // Lu_sequence[0] = suma de cost_u negativos + suma de u_sequence[:,0]
        double sum_neg = 0.0;
        for (int j = 0; j < ncore; ++j)
            if (cost_u[j] < 0) sum_neg += cost_u[j];
        double sum_u = 0.0;
        for (int i = 0; i < mcore; ++i) sum_u += u_sequence[i][0];
        Lu_sequence[0] = sum_neg + sum_u;

        // Main steps
        for (int step = 0; step < _subg_nsteps - 1; ++step) {
            // x: true donde cost_u < 0
            for (int j = 0; j < ncore; ++j) x[j] = (cost_u[j] < 0);

            // s_u = 1 - a_csr.dot(x)
            std::vector<double> s_u(mcore, 1.0);
            for (int i = 0; i < mcore; ++i)
                for (int j = 0; j < ncore; ++j)
                    if (a_core[i][j] && x[j])
                        s_u[i] -= 1.0;

            // s_u_norm = suma de s_u^2
            double s_u_norm = 0.0;
            for (int i = 0; i < mcore; ++i)
                s_u_norm += s_u[i] * s_u[i];

            // update: u_temp = u_sequence[:,i] + stepsize*(UB-Lu_sequence[i])/s_u_norm * s_u
            std::vector<double> u_temp(mcore);
            for (int i = 0; i < mcore; ++i) {
                u_temp[i] = u_sequence[i][step] + _stepsize * (UB - Lu_sequence[step]) / s_u_norm * s_u[i];
                if (u_temp[i] < 0) u_temp[i] = 0;
            }
            for (int i = 0; i < mcore; ++i) u_sequence[i][step + 1] = u_temp[i];

            // cost_u para siguiente paso
            for (int j = 0; j < ncore; ++j) {
                double sum = 0.0;
                for (int i = 0; i < mcore; ++i)
                    if (a_core[i][j])
                        sum += u_sequence[i][step + 1];
                cost_u[j] = cost[j] - sum;
            }

            // Lu_sequence para siguiente paso
            double sum_neg2 = 0.0;
            for (int j = 0; j < ncore; ++j)
                if (cost_u[j] < 0) sum_neg2 += cost_u[j];
            double sum_u2 = 0.0;
            for (int i = 0; i < mcore; ++i) sum_u2 += u_sequence[i][step + 1];
            Lu_sequence[step + 1] = sum_neg2 + sum_u2;

            // Adaptar stepsize cada subg_nadaptive
            if ((step + 1) % _subg_nadaptive == 0) {
                double Lu_max_adapt = -1e100, Lu_min_adapt = 1e100;
                for (int k = step + 1 - _subg_nadaptive; k <= step + 1; ++k) {
                    Lu_max_adapt = std::max(Lu_max_adapt, Lu_sequence[k]);
                    Lu_min_adapt = std::min(Lu_min_adapt, Lu_sequence[k]);
                }
                if (Lu_max_adapt <= 0.0) Lu_max_adapt = SMALLNUMBER;
                double f_change_adapt = (Lu_max_adapt - Lu_min_adapt) / std::fabs(Lu_max_adapt);
                if (f_change_adapt > _max_adapt) _stepsize *= 0.5;
                else if (f_change_adapt < _min_adapt && _stepsize < 1.5) _stepsize *= 1.5;

                // swap (opcional, aquí puedes dejarlo simple si prefieres)
            }
        }

        // Escoge el paso óptimo
        int i_optimal = std::distance(Lu_sequence.begin(), std::max_element(Lu_sequence.begin(), Lu_sequence.end()));
        double Lu_max = Lu_sequence[i_optimal];
        for (int i = 0; i < mcore; ++i) u_this[i] = u_sequence[i][i_optimal];

        niters++;
        a_change = Lu_max - Lu_max0;
        f_change = a_change / std::fabs(Lu_max);
        Lu_max0 = Lu_max;

    }
    int k = 0;
    for (int i = 0; i < mrows; ++i)
        if (!f_covered[i])
            u[i] = u_this[k++];

    return {u_sequence, Lu_sequence};
}

std::vector<bool> SetCoverExacto::subgradient_solution(const std::vector<double>* u_ext) const {
    // Usa el vector u proporcionado, o el de la instancia
    const std::vector<double>& uvec = (u_ext != nullptr) ? *u_ext : this->u;

    std::vector<double> cost_u(ncols, 0.0);
    for (int j = 0; j < ncols; ++j) {
        double sum = 0.0;
        for (int i = 0; i < mrows; ++i)
            if (a[i][j])
                sum += uvec[i];
        cost_u[j] = c[j] - sum;
    }

    std::vector<bool> x(ncols, false);
    for (int j = 0; j < ncols; ++j)
        if (cost_u[j] < 0)
            x[j] = true;
    return x;
}

std::pair<double, double> SetCoverExacto::SolveSCP() {
    auto t0 = std::chrono::high_resolution_clock::now();

    double Lu_min = 0.0;
    int niters_max = _maxiters;
    double maxfracchange = _maxfracchange;

    reset_all(); // including u_naught(), first application
    greedy();
    double scp_min = getTotalCost();

    int niters = 0;
    double f_change = LARGENUMBER;
    double UB = 0.0, LB = 0.0;

    std::vector<double> u_min(mrows), Lu;
    while (f_change > maxfracchange && niters < niters_max) {
        // re-initialize u
        if (niters % 2 == 0)
            reset_u(true); // random = true
        else
            reset_u(false);

        // Primer subgradient: obtener u_tmp, Lu_tmp (puedes ignorar el primero si quieres)
        auto [u_seq_tmp, Lu_tmp] = subgradient();
        // Segundo subgradient: obtener u, Lu reales para este ciclo
        auto [u_seq, Lu_seq] = subgradient();

        // update_core te da los core_rows y core_cols actuales
        auto core_result = update_core();
        auto core_rows = core_result.first; // vector<vector<bool>>, filas del core
        std::vector<int> core_row_indices;
        for (size_t i = 0; i < core_rows.size(); ++i)
            core_row_indices.push_back(i); // O ajusta si necesitas los índices reales

        // scp_all: resultados greedy con cada multiplicador del subgradient
        std::vector<double> scp_all(_subg_nsteps, 0.0);
        for (int i = 0; i < _subg_nsteps; ++i) {
            reset_s();

            // Armar vector u para greedy, tamaño mrows
            std::vector<double> u_for_greedy(mrows, 0.0);
            int k = 0;
            for (int row = 0; row < mrows; ++row) {
                if (!f_covered[row]) {
                    u_for_greedy[row] = u_seq[k][i];
                    ++k;
                } else {
                    u_for_greedy[row] = u[row]; // o 0.0 si prefieres
                }
            }
            greedy(&u_for_greedy);
            scp_all[i] = getTotalCost();
        }

        // Buscar índices como en Python (mínimos, máximos, etc.)
        double min_scp = *std::min_element(scp_all.begin(), scp_all.end());
        std::vector<int> imin_tmp;
        for (int i = 0; i < _subg_nsteps; ++i)
            if (scp_all[i] == min_scp) imin_tmp.push_back(i);

        // El que tenga el máximo Lu en los empates
        int imin = imin_tmp[0];
        double max_lu = Lu_seq[imin];
        for (int idx : imin_tmp) {
            if (Lu_seq[idx] > max_lu) {
                max_lu = Lu_seq[idx];
                imin = idx;
            }
        }
        // El de máximo Lu global
        int imax = std::distance(Lu_seq.begin(), std::max_element(Lu_seq.begin(), Lu_seq.end()));

        if (niters % 5 == 0) {
            // std::cout << "This Best solution: UB=" << scp_all[imin] << ", LB=" << Lu_seq[imin]
            //           << ", UB1=" << scp_all[imax] << ", LB1=" << Lu_seq[imax] << std::endl;
        }

        if (niters == 0 ||
            ((scp_all[imin] <= scp_min) &&
             ((Lu_seq[imin] - Lu_min) > -(std::fabs(Lu_min) * _LB_maxfracchange)))) {
            scp_min = scp_all[imin];

            // Guardar u_min como vector de tamaño mrows (como en Python)
            std::vector<double> umin_temp(mrows, 0.0);
            int k = 0;
            for (int row = 0; row < mrows; ++row) {
                if (!f_covered[row]) {
                    umin_temp[row] = u_seq[k][imin];
                    ++k;
                } else {
                    umin_temp[row] = u[row];
                }
            }
            u_min = umin_temp;

            Lu_min = Lu_seq[imin];
            _stepsize = STEPSIZE;
        }

        LB = Lu_min;
        // Final step: get u_min de vuelta
        u = u_min;
        reset_s();
        UB = getTotalCost();
        // Greedy una vez más con los mejores multiplicadores
        greedy();

        // GAP entre upper y lower bound
        double GAP = (UB - LB) / std::fabs(UB);
        f_change = GAP;

        if (niters % 5 == 0) {
            // std::cout << "Current Best Solution: UB=" << UB << ", LB=" << LB
            //           << ", change=" << f_change * 100.0 << "% @ niters=" << niters << std::endl;
        }

        ++niters;
        if (niters == niters_max) {
            // std::cout << "Iteration in re-initialization reaches maximum number = " << niters << std::endl;
        }
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = t1 - t0;
    double time_used = diff.count() / 60.0; // minutos

    printSelectedSets();

    return {UB, time_used};
}
