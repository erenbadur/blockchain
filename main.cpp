#include "Block.hpp"
#include "Blockchain.hpp"
#include "KeyGenerator.hpp"
#include "Transaction.hpp"
#include <iostream>
#include <utility>

int main() {
    KeyGenerator kg;
    std::pair<std::string, std::string> keyPair = kg.generateKeyPair();
    std::cout << "Private key: " << keyPair.first << std::endl;
    std::cout << "Public key: " << keyPair.second << std::endl;
    std::cout << "\n" << std::endl;
    
    Blockchain myCoin;
    Transaction transaction = Transaction("address1", "address2", 100);
    Transaction transaction2 = Transaction("address2", "address1", 50);
    myCoin.createTransaction(transaction);
    myCoin.createTransaction(transaction2);

    std::cout << "Starting the miner..." << std::endl;
    myCoin.minePendingTransactions("my-address");
    std::cout << "Balance is " << myCoin.getBalanceOfAddress("my-address") << std::endl;
    std::cout << "\n" << std::endl;
    std::cout << "Starting the miner again..." << std::endl;
    myCoin.minePendingTransactions("my-address");
    std::cout << "Balance is " << myCoin.getBalanceOfAddress("my-address") << std::endl;
    std::cout << "\n" << std::endl;
    myCoin.printChain();
    std::cout << "Is blockchain valid? " << (myCoin.isChainValid() ? "true" : "false") << std::endl;
    return 0;
}