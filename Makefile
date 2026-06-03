# ==========================================
# Project Configuration
# ==========================================
EXE = main
OBJS_DIR = .objs

# Automatically find all .cpp files in the directory
SRCS = $(wildcard *.cpp)
# Map .cpp files to their respective .o files inside .objs/
OBJS = $(patsubst %.cpp, $(OBJS_DIR)/%.o, $(SRCS))
# Map .o files to their respective .d (dependency) files
DEPS = $(OBJS:.o=.d)

# ==========================================
# Compiler & Linker Configuration
# ==========================================
CXX = g++
LD = g++

# Homebrew OpenSSL paths for Apple Silicon (ARM)
INCLUDES = -I/opt/homebrew/opt/openssl/include -I/opt/homebrew/include
LIB_PATHS = -L/opt/homebrew/opt/openssl/lib -L/opt/homebrew/lib
OPENSSL_LIBS = -lcrypto -lssl -lsecp256k1

# Compiler Flags: 
# -MMD -MP: Generates header dependency files (.d)
# -g: Includes debug symbols
# $(INCLUDES): Points to OpenSSL sha.h
CXXFLAGS = -std=c++17 -g -Wall -Wextra -pedantic -Wfatal-errors -MMD -MP $(INCLUDES)

# Linker Flags:
# $(LIB_PATHS): Points to OpenSSL binaries
# $(OPENSSL_LIBS): Links the required cryptography and SSL libraries
LDFLAGS = -std=c++17 $(LIB_PATHS) $(OPENSSL_LIBS)

# ==========================================
# Build Rules
# ==========================================
# First rule is the default rule triggered by running `make`
all: $(EXE)

# Link the final executable
$(EXE): $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@

# Compile C++ source files into object files
$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure the objects directory exists
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

# ==========================================
# Utilities
# ==========================================
clean:
	rm -rf $(EXE) $(OBJS_DIR)

.PHONY: all clean

# Include the generated dependency files so Make knows when a header changes
-include $(DEPS)