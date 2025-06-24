#include <iostream>
#include <vector>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <map>

#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;

class Grafos
{
public:
    int n;

    Grafos(int size);
    vector<vector<int>> Conexiones();
    void TSPPropioShow(vector<vector<int>> matriz);
    void TSPComunidadShow(vector<vector<int>> matriz);
    vector<int> TSPPropio(vector<vector<int>> matriz);
    vector<int> TSPComunidad(vector<vector<int>> matriz);
};