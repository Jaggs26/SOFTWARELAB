#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
typedef enum{FALSE,TRUE} boolean;
// BT20CSE071
// Heap management logic 
typedef struct blockMetaData Header;
 struct blockMetaData{
  size_t size;
  boolean is_free;
  Header* prev;
  Header *next;
};

#define META_SIZE sizeof(Header)
#define SBRK_FAILURE (void*)-1


void *header = NULL;

void invalid_Memory()
{
  
  printf("could not allocate the memory\n");
}

Header *request_space(Header *lptr,size_t size)
{


  Header *block = sbrk(0);

  void *request = sbrk(size+META_SIZE);
  if(request==SBRK_FAILURE)
  {
    invalid_Memory();
    return NULL;
  }


  if(lptr!=NULL)
  {
    lptr->next=block;
    block->prev=lptr;
    block->next=NULL;
  }
  else
  {
    block->prev = NULL;
    block->next = NULL;
  }
  block->size = size;
  block->is_free = FALSE;

  return block;
}
boolean canSplit(Header* block,size_t size)
{
  boolean ret_val = FALSE;
  if((block->size) - size > 0) ret_val = TRUE;

  return ret_val;
}

void* split(Header** block, size_t size)
{
  
  Header* freePart = (Header*)((*block)+size);
  // Intialising splitting
  freePart->size = ( (*block)->size )-size;
  freePart->is_free = TRUE;
  freePart->next = (*block)->next;
  freePart->prev = (*block);

  (*block)->size = size;
  (*block)->is_free = FALSE;
  (*block)->next=freePart;
  printf(" new  freeblock-->%ld bytes and allocated block-->%ld bytes\n",freePart->size,(*block)->size);

}
Header*  initilazie_FreeBlock(Header** end,size_t size)
{
  printf("finding the free block of size %ld bytes\n",size);
  Header* current = header;
  while (current!=NULL)
  {
    *end=current;
    if(current->is_free==TRUE&&current->size>=size) return current;
    current=current->next;
  }


  return NULL;
}
void  *myMalloc(size_t size)
{

  // here we Intialise malloc and allocate memory of required size 
  if(size<=0)
  {
    printf(" You have to select more than 0 for allocation \n");
    return NULL;
  }

  Header *block;
  if(header==NULL)
  {
    printf(" first time  allocation of memory \n");
    block = request_space(NULL,size); // head and tail 
    if(block == NULL)
    {
      invalid_Memory();
      return NULL;
    }
    header = block;
 
  }
  else
  {
    Header *end = header;
    block = initilazie_FreeBlock(&end,size);
    if(block == NULL) // if no free block found 
    {
      printf(" No free block found in the memory .\n");
      block = request_space(end,size);// we are requesting space 
      if(block == NULL)
      {
        invalid_Memory();
        return NULL;
      }
     
    }
    else
    { // if there iis a free block 
      printf(" Free block found at %p\n",(block+1) );
      if(canSplit(block, size))
      {
        printf(" It is bigger than size\n");
        split(&block,size);//
        printf(" Splitting is done.\n");
      }
      block->is_free=FALSE;

    }

  }
  

  return (block+1);
}

boolean rightmerge(Header* block)
{
  boolean ret_val = FALSE;
  if(block->next!=NULL&&(block->next)->is_free==TRUE )
  {

    ret_val = TRUE;
  }

  return ret_val;

}

boolean leftmerge(Header* block)
{

  boolean ret_val = FALSE;
  if(block->prev!=NULL&&(block->prev)->is_free==TRUE)
  {
    ret_val=TRUE;
  }

  return ret_val;
}

void rightmergeBlocks(Header** block)
{
  
  printf(" Intialising  rightmerge\n");
  Header*next_block = (*block)->next;
  (*block)->size += next_block->size;
  (*block)->next = next_block->next;
  next_block->prev = (*block)->prev;

}

size_t leftmergeBlocks(Header** block)
{
  
  printf(" Intialising leftmerge\n");

  Header* prev_block = (*block)->prev;
  prev_block->size += (*block)->size;
  (*block)->prev =  prev_block->prev;
  prev_block->next = (*block)->next;

  return prev_block->size;

}

void free(void* ptr)
{ // case 2 called 
 

  printf(" freeing memory located at %p \n",ptr);
  if(ptr==NULL)
  {
    return;
  }
  Header *block = (Header*)ptr - 1;
  printf(" Freed memory of size %ld bytes located at %p\n",block->size,ptr);
  if(rightmerge(block)) // returns boolean so that inside function can be called or neglected 
  {
    printf(" There is a free block next to this block\n"); 
   
    rightmergeBlocks(&block);
  
    printf(" final size of block is %ld bytes\n",block->size);
  }

  if(leftmerge(block))
  {

    printf(" There is  free block before this block\n");
   
    size_t finalSize = leftmergeBlocks(&block);
   
    printf(" final size of block is %ld bytes\n",finalSize);
  }

  block->is_free = TRUE;

  

}

void allocate(size_t size)
{

  void* ptr = myMalloc(size);
  // returns (block+1)
  printf(" Allocated a memory of size %ld bytes at the address %p on heap\n",size,ptr);
 
}

int main()
{
  int option;
  size_t size;
  int* ptr;
  printf("---------------------------------------------\n");
  printf("1.Allocate memory\n");
  printf("2.Free the allocated memory\n");
  printf("To exit press any key other than 1 or 2 \n");
    printf("-------------------------------------------\n");
  
  while(TRUE)
  {
    printf("\nEnter your option:");
    scanf("%d",&option);

    switch (option)
    {

      case 1:

      {
        printf("\nEnter the size in bytes:");
        scanf("%ld",&size);
        allocate(size);
        break;

      }

      case 2:

      {
        printf("\nEnter the address to free:");
        scanf("%p",&ptr);
        free(ptr);
        break;
      }

      default:
      {

        printf("*******END*******\n");
        exit(0);
      }

    }

}

return 0;

}