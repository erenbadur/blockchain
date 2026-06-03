#pragma once
#include "Transaction.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>

class Block {
  public:
    std::string timestamp;
    std::vector<Transaction> transactions;
    std::string previousHash;
    std::string hash;
    int nonce;

    Block();
    Block(std::string timestamp, std::vector<Transaction> transactions, std::string previousHash = "");
    ~Block();

    std::string calculateHash();

    void mineBlock(int difficulty);

  private:
    std::string sha256(const std::string& str);
};