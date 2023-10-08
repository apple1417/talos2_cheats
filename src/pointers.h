#ifndef POINTERS_H
#define POINTERS_H

namespace t2c::pointers {

struct Vec3d {
    double x;
    double y;
    double z;
};

/**
 * @brief Gets the current player position.
 *
 * @return The player's position, or nullptr if no valid pointer exists.
 */
Vec3d* position(void);

/**
 * @brief Gets the current state of god mode.
 * @note Should be set to 1/0.
 *
 * @return The god mode state, or nullptr if no valid pointer exists.
 */
bool* god(void);

/**
 * @brief Gets the current state of turbo.
 * @note Should be set to 1/0.
 *
 * @return The turbo state, or nullptr if no valid pointer exists.
 */
bool* turbo(void);

/**
 * @brief Enables ghost mode.
 */
void enable_ghost(void);

/**
 * @brief Disables ghost mode.
 */
void disable_ghost(void);

/**
 * @brief Initalizes all pointers.
 */
void init(void);

}  // namespace t2c::pointers

#endif /* POINTERS_H */
