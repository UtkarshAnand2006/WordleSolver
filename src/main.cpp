#include "loader.hpp"
#include "solver.hpp"
#include "constants.hpp"
#include "encode.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
      auto guesses = loadWords("data/guesses.txt");
      auto answers = loadWords("data/answers.txt");
      auto patterns = loadPatterns("data/patterns.bin");

      std::vector<int> candidates;

      for (int i = 0; i < NUM_ANSWERS; i++)
      {
            candidates.push_back(i);
      }

      while (true)
      {
            if (candidates.empty())
            {
                  std::cout << "No candidates left. Something went wrong." << std::endl;
                  break;
            }

            if (candidates.size() == 1)
            {
                  std::cout << "The answer is: " << answers[candidates[0]] << std::endl;
                  std::cout << "Congratulations! You've found the answer!" << std::endl;
                  break;
            }

            int guess = bestGuess(candidates, patterns);

            std::cout << "Best guess: " << guesses[guess] << std::endl;

            std::string myGuess;
            std::cout << "Enter your guess: ";
            std::cin >> myGuess;

            std::string feedback;
            std::cout << "Enter feedback (g/y/x): ";
            std::cin >> feedback;

            int observed = encodeFeedback(feedback);

            if (observed == 242)
            {
                  std::cout << "Congratulations! You've found the answer!" << std::endl;
                  break;
            }

            candidates = filterCandidates((int)(std::find(guesses.begin(), guesses.end(), myGuess) - guesses.begin()), observed, candidates, patterns);

            std::cout << "Remaining candidates: " << candidates.size() << std::endl;
            for (int candidate : candidates)
            {
                  std::cout << answers[candidate] << std::endl;
            }
      }

      return 0;
}