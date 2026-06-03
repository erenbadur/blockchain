#pragma once
#include <utility>

class KeyGenerator {
  public:
    KeyGenerator();
    ~KeyGenerator();
    std::pair<std::string, std::string> generateKeyPair();    
};