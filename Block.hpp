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
    int nonce;
    std::string hash;

    Block();
    Block(std::string timestamp, std::vector<Transaction> transactions, std::string previousHash = "");
    ~Block();

    std::string calculateHash();
    void mineBlock(int difficulty);
    bool hasValidTransactions();
};