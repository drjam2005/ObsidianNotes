---
id: IOStructure
aliases: []
tags: []
---
# I/O Structure
- After I/O starts, control returns to user program only upon I/O completion
    - Wait instruction idles the CPU until the nextc interrupt
    - Wait loop (contention for memory access)
    - At most one I/0 request is outstanding at a time, no simultaneous I/O processing
- After I/O starts, control returns to the user program without waiting for I/O completion
    - **System call** 
        - request to the OS to allow the user to wait for I/O completion
    - **Device-status table** 
        - contains entry for each I/O device indicating its type, address, and state
    - OS indexes into I/O device table to determine device status and to modify table entry to include interrupt

