#ifndef ZEN
#define ZEN

#include <M5Core2.h>

class Zen {
public:
  Zen();
  ~Zen();
  void M5begin();
  char execute(char* name);
};

#endif