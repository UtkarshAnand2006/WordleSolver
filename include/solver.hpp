#pragma once
#include <vector>
#include <cstdint>

int bestGuess(const std::vector<int> &candidates, const std::vector<uint8_t> &patterns);

std::vector<int> filterCandidates(int guess, uint8_t observed, const std::vector<int> &candidates, const std::vector<uint8_t> &patterns);