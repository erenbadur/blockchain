#pragma once
#include <string>
#include <openssl/sha.h>

namespace BlockchainUtils {
    std::string sha256(const std::string& str);
}