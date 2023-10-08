#ifndef CHEATS_H
#define CHEATS_H

namespace t2c::cheats {

/**
 * @brief Tries to toggle ghost mode.
 * @note Tracks internal state, instead of copying the game's.
 */
void toggle_ghost(void);

/**
 * @brief Toggles turbo mode.
 */
void toggle_turbo(void);

/**
 * @brief Toggles god mode.
 */
void toggle_god(void);

/**
 * @brief Saves the player's current position.
 */
void save_pos(void);

/**
 * @brief Loads the player's saved position.
 */
void load_pos(void);

/**
 * @brief Checks if we have a saved position.
 *
 * @return True if we have a saved position.
 */
bool has_saved_pos(void);

}  // namespace t2c::cheats

#endif /* CHEATS_H */
