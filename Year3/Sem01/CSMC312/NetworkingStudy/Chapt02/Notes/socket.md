What is a socket?
==
- Everything in Unix is a file!

## 2.1 Two types of Internet Sockets
- Two types:
    - **SOCK_STREAM**
    - **SOCK_DGRAM**
        - also called "connectionless sockets"

### 2.1.1 Stream Sockets
- reliable two-way connected communicaton streams.
- **ALWAYS** arrive in order, and if such, will be error-free.
- `telnet` and `ssh` applications use Stream Sockets, as one would argue that such sessions would require what you type to be in order.
- Also known as **TCP** (Transmission Control Protocol), it makes sure your data arrives sequentially and error-free.

### 2.1.2 Datagram Sockets
- If one sends a datagram, it may arrive, it may not, it may arrive out of order, but the data would be error-free.
- Also known as **UDP** (User Datagram Protocol)
- It is connecitonless, because one does not need to maintain an open connection as you do with TCP
- one would just; 
    - a.) build a packet, 
    - b.) slap an IP header on it with destination information, 
    - c.) send it out

    No connection needed
- Sample applications of these are:
    - user `tftp` (trivial file transfer protocol)
    - `dhcpcd` (a DHCP client)
    - multiplayer games
    - streaming audio
    - video conferencing
    - etc

    Where a dropped packet or two doesn't really matter, as these packets are usually sent hundreds of times a second.

## 2.2 Low level Nonsense and Network Theory
- skipped
