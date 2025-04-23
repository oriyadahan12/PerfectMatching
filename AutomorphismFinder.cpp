#include "AutomorphismFinder.hpp"
#include <algorithm>

std::vector<std::vector<int>> AutomorphismFinder::findAutomorphisms(const std::vector<std::vector<bool>>& adjMatrix, vector <Matching>& pm) {
    std::vector<std::vector<int>> automorphisms;
    int n = adjMatrix.size();
    std::vector<int> perm(n);
    for (int i = 0; i < n; ++i) {
        perm[i] = i;
    }

    unsigned int currIter = 0;
    do {
        bool isAutomorphism = true;
        for (int i = 0; i < n && isAutomorphism; ++i) {
            for (int j = 0; j < n && isAutomorphism; ++j) {
                if (adjMatrix[i][j] != adjMatrix[perm[i]][perm[j]]) {
                    isAutomorphism = false;
                    break;
                }
            }
            if (!isAutomorphism) break;
        }
        if (isAutomorphism) {
            automorphisms.push_back(perm);
        }
        ++currIter;
        if (currIter % 1000000 == 0) std::cout << currIter << std::endl;
    } while (std::next_permutation(perm.begin(), perm.end()));

    
    return removeRotationPermutations(automorphisms, pm);
}

 std::vector <std::vector<int>> AutomorphismFinder::removeRotationPermutations(std::vector<std::vector<int>>& automorphisms, vector <Matching>& pm) {
    std::vector <std::vector<int>> newAutomorphisms ;
    for(unsigned int i = 0; i< automorphisms.size(); i++)
    {
        bool isRotate = isRotationPermutation(automorphisms[i],pm);
        if(!isRotate)
        {
            newAutomorphisms.push_back(automorphisms[i]);
        }
    }
return newAutomorphisms;
}

bool AutomorphismFinder::isRotationPermutation(const std::vector<int>& perm, std::vector <Matching>& pm)
{
    for(unsigned int i = 1; i < pm[0].size()*2; ++i)
    {
        bool isR = pm[0].isRotate(pm[perm[0]], i);
        if(isR){
            for(unsigned int j = 1; j < pm.size(); j++)
            {
                if(!pm[0].isRotate(pm[perm[j]], i))
                {
                    return false;
                }
            }
        }
    }
    return true;
}


