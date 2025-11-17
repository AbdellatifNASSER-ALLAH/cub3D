# cub3D - Complete Project Roadmap

A comprehensive 3D raycasting engine built with C and MiniLibX, inspired by Wolfenstein 3D. This project demonstrates fundamental 3D rendering techniques, game architecture, and real-time graphics programming.

---

## 📋 Table of Contents

1. [Project Overview](#-project-overview)
2. [Core Concepts](#-core-concepts)
3. [Architecture](#-architecture)
4. [Installation & Setup](#-installation--setup)
5. [Project Structure](#-project-structure)
6. [Raycasting Algorithm](#-raycasting-algorithm)
7. [Parsing System](#-parsing-system)
8. [Rendering Pipeline](#-rendering-pipeline)
9. [Player System](#-player-system)
10. [Texture Management](#-texture-management)
11. [Advanced Features](#-advanced-features)
12. [Controls](#-controls)
13. [Map Configuration](#-map-configuration)
14. [Development Workflow](#-development-workflow)
15. [Troubleshooting](#-troubleshooting)
16. [Learning Resources](#-learning-resources)
17. [Future Enhancements](#-future-enhancements)

---

## 🎯 Project Overview

### What is cub3D?

cub3D is a 3D game engine that uses **raycasting** to create a pseudo-3D perspective from a 2D map. Unlike true 3D engines that use polygon rendering, raycasting projects 2D map data onto the screen to create the illusion of three dimensions. This technique was famously used in early first-person shooters like Wolfenstein 3D (1992) and Doom (1993).

### Project Goals

- **Learn Graphics Programming**: Understand how 3D rendering works at a fundamental level
- **Master Raycasting**: Implement the DDA (Digital Differential Analyzer) algorithm
- **Game Architecture**: Build a modular, extensible game engine structure
- **Performance Optimization**: Handle real-time rendering at 60+ FPS
- **Resource Management**: Load and manage textures, handle memory efficiently
- **Input Handling**: Create responsive, smooth player controls

### Key Features

- ✅ Real-time 3D raycasting rendering
- ✅ Textured walls with XPM format support
- ✅ Player movement with collision detection
- ✅ Mouse and keyboard controls
- ✅ Minimap with field of view visualization
- ✅ Configurable maps via `.cub` files
- ✅ Interactive doors system
- ✅ Torch/weapon rendering with attack animation
- ✅ Ceiling and floor color customization
- ✅ Vertical view control (look up/down)

---

## 🧠 Core Concepts

### Raycasting Fundamentals

**Raycasting** is a rendering technique that creates a 3D perspective by:

1. **Casting rays** from the player's position in multiple directions
2. **Finding intersections** between rays and walls in the 2D map
3. **Calculating distances** to determine wall heights
4. **Drawing vertical stripes** on screen based on these distances

The closer a wall, the taller it appears. The further away, the shorter. This creates the illusion of depth.

### Field of View (FOV)

The player has a **60-degree field of view** (π/3 radians). The screen is divided into vertical columns (WIDTH = 1280 pixels), and one ray is cast per column. Each ray's angle is:

```
ray_angle = player_angle - (FOV/2) + (column_index * FOV/WIDTH)
```

### DDA Algorithm

The **Digital Differential Analyzer** is a fast algorithm for ray-grid traversal. Instead of checking every point along a ray, DDA efficiently steps through grid cells:

1. Calculate the distance to the next grid line (horizontal and vertical)
2. Step to the nearest grid line
3. Check if that cell contains a wall
4. Repeat until a wall is hit

### Perspective Correction

To avoid the "fish-eye" effect, we calculate the **perpendicular wall distance** rather than the actual Euclidean distance. This is done by projecting the ray's distance onto the plane perpendicular to the player's view direction:

```
perpendicular_distance = distance * cos(ray_angle - player_angle)
```

---

## 🏗️ Architecture

### System Design

The cub3D engine follows a **modular, data-driven architecture** with clear separation of concerns:

```
┌─────────────────────────────────────────┐
│           Main Game Loop                │
│  (60 FPS via mlx_loop_hook)            │
└────────────┬────────────────────────────┘
             │
     ┌───────┴────────┐
     │                │
┌────▼─────┐    ┌────▼──────┐
│  Input   │    │ Rendering │
│ Handler  │    │  Pipeline │
└────┬─────┘    └────┬──────┘
     │               │
     ▼               ▼
┌─────────┐    ┌──────────┐
│ Player  │    │ Raycaster│
│ Update  │    │   Core   │
└─────────┘    └──────────┘
```

### Core Data Structures

#### 1. `t_game` - Master Game State
```c
typedef struct s_game {
    void        *mlx;           // MiniLibX connection
    void        *win;           // Window pointer
    void        *img;           // Image buffer
    char        *data;          // Pixel data array
    char        **map;          // 2D map array
    t_texture   textures[NB_TEX]; // All loaded textures
    t_player    player;         // Player state
    t_config    config;         // Parsed configuration
} t_game;
```

#### 2. `t_player` - Player State
```c
typedef struct s_player {
    float   x, y;              // Position in pixels
    float   angle;             // View direction (radians)
    float   z_eye;             // Vertical view position (0.0-1.0)
    int     speed;             // Movement speed
    bool    key_up, key_down;  // Movement flags
    bool    key_left, key_right;
    bool    left_rotate, right_rotate;
    bool    up_rotate, down_rotate;
    bool    is_attacking;      // Attack animation state
    int     attack_frame;      // Current animation frame
} t_player;
```

#### 3. `t_ray` - Ray State
```c
typedef struct s_ray {
    float   ray_angle;         // Current ray angle
    float   ray_dirx, ray_diry; // Ray direction vector
    int     mapx, mapy;        // Current map cell
    int     side;              // Which wall side hit (0=vertical, 1=horizontal)
    float   perp_wall_dist;    // Perpendicular distance to wall
    float   wall_height;       // Calculated wall height on screen
    int     start_y, end_y;    // Wall stripe boundaries
} t_ray;
```

#### 4. `t_config` - Map Configuration
```c
typedef struct s_config {
    char    *path;             // Map file path
    char    **lines;           // Raw file lines
    char    *tex[NB_TEX];      // Texture paths
    int     f_rgb[3];          // Floor color (R,G,B)
    int     c_rgb[3];          // Ceiling color (R,G,B)
    char    **map;             // Parsed 2D map
    int     player_count;      // Must be exactly 1
} t_config;
```

---

## 🛠️ Installation & Setup

### Prerequisites

The project requires:
- **GCC** or compatible C compiler
- **Make** build system
- **MiniLibX** graphics library
- **X11** development libraries (Linux)
- **Math library** (`libm`)

### Installation Steps

#### 1. Clone the Repository
```bash
git clone https://github.com/AbdellatifNASSER-ALLAH/cub3D.git
cd cub3D
```

#### 2. Install Dependencies (Linux/Ubuntu)
```bash
sudo apt-get update
sudo apt-get install gcc make
sudo apt-get install libx11-dev libxext-dev
sudo apt-get install libbsd-dev
```

#### 3. Build the Project
```bash
make
```

This will:
- Compile the libft library
- Compile all source files in `srcs/`
- Link with MiniLibX and create the `game` executable

#### 4. Run the Game
```bash
./game maps/cub.cub
```

### Build Commands

- `make` - Build the project
- `make clean` - Remove object files
- `make fclean` - Remove object files and executable
- `make re` - Rebuild everything from scratch
- `make d` - Build and run with GDB debugger

---

## 📁 Project Structure

### Directory Layout

```
cub3D/
├── includes/
│   └── cub3d.h              # All structures, constants, prototypes
├── srcs/
│   ├── main.c               # Entry point, game initialization
│   ├── player.c             # Player movement and rotation logic
│   ├── player_utils.c       # Player collision detection
│   ├── parse/               # Map parsing subsystem
│   │   ├── parse.c          # Main parsing orchestrator
│   │   ├── read_file.c      # File reading utilities
│   │   ├── valid_file.c     # File validation
│   │   ├── extract_configs.c # Texture/color extraction
│   │   ├── map.c            # Map validation and processing
│   │   └── utils.c          # Parsing helper functions
│   └── execution/           # Rendering subsystem
│       ├── draw_vision.c    # Main rendering loop
│       ├── dda.c            # DDA raycasting algorithm
│       ├── texture.c        # Texture loading and sampling
│       ├── pixel_draw.c     # Low-level pixel operations
│       ├── mini_map.c       # Minimap rendering
│       ├── hooks.c          # Keyboard input handlers
│       └── hooks2.c         # Mouse input handlers
├── utils/
│   └── handle_exit.c        # Cleanup and exit handling
├── libft/                   # Custom C library (strings, memory, etc.)
├── textures/                # Wall and sprite textures (XPM format)
├── maps/                    # Sample map files (.cub)
├── Makefile                 # Build configuration
└── README.md               # This file
```

### Key Files Explained

#### **main.c** - Program Entry Point
- Validates command-line arguments
- Calls the parser to load map configuration
- Initializes the game state (`init_game`)
- Sets up MiniLibX hooks for input and rendering
- Starts the game loop

#### **parse.c** - Configuration Parser
- Reads and validates `.cub` map files
- Extracts texture paths (NO, SO, WE, EA)
- Parses ceiling and floor colors (RGB values)
- Validates map structure and player position
- Creates the 2D map array

#### **draw_vision.c** - Rendering Core
- Iterates through all screen columns (WIDTH = 1280)
- Casts one ray per column
- Calculates wall distances and heights
- Draws textured vertical stripes
- Renders ceiling and floor
- Handles torch/weapon overlay

#### **dda.c** - Raycasting Algorithm
- Implements the DDA grid traversal
- Finds ray-wall intersections
- Determines which side of wall was hit
- Optimized for performance (no floating-point ray stepping)

#### **player.c** - Player Controller
- Handles WASD movement
- Applies collision detection
- Manages rotation (arrow keys and mouse)
- Controls vertical view angle (look up/down)

---

## 🔍 Raycasting Algorithm

### Step-by-Step Process

#### Step 1: Initialize Ray Parameters

For each screen column `x`:
```c
ray_angle = player_angle - (FOV/2) + (x * (FOV/WIDTH))
ray_dirx = cos(ray_angle)
ray_diry = sin(ray_angle)
```

This spreads rays across the player's field of view.

#### Step 2: Calculate DDA Step Distances

The DDA algorithm needs to know how far to travel along the ray to reach the next grid line:

```c
delta_distx = |1 / ray_dirx|  // Distance to next vertical grid line
delta_disty = |1 / ray_diry|  // Distance to next horizontal grid line
```

#### Step 3: Determine Initial Step Direction

Based on ray direction, determine which way to step in the grid:

```c
if (ray_dirx < 0) {
    stepx = -1;
    side_distx = (player_x - map_x) * delta_distx;
} else {
    stepx = 1;
    side_distx = (map_x + 1 - player_x) * delta_distx;
}
// Similar for Y direction
```

#### Step 4: Perform DDA Loop

Step through the grid until hitting a wall:

```c
while (!hit) {
    if (side_distx < side_disty) {
        side_distx += delta_distx;
        mapx += stepx;
        side = 0;  // Vertical wall
    } else {
        side_disty += delta_disty;
        mapy += stepy;
        side = 1;  // Horizontal wall
    }
    
    if (map[mapy][mapx] == '1' || map[mapy][mapx] == 'D') {
        hit = 1;  // Wall or door found
    }
}
```

#### Step 5: Calculate Perpendicular Distance

Avoid fish-eye distortion by using perpendicular distance:

```c
if (side == 0)
    perp_dist = (mapx - player_x + (1 - stepx)/2) / ray_dirx;
else
    perp_dist = (mapy - player_y + (1 - stepy)/2) / ray_diry;

// Apply perspective correction
dist = perp_dist * BLOCK * cos(ray_angle - player_angle);
```

#### Step 6: Calculate Wall Height

Project the wall height onto the screen:

```c
wall_height = (BLOCK / dist) * (WIDTH / 2);
start_y = (HEIGHT - wall_height) * player.z_eye;
end_y = start_y + wall_height;
```

The `z_eye` parameter (0.0 to 1.0) allows looking up and down.

#### Step 7: Draw Textured Stripe

Sample the appropriate texture and draw the vertical column:

```c
for (y = start_y; y < end_y; y++) {
    tex_y = (int)((y - start_y) * TEXTURE_HEIGHT / wall_height);
    color = get_texture_color(ray, tex_y, game);
    put_pixel(x, y, color, game);
}
```

---

## 📝 Parsing System

### Map File Format (.cub)

A `.cub` file contains texture paths, colors, and the map layout:

```
NO ./textures/north_wall.xpm
SO ./textures/south_wall.xpm
WE ./textures/west_wall.xpm
EA ./textures/east_wall.xpm

F 190,169,222
C 19,24,98

        1111111111111111111111111
        1000000000110000000000001
        1011000001110000000000001
111111111011000001110000000000001
100000000011000001110111111111111
11110111111111011100000010001
11000000110101011100000010001
10000000000000001100000010001
11000001110101011111011110N0111
11111111 1111111 111111111111
```

### Parsing Pipeline

#### Phase 1: File Validation
```c
valid_file(path, ".cub", &config);
```
- Checks file exists and is readable
- Validates `.cub` extension
- Opens file descriptor

#### Phase 2: Read All Lines
```c
read_file(&config);
```
- Reads entire file into memory
- Stores lines in `config.lines` array
- Counts total lines

#### Phase 3: Extract Configuration
```c
extract_configs(&config, NULL);
```

Parses each line to extract:

**Textures** (must all be present):
- `NO` - North wall texture path
- `SO` - South wall texture path
- `WE` - West wall texture path
- `EA` - East wall texture path
- `DO` - Door texture (optional)
- `T` - Torch texture (optional)
- `TA` - Torch attack texture (optional)

**Colors** (RGB format):
- `F R,G,B` - Floor color (e.g., `F 190,169,222`)
- `C R,G,B` - Ceiling color (e.g., `C 19,24,98`)

#### Phase 4: Map Processing
```c
fill_map(lines, map_start, map_end, &config);
```

Map validation rules:
1. **Characters allowed**: `0` (empty), `1` (wall), `N/S/E/W` (player start), `D` (door), ` ` (space)
2. **Player position**: Exactly one player start marker (N/S/E/W)
3. **Closed map**: Walls (`1`) must surround all accessible areas
4. **No holes**: Spaces at map edges are invalid (would allow player to escape)

### Map Validation Algorithm

```c
// Check each cell
for each cell (x, y) in map:
    if cell is '0' or player marker:
        // Check all 8 neighbors
        if any neighbor is outside map OR is space:
            return ERROR  // Map is not closed
```

---

## 🎨 Rendering Pipeline

### Frame Rendering Flow

Each frame follows this sequence:

```
1. Clear previous frame (implicit via new image data)
2. Cast rays and render 3D view → draw_vision()
3. Render torch/weapon overlay → draw_torch()
4. Draw crosshair/aim → draw_aim()
5. Render minimap → draw_minimap()
6. Draw minimap elements → draw_map()
7. Present frame to window → mlx_put_image_to_window()
```

### Detailed Rendering Steps

#### 1. Draw Vision (Main 3D View)

```c
void draw_vision(t_game *game) {
    for (x = 0; x < WIDTH; x++) {
        // Initialize ray for this column
        init_ray(&ray, &player, x);
        
        // Find wall intersection
        perform_dda(&ray, game);
        
        // Calculate wall distance and height
        calc_dist_and_height(&ray, &player);
        
        // Draw textured stripe
        draw_stripe(x, &ray, game);
    }
}
```

#### 2. Draw Stripe (Vertical Column)

```c
void draw_stripe(int x, t_ray *r, t_game *game) {
    // Draw ceiling
    for (y = 0; y < r->start_y; y++)
        put_pixel(x, y, ceiling_color, game);
    
    // Draw textured wall
    step = TEXTURE_HEIGHT / r->wall_height;
    for (y = r->start_y; y < r->end_y; y++) {
        tex_y = (y - r->start_y) * step;
        color = get_texture_color(r, tex_y, game);
        put_pixel(x, y, color, game);
    }
    
    // Draw floor
    for (y = r->end_y; y < HEIGHT; y++)
        put_pixel(x, y, floor_color, game);
}
```

#### 3. Texture Sampling

```c
int get_texture_color(t_ray *r, int tex_y, t_game *game) {
    // Determine which texture to use
    int tex_index = select_texture(r);
    
    // Calculate texture X coordinate
    float wall_x;
    if (r->side == 0)
        wall_x = r->py + r->perp_wall_dist * r->ray_diry;
    else
        wall_x = r->px + r->perp_wall_dist * r->ray_dirx;
    wall_x -= floor(wall_x);
    
    int tex_x = (int)(wall_x * TEXTURE_WIDTH);
    
    // Sample texture
    return game->textures[tex_index].data[tex_y * TEXTURE_WIDTH + tex_x];
}
```

#### 4. Minimap Rendering

The minimap shows:
- Top-down view of the map
- Player position and direction
- Field of view cone
- Walls and empty spaces

```c
void draw_minimap(t_game *game) {
    // Draw map cells as colored squares
    for (y = 0; y < map_height; y++) {
        for (x = 0; x < map_width; x++) {
            color = (map[y][x] == '1') ? WALL_COLOR : FLOOR_COLOR;
            draw_mini_square(x * MINI_BLOCK, y * MINI_BLOCK, color);
        }
    }
    
    // Draw player
    draw_player_on_minimap(game);
    
    // Draw FOV cone
    draw_fov_lines(game);
}
```

---

## 🎮 Player System

### Movement Mechanics

#### Translation (WASD)

The player moves in the direction they're facing (or sideways):

```c
// Forward (W)
new_x = player.x + cos(player.angle) * speed;
new_y = player.y + sin(player.angle) * speed;

// Backward (S)
new_x = player.x + cos(player.angle + π) * speed;
new_y = player.y + sin(player.angle + π) * speed;

// Right strafe (D)
new_x = player.x + cos(player.angle + π/2) * speed;
new_y = player.y + sin(player.angle + π/2) * speed;

// Left strafe (A)
new_x = player.x + cos(player.angle - π/2) * speed;
new_y = player.y + sin(player.angle - π/2) * speed;
```

#### Rotation (Arrow Keys / Mouse)

```c
// Left arrow or mouse left
player.angle -= ROTATION_SPEED;

// Right arrow or mouse right
player.angle += ROTATION_SPEED;

// Normalize angle to [0, 2π]
if (player.angle > 2π) player.angle -= 2π;
if (player.angle < 0) player.angle += 2π;
```

#### Vertical View (Up/Down Arrows)

```c
// Look up (decrease z_eye = move horizon down)
if (down_arrow && z_eye > 0.05)
    z_eye -= 0.05;

// Look down (increase z_eye = move horizon up)
if (up_arrow && z_eye < 0.95)
    z_eye += 0.05;
```

### Collision Detection

```c
bool is_blocked(float x, float y, t_game *game) {
    // Convert pixel coordinates to map coordinates
    int map_x = (int)(x / BLOCK);
    int map_y = (int)(y / BLOCK);
    
    // Check if the cell contains a wall
    return (map[map_y][map_x] == '1');
}
```

Before moving, check the new position:
```c
if (!is_blocked(new_x, player.y) && !is_blocked(player.x, new_y)) {
    player.x = new_x;
    player.y = new_y;
} else {
    // Sliding collision (move along walls)
    if (!is_blocked(new_x, player.y))
        player.x += (new_x - player.x) * 0.2;
    if (!is_blocked(player.x, new_y))
        player.y += (new_y - player.y) * 0.2;
}
```

### Input Handling

#### Keyboard Events

```c
int key_press(int key, t_game *game) {
    if (key == W) game->player.key_up = true;
    if (key == S) game->player.key_down = true;
    if (key == A) game->player.key_left = true;
    if (key == D) game->player.key_right = true;
    if (key == LEFT) game->player.left_rotate = true;
    if (key == RIGHT) game->player.right_rotate = true;
    if (key == ESC) handle_exit(game);
    return (0);
}

int key_release(int key, t_game *game) {
    if (key == W) game->player.key_up = false;
    // ... similar for other keys
    return (0);
}
```

#### Mouse Movement

```c
int mouse_move(int x, int y, t_game *game) {
    int delta_x = x - game->player.prev_mouse_x;
    
    // Rotate based on mouse movement
    game->player.angle += delta_x * MOUSE_SENSITIVITY;
    
    game->player.prev_mouse_x = x;
    return (0);
}
```

---

## 🖼️ Texture Management

### Texture Loading

Textures are loaded at initialization:

```c
void load_textures(t_game *game) {
    for (int i = 0; i < NB_TEX; i++) {
        if (game->config.tex[i]) {
            t_texture *tex = &game->textures[i];
            tex->img = mlx_xpm_file_to_image(game->mlx, 
                                             game->config.tex[i],
                                             &tex->width, 
                                             &tex->height);
            tex->data = (int *)mlx_get_data_addr(tex->img, &bpp, 
                                                  &size_line, &endian);
        }
    }
}
```

### Texture Format

- **Format**: XPM (X PixMap)
- **Size**: Typically 64x64 pixels
- **Storage**: Integer array where each element is a color (0xRRGGBB)

### Texture Selection

The texture used depends on which wall side was hit:

```c
int select_texture(t_ray *r) {
    if (r->side == 0) {  // Vertical wall
        return (r->ray_dirx > 0) ? EAST : WEST;
    } else {  // Horizontal wall
        return (r->ray_diry > 0) ? SOUTH : NORTH;
    }
}
```

### Texture Sampling

To get a color from a texture:

```c
// tex_x: horizontal position (0 to TEXTURE_WIDTH-1)
// tex_y: vertical position (0 to TEXTURE_HEIGHT-1)
int color = texture->data[tex_y * TEXTURE_WIDTH + tex_x];
```

---

## 🚪 Advanced Features

### Door System

Doors are represented by `D` in the map and can be:
- **Closed**: Block player movement and raycasting
- **Open**: (Future feature) Allow passage

**Rendering**: Doors use a special texture and are detected in DDA:
```c
if (map[mapy][mapx] == '1' || map[mapy][mapx] == 'D')
    hit = 1;  // Both walls and doors stop rays
```

### Torch/Weapon Overlay

A transparent torch sprite is rendered in the foreground:

```c
void draw_torch(t_game *game) {
    t_texture *tex = game->player.is_attacking ? 
                     &textures[TORCH_ATTACK] : 
                     &textures[TORCH];
    
    int screen_x = WIDTH - TORCH_SIZE - TORCH_OFFSET_X;
    int screen_y = HEIGHT - TORCH_SIZE + TORCH_OFFSET_Y;
    
    // Draw torch, skipping transparent pixels
    for (int y = 0; y < TORCH_SIZE; y++) {
        for (int x = 0; x < TORCH_SIZE; x++) {
            int color = sample_texture(tex, x, y);
            if (color != TRANSPARENT_COLOR)
                put_pixel(screen_x + x, screen_y + y, color);
        }
    }
}
```

### Attack Animation

Right-clicking triggers an attack animation:

```c
int mouse_button(int button, int x, int y, t_game *game) {
    if (button == RIGHT_MOUSE_BUTTON) {
        game->player.is_attacking = true;
        game->player.attack_frame = 0;
    }
    return (0);
}

// Animation updates each frame
void update_attack_animation(t_game *game) {
    if (game->player.is_attacking) {
        game->player.attack_frame++;
        if (game->player.attack_frame > ATTACK_ANIMATION_FRAMES) {
            game->player.is_attacking = false;
            game->player.attack_frame = 0;
        }
    }
}
```

### Minimap

Displays a top-down view in the corner:
- **Size**: 200x200 pixels
- **Block size**: 10 pixels per map cell
- **Features**: Player position, orientation, FOV visualization

```c
void draw_minimap(t_game *game) {
    // Draw map cells
    draw_minimap_cells();
    
    // Draw player as a circle
    draw_circle(player_x, player_y, PLAYER_RADIUS, PLAYER_COLOR);
    
    // Draw FOV lines
    float left_angle = player.angle - FOV/2;
    float right_angle = player.angle + FOV/2;
    draw_line(player_pos, left_angle, FOV_LINE_COLOR);
    draw_line(player_pos, right_angle, FOV_LINE_COLOR);
}
```

---

## 🎮 Controls

### Keyboard Controls

| Key | Action |
|-----|--------|
| **W** | Move forward |
| **S** | Move backward |
| **A** | Strafe left |
| **D** | Strafe right |
| **←** | Rotate left |
| **→** | Rotate right |
| **↑** | Look down (lower horizon) |
| **↓** | Look up (raise horizon) |
| **ESC** | Exit game |

### Mouse Controls

| Action | Effect |
|--------|--------|
| **Move left/right** | Rotate camera |
| **Right click** | Trigger attack animation |

### Control Scheme Details

**Simultaneous input**: Multiple keys can be pressed at once (e.g., W+D = diagonal movement)

**Mouse sensitivity**: Adjustable via `MOUSE_SENSITIVITY` constant

**Smooth rotation**: Rotation is applied gradually each frame for smooth animation

---

## 🗺️ Map Configuration

### Creating a Map

#### Step 1: Create a .cub File

```bash
touch my_map.cub
```

#### Step 2: Define Textures

```
NO ./textures/north_wall.xpm
SO ./textures/south_wall.xpm
WE ./textures/west_wall.xpm
EA ./textures/east_wall.xpm
```

#### Step 3: Set Colors

```
F 190,169,222    # Floor RGB
C 19,24,98       # Ceiling RGB
```

#### Step 4: Design the Map

```
        111111
        100001
        101001
        100N01
        111111
```

**Rules**:
- Use `1` for walls
- Use `0` for empty space
- Use `N`, `S`, `E`, or `W` for player start (exactly one required)
- Use `D` for doors (optional, requires door texture)
- Map must be closed (surrounded by walls)
- Irregular shapes allowed, but no holes in walls

### Example Maps

#### Simple Test Map
```
111111
1000N1
111111
```

#### Complex Map with Doors
```
NO ./textures/wall_1.xpm
SO ./textures/wall_2.xpm
WE ./textures/wall_3.xpm
EA ./textures/wall_4.xpm
DO ./textures/door.xpm

F 100,100,100
C 50,50,150

1111111111
1000000001
1001D10001
100000E001
1111111111
```

---

## 💻 Development Workflow

### Debugging

#### Enable Debug Build
```bash
make re CFLAGS="-Wall -Wextra -Werror -g -fsanitize=address"
```

#### Run with GDB
```bash
make d  # Builds and launches gdbtui
```

#### Common Debug Checks

**Check player position**:
```c
printf("Player: x=%.2f, y=%.2f, angle=%.2f\n", 
       player.x, player.y, player.angle);
```

**Visualize rays**:
```c
printf("Ray %d: angle=%.2f, dist=%.2f, wall at (%d,%d)\n",
       x, ray.ray_angle, ray.perp_wall_dist, ray.wallx, ray.wally);
```

### Performance Optimization

#### Profile the Code
```bash
gcc -pg *.c -o game
./game map.cub
gprof game gmon.out > analysis.txt
```

#### Optimization Tips

1. **Minimize divisions**: Pre-calculate reciprocals
2. **Use lookup tables**: For trigonometric functions (if needed)
3. **Avoid redundant calculations**: Store frequently used values
4. **Optimize inner loops**: The DDA and drawing loops are critical

### Adding New Features

#### Example: Adding Sprite Rendering

1. **Add sprite structure**:
```c
typedef struct s_sprite {
    float x, y;      // Position
    int texture_id;  // Which texture to use
} t_sprite;
```

2. **Add sprites to game state**:
```c
typedef struct s_game {
    // ... existing fields
    t_sprite *sprites;
    int sprite_count;
} t_game;
```

3. **Create sprite rendering function**:
```c
void draw_sprites(t_game *game) {
    // Calculate sprite distances
    // Sort by distance (far to near)
    // Draw each sprite as vertical stripes
}
```

4. **Call in render loop**:
```c
int draw_loop(t_game *game) {
    draw_vision(game);
    draw_sprites(game);  // After walls, before UI
    draw_torch(game);
    // ...
}
```

---

## 🔧 Troubleshooting

### Common Issues

#### 1. Black Screen
**Problem**: Window opens but shows only black

**Solutions**:
- Check that map file path is correct
- Verify textures are loading: `printf` in `load_textures()`
- Ensure player starting position is valid (not in a wall)
- Check FOV and ray calculations

#### 2. Segmentation Fault
**Problem**: Program crashes immediately

**Solutions**:
- Verify map is properly closed (no holes in walls)
- Check array bounds in map access
- Ensure textures exist at specified paths
- Run with `-fsanitize=address` to locate memory errors

#### 3. Distorted Graphics (Fish-Eye)
**Problem**: Walls appear curved

**Solutions**:
- Ensure perpendicular distance calculation is correct
- Verify the perspective correction: `dist * cos(ray_angle - player_angle)`
- Check that `player.angle` is in radians, not degrees

#### 4. Textures Not Loading
**Problem**: Error loading XPM files

**Solutions**:
- Verify file paths in .cub file are correct (relative to executable)
- Check XPM file format (use `xpm_check` tool)
- Ensure MiniLibX is properly installed
- Try absolute paths for debugging

#### 5. Slow Performance
**Problem**: Low FPS, choppy rendering

**Solutions**:
- Profile with `gprof` to find bottlenecks
- Reduce window size for testing
- Optimize DDA loop (avoid redundant calculations)
- Check for memory leaks slowing down allocation

### Validation Checklist

Before submitting/sharing:
- [ ] Map file parses without errors
- [ ] Exactly one player start position
- [ ] Map is completely closed
- [ ] All textures load successfully
- [ ] Player moves smoothly
- [ ] Collision detection works
- [ ] No memory leaks (check with valgrind)
- [ ] No segmentation faults
- [ ] Textures display correctly on all wall sides

---

## 📚 Learning Resources

### Raycasting Tutorials

1. **Lode's Raycasting Tutorial**  
   https://lodev.org/cgtutor/raycasting.html  
   The definitive guide to raycasting - comprehensive and clear

2. **Permadi's Ray-Casting Tutorial**  
   https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/  
   Great visual explanations and examples

3. **Wolfenstein 3D Source Code**  
   Study the original implementation for insights

### Graphics Programming

- **Computer Graphics from Scratch** by Gabriel Gambetta
- **Game Engine Black Book: Wolfenstein 3D** by Fabien Sanglard
- **3D Math Primer for Graphics and Game Development**

### C Programming

- **The C Programming Language** by Kernighan & Ritchie
- **Modern C** by Jens Gustedt
- **Expert C Programming** by Peter van der Linden

### Algorithms

- **DDA Line Algorithm**: Understanding grid traversal
- **Bresenham's Line Algorithm**: Alternative to DDA
- **Binary Space Partitioning (BSP)**: For more advanced rendering

---

## 🚀 Future Enhancements

### Potential Features to Implement

#### 1. Floor and Ceiling Textures
Instead of solid colors, render textured floors/ceilings using ray casting for the ground plane.

#### 2. Animated Sprites
Add enemies, items, or decorations as billboarded sprites:
- Calculate sprite screen position
- Sort by distance
- Render with transparency

#### 3. Lighting System
- Point lights with distance attenuation
- Colored lighting
- Shadow casting

#### 4. Interactive Doors
- Door opening/closing animations
- Sliding doors
- Locked doors requiring keys

#### 5. Multiple Floor Heights
- Stairs and elevation changes
- Platform rendering
- Jump mechanics

#### 6. Skybox
Replace ceiling with a skybox texture that rotates with player view

#### 7. Sound System
- Footstep sounds
- Ambient audio
- 3D positional audio for events

#### 8. Particle Effects
- Muzzle flash
- Impact effects
- Smoke/dust particles

#### 9. HUD Elements
- Health/ammo counters
- Objective indicators
- Damage indicators

#### 10. Save/Load System
Save player progress and game state to files

### Advanced Techniques

#### Horizontal Motion Parallax
More realistic wall rendering by accounting for horizontal plane distortion

#### Voxel Space Rendering
Render non-flat terrain using voxel techniques

#### Portal Rendering
Implement portals for recursive rendering (like in Portal game)

---

## 📄 License

This project is created for educational purposes as part of the 42 school curriculum.

---

## 🤝 Contributing

Contributions, issues, and feature requests are welcome!

### How to Contribute

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## 🙏 Acknowledgments

- **id Software** - For pioneering raycasting in Wolfenstein 3D
- **Lode Vandevenne** - For the excellent raycasting tutorial
- **42 School** - For the project specifications and learning framework
- **MiniLibX developers** - For the graphics library

---

## 📞 Contact

**Project Authors**:
- GitHub: [@AbdellatifNASSER-ALLAH](https://github.com/AbdellatifNASSER-ALLAH)
- GitHub: [@a-hakki](https://github.com/a-hakki)

**Project Link**: [https://github.com/AbdellatifNASSER-ALLAH/cub3D](https://github.com/AbdellatifNASSER-ALLAH/cub3D)

---

**Happy Raycasting! 🎮✨**

*This README provides a complete roadmap for understanding, building, and extending the cub3D raycasting engine. Use it as both a learning resource and technical reference.*