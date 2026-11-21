#include <Arduino.h>
#include "TimerManager.h"

const TimerHandle TimerManager::InvalidHandle = {TIMER_MANAGER_MAX_TIMERS, 0};

TimerManager::TimerManager()
{
    nextGeneration = 1;
    Clear();
}

TimerHandle TimerManager::StartOnce(uint32_t delayMs,
                                    TimerCallback callback,
                                    void *context)
{
    return StartTimer(delayMs, false, 0, callback, context);
}

TimerHandle TimerManager::StartRepeating(uint32_t periodMs,
                                         TimerCallback callback,
                                         void *context)
{
    if (periodMs == 0)
        return InvalidHandle;

    return StartTimer(periodMs, true, periodMs, callback, context);
}

TimerHandle TimerManager::StartTimer(uint32_t delayMs,
                                     bool repeating,
                                     uint32_t periodMs,
                                     TimerCallback callback,
                                     void *context)
{
    if (callback == 0)
        return InvalidHandle;

    uint8_t slotId = TIMER_MANAGER_MAX_TIMERS;

    for (uint8_t i = 0; i < TIMER_MANAGER_MAX_TIMERS; ++i)
    {
        if (!timers[i].active)
        {
            slotId = i;
            break;
        }
    }

    if (slotId == TIMER_MANAGER_MAX_TIMERS)
        return InvalidHandle;

    // Save the first generation immediately rather than waiting for Update(),
    // so a returned handle is always safe to cancel or pause.
    uint16_t generation = nextGeneration++;
    if (nextGeneration == 0)
        nextGeneration = 1;

    TimerSlot &slot = timers[slotId];
    slot.active = true;
    slot.paused = false;
    slot.repeating = repeating;
    slot.generation = generation;
    slot.dueTime = (uint32_t)millis() + delayMs;
    slot.periodMs = periodMs;
    slot.remainingMs = delayMs;
    slot.callback = callback;
    slot.context = context;

    TimerHandle handle = {slotId, generation};
    return handle;
}

TimerManager::TimerSlot *TimerManager::GetSlot(TimerHandle handle)
{
    if (handle.id >= TIMER_MANAGER_MAX_TIMERS)
        return 0;

    TimerSlot &slot = timers[handle.id];
    if (!slot.active || slot.generation != handle.generation)
        return 0;

    return &slot;
}

const TimerManager::TimerSlot *TimerManager::GetSlot(TimerHandle handle) const
{
    if (handle.id >= TIMER_MANAGER_MAX_TIMERS)
        return 0;

    const TimerSlot &slot = timers[handle.id];
    if (!slot.active || slot.generation != handle.generation)
        return 0;

    return &slot;
}

bool TimerManager::Cancel(TimerHandle handle)
{
    TimerSlot *slot = GetSlot(handle);
    if (slot == 0)
        return false;

    slot->active = false;
    slot->paused = false;
    slot->callback = 0;
    slot->context = 0;
    return true;
}

bool TimerManager::Pause(TimerHandle handle, uint32_t now)
{
    TimerSlot *slot = GetSlot(handle);
    if (slot == 0 || slot->paused)
        return false;

    int32_t remaining = (int32_t)(slot->dueTime - now);
    slot->remainingMs = remaining > 0 ? (uint32_t)remaining : 0;
    slot->paused = true;
    return true;
}

bool TimerManager::Resume(TimerHandle handle, uint32_t now)
{
    TimerSlot *slot = GetSlot(handle);
    if (slot == 0 || !slot->paused)
        return false;

    slot->dueTime = now + slot->remainingMs;
    slot->paused = false;
    return true;
}

bool TimerManager::IsActive(TimerHandle handle) const
{
    const TimerSlot *slot = GetSlot(handle);
    return slot != 0;
}

void TimerManager::Update(uint32_t now)
{
    for (uint8_t i = 0; i < TIMER_MANAGER_MAX_TIMERS; ++i)
    {
        TimerSlot &slot = timers[i];
        if (!slot.active || slot.paused)
            continue;

        if ((int32_t)(now - slot.dueTime) < 0)
            continue;

        TimerCallback callback = slot.callback;
        void *context = slot.context;

        if (slot.repeating)
        {
            slot.dueTime += slot.periodMs;
        }
        else
        {
            slot.active = false;
            slot.paused = false;
            slot.callback = 0;
            slot.context = 0;
        }

        if (callback != 0)
            callback(context);
    }
}

void TimerManager::Clear()
{
    for (uint8_t i = 0; i < TIMER_MANAGER_MAX_TIMERS; ++i)
    {
        timers[i].active = false;
        timers[i].paused = false;
        timers[i].repeating = false;
        timers[i].generation = 0;
        timers[i].dueTime = 0;
        timers[i].periodMs = 0;
        timers[i].remainingMs = 0;
        timers[i].callback = 0;
        timers[i].context = 0;
    }
}
