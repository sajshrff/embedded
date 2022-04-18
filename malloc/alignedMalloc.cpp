#include<stdio.h>

void* aligned_malloc(size_t bytes, size_t alignment)
{
    if(alignment & (alignment - 1) != 0)
        return NULL;
    void *p1, *p2;
    
    p1 = malloc(bytes + alignment + sizeof(size_t));
    if(p1 == NULL)
        return NULL;
    size_t addr = (size_t)p1 + alignment + sizeof(size_t);
    p2 = (void*)(addr - (addr % alignment));
    
    *((size_t*)p2 - 1) = (size_t)p1;
    
    return p2;
}
void aligned_free(void* p)
{
    free ((void*)(*((size_t*)p - 1)));
}
int main(void) {
    size_t address = (size_t)aligned_malloc(100, 8);
    printf("%x", address);
    return 0;
}
