
#define MEM_POOL_SIZE 1000

class Solution{
    public:
    //metadata for the blocks
    struct block{
        int free;
        int size;
        block* next;
    };
    uint8_t memory[MEM_POOL_SIZE];
    struct block* freeList = (block*)memory;

    void init()
    {
        freeList->size = MEM_POOL_SIZE - sizeof(block);
        freeList->free = 1;
        freeList->next = NULL;
    }
    void split(block* curr, int numBytes)
    {
        block* freeBlock = (curr + numBytes + sizeof(block));
        freeBlock->size = curr->size - (numBytes + sizeof(block));
        freeBlock->free = 1;
        freeBlock->next = curr->next;
        
        curr->size = numBytes;
        curr->free = 0;
        curr->next = freeBlock;
    }
    void* myMalloc(int numBytes)
    {
        block* curr;
        void* result;
        if(!freeList->size)
            init();
        curr = freeList;
        //Find the next available free block of memory
        while(curr->size < numBytes || curr->free != 1 && curr->next != NULL)
        {
            curr = curr->next;
        }
        //Case 1 : numBytes == available size
        if(curr->size == numBytes)
        {
            curr->free = 0;
            result = (void*) ++curr;
            cout << "Exact size block found and allocated" << endl;
            return result;
        }
        //case 2 : numBytes < available size
        else if(curr->size > numBytes + sizeof(block))
        {
            split(curr, numBytes);
            result = (void*) ++curr;
             cout << "Bigger size block found and allocated" << endl;
            return result;
        }
        //case 3 : numBytes > available size
        else
        {
            return NULL;
        }
    }
    void merge(void)
    {
        block* curr = freeList;
        while(curr->next)
        {
            if(curr->free && curr->next->free)
            {
                curr->size += curr->next->size + sizeof(block);
                curr->next = curr->next->next;
            }
            curr = curr->next;
        }
    }
    void myFree(void* ptr)
    {
        if(ptr >= (void*)memory && ptr <= (void*)(memory + MEM_POOL_SIZE))
        {
            block* curr = (block*)ptr;
            --curr;
            curr->free = true;
            merge();
            cout << "Memory freed and merged" << endl;
        }
        else
            cout << "Invalid address to free" << endl;
    }
};  
    int main() {
        Solution m;
    int* p = (int*) m.myMalloc(100*sizeof(int));
        cout << p << endl;
        m.myFree((void*)p);
    return 0;
}
