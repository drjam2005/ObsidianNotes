System Calls or Bust
===

## 5.1 `getaddrinfo()` - Prepare to launch!
- simple usage.
    ```c
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>

    int getaddrinfo(
        const char *node,               // e.g. "www.example.com" or IP
        const char *service,            // e.g. "http" or port number
        const struct addrinfo *hints,   // 
        struct addrinfo **res,          //
    );
    ```
- 3 input parmeters, and gives you a pointer to a linked list, `res`, of results.
    - The `node` parameter is the hostname to connect to, or an IP address.
    - Next is the parameter `service`, which can be a port number, like "80", or the name of a service, like "http" or "ftp" or "telnet" or "smtp" or whatever.
    - Finally, the `hints` parameter points to a `struct addrinfo` that you've already filled out with relevant information


- sample usage:
    - let's say you want to listen to your host's IP address, on port 3490.
        ```c
        int status;
        struct addrinfo hints;
        struct addrinfo *servInfo; // will point to results
        
        memset(&hints, 0, sizeof hints);

        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        if((status = getaddrinfo(NULL, "3490", &hints, &servInfo)) != 0 ) {
            fprintf(stderr, "gai error: %s\n", gai_strerror(status));
            exit(1);
        }

        // servInfo now points to a linked list of 1 or more
        // struct addrinfos

        // code...

        freeaddrinfo(servInfo);
        ```
        - `ai_family` is set to `AF_UNSPEC` in this case, to mean that we don't care if it's an IPv4 or and IPv6
        - `getaddrinfo(..., (struct addrinfo*)list)` uses `list` to point to a linked list of `struct addrinfo`s
        - use `freeaddrinfo()` once done

- more sample usage:
    - let's say you're a client who wants to connect to a particular server, say `www.example.net`, on port `3490`. `NOTE`: no connection yet
        ```c
        int status;
        struct addrinfo hints;
        struct addrinfo *servInfo;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        status = getaddrinfo("www.example.net", "3490", &hints, &servInfo);

        // etc...
        ```
        ```c
        // quick demo

        // refer to "../src/showip.c"
        ```

## 5.2 `socket()` - Get the File Descriptor!
- quick breakdown:
    ```c
    #include <sys/types.h>
    #include <sys/socket.h>

    int socket(int domain, int type, int protocol);
    ```
    one would use the result of `getaddrinfo()` as parameters for socket, i.e.:
    ```c
    int status, s;
    struct addrinfo hints;
    struct addrinfo *servInfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo("www.example.net", "3490", &hints, &servInfo);

    // do the checking and the for loop

    s = socket(servInfo->ai_family, servInfo->ai_socktype, servInfo->ai_protocol);
    ```
    - `socket()` returns to you a *socket descriptor* that you can use in later sys calls, or `-1` on error.

## 5.3 `bind()` - What port am I on?
- binds a `struct sockaddr` to a local `socket` file descriptor
- quick breakdown:
    ```c
    #include <sys/types.h>
    #include <sys/socket.h>

    int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
    ```
    - `sockfd` is the socket file descriptor returned by the aforementioned `socket()`. `my_addr` is a pointer to a `struct sockaddr` (in which you cast your `sockaddr_in\6`s into), `addrlen` is the length in bytes of that address

- sample:
    - let's say you want bind to your host's IP address, on port 3490:
        ```c
        struct addrinfo hints, *res;
        int sockfd;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE; // so that the firset param, `NULL` 
                                     // will auto-fill as host ip address

        getaddrinfo(NULL, "3490", &hints, &res);

        // create a socket
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        // bind it to the port we passed in `getaddrinfo()`
        bind(sockfd, res->ai_addr, res->ai_addrlen);
        ```
        - `bind()` can return `-1` on error
        ```c
        // OLD WAY ! ! !
        int sockfd;
        struct sockaddr_in my_addr;

        sockfd = socket(PF_INET, SOCK_sTREAM, 0);

        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(MYPORT);   // short, network byte order
        my_addr.sin_addr.s_addr = inet_addr("10.12.110.57");
        memset(my_addir.sin_zero, '\0', sizeof my_addir.sin_zero);

        bind(sockfd, (struct sockaddr*)(&my_addr), sizeof my_addr);
        ```
        - one could also use `INADDR_ANY` for `my_addr.sin_addr.s_addr` if one also wants to use the host address
        - `IMPORTANT NOTE`: don't use port numbers below **1024**, as they are RESERVED, you can use port numbers above that untill **65535**
        - in some cases, you won't need to call `bind()`:
            -If you are `connect()`ing to a remote machine and don't care what youre local port is (as is the case with `telnet`, where you only care about the remote port), you can simple call `connect()`, it'll check if the socket is unbound, and will `bind()` it to an unused local port if necessary.

## 5.4 `connect()` - Hey, you!
- attempts to make a connection to the socket
    ```c
    #include <sys/types.h>
    #include <sys/socket.h>

    int connect(int sockfd, struct sockaddr *serv_addr, int addrLen);
    ```
    - `sockfd` is our socket file descriptor (again)
    - `serv_addr` is a pointer to a `struct sockaddr` containing the remote port and IP to connect to
    - `addrlen` is the length in bytes of the `serv_addr` structure

    - sample usage:
         ```c
         struct addrinfo hints, *res, *p;
         int sockfd;

         memset(&hints, 0, sizeof hints);
         hints.ai_family = AF_UNSPEC;
         hints.ai_socktype = SOCK_STREAM;

         getaddrinfo("www.example.net", "3490", &hints, &res);

         // create a socket
         sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

         // connect !
         connect(sockfd, res->ai_addr, res->ai_addrlen);
         ```
## 5.5 `listen()` - Will somebody please call me?
- for listening for incoming connections and handle them in some way
- Two steps:
    - `listen()`
    - `accept()` (to be tackled later)

    ```c
    #include <sys/socket.h>

    int listen(int socketfd, int backlog);
    ```
    - `sockfd`... our socket again...
    - `backlog` is the number of pending connections to wait in a queue.
        - such connections are going to wait in this queue until you hit `accept()` them
    - the function structure roughly looks like this
        ```c
        // set up variables
        getaddrinfo(...);
        socket(...);
        connect(...);   // for clients
        bind(...);      // for servers
        listen(...);    // for servers
        // accept(...) goes here
        ```

## 5.6 `accept()` - "Thank you for calling port 3490."
- extracts the first connection on the queue of pending `connect()`ions
    ```c
    #include <sys/types.h>
    #include <sys/socket.h>

    int accept(int sockfd, struct sockaddr* addr, socklen_t *addrlen);
    ```
    - `sockfd`... :O
    - `addr`, `sockaddr` of the incoming connection
    - `addrlen`, length of addr from incoming connection

    - sample:
        ```c
        #include <string.h>
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <netdb.h>

        #define MYPORT "3490"
        #define BACKLOG 10

        int main(void) {
            struct sockaddr_storage their_addr;
            socklen_t addr_size = sizeof their_addr;
            struct addrinfo hints, *res;
            int sockfd, new_fd;

            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;

            getaddrinfo(NULL, MYPORT, &hints, &res);

            // create a socket

            sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

            bind(sockfd, res->ai_addr, res->ai_addrlen);
            listen(sockfd, BACKLOG);

            new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &addr_size);

            // communication starts !
        }
        ```

## 5.7 `send()` and `recv()` - Talk to me, baby!
- used for sending and receiving data over `SOCK_STREAM` and `SOCK_DGRAM` sockets
    - `NOTE`: `recv()` can be a blocking call, it halts the program untill there is data to receive
    - `send()`
        ```c
        #include <sys/socket.h>

        int send(int sockfd, const void* msg, int len, int flags);
        ```
        - `sockfd`... :3
        - `msg` pointer to the data to be sent
        - `len` length of such data
        - `flags` check out `man send`, set to `0` for now

    `send()` returns number of bytes succesfully sent out, (if it's less than ur `len`, you might have to send the rest)  or `-1` for error
    - sample:
         ```c
         char* msg = "Beej was here!";
         int len, bytes_sent;

         len = strlen(msg);

         // assume sockfd is already set up
         bytes_sent = send(sockfd, msg, len, 0);
         ```
    - `recv()`
        ```c
        #include <sys/socket.h>

        int recv(int socgf, void* buff, int len, int flags);
        ```
        - `sockfd`... :3
        - `buff` pointer to the buff to be received/stored
        - `len` max length of such buff
        - `flags` check out `man recv`, set to `0` for now

    `recv()` returns number of bytes actually read into `buff`, or `-1` for error
    `recv()` returns 0 when the remote side has closed the connection

## 5.8 `sendto()` and `recvfrom()` - Talk to me, DGRAM-style
- cases for `SOCK_DGRAM` sockets
    - `sendto()`
        ```c
        #include <sys/socket.h>

        int sendto(int sockfd, const void *msg, int len, unsigned int flags,
                   const struct sockaddr *to, socklen_t tolen);
        ```
        - same as mentioned above...
        - `to`, a pointer to a `struct sockaddr`, (which is probably a `struct sockaddr_in*` cast to a `struct sockaddr*`)
        - `tolen`, just the size of such data, such as `sizeof *to` or `sizeof struct sockaddr`

        returns types are the same as `send()`

    - `recvfrom()`
        ```c
        #include <sys/socket.h>

        int recvfrom(int sockfd, void *buff, int len, unsigned int flags,
                   struct sockaddr *from, socklen_t *fromlen);
        ```
        - same as mentioned above...
        - `from`, a pointer to a local `struct sockaddr`, in which to find out which `struct sockaddr` it came from
        - `fromlen`, a pointer to a local `int`, just the size of such `struct sockaddr`

        returns types are the same as `recv()`

    - `NOTE`: we will mainly use `struct sockaddr_storage` as a kind of generic between IPv4 and IPv6

## 5.9 `close()` and `shutdown()` - Get outta my face!
- for closing and shutting down connections... duh...
    - `close()`
        ```c
        #include <unistd.h>

        int close(int fd);
        ```
        - `fd`, the file descriptor, in which it is what a socket is, basically (everything in Unix is a file!!!)
        - `close(...)` will prevent and more reads and writes to the socket, any attempts in doing such for this socket will receive an error. Releases the `fd` referring to the socket


    - `shutdown()`
        ```c
        #include <sys/socket.h>

        int shutdown(int sockfd, int how);
        ```
        - `sockfd`... we already know...
        - `how` is one of the following:

            | `how`                | Effect 
            | -------------------- | ------ 
            | `0` or `SHUT_RD`     | Further receives are disallowed
            | `1` or `SHUT_WR`     | Further sends are disallowed
            | `2` or `SHUT_RDWR`   | Further sends and receives are disallowed (like `close()`)

        - `shutdown()` returns 0 on success, and `-1` on error

## 5.10 `getpeername()` - Who are you?
- gives you the `struct sockaddr*` of a given socket
    ```c
    #include <sys/socket.h>

    int getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    ```
    - `sockfd` :3
    - `addr`, pointer to a local `struct sockaddr` to store information from the `sockfd` connection
    - `addrlen`, should be initialized as `sizeof *addr` or `sizeof(struct sockaddr)`
    - returns `-1` on error

## 5.11 `gethostname()` - Who am I?
- super self explanatory
    ```c
    #include <unistd.h>

    int gethostname(char* name, size_t namelen);
    ```
    - `name`, pointer to an array of which the name is to be stored,
    - `namelen`, size of such array
