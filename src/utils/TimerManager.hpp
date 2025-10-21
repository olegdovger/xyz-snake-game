#pragma once
#include <vector>

class PausableClock;

class TimerManager {
public:
  class PausableTimer {
  public:
    virtual ~PausableTimer() = default;
    virtual void pause() = 0;
    virtual void unpause() = 0;
  };

  static TimerManager& getInstance();

  void registerTimer(PausableTimer* timer);

  void unregisterTimer(PausableTimer* timer);

  void pauseAll();

  void unpauseAll();

private:
  TimerManager() = default;
  ~TimerManager() = default;
  TimerManager(const TimerManager&) = delete;
  TimerManager& operator=(const TimerManager&) = delete;

  std::vector<PausableTimer*> timers;
};
