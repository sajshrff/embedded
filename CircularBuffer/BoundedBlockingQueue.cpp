//
//  BoundedBlockingQueue.cpp
//  DS&Algo
//
//  Created by Ramsaran Ramasubbu on 4/16/22.
//  Copyright © 2022 Ramsaran Ramasubbu. All rights reserved.
//

#include "BoundedBlockingQueue.hpp"
#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

class BoundedBlockingQueue {
private:
    pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
    pthread_cond_t full = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
    
public:
    int capacity = 0;
    int *arr;
    int curr_size = 0;
    int head = 0;
    int tail = 0;
    
    BoundedBlockingQueue(int capacity)
    {
        this -> capacity = capacity;
        arr = new int[capacity];
        
        for(int i = 0; i < capacity; i++)
            arr[i] = -1;
    }
    
    void enqueue(int element)
    {
        int s = pthread_mutex_lock(&mu);
        
        while(curr_size == capacity)
            s = pthread_cond_wait(&full, &mu);
        
        arr[tail] = element;
        tail = (tail + 1) % capacity;
        curr_size++;
        
        s = pthread_cond_signal(&empty);
        
        s = pthread_mutex_unlock(&mu);
        
        return;
    }
    
    int dequeue()
    {
        int s = pthread_mutex_lock(&mu);
        
        while(curr_size == 0)
            s = pthread_cond_wait(&empty, &mu);
        
        int val = arr[head];
        arr[head] = -1;
        head = (head + 1) % capacity;
        curr_size--;
        
        s = pthread_cond_signal(&full);
        
        s = pthread_mutex_unlock(&mu);
        
        return val;
    }
    
    int size()
    {
        int s = pthread_mutex_lock(&mu);
            int local_size = curr_size;
        s = pthread_mutex_unlock(&mu);
        
        return local_size;
    }
};
