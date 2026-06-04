#include "Transaction.hpp"
#include "BlockchainUtils.hpp"
#include <secp256k1.h>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>

static std::vector<unsigned char> fromHex(const std::string& hex) {
    if (hex.length() % 2 != 0) {
        throw std::invalid_argument("Invalid hex string length.");
    }
    std::vector<unsigned char> bytes;
    bytes.reserve(hex.length() / 2);
    for (size_t i = 0; i < hex.length(); i += 2) {
        bytes.push_back(static_cast<unsigned char>(std::stoul(hex.substr(i, 2), nullptr, 16)));
    }
    return bytes;
}

static std::string toHex(const unsigned char* data, size_t len) {
    std::stringstream ss;
    for (size_t i = 0; i < len; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
    }
    return ss.str();
}

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

std::string Transaction::calculateHash() const {
    std::string input = this->fromAddress + this->toAddress + std::to_string(this->amount);
    return BlockchainUtils::sha256(input);
}

void Transaction::signTransaction(std::string signingKey) {
    // 1. Convert hex private key to binary buffer (32 bytes)
    std::vector<unsigned char> privKeyBytes = fromHex(signingKey);
    if (privKeyBytes.size() != 32) {
        throw std::invalid_argument("Invalid private key size.");
    }

    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);

    // 2. Derive the public key from the provided private key
    secp256k1_pubkey pubkey;
    if (!secp256k1_ec_pubkey_create(ctx, &pubkey, privKeyBytes.data())) {
        secp256k1_context_destroy(ctx);
        throw std::runtime_error("Failed to generate public key from private key.");
    }

    // Serialize to uncompressed 65-byte format to match KeyGenerator.cpp
    unsigned char pubkey_serialized[65];
    size_t pubkeylen = sizeof(pubkey_serialized);
    secp256k1_ec_pubkey_serialize(ctx, pubkey_serialized, &pubkeylen, &pubkey, SECP256K1_EC_UNCOMPRESSED);

    // 3. Verify key ownership (equivalent to: signingKey.getPublic('hex') !== this.fromAddress)
    if (toHex(pubkey_serialized, pubkeylen) != this->fromAddress) {
        secp256k1_context_destroy(ctx);
        throw std::runtime_error("You cannot sign transactions for other wallets!");
    }

    // 4. Calculate transaction hash and convert the hex output to binary
    std::string hashHex = this->calculateHash();
    std::vector<unsigned char> hashBytes = fromHex(hashHex);

    // 5. Sign the hash
    secp256k1_ecdsa_signature sig;
    if (!secp256k1_ecdsa_sign(ctx, &sig, hashBytes.data(), privKeyBytes.data(), NULL, NULL)) {
        secp256k1_context_destroy(ctx);
        throw std::runtime_error("Failed to sign transaction.");
    }

    // 6. Serialize signature to standard DER format (max 72 bytes) and encode to Hex
    unsigned char der_sig[72];
    size_t der_len = sizeof(der_sig);
    secp256k1_ecdsa_signature_serialize_der(ctx, der_sig, &der_len, &sig);

    this->signature = toHex(der_sig, der_len);

    // Cleanup
    secp256k1_context_destroy(ctx);
}

bool Transaction::isValid() const {
    // 1. Mining reward transactions are valid without a signature
    if (this->fromAddress.empty()) {
        return true;
    }

    // 2. Validate signature presence
    if (this->signature.empty()) {
        throw std::runtime_error("No signature in this transaction");
    }

    // Initialize context specifically for verification
    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);

    // 3. Convert and parse the public key (fromAddress)
    std::vector<unsigned char> pubKeyBytes;
    try {
        pubKeyBytes = fromHex(this->fromAddress);
    } catch (const std::exception& e) {
        secp256k1_context_destroy(ctx);
        return false; // Malformed hex string
    }

    secp256k1_pubkey pubkey;
    if (!secp256k1_ec_pubkey_parse(ctx, &pubkey, pubKeyBytes.data(), pubKeyBytes.size())) {
        secp256k1_context_destroy(ctx);
        return false; // Failed to parse into a valid secp256k1 public key
    }

    // 4. Convert and parse the DER signature
    std::vector<unsigned char> sigBytes;
    try {
        sigBytes = fromHex(this->signature);
    } catch (const std::exception& e) {
        secp256k1_context_destroy(ctx);
        return false;
    }

    secp256k1_ecdsa_signature sig;
    if (!secp256k1_ecdsa_signature_parse_der(ctx, &sig, sigBytes.data(), sigBytes.size())) {
        secp256k1_context_destroy(ctx);
        return false; // Failed to parse DER format
    }

    // 5. Calculate transaction hash and convert to binary
    std::string hashHex = this->calculateHash();
    std::vector<unsigned char> hashBytes;
    try {
        hashBytes = fromHex(hashHex);
    } catch (const std::exception& e) {
        secp256k1_context_destroy(ctx);
        return false;
    }

    // 6. Execute ECDSA verification
    int isVerified = secp256k1_ecdsa_verify(ctx, &sig, hashBytes.data(), &pubkey);

    // Cleanup
    secp256k1_context_destroy(ctx);

    return isVerified == 1;
}