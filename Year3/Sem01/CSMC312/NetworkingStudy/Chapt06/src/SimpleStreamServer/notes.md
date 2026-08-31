// server.c

1. we dedfined a helper function `sigchld_handler()` // no idea what this is
2. we defined `get_in_addr`, which convenietly converts a `struct sockaddr` to a `struct sockaddr_in/6` respectively, and returns it's address for IPv* specific functions
3. main () 
	1. we declare 2 sockets, `sockfd` for our main server socket, and `new_fd` for other incoming socket connections
	2. `struct addrinfo:` `hints`, `*servInfo`, `*p`,
		2.1 hints -> for defining our IP and port of the server
		2.2 *servInfo -> for collecting available socket bindings
		2.3 *p -> used to loop through suck collection
	3. we do standard setup of hints...
	4. we use getaddrinfo to get data from our current hints (still unsure as to what this does)
	5. for loop (for each p in servInfo)
		5.1 if creation of socket given `p` (still no idea what this does), returns -1, we error out and continue
		5.2 no idea what this does
		5.3 we try binding to such available socket from our `p`
		5.4 break
	6. if `p == NULL`, then we didn't find stuff and exit
	7. we listen to such `sockfd` given a max queue size of `BACKLOG`.
	8. // no idea what these are, but the book mentioned it will be tackled later
	9. print waiting of connections
	10. while true loop
		10.1 given our socket `sockfd`, we wait and accept a fd connection and store their information in `sockaddr`.
		10.2 if `new_fd` == -1, we error out and continue
		10.3 we get their IP and store it in a string to print to console
		10.4 if fork returns 0, we do:
			10.4.1 close the `sockfd` // why?
			10.4.2 send "Hello World", to the `new_fd`, and if errors out we log
			10.4.3 close their connection
			10.4.4 close the current fork
		10.5 close their connection (just to be sure)

// chatgpt summarization
```
SERVER STARTUP
────────────────────────────────────────

getaddrinfo()
    │
    │ "Give me candidate addresses for
    │  an IPv4 TCP server on port 3490."
    ↓
servInfo
    │
    ↓
for each candidate:
    │
    ├── socket()
    │     │
    │     └── create kernel socket → sockfd
    │
    ├── setsockopt()
    │     │
    │     └── configure socket
    │
    └── bind()
          │
          └── attach local address + port

listen(sockfd)
    │
    └── turn sockfd into listening socket


SERVER LOOP
────────────────────────────────────────

accept(sockfd)
    │
    └── wait for client
          │
          ↓
       new_fd
          │
          └── connected socket for ONE client

fork()
  │
  ├── PARENT
  │     ├── close(new_fd)
  │     └── go back to accept()
  │
  └── CHILD
        ├── close(sockfd)
        ├── send(new_fd, ...)
        ├── close(new_fd)
        └── exit()
```

// client.c

1. we defined `get_in_addr`, same as before
2. main ()
	1. we create 2 ints, `sockfd` and `numbytes`
		1.1 sockfd -> the socket to be connecting to
		1.2 numbytes -> bytes to be received from connection
	2. `struct addrinfo:` `hints`, `*servInfo`, `*p`,
		2.1. same as before, i think.. can you reiterate to me as to why the CLIENT also needs to do this? also the server too lol
	3. int rv, getaddrinfo return val
	4. allocated string to store address string
	5. for loop (for each p in servInfo)
		5.1 if creation of socket given `p` (still no idea what this does), returns -1, we error out and continue
		5.2 we get the string variant of the connected IP
		5.3 print log
		5.4 attempt connection, if errors out we close such socket and continue finding more sockets
		5.5 break
	6. if p == NULL, no valid connections found and exit program
	7. get string variant again, since it's succesfull now and log
	8. free servInfo
	9. try receiving from socket, if errors out, log and exit program
	10. properly end string and print msg
	11. close and exit
	
