#ifndef DELIMITER_HPP
#define DELIMITER_HPP

#include <iostream>

namespace kovtun
{
  struct DelimiterI
  {
    char expected;
  };

  struct KeyI
  {
    int & number;
  };

  struct DoubleLitI
  {
    double & dl;
  };

  struct CharLitI
  {
    char & cl;
  };

  std::istream & operator>>(std::istream & in, DelimiterI && delimiter);
  std::istream & operator>>(std::istream & in, KeyI && keyStub);
  std::istream & operator>>(std::istream & in, DoubleLitI && doubleLit);
  std::istream & operator>>(std::istream & in, CharLitI && charLit);
}

#endif
