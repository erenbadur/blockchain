#include "Transaction.hpp"
#include <openssl/sha.h>

Transaction::Transaction() {}

Transaction::Transaction(std::string fromAddress, std::string toAddress, int amount) {
    this->fromAddress = fromAddress;
    this->toAddress = toAddress;
    this->amount = amount;
}

Transaction::~Transaction() {}

std::string Transaction::getFromAddress() const {
    return this->fromAddress;
}

std::string Transaction::getToAddress() const {
    return this->toAddress;
}

int Transaction::getAmount() const {
    return this->amount;
}

std::string Transaction::calculateHash() {
    
}

void Transaction::signTransaction(std::string signingKey) {

}

bool Transaction::isValid() {
    
}