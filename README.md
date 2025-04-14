# Deribit Trading System

A high-performance C++ trading system for the Deribit cryptocurrency derivatives exchange.

## Key Features

- **Order Management System**
  - Place, cancel, and modify orders
  - Position tracking
  - Risk management checks

- **Market Data Processing**
  - WebSocket-based real-time data streaming
  - Efficient data distribution
  - Custom subscription management

- **API Integration**
  - REST API client for Deribit
  - OAuth2 authentication
  - Automatic token refresh

## Technical Highlights

- Modern C++17 implementation
- CMake build system
- Catch2 unit testing framework
- Performance-optimized design
- Thread-safe components

## Getting Started

### Prerequisites
- C++17 compiler (GCC 9+, Clang 10+, MSVC 2019+)
- CMake 3.10+
- Boost 1.70+
- OpenSSL

### Build Instructions
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### Configuration
Edit `include/config.hpp` with your:
- Deribit API credentials
- WebSocket server port
- Trading parameters

### Running
```bash
./DeribitTradingSystem
```

## Project Structure
```
├── include/            # Header files
├── src/                # Source files
│   ├── deribit_api/    # Deribit API client
│   ├── order_manager/  # Order management
│   └── market_data/    # Market data processing
├── tests/              # Unit tests
└── docs/               # Documentation
```

## License
MIT License - See [LICENSE](LICENSE) for details.
