# Animated Door Textures - Implementation Guide

## Overview
This implementation adds support for multi-frame animated door textures in cub3D. Doors can now open with smooth animations when the player interacts with them using the 'E' key.

## Features

### 1. Multi-Frame Door Textures
- **Frame Naming Convention**: Door texture frames should follow the pattern:
  ```
  base_name_XX.xpm
  ```
  where XX is a two-digit frame number (00, 01, 02, etc.)

- **Example**:
  ```
  textures/doors/door_open_00.xpm  (Closed door)
  textures/doors/door_open_01.xpm  (Opening - 25%)
  textures/doors/door_open_02.xpm  (Opening - 50%)
  textures/doors/door_open_03.xpm  (Opening - 75%)
  textures/doors/door_open_04.xpm  (Fully open)
  ```

### 2. Configuration File Format

#### Door Texture Path
Specify the base path (without frame number and extension):
```
DO ./textures/doors/door_open
```
The parser will automatically search for frames: `door_open_00.xpm`, `door_open_01.xpm`, etc.

#### Animation Speed (Optional)
Set the door animation speed with the `DA` (Door Animation) parameter:
```
DA 0.05
```
- Valid range: 0.0 to 1.0
- Default: 0.1 (10% per frame)
- Lower values = slower animation
- Higher values = faster animation

#### Complete Example Configuration
```
NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm
DO ./textures/doors/door_open
DA 0.05

F 220,100,0
C 225,30,0

111111111
100000001
1000D0001
100N00001
111111111
```

### 3. Door States

Doors have three states:
1. **CLOSED** - Door is closed and blocks movement
2. **OPENING** - Door is animating (still blocks movement until fully open)
3. **OPEN** - Door is fully open and passable

### 4. Player Interaction

- **Interact Key**: Press `E` to interact with doors
- **Range**: Player must be within approximately 1.5 blocks of the door
- **Direction**: Player must be facing the door
- **Behavior**: Door starts opening animation when activated

### 5. Collision System

- Closed doors block player movement
- Opening doors remain blocking until the animation completes
- Fully open doors are passable
- Once opened, doors stay open (no closing mechanism yet)

## Implementation Details

### Data Structures

#### Door State Tracking
```c
typedef enum e_door_state
{
    DOOR_CLOSED,
    DOOR_OPENING,
    DOOR_OPEN
} t_door_state;

typedef struct s_door
{
    int             x;              // Map X coordinate
    int             y;              // Map Y coordinate
    t_door_state    state;          // Current state
    float           anim_progress;  // 0.0 to 1.0
    int             current_frame;  // Current texture frame
} t_door;
```

#### Texture Management
```c
typedef struct s_texture
{
    void    *img;                           // Current frame image
    int     width;
    int     height;
    int     *data;                          // Current frame data
    int     frame_count;                    // Number of frames
    void    *frames[MAX_DOOR_FRAMES];       // All frame images
    int     *frame_data[MAX_DOOR_FRAMES];   // All frame data
} t_texture;
```

### Animation System

The animation system works as follows:

1. **Frame Detection**: On texture loading, the system automatically detects available frames
2. **State Updates**: Each game loop updates door animation progress
3. **Frame Selection**: Current frame is calculated based on animation progress
4. **Rendering**: Raycaster uses the current frame when drawing doors

### Key Functions

- `load_door_textures()`: Loads all door frame textures
- `find_and_init_doors()`: Scans map and creates door instances
- `update_door_animations()`: Updates animation progress each frame
- `interact_with_door()`: Handles player interaction
- `is_door_passable()`: Checks if door allows passage

## Sample Assets

Five sample door frames are provided in `textures/doors/`:
- `door_open_00.xpm` - Fully closed (brown door)
- `door_open_01.xpm` - 25% open (slight gap visible)
- `door_open_02.xpm` - 50% open (half open)
- `door_open_03.xpm` - 75% open (mostly open)
- `door_open_04.xpm` - Fully open (completely white/transparent)

## Test Map

A test map is provided in `maps/door_animated_test.cub` featuring:
- Multiple doors at different locations
- Pre-configured animation speed
- Demonstrates the animated door system

## Usage

1. **Compile**: 
   ```bash
   make
   ```

2. **Run with animated doors**:
   ```bash
   ./game maps/door_animated_test.cub
   ```

3. **Controls**:
   - `W/A/S/D` - Move
   - `Arrow Keys` - Rotate view
   - `E` - Interact with door (open)
   - `ESC` - Exit

## Technical Notes

### Frame Loading
- System attempts to load frames from `_00` to `_09` (MAX_DOOR_FRAMES = 10)
- Loading stops when a frame file is not found
- At least 1 frame is required (frame 00 or the base path)
- If only a single texture is provided, it works as a static door

### Animation Easing
- Current implementation uses linear interpolation
- Frame index is calculated as: `frame = progress * (frame_count - 1)`
- Progress increments by `door_anim_speed` per game loop

### Performance
- All door frames are loaded into memory at startup
- No runtime file I/O during animation
- Minimal overhead for door state tracking

## Future Enhancements

Possible improvements:
- Door closing animations (reverse the opening sequence)
- Different door types (sliding, rotating, locked)
- Sound effects for door opening/closing
- Easing functions (ease-in, ease-out) for smoother animation
- Per-door configuration (different speeds for different doors)
- Key-based locked doors

## Compatibility

- Backward compatible with existing maps that use static door textures
- Maps without doors work without any changes
- Single-frame door textures still function normally

## Troubleshooting

### Door frames not loading
- Check frame naming: must follow `basename_XX.xpm` pattern
- Ensure frame numbers are zero-padded (00, 01, not 0, 1)
- Verify all frame files exist in the specified path

### Animation not smooth
- Adjust DA parameter (try values between 0.02 and 0.2)
- Ensure game is running at consistent frame rate

### Door not opening
- Verify you're pressing 'E' key
- Check you're close enough to the door (within 1.5 blocks)
- Ensure you're facing the door

### Collision issues
- Make sure door animation completes fully
- Check that door state transitions to OPEN
- Verify map has 'D' characters at door locations
