//
//  main.c
//  malloc
//
//  Created by Jarry Shaw on 03/30/2018.
//  Copyright © 2018 Jarry Shaw. All rights reserved.
//

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define FREE 0              // memory block is free
#define USED 1              // memory block is in use

#define SUCCESS 0           // procedure succeeded
#define FULL 1              // memory is full
#define MemoryOflw 2        // memory overflow
#define INVALID 3           // invlid format
#define NotFound 4          // ID not found
#define NotUsed 5           // ID not used

typedef struct map {        // memory type
    int status;             // status code
    unsigned addr;          // memory address
    unsigned size;          // memory size
    unsigned id;            // job ID
} memType;

typedef struct node {       // memory node type
    memType memory;         // memory data
    struct node * prefix;   // prefix node
    struct node * suffix;   // suffix node
} memNode, * memList;

int TEST = 0;               // test mode flag
memList this;               // search start pointer
memType that;               // last allocated pointer
unsigned MAX_SIZE;          // memory max size
unsigned counter = 0;       // job ID counter

void simStart(void);        // start simulation
void simHelp(void);         // print manual
void simAllocate(void);     // simulate malloc
void simRelease(void);      // simulate mfree
void simShow(void);         // show memory list status
void simTest(void);         // test simulation

void memInit(void);         // initialise memory list
int memRelease(unsigned id);     // release memory node
int memAllocate(unsigned size);  // allocate memory node

void simStart() {
    memInit();
    char input;
    simHelp();
    while (1) {
        printf("Please type the command: ");
        if (scanf("%1s", &input) == EOF) return;
        switch (input) {
            case 'A': case 'a': simAllocate(); continue;
            case 'R': case 'r': simRelease(); continue;
            case 'S': case 's': simShow(); continue;
            case 'H': case 'h': simHelp(); continue;
            case 'Q': case 'q': return;
            case 'T': case 't': simTest(); return;
            default: printf("Invalid command.\n"); continue;
        }
    }
}

void simRelease() {
    printf("\n======================== Release =========================\n\n");
    
    unsigned id;
    if (TEST) {
        id = (unsigned)((double)rand() / (RAND_MAX + 1L) * (counter + 1L));
    } else {
        printf("Please indicate the job ID: ");
        if (scanf("%u", &id) == EOF) return;
    }
    printf("Releasing memory for job ID %u...\n", id);
    
    int returncode;
    returncode = memRelease(id);
    if (returncode == SUCCESS) {
        unsigned addr = that.addr;
        unsigned size = that.size;
        printf("Release %ukB memory from job ID %u at %u.\n", size, id, addr);
    } else if (returncode == NotFound) {
        printf("No job ID %u found in memory.\n", id);
    } else if (returncode == NotUsed) {
        printf("Memory for job ID %u is not used.\n", id);
    }
    
    printf("\n==========================================================\n\n");
}

void simAllocate() {
    printf("\n======================= Allocation =======================\n\n");
    
    unsigned size;
    if (TEST) {
        size = (unsigned)((double)rand() / (RAND_MAX + 1L) * (MAX_SIZE + 1L));
    } else {
        printf("Please indicate the job size: ");
        if (scanf("%u", &size) == EOF) return;
    }
    printf("Allocating for %ukB memory...\n", size);
    
    int returncode;
    returncode = memAllocate(size);
    if (returncode == SUCCESS) {
        unsigned id = that.id;
        unsigned addr = that.addr;
        printf("Allocated %ukB memory for job ID %u at %u.\n", size, id, addr);
    } else if (returncode == FULL) {
        unsigned id = that.id;
        unsigned addr = that.addr;
        printf("Allocated %ukB memory for job ID %u at %u, but no more space left.\n", size, id, addr);
    } else if (returncode == MemoryOflw) {
        printf("Not enough space for %ukB memory.\n", size);
    } else if (returncode == INVALID) {
        printf("Invalid memory request %ukB.\n", size);
    }
    
    printf("\n==========================================================\n\n");
}

void simHelp() {
    printf("\n=================== Available Commands ===================\n\n");
    
    printf(" Command |                   Description                  \n");
    printf("---------|------------------------------------------------\n");
    printf("    a    |   assign new memory block according to size    \n");
    printf("    r    |   release used memory block according to ID    \n");
    printf("    s    |        show current memory usage status        \n");
    printf("    h    |             show this help message             \n");
    printf("    q    |                quit simulation                 \n");
    
    printf("\n==========================================================\n\n");
}

void simShow() {
    printf("\n===================== Memory Status ======================\n\n");
    
    printf("    No.   |   Status   |   Address   |   Size   |   ID    \n");
    printf("==========|============|=============|==========|=========\n");
    
    int inc = 1;
    memNode * temp = this;
    
    do {
        char * status;
        int id = temp->memory.id;
        int addr = temp->memory.addr;
        int size = temp->memory.size;

        if (temp->memory.status == FREE) status = "   FREE   ";
        else  status = "   USED   ";
        
        printf(" %8d | %10s | %11d | %8d | %7d \n", inc, status, addr, size, id);
        
        inc++;
        temp = temp->suffix;
    } while (temp != this);
    
    printf("\n==========================================================\n\n");
}

void simTest() {
    printf("\n==================== Test Simulation =====================\n\n");
    
    TEST = 1;
    unsigned round;
    printf("Please indicate test rounds: ");
    if (scanf("%u", &round) == EOF) return;
    printf("\nStart testing for %u rounds...\n\n", round);
    
    srand((unsigned)time(NULL));
    for (int i=1; i<=round; ++i) {
        switch (rand() % 2) {
            case 0:
                printf("Round %d: Calling command 'a'...\n", i);
                simAllocate();
                simShow();
                continue;
            case 1:
                printf("Round %d: Calling command 'r'...\n", i);
                simRelease();
                simShow();
                continue;
        }
    }
    
    printf("\n===================== Test Finished ======================\n\n");
}

void memInit() {
    this = (memNode *)malloc(sizeof(memNode));
    
    // initialise memory node
    this->prefix = this;
    this->suffix = this;
    this->memory.status = FREE;
    this->memory.addr = 0;
    this->memory.size = MAX_SIZE;
    this->memory.id = counter;
}

int memAllocate(unsigned size) {
    // job size should be smaller than MAX_SIZE
    if (size > MAX_SIZE) return MemoryOflw;
    else if (size == 0) return INVALID;
    
    // pointer to current node
    memNode * temp = this;
    
    // allocation procedure
    do {
        // if memory block is free
        if (temp->memory.status == FREE) {
            // if size matches
            if (size == temp->memory.size) {
                temp->memory.id = ++counter;
                temp->memory.status = USED;
                
                // record allocated block
                that = temp->memory;
                
                // renew memory pointer
                memNode * new = this;
                do {
                    if (new->memory.status == FREE) {
                        // release success
                        this = new;
                        return SUCCESS;
                    }
                    new = new->suffix;
                } while (new != temp);
                
                // memory is now full
                return FULL;
            }
            
            // if size is smaller
            if (size < temp->memory.size) {
                memNode * new = (memNode *)malloc(sizeof(memNode));
                
                // initialise new memory node
                new->memory.id = ++counter;
                new->memory.addr = temp->memory.addr;
                new->memory.size = size;
                new->memory.status = USED;
                new->prefix = temp->prefix;
                new->suffix = temp;
                
                // renew original memory node
                temp->memory.addr += size;
                temp->memory.size -= size;
                temp->memory.id = 0;
                temp->memory.status = FREE;
                temp->prefix = new;
                
                // update memory node linking
                new->prefix->suffix = new;
                
                // record allocated block
                this = temp;
                that = new->memory;
                
                // release success
                return SUCCESS;
            }
        }
        temp = temp->suffix;
    } while (temp != this);
    
    // allocation failed
    return MemoryOflw;
}

int memRelease(unsigned id) {
    // job id should be possitive integer
    if (id == 0) return NotUsed;
    
    // pointer to current node
    memNode * temp = this;
    
    do {
        // if job ID matches
        if (temp->memory.id == id) {
            // record released block
            that = temp->memory;
            
            // renew node status
            temp->memory.id = 0;
            temp->memory.status = FREE;
            
            // if prefix node is free
            if (temp->prefix->memory.status == FREE) {
                // merge two memory nodes
                temp->prefix->memory.size += temp->memory.size;
                
                // renew memory node linking
                temp->prefix->suffix = temp->suffix;
                temp->suffix->prefix = temp->prefix;
                memNode * new = temp->prefix;
                free(temp); temp = new;
            }
            
            // if suffix node is free
            if (temp->suffix != temp && temp->suffix->memory.status == FREE) {
                // merge two memory nodes
                temp->suffix->memory.size += temp->memory.size;
                
                // renew memory node linking
                temp->prefix->suffix = temp->suffix;
                temp->suffix->prefix = temp->prefix;
                memNode * new = temp->suffix;
                free(temp); temp = new;
            }
            
            // release success
            this = temp;
            return SUCCESS;
        }
        temp = temp->suffix;
    } while (temp != this);
    
    // release failed
    return NotFound;
}

int main(int argc, const char * argv[]) {
    printf("=================== Memory Allocation ====================\n\n");
    printf("Please indicate the maximum size of memory: ");
    if (scanf("%u", &MAX_SIZE) != EOF) {
        printf("Now, the maximum size of memory is %ukB.\n\n", MAX_SIZE);
        printf("======================= Simulation =======================\n");
        simStart();
    }
    printf("\nQuitting...\n");
    printf("\n==========================================================\n\n");
    return 0;
}
