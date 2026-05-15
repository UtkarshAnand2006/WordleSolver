#pragma once
#include <vector>
#include <string>
#include <cstdint>

std::vector<std::string> loadWords(const std::string &path);
std::vector<uint8_t> loadPatterns(const std::string &path);