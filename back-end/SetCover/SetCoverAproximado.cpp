#include "SetCoverAproximado.h"

int SetCoverAproximado::Covered(std::vector<element> &universe)
{
    int count = 0;
    for (auto &e : universe)
    {
        if (e.Marcado)
        {
            count++;
        }
    }
    return count;
}

void SetCoverAproximado::Mark(std::vector<element> &universe, int id)
{
    for (auto &e : universe)
    {
        if (e.id == id)
        {
            e.Marcado = true;
            break;
        }
    }
}

std::vector<int> SetCoverAproximado::setCover(std::vector<element> &universe, std::vector<std::vector<int>> &sets)
{
    std::vector<int> selectedIndices;
    int totalCovered = 0;
    int universeSize = universe.size();
    int currentCovered = 0;
    int maxCovered = 0;
    int bestSetIndex = -1;

    while (totalCovered < universeSize)
    {
        for (int i = 0; i < sets.size(); ++i)
        {
            int count = 0;
            for (int id : sets[i])
            {
                if (!universe[id].Marcado)
                {
                    count++;
                }
            }

            if (count > maxCovered)
            {
                maxCovered = count;
                bestSetIndex = i;
            }
        }

        if (maxCovered == 0)
        {
            break;
        }

        for (int id : sets[bestSetIndex])
        {
            Mark(universe, id);
        }
        selectedIndices.push_back(bestSetIndex);
        totalCovered = Covered(universe);
        maxCovered = 0;
        bestSetIndex = -1;
    }

    return selectedIndices;
}


std::vector<std::vector<int>> SetCoverAproximado::generateRandomSets(int numSets, int minSetSize, int maxSetSize, int universeSize)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> setSizeDist(minSetSize, maxSetSize);
    std::uniform_int_distribution<> elemDist(0, universeSize - 1);

    std::vector<std::vector<int>> sets;
    std::vector<bool> isCovered(universeSize, false);

    for (int i = 0; i < numSets; ++i)
    {
        int setSize = setSizeDist(gen);
        std::set<int> tempSet;

        for (int j = 0; j < setSize; ++j)
        {
            int elem = elemDist(gen);
            tempSet.insert(elem);
        }
        std::vector<int> set(tempSet.begin(), tempSet.end());
        sets.push_back(set);
        for (int n : tempSet)
        {
            isCovered[n] = true;
        }
    }

    std::vector<int> missingElements;
    for (int i = 0; i < universeSize; ++i)
    {
        if (!isCovered[i])
        {
            missingElements.push_back(i);
        }
    }

    if (!missingElements.empty())
    {
        for (int elem : missingElements)
        {
            std::uniform_int_distribution<> setIndexDist(0, numSets - 1);
            int randomSetIndex = setIndexDist(gen);
            sets[randomSetIndex].push_back(elem);
        }
    }
    return sets;
}

void SetCoverAproximado::printSets(const std::vector<std::vector<int>> &sets, const std::vector<int> &selectedIndices, int universeSize)
{
    std::cout << "\nConjuntos seleccionados por el algoritmo aproximado:\n";
    for (int idx : selectedIndices)
    {
        std::cout << "Conjunto " << idx << ": { ";
        for (int elem : sets[idx])
        {
            std::cout << elem << " ";
        }
        std::cout << "}  n = " << sets[idx].size() << std::endl;
    }

      if (estanTodosCubiertosAprox(sets, selectedIndices, universeSize)) {
        std::cout << "¡Todos los elementos del universo están cubiertos!\n";
    } else {
        std::cout << "Advertencia: NO todos los elementos del universo están cubiertos.\n";
    }

    std::cout << "Seleccionados: " << selectedIndices.size() << "; ";
}

bool SetCoverAproximado::estanTodosCubiertosAprox(const std::vector<std::vector<int>>& sets, const std::vector<int>& selectedIndices, int universeSize) {
    std::unordered_set<int> cubiertos;
    for (int idx : selectedIndices) {
        for (int elem : sets[idx]) {
            cubiertos.insert(elem);
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

