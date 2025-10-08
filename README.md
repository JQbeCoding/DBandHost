# DBandHost

A custom C-based HTTP server with file-based database for managing employee data at MayDayz Smokn BBQ.

## Overview

This project implements a lightweight web application using pure C and POSIX APIs, without relying on external web frameworks. It consists of a TCP-based HTTP server that serves HTML pages and will interact with a custom file-based database system.

## Project Structure

```
DBandHost/
├── http_server.c    # HTTP server implementation
├── DB.c            # Database logic and file I/O operations
├── index.html      # Frontend employee interface
└── README.md       # Project documentation
```

## Components

### HTTP Server (`http_server.c`)

A from-scratch HTTP server built using POSIX socket APIs that:

- Creates a TCP socket server listening on port 8080
- Handles incoming HTTP GET requests
- Serves the `index.html` file to clients
- Uses a connection queue with configurable backlog (currently set to 10)
- Implements proper socket binding and listening

**Key Features:**
- IPv4 support via `AF_INET`
- Stream-based communication with `SOCK_STREAM` (TCP)
- Dynamic file reading with proper buffer management
- HTTP/1.1 response headers with correct Content-Type and Content-Length

### Database (`DB.c`)

A custom database implementation that will:

- Define data structures for employee information
- Handle manual query construction and execution
- Provide file-based read/write operations
- Serve as the data layer for the employee management system

*Note: Currently in development*

### Frontend (`index.html`)

A simple HTML5 interface that will display:
- Employee login screen
- Employee dashboard (in development)

## Getting Started

### Prerequisites

- GCC or compatible C compiler
- Unix-like operating system (Linux, macOS)
- Basic understanding of C and networking

### Building and Running

1. Compile the HTTP server:
```bash
gcc http_server.c -o server
```

2. Run the server:
```bash
./server
```

3. Access the application:
```
http://localhost:8080
```

### Configuration

- **Port**: Modify the `PORT` macro in `http_server.c` (default: 8080)
- **Backlog**: Adjust `BACKLOG` macro for production (recommended: 1024)
- **Buffer Size**: Change `BUFFER` macro if needed (default: 1024 bytes)

## Technical Details

### Socket Implementation

The server uses low-level POSIX socket APIs:
- `socket()` - Creates the server socket
- `bind()` - Binds socket to address and port
- `listen()` - Listens for incoming connections
- `accept()` - Accepts client connections
- `recv()` - Receives data from clients
- `send()` - Sends responses to clients

### HTTP Response Format

```
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: [file_size]

[HTML content]
```

## Roadmap

- [ ] Complete database implementation with struct definitions
- [ ] Add query handling functionality
- [ ] Implement employee authentication
- [ ] Build employee dashboard interface
- [ ] Add database CRUD operations
- [ ] Implement request routing for multiple pages
- [ ] Add error handling and logging

## Development Notes

- The server currently only handles GET requests for `index.html`
- Database queries and employee management features are planned
- Built without external libraries for educational purposes and fine-grained control

## Author

**Ja'Quis Franklin**  
Developer at MayDayz Smokn BBQ  
Date: 10/07/2025
