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
 * @brief Initalizes all pointers.
 */
void init(void);

}  // namespace t2c::pointers

#endif /* POINTERS_H */
