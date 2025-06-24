#include "CGrafos.hpp"
#include <iomanip>
CGrafos::CGrafos() {}

bool CGrafos::VerificarColor(std::vector<std::vector<int>> &g, int nodo, std::vector<int> &colores, int colorIntento)
{
    for (int vecino : g[nodo])
    {
        if (colores[vecino] == colorIntento)
        {
            return false;
        }
    }
    return true;
}

bool CGrafos::btColor(std::vector<std::vector<int>> &g, std::vector<int> &colores, int nodo, int cantColores)
{

    if (nodo == g.size())
    {
        return true;
    }

    for (int color = 0; color < cantColores; color++)
    {

        if (VerificarColor(g, nodo, colores, color))
        {

            colores[nodo] = color;

            if (btColor(g, colores, nodo + 1, cantColores))
            {
                return true;
            }

            colores[nodo] = -1;
        }
    }

    return false;
}

bool CGrafos::BtPropio(std::vector<std::vector<int>> &g, std::vector<int> &colores, int cantColores)
{
    int cantNodos = g.size();
    if (cantNodos == 0) {
        return true; 
    }

    colores.assign(cantNodos, -1); 

    std::stack<int> s;
    s.push(0); 

    while (!s.empty())
    {
        int nodoActual = s.top(); 

        
        if (nodoActual == cantNodos) {
            return true;
        }

        int startColor = (colores[nodoActual] == -1) ? 0 : colores[nodoActual] + 1;

        bool colorEncontrado = false;
        for (int color = startColor; color < cantColores; ++color)
        {
            if (VerificarColor(g, nodoActual, colores, color))
            {
                colores[nodoActual] = color; 
                s.push(nodoActual + 1);    
                colorEncontrado = true;
                break; 
            }
        }

        if (!colorEncontrado)
        {
            
            colores[nodoActual] = -1;
        
            s.pop();
            
            if (s.empty()) {
                return false; 
            }
            
        }
        
    }

    return false;
}

double CGrafos::medirTiempo(const std::string &nombreAlgoritmo, std::function<bool(std::vector<int> &)> algoritmoAFuncion, std::vector<int> &colores)
{
    auto inicio = std::chrono::high_resolution_clock::now();

    volatile bool exito = algoritmoAFuncion(colores); 

    auto fin = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::nano> duracion = fin - inicio;

    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Tiempo de ejecucion: " << duracion.count() << " nanosegundos\n";

    return duracion.count();
}

std::vector<int> CGrafos::reordenar(std::vector<std::vector<int>> &g)
{
    int n = g.size();
    std::vector<int> orden(n);
    for (int i = 0; i < n; ++i)
    {
        orden[i] = i;
    }

    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (g[orden[j]].size() < g[orden[j + 1]].size())
            {
                std::swap(orden[j], orden[j + 1]);
            }
        }
    }
    return orden;
}

bool CGrafos::esAdyacente(std::vector<std::vector<int>> &g, int nodo1, int nodo2)
{
    for (int vecino : g[nodo1])
    {
        if (vecino == nodo2)
        {
            return true;
        }
    }
    return false;
}

bool CGrafos::WelshPowellAproximado(std::vector<std::vector<int>> &g, std::vector<int> &colores)
{
    std::vector<int> orden = reordenar(g);
    int n = g.size();
    colores.assign(n, -1);
    int cantColores = 0;
    int nodo = 0;
    int candidato = 0;
    bool adyacenteAColor = false;

    for (int i = 0; i < n; i++)
    {
        nodo = orden[i];
        if (colores[nodo] == -1)
        {
            colores[nodo] = cantColores;
            for (int j = i + 1; j < n; j++)
            {
                candidato = orden[j];
                if (colores[candidato] == -1)
                {
                    adyacenteAColor = false;
                    for (int k = 0; k < n; ++k)
                    {
                        if (colores[k] == cantColores && esAdyacente(g, candidato, k))
                        {
                            adyacenteAColor = true;
                            break;
                        }
                    }
                    if (!adyacenteAColor)
                    {
                        colores[candidato] = cantColores;
                    }
                }
            }
        }
        cantColores++;
    }

    return true;
}
