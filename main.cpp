#include "Block.hpp"
#include "Blockchain.hpp"
#include "KeyGenerator.hpp"
#include "Transaction.hpp"
#include <iostream>
#include <utility>

int main() {
    KeyGenerator kg;
    std::pair<std::string, std::string> keyPair = kg.generateKeyPair();
    std::string myPrivateKey = keyPair.first;
    std::string myWalletAddress = keyPair.second;
    std::cout << "Private key: " << myPrivateKey << "\n";
    std::cout << "Public key: " << myWalletAddress << "\n\n";
    
    Blockchain myCoin;
    Transaction tx1 = Transaction(myWalletAddress, "recipient_public_key", 10);
    tx1.signTransaction(myPrivateKey);
    myCoin.addTransaction(tx1);

    std::cout << "Starting the miner..." << std::endl;
    myCoin.minePendingTransactions(myWalletAddress);
    std::cout << "\nBalance is " << myCoin.getBalanceOfAddress(myWalletAddress) << std::endl << "\n";
    myCoin.printChain();
    std::cout << "Is the blockchain valid? " << (myCoin.isChainValid() ? "Yes" : "No") << std::endl << "\n";
    myCoin.getAllTransactionsForWallet(myWalletAddress);
    return 0;
}