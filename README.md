# Operating System

 > This repo maintains assignment/lab codes from Operating System (SJTU).

 - [Memory Allocation](#malloc)
 - [Process Management](#fork)
 - [File Interface](#file)
 - [Shell Scripting](#shell)

---

<a name="malloc"> </a>

## Memory Allocation

&emsp; Implement a program to simulate the procedure of allocating and release of memory using __Loop First Fit Algorithm__. Test case shows as below.

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

<a name="fork"> </a>

## Process Management

### Multiprocessing

&emsp; Create a child process, and cooperate with parent process. Test case shows as below.

```
=================== Process Management ===================


=================== Available Commands ===================

 Command |                   Description                  
---------|------------------------------------------------
    f    |          fork, wait, change and etc.           
    s    |         signal, share memory and etc.          
    h    |             show this help message             
    q    |                quit simulation                 

==========================================================

Please type the command: f

=================== Process Management ===================


=================== Available Commands ===================

 Command |                   Description                  
---------|------------------------------------------------
    d    |                 run `jsdaily`                  
    p    |                 run `jspcapy`                  
    h    |             show this help message             
    q    |                quit simulation                 

==========================================================

Please type the command: d

===================== Parent Process =====================


===================== Child Process ======================

usage: jsdaily [-h] command

Package Day Care Manager

optional arguments:
  -h, --help     show this help message and exit
  -V, --version  show program's version number and exit

Commands:
  jsdaily provides a friendly CLI workflow for the administrator of macOS to
  manipulate packages

================== End of Child Process ==================

Child process ID 63745 finished with status 0.

================= End of Parent Process ==================


=================== Available Commands ===================

 Command |                   Description                  
---------|------------------------------------------------
    d    |                 run `jsdaily`                  
    p    |                 run `jspcapy`                  
    h    |             show this help message             
    q    |                quit simulation                 

==========================================================

Please type the command: p

===================== Parent Process =====================


===================== Child Process ======================

usage: jspcapy [-h] [-V] [-o file-name] [-f format] [-j] [-p] [-t] [-a] [-F]
               [-v]
               input-file-name

PCAP file extractor and formatted exporter

positional arguments:
  input-file-name       The name of input pcap file. If ".pcap" omits, it will
                        be automatically appended.

optional arguments:
  -h, --help            show this help message and exit
  -V, --version         show program's version number and exit
  -o file-name, --output file-name
                        The name of input pcap file. If format extension
                        omits, it will be automatically appended.
  -f format, --format format
                        Print a extraction report in the specified output
                        format. Available are all formats supported by
                        jsformat, e.g.: json, plist, and tree.
  -j, --json            Display extraction report as json. This will yield
                        "raw" output that may be used by external tools. This
                        option overrides all other options.
  -p, --plist           Display extraction report as macOS Property List
                        (plist). This will yield "raw" output that may be used
                        by external tools. This option overrides all other
                        options.
  -t, --tree            Display extraction report as tree view text. This will
                        yield "raw" output that may be used by external tools.
                        This option overrides all other options.
  -a, --auto-extension  If output file extension omits, append automatically.
  -F, --files           Split each frame into different files.
  -v, --verbose         Show more information.

================== End of Child Process ==================

Child process ID 63748 finished with status 0.

================= End of Parent Process ==================


=================== Available Commands ===================

 Command |                   Description                  
---------|------------------------------------------------
    d    |                 run `jsdaily`                  
    p    |                 run `jspcapy`                  
    h    |             show this help message             
    q    |                quit simulation                 

==========================================================

Please type the command: q

Quitting...

==========================================================
```

### Process Communication

&emsp; Use semaphores to communicate between child process (receiver) and parent process (sender). Test case shows as below.

```
=================== Process Management ===================


=================== Available Commands ===================

 Command |                   Description                  
---------|------------------------------------------------
    f    |          fork, wait, change and etc.           
    s    |         signal, share memory and etc.          
    h    |             show this help message             
    q    |                quit simulation                 

==========================================================

Please type the command: s

================= Process Communication ==================

Open source file.
[3464] Sent `/' to child process.
Open destination file.
[705] Received `/' from parent process.
[3485] Sent `/' to child process.
[730] Received `/' from parent process.
[3506] Sent `\n' to child process.
[755] Received `\n' from parent process.
[3527] Sent `/' to child process.
[770] Received `/' from parent process.
[3543] Sent `/' to child process.
[785] Received `/' from parent process.
[3558] Sent ` ' to child process.
[800] Received ` ' from parent process.
[3576] Sent ` ' to child process.
[816] Received ` ' from parent process.
[3592] Sent `m' to child process.
[831] Received `m' from parent process.
[3607] Sent `a' to child process.
[845] Received `a' from parent process.
[3622] Sent `i' to child process.
[860] Received `i' from parent process.
[3637] Sent `n' to child process.
[876] Received `n' from parent process.
[3653] Sent `.' to child process.
[891] Received `.' from parent process.
[3669] Sent `c' to child process.
[907] Received `c' from parent process.
[3685] Sent `\n' to child process.
[922] Received `\n' from parent process.
[... Ellipses due to massive length ...]
[121965] Sent `}' to child process.
[117711] Received `}' from parent process.
[121978] Sent `\n' to child process.
[117723] Received `\n' from parent process.
Parent process sending completed.
Child process receiving completed.
[ID 63790] - finished
[ID 63791] - finished
```

<a name="file"> </a>

## File Interface

&emsp; Use file interface functions from C standard library to implement a file duplicator program. Test case shows as below.

```
===================== File Interface =====================

=================== Available Commands ===================
 Command |                   Description                  
---------|------------------------------------------------
    0    |       read/write & fread/fwrite @ 1,1,1        
    1    |    read/write & fread/fwrite @ 1024,1024,1     
    2    |                 fscanf/fprintf                 
    3    |                  fgetc/fputc                   
    4    |                  fgets/fputs                   
    5    |                      pipe                      
    6    |                      FIFO                      
    7    |                      mmap                      
    h    |             show this help message             
    q    |                quit simulation                 
==========================================================

Please type the command: 0
read/write@1 = 150180696
fread/fwrite@1 = 2814018

Please type the command: 1
read/write@1024 = 324933
fread/fwrite@1024 = 79689

Please type the command: 2
fscanf/fprintf = 7225860

Please type the command: 3
fgetc/fputc = 2786520

Please type the command: 4
fgets/fputs = 546048

Please type the command: 5
pipe[parent] = 293367
pipe[child] = 474582

[program restarted]

Please type the command: 6
fifo[parent] = 259164
fifo[child] = 4362075

[program restarted]

Please type the command: 7
mmap = 70761

Please type the command: q
Quitting...

==========================================================
```

<a name="shell"> </a>

## Shell Scripting

### `findit`

&emsp; Find files matching certain pattern. Test case shows as below.

```
$ findit main
+ grep --colour=auto --line-number --recursive --include '*.[ch]' main .
./Brainfuck/utilities/qdb.c:25:int main(int argc, char **argv){
./Befunge/utilities/Befunge-93/src/bef2c.c:121:int main (int, char **);
./Befunge/utilities/Befunge-93/src/bef2c.c:125:int main (argc, argv)
./Befunge/utilities/Befunge-93/src/bef2c.c:206:  fprintf (fo, "void main ()\n{\n  signed long a; signed long b; char c; srand (time (0));\n\n");
./Befunge/utilities/Befunge-93/src/bef.c:98:          explicitly pops remaining stack elements at end of execution.
./Befunge/utilities/Befunge-93/src/bef.c:117:            locations remain spaces.
./Befunge/utilities/Befunge-93/src/bef.c:207:int main (int, char **);
./Befunge/utilities/Befunge-93/src/bef.c:211:int main (argc, argv)
./Befunge/utilities/Befunge-93/src/befprof.c:141:int main (int, char **);
./Befunge/utilities/Befunge-93/src/befprof.c:145:int main (argc, argv)
./C/C.c:11:int main(int argc, const char * argv[]) {
```

### `tree`

&emsp; List contents of directories in a tree-like format.

```
$ tree ./jspcapy
./jspcapy
    .DS_Store
    .git
        [omits irrelevant files]
    .gitattributes
    .gitignore
    LICENSE
    MANIFEST.in
    README.md
    build
        bdist.macosx-10.13-x86_64
        lib
            jspcapy.py
    deprecated
        .DS_Store
        FileError.pcap
        in0.pcap
        out
        pcap.py
        test.py
    dist
        jspcapy-0.1.5.tar.gz
        jspcapy-0.2.0.tar.gz
        jspcapy-0.2.1.tar.gz
        jspcapy-0.2.2.tar.gz
        jspcapy-0.2.4.tar.gz
        jspcapy-0.2.5.tar.gz
        jspcapy-0.3.0.post1-py2.py3-none-any.whl
        jspcapy-0.3.0.post1.tar.gz
        jspcapy-0.3.0.tar.gz
        jspcapy-0.4.0-py2.py3-none-any.whl
        jspcapy-0.4.0.tar.gz
    jsformat -> ../jsformat/src
    jspcap -> ../jspcap/src
    jspcapy.egg-info
        PKG-INFO
        SOURCES.txt
        dependency_links.txt
        entry_points.txt
        requires.txt
        top_level.txt
    jspcapy.py
    sample
        .DS_Store
        in.pcap
        out.json
        out.plist
        out.txt
    setup.cfg
    setup.py
    tox.ini
```
