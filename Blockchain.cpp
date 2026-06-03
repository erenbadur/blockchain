#include "Blockchain.hpp"
#include <iostream>
#include <chrono>

Blockchain::Blockchain() {
    chain.push_back(createGenesisBlock());
    this->difficulty = 5;
    this->miningReward = 100;
}

Blockchain::~Blockchain() {}

Block Blockchain::createGenesisBlock() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::vector<Transaction> genesisTransactions;
    return Block(std::to_string(ms), genesisTransactions, "0");
}

Block Blockchain::getLatestBlock() {
    return chain.back();
}

void Blockchain::minePendingTransactions(std::string miningRewardAddress) {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    Block block(
        std::to_string(ms), 
        this->pendingTransactions, 
        this->getLatestBlock().hash
    );
    block.mineBlock(this->difficulty);
    std::cout << "Block successfully mined!" << std::endl;
    this->chain.push_back(block);
    this->pendingTransactions.clear();
    this->pendingTransactions.push_back(Transaction("", miningRewardAddress, this->miningReward));
}

void Blockchain::createTransaction(Transaction transaction) {
    this->pendingTransactions.push_back(transaction);
}

int Blockchain::getBalanceOfAddress(std::string address) {
    int balance = 0;
    // SYNTAX
    for (const auto& block : this->chain) {
        for (const auto& trans : block.transactions) {
            if (trans.getFromAddress() == address) {
                balance -= trans.getAmount();
            }
            if (trans.getToAddress() == address) {
                balance += trans.getAmount();
            }
        }
    }
    return balance;
}

bool Blockchain::isChainValid() {
    for (size_t i = 1; i < this->chain.size(); i++) {
        Block currentBlock = this->chain[i];
        Block previousBlock = this->chain[i - 1];

        if (currentBlock.hash != currentBlock.calculateHash()) {
            return false;
        }
        if (currentBlock.previousHash != previousBlock.hash) {
            return false;
        }
    }
    return true;
}

void Blockchain::printChain() {
    for (size_t i = 0; i < chain.size(); i++) {
        std::cout << "[\n";
        std::cout << "  Timestamp: " << chain[i].timestamp << "\n";
        std::cout << "  PreviousHash: " << chain[i].previousHash << "\n";
        std::cout << "  Hash: " << chain[i].hash << "\n";
        std::cout << "  Nonce: " << chain[i].nonce << "\n";
        std::cout << "]\n";
    }
}