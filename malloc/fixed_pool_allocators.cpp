#define MEM_POOL_SIZE 1000
class Allocator{
    public:
    Allocator(int numChunks) : mNumChunks(numChunks)
    {
        alloc_ptr = NULL;
    }
    struct chunk{
        chunk* next;
    };
    void* allocate(size_t chunkSize);
    void deallocate(void* ptr);
    private:
    int mNumChunks;
    chunk* alloc_ptr;
    chunk* allocateBlock(size_t chunkSize); 
    uint8_t memory[MEM_POOL_SIZE];
};
void* Allocator::allocate(size_t chunkSize)
{
    if(alloc_ptr == NULL)
        alloc_ptr = allocateBlock(chunkSize);
    
    if(alloc_ptr == NULL)
        return NULL;
    void* availableChunk = reinterpret_cast<void*>(alloc_ptr);
    alloc_ptr = alloc_ptr->next;
    return availableChunk;
}
void Allocator::deallocate(void* ptr)
{
    if(ptr >= (void*)memory && ptr <= (void*)(memory + MEM_POOL_SIZE))
    {
        reinterpret_cast<chunk*>ptr->next = alloc_ptr;
        alloc_ptr = reinterpret_cast<chunk*>ptr;
    }
    else
        cout << "Invalid pointer to deallocate" << endl;
}

chunk* Allocator::allocateBlock(size_t chunkSize)
{
    size_t blockSize = chunkSize * mNumChunks;
    if(blockSize <= MEM_POOL_SIZE)
    {
        chunk* blockPtr = reinterpret_cast<chunk*>(memory);
        chunk *chunkPtr = blockPtr;
        
        //Form the links : Traverse upto the 2nd last chunk
        for(int i = 0; i < mNumChunks - 1; i++)
        {
            chunkPtr->next = reinterpret_cast<chunk*>(reinterpret_cast<uint8_t*>(chunkPtr) + chunkSize);
            chunkPtr = chunkPtr->next;
        }
        chunkPtr->next = NULL;
        return blockPtr;
    }
    else
        return NULL;
}
int main() {
    Allocator* small = new Allocator(20);
    void* smallPtr = small->allocate(50);
    
    Allocator* medium = new Allocator(10);
    void* mediumPtr = medium->allocate(100);
    
    std::cout << "Hello World!\n";
}
