#include "dolphin/os.h"

#define PushTail(queue, mutex, link)                                                               \
    do                                                                                             \
    {                                                                                              \
        OSMutex* __prev;                                                                           \
                                                                                                   \
        __prev = (queue)->tail;                                                                    \
        if (__prev == NULL)                                                                        \
            (queue)->head = (mutex);                                                               \
        else                                                                                       \
            __prev->link.next = (mutex);                                                           \
        (mutex)->link.prev = __prev;                                                               \
        (mutex)->link.next = NULL;                                                                 \
        (queue)->tail = (mutex);                                                                   \
    } while (0)

#define PopHead(queue, mutex, link)                                                                \
    do                                                                                             \
    {                                                                                              \
        OSMutex* __next;                                                                           \
                                                                                                   \
        (mutex) = (queue)->head;                                                                   \
        __next = (mutex)->link.next;                                                               \
        if (__next == NULL)                                                                        \
            (queue)->tail = NULL;                                                                  \
        else                                                                                       \
            __next->link.prev = NULL;                                                              \
        (queue)->head = __next;                                                                    \
    } while (0)

#define PopItem(queue, mutex, link)                                                                \
    do                                                                                             \
    {                                                                                              \
        OSMutex* __next;                                                                           \
        OSMutex* __prev;                                                                           \
                                                                                                   \
        __next = (mutex)->link.next;                                                               \
        __prev = (mutex)->link.prev;                                                               \
                                                                                                   \
        if (__next == NULL)                                                                        \
            (queue)->tail = __prev;                                                                \
        else                                                                                       \
            __next->link.prev = __prev;                                                            \
                                                                                                   \
        if (__prev == NULL)                                                                        \
            (queue)->head = __next;                                                                \
        else                                                                                       \
            __prev->link.next = __next;                                                            \
    } while (0)

void __OSUnlockAllMutex(OSThread* thread)
{
    OSMutex* mutex;

    while (thread->queueMutex.head)
    {
        PopHead(&thread->queueMutex, mutex, link);
        mutex->count = 0;
        mutex->thread = NULL;
        OSWakeupThread(&mutex->queue);
    }
}

int __OSCheckMutex(OSMutex* mutex)
{
    OSThread* thread;
    OSThreadQueue* queue;
    s32 priority;

    priority = 0;
    queue = &mutex->queue;

    if (queue->head != NULL && queue->head->link.prev != NULL)
    {
        return 0;
    }
    if (queue->tail != NULL && queue->tail->link.next != NULL)
    {
        return 0;
    }
    thread = queue->head;
    while (thread)
    {
        if (thread->link.next != NULL && (thread != thread->link.next->link.prev))
        {
            return 0;
        }
        if (thread->link.prev != NULL && (thread != thread->link.prev->link.next))
        {
            return 0;
        }
        if (thread->state != 4)
        {
            return 0;
        }
        if (thread->priority < priority)
        {
            return 0;
        }
        priority = thread->priority;
        thread = thread->link.next;
    }
    if (mutex->thread)
    {
        if (mutex->count <= 0)
        {
            return 0;
        }
    }
    else if (mutex->count != 0)
    {
        return 0;
    }
    return 1;
}

int __OSCheckDeadLock(OSThread* thread)
{
    OSMutex* mutex = thread->mutex;

    while (mutex && mutex->thread)
    {
        if (mutex->thread == thread)
        {
            return 1;
        }
        mutex = mutex->thread->mutex;
    }
    return 0;
}

int __OSCheckMutexes(OSThread* thread)
{
    OSMutex* mutex = thread->queueMutex.head;

    while (mutex)
    {
        if (mutex->thread != thread)
        {
            return 0;
        }
        if (__OSCheckMutex(mutex) == 0)
        {
            return 0;
        }
        mutex = mutex->link.next;
    }
    return 1;
}
