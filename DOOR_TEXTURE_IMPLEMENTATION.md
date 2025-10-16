# Door Texture Implementation

## Overview
This implementation adds support for door textures in the cub3D raycaster. The feature allows map creators to define a texture for doors ('D' characters in the map) and handles validation to ensure consistency between the map and configuration.

## Features

### 1. Door Texture Parsing
- **Identifier**: `DO` (similar to `NO`, `SO`, `EA`, `WE`)
- **Format**: `DO ./path/to/texture.xpm`
- **Location**: Must appear before the map starts, alongside other texture definitions

### 2. Validation Rules
- ✅ **Optional texture**: Door texture is optional if there are no doors in the map
- ✅ **Required when used**: If the map contains 'D' characters, a door texture MUST be defined
- ❌ **Error case**: If the map has doors but no door texture is defined, parsing will fail with error: "Map contains door but no door texture defined"
- ✅ **Unused texture**: If door texture is defined but no doors exist in the map, no error occurs

### 3. Door Rendering
- When a ray hits a door ('D' in the map), the door texture is applied
- Door texture is loaded and rendered the same way as wall textures
- Proper texture mapping ensures doors look correct from all angles

## Code Changes

### Header File (`includes/cub3d.h`)
```c
// Added DOOR to texture enum
typedef enum s_tex
{
    NORTH,
    SOUTH,
    EAST,
    WEST,
    DOOR,    // NEW
    NB_TEX
} t_tex;

// Updated config struct
typedef struct s_config
{
    // ... existing fields ...
    int door_found;           // NEW: tracks if DO texture was defined
    int has_door_in_map;      // NEW: tracks if 'D' exists in map
} t_config;

// Updated game struct
typedef struct s_game
{
    // ... existing fields ...
    t_texture textures[5];    // CHANGED: was 4, now 5 to include door
} t_game;
```

### Parsing (`srcs/parse/extract_configs.c`)
- Added parsing for "DO " identifier
- Updated `do_tex()` function to handle door texture (case n == 4)
- Door texture follows same validation as other textures (.xpm extension check)

### Map Validation (`srcs/parse/map.c`)
- When 'D' is encountered, sets `cfg->has_door_in_map = 1`
- Maintains existing door validation (must be between walls)

### Parse Flow (`srcs/parse/parse.c`)
- Added validation after `fill_map()`:
  ```c
  if (cfg->has_door_in_map && !cfg->door_found)
      exit_err("Map contains door but no door texture defined", 1, cfg);
  ```

### Texture Loading (`srcs/execution/texture.c`)
- `load_textures()`: Conditionally loads door texture if `door_found` is true
- `get_texture_color()`: Checks if hit position is a door and uses door texture:
  ```c
  if (game->map[r->wally][r->wallx] == 'D' && game->config.door_found)
      tex_index = DOOR;
  ```

## Configuration File Format

### Example with Door Texture
```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm
DO ./textures/door.xpm

F 220,100,0
C 225,30,0

1111111111111111111111111
1000000000000000000000001
10000000001D0000000N00001
1000000000000000000000001
1111111111111111111111111
```

### Example without Door Texture (valid if no doors in map)
```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm

F 220,100,0
C 225,30,0

1111111111111111111111111
1000000000000000000000001
1000000000000000000N00001
1000000000000000000000001
1111111111111111111111111
```

## Test Maps

Three test map files have been created in the `maps/` directory:

1. **`door_test.cub`**: Valid map with door texture and door in map
2. **`door_no_texture.cub`**: Invalid map with door in map but no texture (should fail)
3. **`texture_no_door.cub`**: Valid map with door texture but no doors (should work)

## Testing

### Manual Testing
Run the game with different configurations:

```bash
# Should work - door texture defined and door in map
./game maps/door_test.cub

# Should fail with error message
./game maps/door_no_texture.cub

# Should work - door texture defined but not used
./game maps/texture_no_door.cub

# Should work - no door texture and no doors
./game maps/simple.cub
```

### Expected Behaviors
1. ✅ Door texture is rendered when ray hits 'D' in map
2. ✅ Error message shown when door exists but no texture defined
3. ✅ No error when door texture defined but not used
4. ✅ Game works normally with maps without doors

## Implementation Notes

- **Minimal changes**: Only essential modifications were made to existing code
- **Backward compatible**: Existing maps without doors work without any changes
- **Consistent**: Door texture follows same pattern as other textures (NO, SO, EA, WE)
- **Safe**: Validation prevents runtime errors from missing door textures
- **Flexible**: Door texture is optional when not needed

## Future Enhancements

Possible improvements for future versions:
- Interactive doors (open/close with key press)
- Animated door textures
- Different door types (locked, sliding, etc.)
- Sound effects for doors
