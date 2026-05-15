#pragma once
#include <vector>
#include <cstdint>

double computeEntropy(int guess, const std::vector<int> &candidates, const std::vector<uint8_t> &patterns);