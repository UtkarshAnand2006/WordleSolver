#include "entropy.hpp"
#include "constants.hpp"
#include <cmath>

double computeEntropy(int guess, const std::vector<int> &candidates, const std::vector<uint8_t> &patterns)
{
      std::vector<int> patternCounts(243, 0);

      for (int candidate : candidates)
      {
            uint8_t pattern = patterns[guess * NUM_ANSWERS + candidate];
            patternCounts[pattern]++;
      }

      double entropy = 0.0;
      int totalCandidates = candidates.size();

      for (int count : patternCounts)
      {
            if (count > 0)
            {
                  double probability = static_cast<double>(count) / totalCandidates;
                  entropy -= probability * std::log2(probability);
            }
      }

      return entropy;
}