#include "grafos.h"

Grafos::Grafos(int size)
{
    this->n = size;
}

vector<vector<int>> Grafos::Conexiones()
{
    vector<vector<int>> matriz_ady(n, vector<int>(n, -1));

    // Generamos un grafo completo conectando todos los nodos entre si
    srand(time(0));
    int peso;

    for (int x = 0; x < n; ++x)
    {
        for (int y = x + 1; y < n; y++)
        {
            peso = rand() % 10000 + 1;
            // Hacemos que sea simetrico ya que no es dirigido el grafo
            matriz_ady[x][y] = peso;
            matriz_ady[y][x] = peso;
        }
    }

    return matriz_ady;
}

void Grafos::TSPPropioShow(vector<vector<int>> matriz)
{
    int min = INT_MAX, j = 0, sum = 0;
    int pos;
    vector<int> visitados;
    visitados.push_back(0); // Comienza desde el nodo 0 por implementación

    while (visitados.size() < n)
    {
        for (int x = 0; x < n; x++)
        {
            if (matriz[j][x] != -1 && find(visitados.begin(), visitados.end(), x) == visitados.end())
            {
                if (matriz[j][x] < min)
                {
                    min = matriz[j][x];
                    pos = x;
                }
            }
        }
        visitados.push_back(pos);
        sum += min;
        j = pos;
        min = INT_MAX;
    }
    sum += matriz[pos][0];
    visitados.push_back(0);

    cout << "Costo: " << sum << " Ruta del TSP Propio: ";

    for (int i = 0; i < visitados.size(); ++i)
    {
        cout << visitados[i] << " , ";
    }
}

void Grafos::TSPComunidadShow(vector<vector<int>> tsp)
{

    vector<bool> visited(n, false);
    vector<int> route;
    int totalCost = 0;
    int currentCity = 0;

    visited[currentCity] = true;
    route.push_back(currentCity);

    for (int count = 1; count < n; ++count)
    {
        int nextCity = -1;
        int minDist = INT_MAX;

        for (int j = 0; j < n; ++j)
        {
            if (!visited[j] && tsp[currentCity][j] < minDist)
            {
                minDist = tsp[currentCity][j];
                nextCity = j;
            }
        }

        if (nextCity != -1)
        {
            visited[nextCity] = true;
            totalCost += minDist;
            currentCity = nextCity;
            route.push_back(currentCity);
        }
    }

    // Volver al punto de partida
    totalCost += tsp[currentCity][0];
    route.push_back(0);

    // Imprimir resultado
    cout << "Minimum Cost is: " << totalCost << " Route: ";
    for (int i = 0; i < route.size(); ++i)
    {
        cout << route[i];
        if (i < route.size() - 1)
            cout << " -> ";
    }
    cout << endl;
}

vector<int> Grafos::TSPPropio(vector<vector<int>> matriz)
{
    int min = 100, j = 0, sum = 0;
    int pos;
    vector<int> visitados;
    visitados.push_back(0); // Comienza desde el nodo 0 por implementación

    while (visitados.size() < n)
    {
        for (int x = 0; x < n; x++)
        {
            if (matriz[j][x] != -1 && find(visitados.begin(), visitados.end(), x) == visitados.end())
            {
                if (matriz[j][x] < min)
                {
                    min = matriz[j][x];
                    pos = x;
                }
            }
        }
        visitados.push_back(pos);
        sum += min;
        j = pos;
        min = 100;
    }
    sum += matriz[pos][0];
    visitados.push_back(0);

    return visitados;
}

vector<int> Grafos::TSPComunidad(vector<vector<int>> tsp)
{
    vector<bool> visited(n, false);
    vector<int> route;
    int totalCost = 0;
    int currentCity = 0;

    visited[currentCity] = true;
    route.push_back(currentCity);

    for (int count = 1; count < n; ++count)
    {
        int nextCity = -1;
        int minDist = INT_MAX;

        for (int j = 0; j < n; ++j)
        {
            if (!visited[j] && tsp[currentCity][j] < minDist)
            {
                minDist = tsp[currentCity][j];
                nextCity = j;
            }
        }

        if (nextCity != -1)
        {
            visited[nextCity] = true;
            totalCost += minDist;
            currentCity = nextCity;
            route.push_back(currentCity);
        }
    }
    totalCost += tsp[currentCity][0];
    route.push_back(0);
    return route;
}
