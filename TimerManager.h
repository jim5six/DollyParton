#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include <stdint.h>

#ifndef TIMER_MANAGER_MAX_TIMERS
#define TIMER_MANAGER_MAX_TIMERS 24
#endif

typedef void (*TimerCallback)(void *context);

struct TimerHandle {
    uint8_t id;
    uint16_t generation;

    bool operator==(const TimerHandle &other) const {
        return id == other.id && generation == other.generation;
    }

    bool operator!=(const TimerHandle &other) const {
        return !(*this == other);
    }
};

class TimerManager {
public:
    TimerManager();

    TimerHandle StartOnce(uint32_t delayMs,
                          TimerCallback callback,
                          void *context = 0);

    TimerHandle StartRepeating(uint32_t periodMs,
                               TimerCallback callback,
                               void *context = 0);

    bool Cancel(TimerHandle handle);
    bool Pause(TimerHandle handle, uint32_t now);
    bool Resume(TimerHandle handle, uint32_t now);
    bool IsActive(TimerHandle handle) const;

    void Update(uint32_t now);
    void Clear();

    static const TimerHandle InvalidHandle;

private:
    struct TimerSlot {
        bool active;
        bool paused;
        bool repeating;
        uint16_t generation;
        uint32_t dueTime;
        uint32_t periodMs;
        uint32_t remainingMs;
        TimerCallback callback;
        void *context;
    };

    TimerSlot timers[TIMER_MANAGER_MAX_TIMERS];
    uint16_t nextGeneration;

    TimerHandle StartTimer(uint32_t delayMs,
                           bool repeating,
                           uint32_t periodMs,
                           TimerCallback callback,
                           void *context);

    TimerSlot *GetSlot(TimerHandle handle);
    const TimerSlot *GetSlot(TimerHandle handle) const;
};

#endif
