#pragma once
#include <string>

class Transaction {
  public:
    Transaction();
    Transaction(std::string fromAddress, std::string toAddress, int amount);
    ~Transaction();
    std::string getFromAddress() const;
    std::string getToAddress() const;
    int getAmount() const;
    std::string calculateHash();
    void signTransaction(std::string signingKey);
    bool isValid();

  private:
    std::string fromAddress;
    std::string toAddress;
    int amount;
};