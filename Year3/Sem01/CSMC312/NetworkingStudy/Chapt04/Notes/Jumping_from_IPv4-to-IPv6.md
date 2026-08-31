Jumping from IPv4 to IPv6
===
 Just the code changes when switching

1. Use `getaddrinfo()` to get all the `struct sockaddr` info, instead of manually writing them by hand.
2. any hardcoded code related to IPv4, wrap them in a helper function.
3. Change `AF_INET` to `AF_INET6`.
4. Change `PF_INET` to `PF_INET6`.
5. Change `INADDR_ANY` to `in6addr_any` assignments, which are slightly different:
    ```c
    struct sockaddr_in sa;
    struct sockaddr_in6 sa6;

    sa.sin_addr.s_addr = INADDR_ANY;
    sa6.sin6_addr = in6addr_any;
    ```
    And the value `IN6ADDR_ANY_INIT` can be used as an initializer when the `struct in6_addr` is deeclared like so:
    ```c
    struct in6_addr ia6 = IN6ADDR_ANY_INIT;
    ```
6. Instead of `struct sockaddr_in` use `struct sockaddr_in6`, being sure to add "6" to the fields appropriate, there is no `sin6_zero` field.
7. Instead of `struct in_addr` use `struct in6_addr`, being sure to add "6" to the fields as appropriate.
8. Instead of `inet_aton()` or `inet_addr()`, use `inet_pton()`
9. Instead of `inet_ntoa()`, use `inet_ntop()`
10. Instead of `gethostbyname()`, use the superior `getaddrinfo()`.
11. Instead of `gethostbyaddr()`, use the superior `getnameinfo()`.
12. `INADDR_BROADCAST` no longer works. Use IPv6 multicast instead.
