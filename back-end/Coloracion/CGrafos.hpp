#include <iostream>
#include <vector>
#include <functional>
#include <chrono>
#include<stack>
class CGrafos{
    public:
    CGrafos();
    bool btColor(std::vector<std::vector<int>>&, std::vector<int>&,int,int);
    bool BtPropio(std::vector<std::vector<int>>&, std::vector<int>&,int);
    double medirTiempo(const std::string&, std::function<bool(std::vector<int>&)>, std::vector<int>&);
    bool WelshPowellAproximado(std::vector<std::vector<int>>& g, std::vector<int>& colores);
    private:
    bool VerificarColor(std::vector<std::vector<int>>&,int, std::vector<int>&, int);
    bool esAdyacente(std::vector<std::vector<int>>& g, int , int);
    std::vector<int> reordenar(std::vector<std::vector<int>>& g);

};