#include "KeyGenerator.hpp"
#include <secp256k1.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include <stdexcept>
#include <iomanip>
#include <sstream>

KeyGenerator::KeyGenerator() {}
KeyGenerator::~KeyGenerator() {}

static std::string toHex(const unsigned char* data, size_t len) {
    std::stringstream ss;
    for (size_t i = 0; i < len; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
    }
    return ss.str();
}

std::pair<std::string, std::string> KeyGenerator::generateKeyPair() {
    // 1. Initialize Context
    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
    if (!ctx) {
        throw std::runtime_error("Failed to create secp256k1 context.");
    }

    // 2. Generate a valid 32-byte private key
    unsigned char seckey[32];
    while (true) {
        if (RAND_bytes(seckey, sizeof(seckey)) != 1) {
            secp256k1_context_destroy(ctx);
            throw std::runtime_error("Failed to generate secure random bytes.");
        }
        if (secp256k1_ec_seckey_verify(ctx, seckey)) {
            break;
        }
    }

    // 3. Generate the public key
    secp256k1_pubkey pubkey;
    if (!secp256k1_ec_pubkey_create(ctx, &pubkey, seckey)) {
        secp256k1_context_destroy(ctx);
        throw std::runtime_error("Failed to create public key.");
    }

    // 4. Serialize the public key in uncompressed format (65 bytes)
    unsigned char pubkey_serialized[65];
    size_t pubkeylen = sizeof(pubkey_serialized);
    secp256k1_ec_pubkey_serialize(ctx, pubkey_serialized, &pubkeylen, &pubkey, SECP256K1_EC_UNCOMPRESSED);

    // 5. Convert to Hex Strings
    std::string privateKeyHex = toHex(seckey, sizeof(seckey));
    std::string publicKeyHex = toHex(pubkey_serialized, pubkeylen);

    // 6. Cleanup memory and securely sanitize the private key buffer
    secp256k1_context_destroy(ctx);
    OPENSSL_cleanse(seckey, sizeof(seckey));

    return {privateKeyHex, publicKeyHex};
}