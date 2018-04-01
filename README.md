# Operating System

 > This repo maintains assignmet/lab codes from Operating System (SJTU).


### malloc

&emsp; Implement a program to simulate the procedure of allocationg and release of memory using __Loop First Fit Algorithm__. Test case shows as below.

```
=================== Memory Allocation ====================

Please indicate the maximum size of memory: 100
Now, the maximum size of memory is 100kB.

======================= Simulation =======================

=================== Available Commands ===================

 Command |                   Description                  
---------|------------------------------------------------
    a    |   assign new memory block according to size    
    r    |   release used memory block according to ID    
    s    |        show current memory usage status        
    h    |             show this help message             
    q    |                quit simulation                 

==========================================================

Please type the command: t

==================== Test Simulation =====================

Please indicate test rounds: 3

Start testing for 3 rounds...

Round 1: Calling command 'a'...

======================= Allocation =======================

Allocating for 42kB memory...
Allocated 42kB memory for job ID 1 at 0.

==========================================================


===================== Memory Status ======================

    No.   |   Status   |   Address   |   Size   |   ID    
==========|============|=============|==========|=========
        1 |    FREE    |          42 |       58 |       0 
        2 |    USED    |           0 |       42 |       1 

==========================================================

Round 2: Calling command 'r'...

======================== Release =========================

Releasing memory for job ID 0...
Memory for job ID 0 is not used.

==========================================================


===================== Memory Status ======================

    No.   |   Status   |   Address   |   Size   |   ID    
==========|============|=============|==========|=========
        1 |    FREE    |          42 |       58 |       0 
        2 |    USED    |           0 |       42 |       1 

==========================================================

Round 3: Calling command 'a'...

======================= Allocation =======================

Allocating for 91kB memory...
Not enough space for 91kB memory.

==========================================================


===================== Memory Status ======================

    No.   |   Status   |   Address   |   Size   |   ID    
==========|============|=============|==========|=========
        1 |    FREE    |          42 |       58 |       0 
        2 |    USED    |           0 |       42 |       1 

==========================================================


===================== Test Finished ======================


Quitting...

==========================================================

```