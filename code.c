#include<stdio.h>
#include<unistd.h>
#include<stddef.h>
#include<pthread.h>

// the global allocation lock
pthread_mutex_t global_lock;


// what is used for alignning the header (so i can add one to it to acess the actuall data)
typedef char ALIGN[16];
union header  *get_free_block(size_t size);

// the header union 
union header
{
    struct  {
        size_t size;
        unsigned free;
        union header *next;
    }s;

    ALIGN stb;
};

typedef union header header_t;


// set the header
void set_header(header_t *h,size_t si , unsigned f)
{
    h->s.size = si;
    h->s.free = f;
    h->s.next = NULL;
}



// heap list
header_t *head , *tail;


// allocate memory 
void *allocate(size_t size)
{
    if(!size) 
        return NULL;

pthread_mutex_lock(&global_lock);


header_t *header = get_free_block(size);
// if you had a free middle space
if(header)
{
    header->s.free=0;
    pthread_mutex_unlock(&global_lock);
    // the +1 is to go to the data part rather than the header.
    return (void*)(header+1);

}


// if the header is at the end 
size_t total_size = size+sizeof(header_t);
void *block = sbrk(total_size);
// if there is no space:
if(block==(void*)-1)
{
    pthread_mutex_unlock(&global_lock);
    return NULL;
}
// if there is a space continue with setting the header : 
header=block;
// after incrementing the block , you need to set the header : 
set_header(header,total_size,0);
if (!head)
    head = header;


// to avoid getting it circular
if(tail)
    tail->s.next = header;
   
tail=header;

pthread_mutex_unlock(&global_lock);
return (void*)(header+1);
}

// find the first free slot
header_t*  get_free_block(size_t size)
{
header_t *current = head;

    while (current != NULL)
    {
        if(current->s.free==1 && size <= current->s.size)
            return current;

        current=current->s.next;
    }
    return NULL;
}







int main()
{
    printf("before : %p\n",sbrk(0));

    char *s = allocate(1);
    *s = 'k';
    printf("after char : %p\n", sbrk(0));
    printf("%c is allocated at  : %p\n\n", *s,s);

    
    int *i = allocate(4);
    *i = 4;
    printf("after int : %p\n", sbrk(0));
    printf("%d is allocated at  : %p\n\n", *i,i);


    double  *d = allocate(8);
    *d = 8;
    printf("after double : %p\n", sbrk(0));
    printf("%f is allocated at  : %p\n\n", *d,d);




}
