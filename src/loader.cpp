#include "loader.hpp"
#include <fstream>
#include <iostream>

std::vector<std::string> loadWords(const std::string &path)
{
      std::ifstream file(path);

      std::vector<std::string> words;
      std::string word;

      while (file >> word)
      {
            words.push_back(word);
      }

      return words;
}

std::vector<uint8_t> loadPatterns(const std::string &path)
{
      std::ifstream file(path, std::ios::binary);

      file.seekg(0, std::ios::end);
      size_t size = file.tellg();
      file.seekg(0);

      std::vector<uint8_t> patterns(size);
      file.read(reinterpret_cast<char *>(patterns.data()), size);
      file.close();

      return patterns;
}