#include "Block.hpp"

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
    return sha256(input);        
}

void Block::mineBlock(int difficulty) {
    std::string target(difficulty, '0'); 
        
    while(this->hash.substr(0, difficulty) != target) {
        this->nonce++;
        this->hash = this->calculateHash();
    }
    std::cout << "Block mined: " << this->hash << "\n";
}

std::string Block::sha256(const std::string& str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(str.c_str()), str.size(), hash);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}