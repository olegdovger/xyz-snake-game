#pragma once
#include <vector>

// Forward declaration to avoid circular dependency
class PausableClock;

/**
 * @brief Global timer manager that can pause/unpause all game timers
 */
class TimerManager {
public:
  /**
   * @brief Interface for timers that can be paused
   */
  class PausableTimer {
  public:
    virtual ~PausableTimer() = default;
    virtual void pause() = 0;
    virtual void unpause() = 0;
  };

  /**
   * @brief Get the singleton instance
   */
  static TimerManager& getInstance();

  /**
   * @brief Register a timer to be managed
   */
  void registerTimer(PausableTimer* timer);

  /**
   * @brief Unregister a timer
   */
  void unregisterTimer(PausableTimer* timer);

  /**
   * @brief Pause all registered timers
   */
  void pauseAll();

  /**
   * @brief Unpause all registered timers
   */
  void unpauseAll();

  /**
   * @brief Check if timers are currently paused
   */
  bool isPaused() const { return paused; }

private:
  TimerManager() = default;
  ~TimerManager() = default;
  TimerManager(const TimerManager&) = delete;
  TimerManager& operator=(const TimerManager&) = delete;

  std::vector<PausableTimer*> timers;
  bool paused = false;
};
