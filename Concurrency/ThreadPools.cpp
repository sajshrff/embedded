//
//  ThreadPools.cpp
//  DS&Algo
//
//  Created by Ramsaran Ramasubbu on 4/30/22.
//  Copyright © 2022 Ramsaran Ramasubbu. All rights reserved.
//

#include "ThreadPools.hpp"
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

typedef struct Task
{
    int a, b;
}Task;

Task TaskQueue[256];
int taskCount = 0;

pthread_mutex_t qmutex = PTHREAD_MUTEX_INITIALIZER;

void executeTask(Task* task)
{
    int result = task -> a + task -> b;
    printf("The sum of %d and %d is %d \n", task -> a, task -> b, result);
}

void* StartThread(void*)
{
    while(1)
    {
        Task task;
        int found = 0;
        
        pthread_mutex_lock(&qmutex);
        if(taskCount > 0)
        {
            found = 1;
            task = TaskQueue[0];
            int i = 0;
            for(i = 0; i < taskCount - 1; i++)
                TaskQueue[i] = TaskQueue[i+1];
            taskCount--;
        }
        pthread_mutex_unlock(&qmutex);
        
        if(found)
            executeTask(&task);
    }
    return NULL;
}

void submitTask(Task task)
{
    pthread_mutex_lock(&qmutex);
    
    TaskQueue[taskCount] = task;
    taskCount++;
    pthread_mutex_unlock(&qmutex);
}


void ThreadPools_Execute()
{
    int thread_num = 5;
    pthread_t thread[thread_num];
    
    for(int i = 0; i < thread_num; i++)
        pthread_create(&thread[i], NULL, StartThread, NULL);
    
    srand((int)time(NULL));
    for(int i = 0; i < 100; i++)
    {
        Task t = {
            t.a = rand() % 100,
            t.b = rand() % 100
        };
        submitTask(t);
    }
    
    for(int i = 0; i < thread_num; i++)
        pthread_join(thread[i], NULL);
    
    pthread_mutex_destroy(&qmutex);
    
    return;
}
