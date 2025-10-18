#pragma once

/**
 * @brief Audio volume constants for the game
 *
 * Contains all volume levels used throughout the game for consistent audio management.
 * All volumes are expressed as percentages (0.0f to 100.0f).
 */
namespace AudioConstants {
    // Background music volumes
    namespace Music {
        constexpr float BACKGROUND_MUSIC_VOLUME = 25.0f;  // 25% volume for background music
    }

    // Sound effect volumes
    namespace SoundEffects {
        constexpr float MENU_NAVIGATION_VOLUME = 30.0f;   // 30% volume for menu navigation sounds
        constexpr float MENU_SELECTION_VOLUME = 40.0f;    // 40% volume for menu selection sounds
        constexpr float GAME_OVER_VOLUME = 50.0f;         // 50% volume for game over sound
        constexpr float EAT_APPLE_VOLUME = 30.0f;         // 30% volume for eating apple sounds
        constexpr float START_GAME_VOLUME = 20.0f;        // 20% volume for start game sound
        constexpr float COUNTDOWN_VOLUME = 15.0f;         // 15% volume for countdown sounds
    }
}
