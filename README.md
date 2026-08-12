# coolforce

A high-performance, dictionary-based hash cracker written in C. Currently supports MD5, with extensible architecture for additional hashing algorithms.

## Description

**coolforce** is a command-line tool designed to crack password hashes using a wordlist-based brute-force attack (dictionary attack). It reads a target hash and a wordlist file, computes the hash of each word, and compares it against the target. When a match is found, the original password is revealed.

The program is built with a modular architecture, making it easy to add support for new hash algorithms beyond MD5. The core logic uses efficient binary comparisons (`memcmp`) to maximize performance, avoiding the overhead of hex string conversions during the main cracking loop.

## Features

- **MD5 hash cracking** — currently supported algorithm
- **Efficient binary comparison** — no hex conversion overhead in the main loop
- **Extensible architecture** — easily add new algorithms (SHA-1, SHA-256, NTLM, etc.)
- **Standard library only** — no external dependencies
- **Clean CLI interface** — simple argument parsing with `getopt`

## Future Enhancements

- **Multi-threading** — split the wordlist across CPU cores for faster cracking
- **Additional algorithms** — SHA-1, SHA-256, SHA-512, NTLM, bcrypt, and more
- **Rules engine** — apply transformations to words (leet speak, append digits, etc.)
- **Progress bar** — visual feedback during long runs
- **Resume support** — continue from where the cracker left off
- **GPU acceleration** — via OpenCL/CUDA (long-term goal)

## Build

### Requirements

- GCC (or another C compiler)
- Make

### Build

```bash
make
