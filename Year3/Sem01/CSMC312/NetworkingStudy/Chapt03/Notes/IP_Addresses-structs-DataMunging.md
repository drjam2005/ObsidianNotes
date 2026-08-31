# IP Addresses, `struct`s, and Data Munging

## 3.1 IP Addresses, versions 4 and 6
- IPv4 (Internet Protocol Versoin 4).
    - made up of four (4) bytes, (A.K.A. four "octets")
    - continously written in "dots and numbers" form,
        - example:
            - 192.0.2.11
            - 0.0.0.0
            - 127.0.0.1

- IPv6 (Internet Protocol Version 6).
    - made up of 128 bits (instead of the 32 bits from before)
    - continously written in "colon and numbers" form,
        - exmaple:
            - 2001:0db8:c9d2:aee5:73e3:934a:a5ae:9551
            - 2001:0db8:c9d2:0012:0000:0000:0000:0051
                - 2001:db8:c9d2:12::0051
            - 2001:0db8:c9d2:0000:0000:0000:0000:0000
                - 2001:db8:c9d2::
            - 0000:0000:0000:0000:0000:0000:0000:0001
                - ::1

    - ::1 -> *loopback address*, meaning "this machine i'm working on right now". for IPv4, it is "127.0.0.1"

### 3.1.1 Subnets
- skipped

### 3.1.2 Port Numbers
- both TCP and UDP use another address, it is the `port number`, 16-bit number.
- Think of an `IP Address` as a hotel, and the `port number` as the room number.

## 3.2 Byte Order
- Two types
    - Big-Endian:
        - for a number, let's say, 42069 (A455 in hex), it will be stored: `A4` and `55` in order, bigger first, then small
    - Little-Endian:
        - for a number, let's say, 42069 (A455 in hex), it will be stored: `55` and `A4` in order, smaller first, then big

- Functions:

    | Functon  | Description 
    | -------- | -----------
    | htons()  | `h`ost `to` `n`etwork `s`hort
    | htonl()  | `h`ost `to` `n`etwork `l`ong
    | ntohs()  | `n`etwork `to` `h`ost `s`hort
    | ntohl()  | `n`etwork `to` `h`ost `l`ong

- Main idea:
    - when sending out data, convert from `h`ost ordering `to` `n`etwork ordering
    - when receiving in data, convert from `n`etwork ordering `to` `h`ost ordering


## 3.3 `struct`s
- Socket descriptor:
    ```c
    // C
    int
    ```
### 3.3.1 IPv4 layout
- `struct addrinfo`
    - used to prep the socket address structures for subsequent use.
    - one of the first things one will call when making a connection
    ```c
    struct addrinfo {
        int                 ai_flags;      // AI_PASSIVE, AI_CANONNAME, etc.
        int                 ai_family;     // AF_INET, AF_INET6, AF_UNSPEC
        int                 ai_socktype;   // SOCK_STREAM, SOCK_DGRAM
        int                 ai_protocol;   // use 0 for "any"
        size_t              ai_addrlen;    // size of ai_addr in bytes
        struct sockaddr     *ai_addr;      // struct sockaddr_in or _int6
        char                *ai_canonname; // fill canonical hostname
        struct addrinfo     *ai_next;      // linked list, next node
    };
    ```
    - one would load up this, and then call `getaddrinfo()`, returns a pointer to a linked list of these structures
        - one can force IPv4 or IPv5 in the `ai_family` field, or `AF_UNSPEC` to use whatevr.
        - `ai_next` points at the next element, book uses first result that worked
        - `ai_addr` is a pointer to a `struct sockaddr` (take note of later)
    - one would not really need to load this up manually, one would just call `getaddrinfo()` to fill out one's `struct addrinfo`

- `struct sockaddr` 
     ```c
     struct sockaddr {
        unsigned short  sa_family;   // address family, AF_xx  
        char            sa_data[14]; // 14 bytes of protocol address
     };
     ```
    - `sa_family` can be a lot of things, but it'll be `AF_INET` (IPv4) or `AF_INET6` (IPv6) for the sake of this study
    - `sa_data` contains a desntination address and port number for the socket.

    for one to deal with `struct sockaddr`, one would create a similar structure `struct sockaddr_in` ("in" for "internet") to be used with IPv4.

    one would use the pointer of this `struct sockaddr_in` and cast it to a `strurct sockaddr` when it is to be used, and vice versa when to parse.

- `struct sockaddr_in`
    ```c
    struct sockaddr_in {
        short int           sin_family;  // Address family, AF_INET
        unsigned short int  sin_port;    // Port number
        struct in_addr      sin_addr;    // Internet address
        unsigned char       sin_zero[8]; // Same size as struct sockaddr
    };
    ```
    - this makes it easy to reference elements of `struct sockaddr`
    - `NOTE`: `sin_zero` should be set to all zeros with `memset()`
    - `sin_family` corresponds to `sa_family`

- `struct in_addr`
    ```c
    struct in_addr {
        uint32_t s_addr;    // 32-bit int (4 bytes)
    };
    ```

    if one declares a variable `ina` of type `struct sockaddr_in`, then `ina.sin_addr.s_addr` reference sthe 4-byte IP address (in Network Byte Order).

### 3.3.2 IPv6 layout
- `struct sockaddr_in6`  && `struct in6_addr`
    ```c
    struct sockaddr_in6 {
        u_int16_t       sin6_family;    // address family, AF_INET6
        u_int16_t       sin6_port;      // port, Network Byte Order
        u_int16_t       sin6_flowinfo;  // IPv6 flow information
        struct in6_addr sin6_addr;      // IP6 address
        u_int16_t       sin6_scope_id;  // Scope ID
    };

    struct in6_addr {
        unsigned char   s6_addr[16];    // IPv6 address
    };
    ```

### 3.3.3 `struct sockaddr_storage`
- designed to be large enough to hold both IPv4 and IPv6 structures.
- for cases in which you don't know which to fill out first
- `struct sockaddr_storage`
    ```c
    struct sockaddr_storage {
        sa_family_t ss_family;
        char    __ss_pad1[_SS_PAD1SIZE];
        int64_t __ss_align;
        char    __ss_pad2[_SS_PAD2SIZE];
    }
    ```
-   the `ss_family` would reveal if it's `AF_INET` or `AF_INET6`, then you can cast it accordingly


## 3.4 IP Addresses, Part Deux
- Manipulation of IP Addresses
    - example:
        - Let's say you have a `struct sockaddr_in ina;`, and an IP Address "`10.12.110.57`" or "`2001:db8:63b3:1::3490`" that you want to store it into.
        - The function you want to use is `inet_pton()`
            - converts an IP address in numbers-and-dots notation into either a `struct_in_addr` or a `struct in6_addr` based on paremeters either `AF_INET` or `AF_INET6`
        - `pton` stands for "presentation to network"
        - example as follows:
            ```c
            struct sockaddr_in sa;
            struct sockaddr_in6 sa6;

            inet_pton(AF_INET, "10.12.110.56", &(sa.sin_addr));
            inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr));
            ```
    - error prone, `inet_pton()` returns `-1` on error, or `0` of the address is messed up.
    - printing of such addresses to stdout can be done like this
        ```c
            struct sockaddr_in sa;
            struct sockaddr_in6 sa6;

            inet_pton(AF_INET, "10.12.110.56", &(sa.sin_addr));
            inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr));

            // example filling of address^

            char ip4[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);

            char ip6[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET, &(sa6.sin_addr), ip6, INET6_ADDRSTRLEN);

            printf("IPv4 address is: %s\n", ip4);
            printf("IPv6 address is: %s\n", ip6);
        ```
