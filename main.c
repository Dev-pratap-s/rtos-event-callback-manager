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

