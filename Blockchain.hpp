#pragma once
#include "Block.hpp"
#include "Transaction.hpp"

class Blockchain {
  public:
    Blockchain();
    ~Blockchain();

    Block createGenesisBlock();
    Block getLatestBlock();
    void minePendingTransactions(std::string miningRewardAddress);
    void createTransaction(Transaction transaction);
    int getBalanceOfAddress(std::string address);
    bool isChainValid();
    void printChain();

  private:
    std::vector<Block> chain;
    int difficulty;
    std::vector<Transaction> pendingTransactions;
    int miningReward;
};