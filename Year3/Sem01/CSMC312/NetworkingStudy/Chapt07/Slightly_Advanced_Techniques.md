Slightly Advanced Techniques
===
## 7.1 Blocking
- aka `sleep`, a lot of functions block, `accept()` blocks, all `recv()` functions block.
- if one wants a function to NOT block, one can call `fcntl()`:
- sample:
    ```c
    #include <unistd.h>
    #include <fcntl.h>

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    fcntl(sockfd, F_SETFL, O_NON_BLOCK);
    ```
