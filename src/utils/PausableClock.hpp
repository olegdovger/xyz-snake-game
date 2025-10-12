#pragma once
#include <chrono>
#include "TimerManager.hpp"

/**
 * @brief A clock that can be paused and unpaused globally
 */
class PausableClock : public TimerManager::PausableTimer {
public:
  PausableClock();
  ~PausableClock();

  /**
   * @brief Get elapsed time in seconds, accounting for pause time
   */
  float getElapsedTime() const;

  /**
   * @brief Restart the clock
   */
  void restart();

  /**
   * @brief Pause this clock
   */
  void pause() override;

  /**
   * @brief Unpause this clock
   */
  void unpause() override;

private:
  std::chrono::steady_clock::time_point startTime;
  float pausedTime;
  bool isPausedLocally;
};
