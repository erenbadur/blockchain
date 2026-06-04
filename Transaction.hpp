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
    std::string calculateHash() const;
    void signTransaction(std::string signingKey);
    bool isValid() const;

  private:
    std::string fromAddress;
    std::string toAddress;
    int amount;
    std::string signature;
};