# Usage Instructions

## Prerequisites
- Deribit Test account with API keys
- C++17 compatible compiler
- CMake 3.10+

## Building the System
```bash
mkdir build && cd build
cmake ..
make
```

## Configuration
1. Set your API credentials in `config.hpp`
2. Configure WebSocket ports if needed

## Running
```bash
./DeribitTradingSystem
```

## API Endpoints
- `/orders` - Order management
- `/marketdata` - Market data streaming
- `/positions` - Position tracking
