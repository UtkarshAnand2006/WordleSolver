#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

using namespace std;

constexpr int G = 14855;
constexpr int A = 2315;

vector<string> loadWords(const string &filename)
{
      vector<string> words;
      ifstream file(filename);
      string word;
      while (file >> word)
      {
            words.push_back(word);
      }
      return words;
}

uint8_t getPattern(const string &guess, const string &answer)
{
      int result[5] = {0};
      int freq[26] = {0};

      for (char c : answer)
      {
            freq[c - 'a']++;
      }

      for (int i = 0; i < 5; i++)
      {
            if (guess[i] == answer[i])
            {
                  result[i] = 2;
                  freq[guess[i] - 'a']--;
            }
      }

      for (int i = 0; i < 5; i++)
      {
            if (result[i] == 0 && freq[guess[i] - 'a'] > 0)
            {
                  result[i] = 1;
                  freq[guess[i] - 'a']--;
            }
      }

      int pattern = 0;
      for (int i = 0; i < 5; i++)
      {
            pattern = pattern * 3 + result[i];
      }

      return static_cast<uint8_t>(pattern);
}

int main()
{
      vector<string> guesses = loadWords("data/guesses.txt");
      vector<string> answers = loadWords("data/answers.txt");

      vector<uint8_t> patterns(G * A);

      for (int g = 0; g < G; g++)
      {
            if (g % 100 == 0)
            {
                  cout << "Processing guess " << g << " / " << G << endl;
            }

            for (int a = 0; a < A; a++)
            {
                  patterns[g * A + a] = getPattern(guesses[g], answers[a]);
            }
      }

      ofstream out("data/patterns.bin", ios::binary);
      out.write(reinterpret_cast<char *>(patterns.data()), sizeof(uint8_t) * patterns.size());
      out.close();

      cout << "Precomputation completed and saved to data/patterns.bin" << endl;

      return 0;
}