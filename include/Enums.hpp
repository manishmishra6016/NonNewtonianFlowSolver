#pragma once

// If no geometry file is provided in the input file, lid driven cavity case
// will run by default. In the Grid.cpp, geometry will be created following
// PGM convention, which is:
// 0: fluid, 3: fixed wall, 4: moving wall
namespace LidDrivenCavity {
const int moving_wall_id = 8;
const int fixed_wall_id = 4;
const double wall_velocity = 1.0;
} // namespace LidDrivenCavity

enum class border_position {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
};

namespace border {
const int TOP = 0;
const int BOTTOM = 1;
const int LEFT = 2;
const int RIGHT = 3;
} // namespace border

namespace neighbour {
const int BOTTOM = 1;
const int LEFT = 0;
const int RIGHT = 2;
const int TOP = 3;
} // namespace neighbour

enum class cell_type {
    FLUID,
    BOUNDARY_FLUID,
    INFLOW,
    OUTFLOW,
    ADIABATIC_WALL,
    HOT_WALL,
    COLD_WALL,
    FIXED_WALL,
    MOVING_WALL,
    DEFAULT
};
