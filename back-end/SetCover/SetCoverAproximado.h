#pragma once
#include <vector>
#include <set>
#include <random>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <unordered_set>

struct element
{
    bool Marcado = false;
    int id;
    element(int id) : id(id) {}
};

class SetCoverAproximado
{
public:
    static int Covered(std::vector<element> &universe);
    static void Mark(std::vector<element> &universe, int id);
    static std::vector<int> setCover(std::vector<element> &universe, std::vector<std::vector<int>> &sets);
    static std::vector<std::vector<int>> generateRandomSets(int numSets, int minSetSize, int maxSetSize, int universeSize);
    static void printSets(const std::vector<std::vector<int>> &sets,const std::vector<int> &selectedIndices, int universeSize);

private:

   static bool estanTodosCubiertosAprox(const std::vector<std::vector<int>>& sets, const std::vector<int>& selectedIndices, int universeSize);

};
