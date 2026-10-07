#include <stdio.h>
#include <string.h>

#define QUEUE_SIZE 10
#define MAX_CALLBACKS 6



/* Event types */
typedef enum
{
    PAYMENT_RECEIVED,
    PAYMENT_SUCCESS,
    PAYMENT_FAILED,
    NETWORK_CONNECTED,
    NETWORK_DISCONNECTED,
    LOW_BATTERY
} EventType;

* Event structure */
typedef struct
{
    EventType type;
    int amount;
} Event;


void paymentReceivedCallback(Event event)
{
    printf("Callback: Payment received: %d\n", event.amount);
}

void paymentSuccessCallback(Event event)
{
    printf("Callback: Payment successful: %d\n", event.amount);
}

void paymentFailedCallback(Event event)
{
    printf("Callback: Payment failed: %d\n", event.amount);
}

void networkCallback(Event event)
{
    if (event.type == NETWORK_CONNECTED)
    {
        printf("Callback: Network connected\n");
    }
    else if (event.type == NETWORK_DISCONNECTED)
    {
        printf("Callback: Network disconnected\n");
    }
}

void batteryCallback(Event event)
{
    printf("Callback: Low battery warning\n");
}



typedef struct
{
    Event events[QUEUE_SIZE];
    int front;
    int rear;
    int count;
} EventQueue;


EventQueue eventQueue;




void initQueue(void)
{
    eventQueue.front = 0;
    eventQueue.rear = 0;
    eventQueue.count = 0;
}

int isQueueEmpty(void)
{
    return eventQueue.count == 0;
}

int isQueueFull(void)
{
    return eventQueue.count == QUEUE_SIZE;
}

int pushEvent(Event event)
{
    if (isQueueFull())
    {
        printf("Error: event queue is full\n");
        return 0;
    }

    eventQueue.events[eventQueue.rear] = event;

    eventQueue.rear =
        (eventQueue.rear + 1) % QUEUE_SIZE;

    eventQueue.count++;

    return 1;
}

int popEvent(Event *event)
{
    if (isQueueEmpty())
    {
        return 0;
    }

    *event = eventQueue.events[eventQueue.front];

    eventQueue.front =
        (eventQueue.front + 1) % QUEUE_SIZE;

    eventQueue.count--;

    return 1;
}










main()


/* Callback function pointer */
typedef void (*EventCallback)(Event event);
