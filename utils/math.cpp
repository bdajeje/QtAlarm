#include "math.hpp"

#include <QTime>

namespace utils {

int random(int included_from, int excluded_to)
{
  // Init random seed only once
  static bool seeded = false;
  if(!seeded)
  {
    qsrand((uint)QTime::currentTime().msec());
    seeded = true;
  }

  return qrand() % (excluded_to - included_from) + included_from;
}

} // utils
