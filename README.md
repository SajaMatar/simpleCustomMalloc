WHAT IT DOES ?
allocates run time (heap) memory 

&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
the used techniqes ?
$ ) mutex and locks 
****** to make sure no 2threads allocate memory at the same time.
$ ) unions
****** to make sure that the memory aligns (16 of no header ,, 20 else )
$ ) linked list
****** our heap will be a linked list of header objects (and the header will be followes by the actual data)


&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
the main idea ? 

you check weather a block of memory is free or not (using get_free_block(size))
depending on it you will allocate either an in or out of current allocated heap  memory (if the heap can accomodate the required block)

the allocate(size) function deals with the cases :
1) invalid size pass -> exit with NULL
2) got free block -> change the free status , exit with the data address (header+1)
3) dont have a free block 
   3.1) got enough space -> allocate at the end , set the tail , exit with (header+1)
   3.2) dont have enough space -> exit with NULL
4) set the head if needed 


the deallocate(void \*block ) fucntion:
1) the location so we 
        1.1) can mark it as free (in in middle)
        1.2) release it to os if at the end



&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
HEADERS : 
stddef.h -> the other types such as size_t
unistd.h  -> declares sbrk()
pthread.h -> mutex

&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
VARIABLES :
-- globals 
global_lock (mutex)
head + tail (heap list) 

-- locals : 
   - allocate:
       header : (header_t) -> used to checks for free blocks (if true it will be sat directly)
       block  : in case it was allocated at the end , then it would be passed to header (to be properly sat)

   the others are self explainitory. 


&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
sample output :: (for the program break pointer)  
/* notes before we start , the header size is 0x18 */


before : 0x55a106304000
after char : 0x55a106325019
k is allocated at  : 0x55a106325018

after int : 0x55a106325035
4 is allocated at  : 0x55a106325031

after double : 0x55a106325055
8.000000 is allocated at  : 0x55a10632504d

&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
generating .so file and using it ?
preload it (linker) and use it as a shared library to use the allocate() function in other files
however, the .so file must be have the same path as the project you are trying to compile
