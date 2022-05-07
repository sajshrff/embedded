//
//  SleepingBarber.cpp
//  DS&Algo
//
//  Created by Ramsaran Ramasubbu on 4/28/22.
//  Copyright © 2022 Ramsaran Ramasubbu. All rights reserved.
//

#include "SleepingBarber.hpp"
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

sem_t *CustomerReady; //used by barber
sem_t *BarberReady; //used by customer
sem_t *SeatAccess;
int available_seats = 3;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void log(string s)
{
    pthread_mutex_lock(&m);
    cout << "Barber -> " << s << endl;
    pthread_mutex_unlock(&m);
}

void log_ID(string s, int id)
{
    pthread_mutex_lock(&m);
    cout << "Customer -> " << "ID : " << id << "  " << s << endl;
    pthread_mutex_unlock(&m);
}

void* Barber(void *id)
{
    while(1)
    {
        log("Check if a customer is available (goes to sleep if no signal from Customer)");
        sem_wait(CustomerReady);
        
        log("But, Got Signals from customer, Waits to get access to Waiting area");
        sem_wait(SeatAccess);
        
        log("Get access to Waiting Area, Increments the seats");
        available_seats++;
        
        log("Signal that the Barber is ready to cut");
        sem_post(BarberReady);
        
        log("Releases the Waiting area's access");
        sem_post(SeatAccess);
        
        log("Barber Starts Cutting");
        
        sleep(0.5);
    }
    
    return NULL;
    
}

void* Customer(void *id)
{
    log_ID("Entering the Barber Shop" , *((int *)id));
    
    sem_post(SeatAccess); // initialized to 1
    log_ID("Gets access to Waiting area",  *((int *)id));
    
    if(available_seats > 0){
        available_seats--;  // sit down in a chair
        
        log_ID("Signals Barber that I am ready", *((int *)id));
        sem_post(CustomerReady);
        
        log_ID("Gives back the Waiting area access", *((int *)id));
        sem_post(SeatAccess);
        
        log_ID("Wait till Barber is ready", *((int *)id));
        sem_wait(BarberReady);
        
        log_ID("But, Got Signal from Barber", *((int *)id));
    }
    else
    {
        log_ID("No Seat Available, Release Access to Waiting Area and Leaves", *((int *)id));
        sem_post(SeatAccess);
    }
    
    return NULL;
}

void SleepingBarber_Execute()
{
    int customer_count = 5;
    
    pthread_t customer_thread[10], barber_thread;
    
    BarberReady = sem_open("/BarberReady_sem", O_CREAT, 0644, 0);
    if (BarberReady == SEM_FAILED) {
         perror("Failed to open semphore for empty");
         exit(-1);
    }

    CustomerReady = sem_open("/CustomerReady_sem", O_CREAT, 0644, 0);
    if (CustomerReady == SEM_FAILED) {
         sem_close(CustomerReady);
         perror("Failed to open semphore for full");
         exit(-1);
    }
    
    SeatAccess = sem_open("/SeatAccess_sem", O_CREAT, 0644, 1);
    if (SeatAccess == SEM_FAILED) {
         sem_close(SeatAccess);
         perror("Failed to open semphore for full");
         exit(-1);
    }
    
    int id[10] = {1,2,3,4,5,6,7,8,9,10};
    
    pthread_create(&barber_thread, NULL, Barber, (void*)&id);

    for(int i = 0; i < customer_count; i++)
        pthread_create(&customer_thread[i], NULL, Customer, (void*)&id[i]);
    
    pthread_join(barber_thread, NULL);
    
    for(int i = 0; i < customer_count; i++)
        pthread_join(customer_thread[i], NULL);
    
    sem_unlink("/BarberReady_sem");
    sem_unlink("/CustomerReady_sem");
    sem_unlink("/SeatAccess_sem");
    
    sem_close(BarberReady);
    sem_close(CustomerReady);
    sem_close(SeatAccess);
    
    return;
}


