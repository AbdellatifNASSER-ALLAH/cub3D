# Torch Texture Implementation

## Overview
This implementation adds support for torch textures in the cub3D raycaster. The feature allows map creators to place torches ('T' characters in the map) with animated textures that alternate between normal and attack states.

## Features

### 1. Torch Texture Parsing
- **Identifiers**: 
  - `TO` - Normal torch texture (required if torches exist in map)
  - `TA` - Attack/animated torch texture (optional, for animation)
- **Format**: 
  - `TO ./path/to/torch.xpm`
  - `TA ./path/to/attack_torch.xpm`
- **Location**: Must appear before the map starts, alongside other texture definitions

### 2. Validation Rules
- ✅ **Optional texture**: Torch textures are optional if there are no torches in the map
- ✅ **Required when used**: If the map contains 'T' characters, a torch texture MUST be defined
- ❌ **Error case**: If the map has torches but no torch texture is defined, parsing will fail with error: "Map contains torch but no torch texture defined"
- ✅ **Unused texture**: If torch textures are defined but no torches exist in the map, no error occurs
- ✅ **Attack texture optional**: The TA (attack torch) texture is optional. If not provided, only the normal torch texture will be displayed

### 3. Torch Placement Rules
- Torches must be placed between two walls, either:
  - Vertically: Wall above AND wall below
  - Horizontally: Wall to the left AND wall to the right
- Torches cannot touch spaces or map edges
- Torches act as solid obstacles (players cannot walk through them)

### 4. Torch Rendering
- When a ray hits a torch ('T' in the map), the torch texture is applied
- If both TO and TA textures are defined, the torch will animate by alternating between them
- Animation switches every 30 frames (approximately every 0.5 seconds at 60 FPS)
- Torches render correctly from all angles using proper texture mapping

### 5. Animation System
- The game tracks an animation frame counter (`torch_anim_frame`)
- Frame counter increments with each game loop iteration
- Texture selection: `(frame / 30) % 2` determines which texture to show
- This creates a flickering torch effect, simulating fire animation

## Code Changes

### Header File (`includes/cub3d.h`)
```c
// Added TORCH and TORCH_ATTACK to texture enum
typedef enum s_tex
{
    NORTH,
    SOUTH,
    EAST,
    WEST,
    DOOR,
    TORCH,         // NEW
    TORCH_ATTACK,  // NEW
    NB_TEX
} t_tex;

// Updated config struct
typedef struct s_config
{
    // ... existing fields ...
    int torch_found;         // NEW: tracks if TO texture was defined
    int torch_attack_found;  // NEW: tracks if TA texture was defined
    int has_torch_in_map;    // NEW: tracks if 'T' exists in map
} t_config;

// Updated game struct
typedef struct s_game
{
    // ... existing fields ...
    t_texture textures[7];   // CHANGED: was 6, now 7 to include torch_attack
    int torch_anim_frame;    // NEW: animation frame counter
} t_game;
```

### Parsing (`srcs/parse/extract_configs.c`)
- Added parsing for "TO " identifier (normal torch)
- Added parsing for "TA " identifier (attack torch)
- Updated `do_tex()` function to handle torch textures (cases n == 5 and n == 6)
- Torch textures follow same validation as other textures (.xpm extension check)

### Map Validation (`srcs/parse/map.c`)
- When 'T' is encountered, sets `cfg->has_torch_in_map = 1`
- Validates torch placement (must be between walls vertically or horizontally)
- Torches cannot touch spaces or map edges

### Parse Flow (`srcs/parse/parse.c`)
- Added validation after `fill_map()`:
  ```c
  if (cfg->has_torch_in_map && !cfg->torch_found)
      exit_err("Map contains torch but no torch texture defined", 1, cfg);
  ```
- Initializes torch-related config fields to 0

### Texture Loading (`srcs/execution/texture.c`)
- `load_textures()`: Conditionally loads torch textures if found
- `get_texture_color()`: 
  - Checks if hit position is a torch
  - If both textures exist, alternates based on animation frame
  - Uses torch texture when hit position is 'T':
  ```c
  if (game->map[r->wally][r->wallx] == 'T' && game->config.torch_found)
  {
      if (game->config.torch_attack_found && (game->torch_anim_frame / 30) % 2)
          tex_index = TORCH_ATTACK;
      else
          tex_index = TORCH;
  }
  ```

### Raycasting (`srcs/execution/dda.c`)
- `perform_dda()`: Treats 'T' as a wall hit (stops ray)
- Torches are detected alongside '1' (walls) and 'D' (doors)

### Collision Detection (`srcs/main.c`, `srcs/execution/mini_map.c`)
- `touch()`: Updated to treat torches as solid obstacles
- `touch2()`: Updated minimap collision detection for torches
- Players cannot walk through torches

### Rendering (`srcs/execution/draw_vision.c`)
- `select_color()`: Added fallback color for torches (orange: 0xFFA500)
- Used when texture rendering is not available

### Game Loop (`srcs/main.c`)
- `init_game()`: Initializes `torch_anim_frame` to 0
- `draw_loop()`: Increments `torch_anim_frame` each frame for animation

## Configuration File Format

### Example with Animated Torch
```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm
TO ./textures/torch.xpm
TA ./textures/attack_torch.xpm

F 220,100,0
C 225,30,0

1111111111111111111111111
1000000000000000000000001
10000000001T0000000N00001
1000000000000000000000001
1111111111111111111111111
```

### Example with Static Torch (No Animation)
```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm
TO ./textures/torch.xpm

F 220,100,0
C 225,30,0

1111111111111111111111111
1000000000000000000000001
10000000001T0000000N00001
1000000000000000000000001
1111111111111111111111111
```

### Example with Multiple Torches and Door
```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm
DO ./textures/door.xpm
TO ./textures/torch.xpm
TA ./textures/attack_torch.xpm

F 220,100,0
C 225,30,0

1111111111111111111111111
1000000000000000000000001
10T000000001D00000T0N00001
1000000000000000000000001
1000000T00000000000T00001
1111111111111111111111111
```

## Test Maps

Test map files have been created in the `maps/` directory:

1. **`torch_test.cub`**: Simple map with one animated torch
2. **`torch_complex.cub`**: Complex map with multiple torches, doors, and animation

## Testing

### Manual Testing
Run the game with different configurations:

```bash
# Animated torch
./game maps/torch_test.cub

# Multiple torches with doors
./game maps/torch_complex.cub
```

### Expected Behaviors
1. ✅ Torch texture is rendered when ray hits 'T' in map
2. ✅ Torches animate between normal and attack textures when both are defined
3. ✅ Animation switches smoothly every ~0.5 seconds
4. ✅ Error message shown when torch exists but no texture defined
5. ✅ No error when torch textures defined but not used
6. ✅ Static torch display when only TO texture is provided
7. ✅ Torches act as solid walls (collision detection)
8. ✅ Game works normally with maps without torches

## Implementation Notes

- **Minimal changes**: Only essential modifications were made to existing code
- **Backward compatible**: Existing maps without torches work without any changes
- **Consistent**: Torch textures follow same pattern as other textures (NO, SO, EA, WE, DO)
- **Safe**: Validation prevents runtime errors from missing torch textures
- **Flexible**: Torch textures are optional when not needed
- **Animated**: Optional attack texture creates a flickering effect
- **Performance**: Animation is frame-based, not time-based (simple and efficient)

## Future Enhancements

Possible improvements for future versions:
- Interactive torches (toggle on/off with key press)
- More animation frames for smoother torch flickering
- Torch lighting effects (illuminating nearby walls)
- Different torch types (colors, intensity levels)
- Sound effects for torch crackling
- Torch can be picked up as an item
- Torches that can be placed/removed dynamically
- Brightness/fog effects based on torch proximity
