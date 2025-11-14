# cub3D - Complete Technical Documentation

## Table of Contents
1. [Project Overview](#project-overview)
2. [Architecture Overview](#architecture-overview)
3. [Program Flow: From Main to End](#program-flow-from-main-to-end)
4. [The DDA Algorithm Explained](#the-dda-algorithm-explained)
5. [Texture Implementation Based on DDA](#texture-implementation-based-on-dda)
6. [Key Data Structures](#key-data-structures)

---

## Project Overview

cub3D is a raycasting 3D engine inspired by Wolfenstein 3D, built using the MiniLibX graphics library. The project demonstrates fundamental 3D rendering techniques using 2D raycasting, creating a pseudo-3D environment from a 2D map.

**Core Technologies:**
- **Raycasting**: Casting rays from the player's position to determine visible walls
- **DDA Algorithm**: Digital Differential Analyzer for efficient ray-grid intersection
- **Texture Mapping**: Applying 2D textures to walls based on ray collision points
- **MiniLibX**: X11-based graphics library for rendering

---

## Architecture Overview

The cub3D engine follows a modular architecture with clear separation of concerns:

```
┌─────────────────────────────────────────────────────────────────┐
│                        MAIN PROGRAM                              │
│                     (srcs/main.c)                                │
└────────────────┬────────────────────────────────────────────────┘
                 │
        ┌────────┴──────────┐
        │                   │
        ▼                   ▼
┌──────────────┐    ┌──────────────────┐
│   PARSING    │    │  INITIALIZATION  │
│   MODULE     │    │     MODULE       │
├──────────────┤    ├──────────────────┤
│ • parse.c    │───▶│ • init_game()    │
│ • read_file  │    │ • load_textures()│
│ • valid_file │    │ • init_player()  │
│ • map.c      │    └────────┬─────────┘
└──────────────┘             │
                             │
                             ▼
                    ┌─────────────────┐
                    │   GAME LOOP     │
                    │  (draw_loop)    │
                    └────────┬────────┘
                             │
            ┌────────────────┼────────────────┐
            │                │                │
            ▼                ▼                ▼
    ┌──────────────┐  ┌──────────────┐  ┌──────────────┐
    │    INPUT     │  │   RAYCASTING │  │   RENDERING  │
    │   HANDLING   │  │     ENGINE   │  │    ENGINE    │
    ├──────────────┤  ├──────────────┤  ├──────────────┤
    │• hooks.c     │  │• draw_vision │  │• pixel_draw  │
    │• player.c    │  │• dda.c       │  │• texture.c   │
    │• move_player │  │• init_ray    │  │• draw_stripe │
    └──────────────┘  │• perform_dda │  │• minimap.c   │
                      └──────────────┘  └──────────────┘
```

**Key Components:**

1. **Parsing Module** (`srcs/parse/`): Validates and loads `.cub` configuration files
2. **Execution Module** (`srcs/execution/`): Core rendering and raycasting logic
3. **Input Module** (`srcs/hooks.c`, `srcs/player.c`): Handles keyboard and mouse input
4. **Utilities** (`utils/`): Helper functions for memory management and error handling

**Data Flow:**

```
.cub file → Parser → Config → Game Init → Textures Loaded
                                    ↓
                            ┌──────────────┐
                            │  Game Loop   │
                            └──────┬───────┘
                                   │
    Input Events →  Player State → │ → Raycasting (DDA) → Texture Sampling → Screen
                                   │
                            Render Buffer → Display
```

---

## Program Flow: From Main to End

### 1. Program Initialization (`main()`)

The program starts in `srcs/main.c`:

```
main(ac, av)
    │
    ├─> Validate arguments (requires exactly 1 map file)
    │
    ├─> parse(&game.config, av[1])
    │   │
    │   ├─> init_config()           // Initialize configuration structure
    │   ├─> valid_file()            // Validate .cub file format
    │   ├─> read_file()             // Read all lines from file
    │   ├─> extract_configs()       // Parse textures (NO, SO, EA, WE), colors (F, C)
    │   └─> fill_map()              // Extract and validate the 2D map
    │
    ├─> init_game(&game)
    │   │
    │   ├─> mlx_init()              // Initialize MiniLibX
    │   ├─> load_textures()         // Load XPM texture files into memory
    │   │   └─> For each direction (NORTH, SOUTH, EAST, WEST, TORCH, DOOR)
    │   │       └─> mlx_xpm_file_to_image()
    │   │
    │   ├─> get_player_cord()       // Find player position (N/S/E/W) in map
    │   ├─> init_player()           // Initialize player state (keys, speed, z_eye)
    │   ├─> mlx_new_window()        // Create game window (1280x920)
    │   └─> mlx_new_image()         // Create image buffer for rendering
    │
    └─> Setup event hooks and start game loop
        │
        ├─> mlx_hook(KeyPress)      // key_press()
        ├─> mlx_hook(KeyRelease)    // key_release()
        ├─> mlx_hook(MouseMove)     // mouse_move()
        ├─> mlx_hook(MouseButton)   // mouse_button()
        ├─> mlx_hook(DestroyNotify) // handle_exit()
        ├─> mlx_loop_hook()         // draw_loop() - called every frame
        └─> mlx_loop()              // Start infinite event loop
```

### 2. Game Loop (`draw_loop()`)

Called continuously by MiniLibX, this is the heart of the rendering system:

```
draw_loop(game)
    │
    ├─> move_player(game)
    │   │
    │   ├─> apply_rotation()        // Handle angle changes (keyboard/mouse)
    │   ├─> calc_move()             // Calculate new position based on keys
    │   └─> Collision detection     // Check against walls (is_blocked())
    │
    ├─> draw_vision(game)           // *** MAIN RAYCASTING ROUTINE ***
    │   │
    │   │   For each screen column (x = 0 to WIDTH):
    │   │   
    │   ├─> init_ray(&r, player, x)
    │   │   │
    │   │   ├─> Calculate ray angle: player->angle - (FOV/2) + (x * FOV/WIDTH)
    │   │   ├─> Calculate ray direction: (cos(angle), sin(angle))
    │   │   ├─> Initialize starting map position
    │   │   └─> Calculate DDA step values (delta_dist, side_dist)
    │   │
    │   ├─> perform_dda(&r, game)   // *** DDA ALGORITHM (see section below) ***
    │   │   └─> Returns: wall hit position, side hit
    │   │
    │   ├─> calc_dist_and_height(&r, player)
    │   │   │
    │   │   ├─> Calculate perpendicular distance to wall
    │   │   ├─> Apply fisheye correction: dist * cos(ray_angle - player_angle)
    │   │   ├─> Calculate wall height: (BLOCK / dist) * (WIDTH / 2)
    │   │   └─> Calculate screen Y range: start_y, end_y
    │   │
    │   └─> draw_stripe(x, &r, game)
    │       │
    │       ├─> Draw ceiling (y < start_y)
    │       ├─> Draw textured wall (start_y <= y < end_y)
    │       │   │
    │       │   ├─> get_texture_color(r, tex_y, game)  // *** TEXTURE MAPPING ***
    │       │   │   │
    │       │   │   ├─> Select texture based on ray side and direction
    │       │   │   ├─> Calculate X position on texture (wall_x)
    │       │   │   └─> Sample texture pixel at (tex_x, tex_y)
    │       │   │
    │       │   └─> put_pixel(x, y, color, game)
    │       │
    │       └─> Draw floor (y >= end_y)
    │
    ├─> draw_torch(game)            // Draw weapon sprite
    ├─> draw_aim(game)              // Draw crosshair
    ├─> draw_minimap(game)          // Draw minimap overlay
    ├─> draw_map(game)              // Draw full map (if enabled)
    │
    └─> mlx_put_image_to_window()   // Display rendered frame
```

### 3. Input Handling

The program responds to user input through event hooks:

**Keyboard (`key_press()` / `key_release()`):**
- W/A/S/D: Set movement flags
- Arrow keys: Set rotation flags
- ESC: Exit game

**Mouse (`mouse_move()`):**
- Calculate delta from screen center
- Update player angle (horizontal look)
- Update z_eye (vertical look / head tilt)
- Recenter mouse to screen center

**Mouse Button (`mouse_button()`):**
- Right-click: Trigger attack animation

### 4. Program Termination

When the user closes the window or presses ESC:

```
handle_exit(game)
    │
    ├─> free_game(game)
    │   └─> Free all allocated memory (textures, config, map)
    │
    └─> exit(0)
```

---

## The DDA Algorithm Explained

**DDA (Digital Differential Analyzer)** is an efficient grid traversal algorithm used to find which wall a ray hits in a grid-based map.

### Why DDA?

In a raycasting engine, we need to:
1. Cast a ray from the player's position
2. Find the first wall it hits in a 2D grid map
3. Do this efficiently (thousands of rays per frame)

DDA solves this by stepping through the grid one square at a time, checking only the squares the ray passes through.

### The Algorithm Step-by-Step

#### Step 1: Initialize Ray Direction (`init_ray_dir_and_map()`)

```c
// Calculate ray angle for this screen column
ray_angle = player->angle - (FOV / 2) + (x * (FOV / WIDTH))

// Convert to direction vector
ray_dirx = cos(ray_angle)
ray_diry = sin(ray_angle)

// Starting grid position
mapx = (int)(player->x / BLOCK)
mapy = (int)(player->y / BLOCK)
```

**Purpose**: Determine which direction the ray is pointing and which map square it starts in.

#### Step 2: Calculate Delta Distances (`init_ray_delta()`)

```c
// How far the ray travels to cross one grid square in X
if (ray_dirx == 0)
    delta_distx = 1e30  // Infinite (ray is vertical)
else
    delta_distx = fabs(1.0 / ray_dirx)

// How far the ray travels to cross one grid square in Y
if (ray_diry == 0)
    delta_disty = 1e30  // Infinite (ray is horizontal)
else
    delta_disty = fabs(1.0 / ray_diry)
```

**Purpose**: Pre-calculate how much distance the ray travels when crossing one grid line.

**Visualization:**
```
If ray_dirx = 0.5, then delta_distx = 2.0
This means: to cross 1 grid square horizontally, 
the ray travels 2 units along its direction vector.
```

#### Step 3: Calculate Initial Side Distances and Step Direction (`init_ray_steps()`)

```c
if (ray_dirx < 0) {
    stepx = -1              // Move left in grid
    // Distance from player position to first left grid line
    side_distx = (px - mapx) * delta_distx
}
else {
    stepx = 1               // Move right in grid
    // Distance from player position to first right grid line
    side_distx = (mapx + 1.0 - px) * delta_distx
}

// Same logic for Y direction
```

**Purpose**: 
- Determine which direction to step in the grid (+1 or -1)
- Calculate initial distance to the first grid line in each direction

**Visualization:**
```
Player at (2.3, 4.7) looking right (ray_dirx > 0):
    
    Grid:  |2.0|   |3.0|   |4.0|
           
    Player: ──────P──>
                2.3  3.0
                
    Distance to next vertical grid line = 3.0 - 2.3 = 0.7
    side_distx = 0.7 * delta_distx
```

#### Step 4: DDA Loop (`perform_dda()`)

This is the core algorithm that finds the wall:

```c
while (!hit) {
    // Which grid line is closer?
    if (side_distx < side_disty) {
        // X grid line is closer
        side_distx += delta_distx    // Move to next X line
        mapx += stepx                 // Advance in grid
        side = 0                      // Hit vertical wall (N/S)
    }
    else {
        // Y grid line is closer
        side_disty += delta_disty    // Move to next Y line
        mapy += stepy                 // Advance in grid
        side = 1                      // Hit horizontal wall (E/W)
    }
    
    // Check if we hit a wall
    if (map[mapy][mapx] == '1' || map[mapy][mapx] == 'D') {
        hit = 1
        wallx = mapx
        wally = mapy
    }
}
```

**Purpose**: Efficiently traverse the grid until hitting a wall.

**Key Insight**: 
- We always step to the **nearest** grid line
- This ensures we check every grid square the ray passes through
- No gaps, no overshooting

**Visual Example:**

```
Start: Player at (1, 1), looking at angle 30°

Grid:
    0   1   2   3   4
  +---+---+---+---+---+
0 | 1 | 1 | 1 | 1 | 1 |
  +---+---+---+---+---+
1 | 1 | P | . | . | 1 |  ← Player here, ray shoots →
  +---+---+---+---+---+
2 | 1 | . | . | 1 | 1 |
  +---+---+---+---+---+
3 | 1 | 1 | 1 | 1 | 1 |
  +---+---+---+---+---+

Iteration 1:
    side_distx = 0.8, side_disty = 1.2
    side_distx < side_disty → step in X
    New position: (2, 1)
    Check map[1][2] = '.' → continue

Iteration 2:
    side_distx = 2.8, side_disty = 1.2
    side_disty < side_distx → step in Y
    New position: (2, 2)
    Check map[2][2] = '.' → continue

Iteration 3:
    side_distx = 2.8, side_disty = 3.2
    side_distx < side_disty → step in X
    New position: (3, 2)
    Check map[2][3] = '1' → HIT! side = 0
```

#### Step 5: Calculate Wall Distance

After DDA finds the wall, calculate the perpendicular distance:

```c
if (side == 0) {
    // Hit vertical wall
    perp_wall_dist = (mapx - px + (1 - stepx) / 2) / ray_dirx
}
else {
    // Hit horizontal wall
    perp_wall_dist = (mapy - py + (1 - stepy) / 2) / ray_diry
}

// Apply fisheye correction
dist = perp_wall_dist * BLOCK * cos(ray_angle - player->angle)
```

**Purpose**: 
- Get the perpendicular distance (not the actual ray length)
- This prevents the "fisheye" distortion effect
- Closer walls appear taller, distant walls appear shorter

### DDA Advantages

1. **Efficient**: Only checks grid squares the ray passes through
2. **Fast**: Integer operations and simple comparisons
3. **Accurate**: Never misses a wall
4. **Scalable**: Performance is O(distance to wall), not O(map size)

---

## Texture Implementation Based on DDA

After DDA finds a wall, we need to:
1. Select the correct texture (North, South, East, or West wall)
2. Find the exact X coordinate on the texture
3. Sample texture pixels for each Y coordinate on screen

### Step 1: Texture Selection (`get_texture_index()`)

The texture depends on which side of the wall was hit:

```c
if (map[wally][wallx] == 'D' && door_texture_exists) {
    tex_index = DOOR         // Special door texture
}
else if (side == 0) {        // Vertical wall (North/South)
    if (ray_dirx > 0)
        tex_index = EAST     // Hit from left, looking right
    else
        tex_index = WEST     // Hit from right, looking left
}
else {                       // Horizontal wall (East/West)
    if (ray_diry > 0)
        tex_index = SOUTH    // Hit from top, looking down
    else
        tex_index = NORTH    // Hit from bottom, looking up
}
```

**Why check ray direction?**
- The side tells us if it's a vertical or horizontal wall
- The ray direction tells us which face of that wall we're looking at

**Visualization:**
```
    North texture
         ↓
    +-------+
    |   1   |
West|       |East  ← Player looking this way hits EAST texture
    |   1   |
    +-------+
         ↑
    South texture
```

### Step 2: Calculate Texture X Coordinate (`calculate_tex_x()`)

We need to find where on the texture the ray hit:

```c
// Find the exact wall hit position (fractional part)
if (side == 0) {
    // Vertical wall: use Y coordinate of intersection
    wall_x = py + perp_wall_dist * ray_diry
}
else {
    // Horizontal wall: use X coordinate of intersection
    wall_x = px + perp_wall_dist * ray_dirx
}

// Get fractional part (0.0 to 1.0)
wall_x -= (int)wall_x

// Map to texture width
tex_x = (int)(wall_x * texture_width)

// Flip texture if needed for correct orientation
if ((side == 0 && ray_dirx < 0) || (side == 1 && ray_diry > 0))
    tex_x = texture_width - tex_x - 1
```

**Example:**
```
Wall segment from grid (2, 3) to (3, 3), texture is 64 pixels wide

Ray hits at world position (2.75, 3.0)
wall_x = 0.75 (75% along the wall)
tex_x = 0.75 * 64 = 48

So we sample column 48 from the texture.
```

### Step 3: Draw Textured Stripe (`draw_stripe()`)

For each pixel in the vertical column:

```c
// Calculate texture Y coordinate
step = (float)TEXTURE_HEIGHT / wall_height

for (y = start_y; y < end_y && y < HEIGHT; y++) {
    // Map screen Y to texture Y
    tex_y = (int)((y - start_y) * step)
    
    // Clamp to valid range
    if (tex_y < 0) tex_y = 0
    if (tex_y >= TEXTURE_HEIGHT) tex_y = TEXTURE_HEIGHT - 1
    
    // Get color from texture
    color = texture_data[tex_y * texture_width + tex_x]
    
    // Draw pixel
    put_pixel(x, y, color, game)
}
```

**How texture mapping works:**

```
Screen column (x = 640):

    y=0   ─────────────────  Ceiling (solid color)
    
    y=200 ─────────────────  ← start_y
          │               │
          │   Textured    │  Wall height = 400 pixels
          │     Wall      │  Texture height = 64 pixels
          │               │  step = 64 / 400 = 0.16
    y=600 ─────────────────  ← end_y
    
    y=920 ─────────────────  Floor (solid color)

Mapping:
    Screen Y | Offset | tex_y calculation      | Texture Y
    ---------|--------|------------------------|----------
    200      | 0      | (200-200) * 0.16 = 0   | 0
    300      | 100    | (300-200) * 0.16 = 16  | 16
    400      | 200    | (400-200) * 0.16 = 32  | 32
    500      | 300    | (500-200) * 0.16 = 48  | 48
    600      | 400    | (600-200) * 0.16 = 64  | 63 (clamped)
```

### Complete Texture Mapping Pipeline

```
DDA Result → wall position, wall side
    ↓
Select Texture → NORTH / SOUTH / EAST / WEST
    ↓
Calculate tex_x → which column of texture (0-63)
    ↓
For each screen Y:
    Calculate tex_y → which row of texture (0-63)
    Sample pixel → texture_data[tex_y * width + tex_x]
    Draw pixel → put_pixel(x, y, color)
```

### Why This Works

1. **DDA provides accurate hit position**: We know exactly where the ray intersected the wall
2. **Perpendicular distance prevents distortion**: Walls appear flat, not curved
3. **Texture coordinates are normalized**: Work with any texture size
4. **Sampling is efficient**: Direct array access to texture data

### Performance Considerations

- Textures are pre-loaded into memory as integer arrays
- No file I/O during rendering
- Simple integer arithmetic for texture coordinates
- Each column drawn independently (can be parallelized)

---

## Key Data Structures

### `t_game` - Main Game State
```c
typedef struct s_game {
    void        *mlx;           // MiniLibX instance
    void        *win;           // Window handle
    void        *img;           // Image buffer
    char        *data;          // Image data pointer
    char        **map;          // 2D map array
    t_texture   textures[7];    // Texture array (N,S,E,W,torch,attack,door)
    t_player    player;         // Player state
    t_config    config;         // Configuration (from .cub file)
} t_game;
```

### `t_player` - Player State
```c
typedef struct s_player {
    float   x, y;               // Position in world coordinates
    float   z_eye;              // Vertical look (0.0-1.0, 0.5 = horizon)
    float   angle;              // Viewing angle in radians
    int     speed;              // Movement speed
    bool    key_up, key_down;   // WASD state
    bool    key_left, key_right;
    bool    left_rotate, right_rotate;  // Arrow key state
    bool    up_rotate, down_rotate;
} t_player;
```

### `t_ray` - Ray Information
```c
typedef struct s_ray {
    float   ray_angle;          // Ray angle
    float   ray_dirx, ray_diry; // Ray direction vector
    int     mapx, mapy;         // Current grid position
    int     side;               // 0 = vertical, 1 = horizontal wall
    int     wallx, wally;       // Wall grid coordinates
    int     stepx, stepy;       // DDA step direction (-1 or +1)
    float   perp_wall_dist;     // Perpendicular distance to wall
    float   dist;               // Corrected distance (with fisheye fix)
    float   wall_height;        // Calculated wall height in pixels
    int     start_y, end_y;     // Screen Y range for wall
    float   delta_distx, delta_disty;   // DDA delta values
    float   side_distx, side_disty;     // DDA side distances
    int     hit;                // Wall hit flag
} t_ray;
```

### `t_texture` - Texture Data
```c
typedef struct s_texture {
    void    *img;               // MiniLibX image pointer
    int     width, height;      // Texture dimensions
    int     *data;              // Pixel data (ARGB integers)
} t_texture;
```

---

## Quick Reference Guide

### Most Important Functions

| Function | Location | Purpose |
|----------|----------|---------|
| `main()` | `srcs/main.c` | Entry point, orchestrates initialization and game loop |
| `parse()` | `srcs/parse/parse.c` | Loads and validates .cub configuration file |
| `init_game()` | `srcs/main.c` | Initializes MiniLibX, loads textures, sets up window |
| `draw_loop()` | `srcs/main.c` | Main game loop, called every frame |
| `draw_vision()` | `srcs/execution/draw_vision.c` | Raycasting engine, renders 3D view |
| `init_ray()` | `srcs/execution/dda.c` | Initializes ray for DDA algorithm |
| `perform_dda()` | `srcs/execution/dda.c` | DDA algorithm, finds wall intersections |
| `get_texture_color()` | `srcs/execution/texture.c` | Samples texture pixel for rendering |
| `move_player()` | `srcs/player.c` | Updates player position with collision detection |
| `key_press()` / `key_release()` | `srcs/execution/hooks.c` | Handles keyboard input |
| `mouse_move()` | `srcs/execution/hooks.c` | Handles mouse movement for camera control |

### Key Constants

| Constant | Value | Purpose |
|----------|-------|---------|
| `WIDTH` | 1280 | Screen width in pixels |
| `HEIGHT` | 920 | Screen height in pixels |
| `BLOCK` | 64 | Size of one map grid square |
| `TEXTURE_WIDTH` | 64 | Width of texture images |
| `TEXTURE_HEIGHT` | 64 | Height of texture images |
| `FOV` | 1.047 (~60°) | Field of view in radians |
| `PI` | 3.14159... | Pi constant |

### Texture Indices

| Index | Name | Wall Face |
|-------|------|-----------|
| 0 | `NORTH` | Top-facing wall |
| 1 | `SOUTH` | Bottom-facing wall |
| 2 | `EAST` | Right-facing wall |
| 3 | `WEST` | Left-facing wall |
| 4 | `TORCH` | Weapon sprite (idle) |
| 5 | `TORCH_ATTACK` | Weapon sprite (attacking) |
| 6 | `DOOR` | Door texture (optional) |

### File Organization

```
cub3D/
├── srcs/
│   ├── main.c                 # Entry point, game loop
│   ├── player.c               # Player movement and collision
│   ├── parse/                 # Configuration parsing
│   │   ├── parse.c
│   │   ├── read_file.c
│   │   ├── valid_file.c
│   │   ├── extract_configs.c
│   │   ├── map.c
│   │   └── utils.c
│   └── execution/             # Core rendering engine
│       ├── draw_vision.c      # Main raycasting loop
│       ├── dda.c              # DDA algorithm
│       ├── texture.c          # Texture sampling
│       ├── pixel_draw.c       # Pixel manipulation
│       ├── mini_map.c         # Minimap rendering
│       ├── hooks.c            # Input handling
│       └── hooks2.c           # Additional input handlers
├── includes/
│   └── cub3d.h               # All data structures and prototypes
├── textures/                  # XPM texture files
├── maps/                      # .cub map files
├── libft/                     # Custom C library (submodule)
└── utils/
    └── handle_exit.c         # Cleanup and exit

```

---

## Summary

The cub3D engine demonstrates classic raycasting:

1. **Parse configuration** and load resources (textures, map)
2. **Game loop** continuously:
   - Updates player position based on input
   - Casts rays for each screen column
   - Uses **DDA** to find wall intersections efficiently
   - Applies **texture mapping** based on DDA results
   - Renders ceiling, textured walls, and floor
   - Displays overlays (weapon, minimap, crosshair)
3. **DDA algorithm** efficiently traverses the grid to find walls
4. **Texture mapping** uses the DDA hit information to:
   - Select the correct texture
   - Calculate texture coordinates
   - Sample and draw textured pixels

This architecture is modular, efficient, and serves as an excellent foundation for learning 3D graphics programming and game engine development.

### Learning Path

For developers new to raycasting, we recommend studying the code in this order:

1. **Start with data structures** (`includes/cub3d.h`) - Understand how data is organized
2. **Study the DDA algorithm** (`srcs/execution/dda.c`) - Core of raycasting
3. **Examine texture mapping** (`srcs/execution/texture.c`) - How walls get textured
4. **Review the rendering pipeline** (`srcs/execution/draw_vision.c`) - How it all comes together
5. **Explore input handling** (`srcs/execution/hooks.c`, `srcs/player.c`) - User interaction
6. **Experiment with modifications** - Add features, change textures, modify FOV

This project provides a solid foundation for understanding game engines, 3D graphics, and real-time rendering techniques.
