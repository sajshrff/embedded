#include<bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

void *alignedMalloc(size_t bytes, size_t aligned)
{
    void *p1, *p2;
    
    if (aligned & (aligned-1) != 0)
    {
        cout << "Aligned valued is not power of 2 " << endl;
        return NULL;
    }
    if (bytes < 0 || aligned < 0)
        return NULL;
        
    size_t total_size = bytes + aligned + sizeof(size_t);
    
    p1 = (void *)malloc(total_size);
    if (NULL == p1)
        return NULL;
    
    size_t addr1 = (size_t)p1 + sizeof(size_t) + (aligned-1);
    size_t addr2 = (size_t)(~(aligned-1));
    
    p2 = (void *)(addr1 & addr2);
    *((size_t *) p2 - sizeof(size_t)) = (size_t) p1;
    
    return p2;
}

void aligned_free(void *p)
{
    void *p_new = (void *)(*((size_t *)p - sizeof(size_t)));
    free(p_new);
    return;
}

int main()
{
    size_t *addr1;
    addr1 = (size_t *)alignedMalloc(19, 8);
    if (addr1 != NULL)
    {
        cout << addr1 << endl;
        aligned_free((void *)addr1);
    }
    
    addr1 = (size_t *)alignedMalloc(21, 16);
    if (addr1 != NULL)
    {
        cout << addr1 << endl;
        aligned_free((void *)addr1);
    }
    
    addr1 = (size_t *)alignedMalloc(64, 32);
    if (addr1 != NULL)
    {
        cout << addr1 << endl;
        aligned_free((void *)addr1);
    }
    
    return 0;
}
