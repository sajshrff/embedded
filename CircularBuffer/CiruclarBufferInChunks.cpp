//
//  CiruclarBufferInChunks.cpp
//  DS&Algo
//
//  Created by Ramsaran Ramasubbu on 4/10/22.
//  Copyright © 2022 Ramsaran Ramasubbu. All rights reserved.
//

#include "CiruclarBufferInChunks.hpp"
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>

using namespace std;

class Buffer
{
private:
    int tail = 0;
    int head = 0;
    int capacity = 0;
    int size = 0;
    char *arr;
    
public:
    Buffer(int capacity)
    {
        arr = new char[capacity];
        for(int i = 0; i < capacity; i++)
            arr[i] = '\0';
    }
    
    int write(vector<char> src)
    {
        int n = (int)src.size();
        if(n == 0 || size == capacity)
            return 0;
        
        int i = 0;
        for(i = 0; i < n; i++)
        {
            arr[tail] = src[i];
            tail = (tail + 1) % capacity;
            size++;
            if(size > capacity)
                break;
        }
        return i;
    }

    vector<char> read(int n)
    {
        vector<char> buff;
        if(n == 0)
            return buff;
        
        for(int i = 0; i < n; i++)
        {
            buff.push_back(arr[head]);
            arr[head] = '\0';
            head = (head + 1) % capacity;
            size--;
            if(size == 0)
                break;
        }
        
        return buff;
    }
};
