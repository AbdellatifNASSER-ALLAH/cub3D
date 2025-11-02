# cub3D

A simple, modular raycasting 3D engine using MiniLibX, inspired by Wolfenstein 3D.

## Features

- 3D rendering with wall textures (XPM format)
- Player movement and rotation (keyboard & mouse)
- Minimap display with field of view
- Configurable map and textures
- Designed for easy extension and experimentation

## File Structure

- **srcs/main.c**: Main game loop, rendering, and utility functions. Handles initialization, raycasting, drawing, and game logic.
- **srcs/execution/hooks.c**: Input handling (keyboard and mouse). Manages movement, rotation, and player controls.
- **srcs/player.c**: Player movement and rotation logic. Handles collision detection and movement calculations.
- **includes/cub3d.h**: Data structures, constants, and function prototypes. Defines the core architecture for the game state, player, textures, and configuration.
- **textures/**: Directory for wall textures (XPM format). Textures are mapped to North, South, East, and West walls.

## Key Functions

- `init_game`: Sets up the game, loads the map, player, and textures, and creates the window and image buffer.
- `draw_vision`: Main rendering loop. Casts a ray for each screen column, calculates wall hit, distance, and draws the 3D scene using textures.
- `dda_cast_ray`: Performs the DDA (Digital Differential Analyzer) algorithm to find where a ray hits a wall.
- `get_wall_color`: Determines which texture column to use for the wall hit, based on the side and hit position.
- `draw_stripe`: Draws a vertical stripe for a wall, sampling the correct texture pixel for each y-coordinate.
- `move_player`: Applies movement and rotation to the player, with collision detection.
- `key_press`, `key_release`, `mouse_move`: Input event handlers for movement and camera control.

## Controls

- **W/A/S/D**: Move player
- **Arrow keys**: Rotate view
- **Mouse**: Look around
- **ESC**: Exit

## Torch/Flashlight Effect

The game features a centered torch (flashlight) effect that creates a circular bright area at the center of the screen while darkening the edges. This enhances immersion and creates a more atmospheric viewing experience.

### Customizing the Torch Effect

The torch effect can be easily customized by modifying constants in `srcs/execution/torch_effect.c`:

- **TORCH_ENABLED** (default: 1) - Set to 0 to completely disable the torch effect, or 1 to enable it.
- **TORCH_RADIUS_PIXELS** (default: 200) - Controls the size of the bright circle in pixels. Larger values create a wider lit area.
- **TORCH_MIN_BRIGHTNESS** (default: 0.15) - Controls how dark the edges become, ranging from 0.0 (completely black) to 1.0 (no darkening). Lower values create more dramatic contrast.

To adjust these settings, edit the values at the top of `srcs/execution/torch_effect.c` and recompile with `make`.

## Textures

Place your wall textures in the `textures/` directory as XPM files.  
Textures are mapped to North, South, East, and West walls.  
To add new wall types, update the map parser and texture loading logic.

## Build & Run

```sh
make
./cub3D [map_file]
```

## Extending the Engine

This project is designed for learning and experimentation.  
To add new features (doors, pickups, enemies, new wall types):

- Extend the map parser and rendering logic to handle new symbols and textures.
- Add new functions for interaction and rendering, and update the game loop.
- Modify input handlers and player struct for new movement styles.
- Update rendering functions for effects like shading, fog, or lighting.

## Implementation Advice

- Start simple: Get basic raycasting and movement working before adding features.
- Keep functions modular: Each function should do one thing (e.g., raycasting, drawing, input).
- Use the data structures: The `t_game`, `t_player`, and `t_texture` structs are designed for easy expansion.
- Document your code: Add comments and update this README as you add features.

---

Use this project as a foundation for your own raycasting game.  
The architecture is designed for clarity, learning, and extension.