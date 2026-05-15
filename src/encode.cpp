#include "encode.hpp"

int encodeFeedback(const std::string &feedback)
{
      int code = 0;

      for (char c : feedback)
      {
            code *= 3;
            if (c == 'g')
                  code += 2;
            else if (c == 'y')
                  code += 1;
      }

      return code;
}