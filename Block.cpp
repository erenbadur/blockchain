#include "Block.hpp"
#include "BlockchainUtils.hpp"

Block::Block() {}

Block::Block(std::string timestamp, std::vector<Transaction> transactions, std::string previousHash) {
    this->timestamp = timestamp;
    this->transactions = transactions;
    this->previousHash = previousHash;
    this->hash = this->calculateHash();
    this->nonce = 0;
}

Block::~Block() {}

std::string Block::calculateHash() {
    std::string txString = "";
    // SYNTAX
    for (const auto& tx : this->transactions) {
        txString += tx.getFromAddress() + tx.getToAddress() + std::to_string(tx.getAmount());
    }
    std::string input = this->previousHash + this->timestamp + txString + std::to_string(this->nonce);
    return BlockchainUtils::sha256(input);       
}

void Block::mineBlock(int difficulty) {
    std::string target(difficulty, '0'); 
        
    while(this->hash.substr(0, difficulty) != target) {
        this->nonce++;
        this->hash = this->calculateHash();
    }
    std::cout << "BLOCK MINED: " << this->hash << "\n";
}

bool Block::hasValidTransactions() {
    for (const auto& tx : this->transactions) {
        if (!tx.isValid()) {
            return false;
        }
    }
    return true;
}