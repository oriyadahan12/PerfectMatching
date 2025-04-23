#include <vector>
#include "PerfectMatching.hpp"

class AutomorphismFinder {
public:
    static std::vector<std::vector<int>> findAutomorphisms(const std::vector<std::vector<bool>>& adjMatrix, std::vector <Matching>& pm);

private:
    static std::vector <std::vector<int>> removeRotationPermutations(std::vector<std::vector<int>>& automorphisms, std::vector <Matching>& pm);

    static bool isRotationPermutation(const std::vector<int>& perm, std::vector <Matching>& pm);

};