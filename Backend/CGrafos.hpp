#include <iostream>
#include <vector>
class CGrafos{
    public:
    CGrafos();
    bool btColor(std::vector<std::vector<int>>&, std::vector<int>&,int,int);
    bool BtPropio(std::vector<std::vector<int>>&, std::vector<int>&,int);
    private:
    bool Coloring(std::vector<std::vector<int>>&,int, std::vector<int>&, int);


};