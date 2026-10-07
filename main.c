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















main()


/* Callback function pointer */
typedef void (*EventCallback)(Event event);
