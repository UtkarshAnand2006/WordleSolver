#include "solver.hpp"
#include "constants.hpp"
#include "entropy.hpp"

int bestGuess(const std::vector<int> &candidates, const std::vector<uint8_t> &patterns)
{
      double maxEntropy = -1.0;
      int bestGuess = -1;

      for (int guess = 0; guess < NUM_GUESSES; ++guess)
      {
            double entropy = computeEntropy(guess, candidates, patterns);
            if (entropy > maxEntropy)
            {
                  maxEntropy = entropy;
                  bestGuess = guess;
            }
      }

      return bestGuess;
}

std::vector<int> filterCandidates(int guess, uint8_t feedback, const std::vector<int> &candidates, const std::vector<uint8_t> &patterns)
{
      std::vector<int> filteredCandidates;

      for (int candidate : candidates)
      {
            if (patterns[guess * NUM_ANSWERS + candidate] == feedback)
            {
                  filteredCandidates.push_back(candidate);
            }
      }

      return filteredCandidates;
}