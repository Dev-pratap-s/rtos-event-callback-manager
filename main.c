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


typedef struct
{
    EventType eventType;
    EventCallback callback;
    int registered;
} CallbackRegistration;

CallbackRegistration registrations[MAX_CALLBACKS];


int findRegistration(EventType type)
{
    for (int i = 0; i < MAX_CALLBACKS; i++)
    {
        if (registrations[i].registered &&
            registrations[i].eventType == type)
        {
            return i;
        }
    }

    return -1;
}


void registerCallback(EventType type, EventCallback callback)
{
    int index = findRegistration(type);

    if (index != -1)
    {
        printf("Error: callback already registered\n");
        return;
    }

    for (int i = 0; i < MAX_CALLBACKS; i++)
    {
        if (!registrations[i].registered)
        {
            registrations[i].eventType = type;
            registrations[i].callback = callback;
            registrations[i].registered = 1;

            printf("Callback registered\n");
            return;
        }
    }

    printf("Error: registration limit reached\n");
}


void processEvent(void)
{
    Event event;

    if (!popEvent(&event))
    {
        printf("No events in queue\n");
        return;
    }

    int index = findRegistration(event.type);

    if (index == -1)
    {
        printf("Error: no callback registered\n");
        return;
    }

    printf("Processing event\n");

    registrations[index].callback(event);
}

registrations[index].callback(event);


const char *getEventName(EventType type)
{
    switch (type)
    {
        case PAYMENT_RECEIVED:
            return "PAYMENT_RECEIVED";

        case PAYMENT_SUCCESS:
            return "PAYMENT_SUCCESS";

        case PAYMENT_FAILED:
            return "PAYMENT_FAILED";

        case NETWORK_CONNECTED:
            return "NETWORK_CONNECTED";

        case NETWORK_DISCONNECTED:
            return "NETWORK_DISCONNECTED";

        case LOW_BATTERY:
            return "LOW_BATTERY";

        default:
            return "UNKNOWN";
    }
}

void listEvents(void)
{
    if (isQueueEmpty())
    {
        printf("Event queue is empty\n");
        return;
    }

    printf("Events in queue:\n");

    for (int i = 0; i < eventQueue.count; i++)
    {
        int index =
            (eventQueue.front + i) % QUEUE_SIZE;

        Event event = eventQueue.events[index];

        printf("%d. %s",
               i + 1,
               getEventName(event.type));

        if (event.amount > 0)
        {
            printf(" | Amount: %d", event.amount);
        }

        printf("\n");
    }
}

void showStatus(void)
{
    printf("\n========== STATUS ==========\n");

    printf("Queue size      : %d\n", QUEUE_SIZE);
    printf("Events in queue : %d\n",
           eventQueue.count);
    printf("Free spaces     : %d\n",
           QUEUE_SIZE - eventQueue.count);

    printf("============================\n");
}


int main(void)
{
    char command[20];
    char eventName[50];
    char callbackName[50];

    EventType eventType;
    EventCallback callback;

    int amount;

    initQueue();

    for (int i = 0; i < MAX_CALLBACKS; i++)
    {
        registrations[i].registered = 0;
        registrations[i].callback = NULL;
    }

    printf("========================================\n");
    printf("     RTOS Event & Callback Manager\n");
    printf("========================================\n");

    printf("\nCommands:\n");
    printf("REGISTER <event> <callback>\n");
    printf("PUSH <event> [amount]\n");
    printf("PROCESS\n");
    printf("STATUS\n");
    printf("LIST\n");
    printf("EXIT\n\n");

    while (1)
    {
        printf("> ");

        if (scanf("%19s", command) != 1)
        {
            break;
        }

        /* REGISTER */
        if (strcmp(command, "REGISTER") == 0)
        {
            if (scanf("%49s %49s",
                      eventName,
                      callbackName) != 2)
            {
                printf("Error: invalid REGISTER command\n");
                continue;
            }

            if (!parseEventType(eventName, &eventType))
            {
                printf("Error: invalid event type\n");
                continue;
            }

            callback = parseCallback(callbackName);

            if (callback == NULL)
            {
                printf("Error: invalid callback\n");
                continue;
            }

            registerCallback(eventType, callback);
        }

        /* PUSH */
        else if (strcmp(command, "PUSH") == 0)
        {
            if (scanf("%49s", eventName) != 1)
            {
                printf("Error: invalid PUSH command\n");
                continue;
            }

            if (!parseEventType(eventName, &eventType))
            {
                printf("Error: invalid event type\n");
                continue;
            }

            Event event;

            event.type = eventType;
            event.amount = 0;

            if (eventType == PAYMENT_RECEIVED ||
                eventType == PAYMENT_SUCCESS ||
                eventType == PAYMENT_FAILED)
            {
                if (scanf("%d", &amount) != 1)
                {
                    printf("Error: payment event requires amount\n");
                    continue;
                }

                if (amount <= 0)
                {
                    printf("Error: amount must be greater than 0\n");
                    continue;
                }

                event.amount = amount;
            }

            if (pushEvent(event))
            {
                printf("Event pushed: %s\n",
                       getEventName(event.type));
            }
        }

        /* PROCESS */
        else if (strcmp(command, "PROCESS") == 0)
        {
            processEvent();
        }

        /* STATUS */
        else if (strcmp(command, "STATUS") == 0)
        {
            showStatus();
        }

        /* LIST */
        else if (strcmp(command, "LIST") == 0)
        {
            listEvents();
        }

        /* EXIT */
        else if (strcmp(command, "EXIT") == 0)
        {
            printf("Exiting...\n");
            break;
        }

        /* UNKNOWN */
        else
        {
            printf("Error: unknown command\n");
        }
    }

    return 0;
}


/* Callback function pointer */
typedef void (*EventCallback)(Event event);
