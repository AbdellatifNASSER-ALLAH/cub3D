# Quick Start Guide - Animated Door Textures

## Prerequisites
- MLX library installed
- X11 development libraries
- Make and GCC

## Build and Run

```bash
# Clone or pull the latest changes
git checkout copilot/add-animated-door-textures

# Build the project
make

# Run with the animated door test map
./game maps/door_animated_test.cub
```

## Controls
- `W` / `A` / `S` / `D` - Move forward/left/backward/right
- `Arrow Keys` - Rotate view
- `E` - Interact with door (open)
- `ESC` - Exit game

## Testing the Feature

### Basic Test
1. Launch the game with the test map
2. Walk up to a door (brown texture with handle)
3. Face the door
4. Press `E`
5. Watch the door animate from brown → gray → white
6. Wait for animation to complete (~20 frames at default speed)
7. Walk through the now-open door

### Expected Behavior
- **Before pressing E**: Door is solid brown, blocks movement
- **After pressing E**: Door gradually opens over ~2 seconds
- **During opening**: Door still blocks movement
- **When fully open**: Door is white/transparent, you can walk through

### Test Map Features
The `door_animated_test.cub` map contains:
- One door blocking a vertical corridor (center)
- Two doors in a horizontal wall (bottom)
- Various open spaces to test movement

## Troubleshooting

### "Error loading texture"
- Check that texture files exist in `textures/doors/`
- Verify frame naming: `door_open_00.xpm` through `door_open_04.xpm`

### Door doesn't open
- Make sure you're close enough (within ~1.5 blocks)
- Ensure you're facing the door
- Press `E` firmly (key may need to be held briefly)

### Can't walk through door
- Wait for animation to complete fully
- Door turns white when fully open
- Check console for any errors

### Animation is too fast/slow
- Edit the map file's `DA` parameter
- Default: `DA 0.1` (10% per frame)
- Slower: `DA 0.05` (5% per frame)
- Faster: `DA 0.2` (20% per frame)

## Creating Custom Door Textures

### Frame Naming Convention
```
base_name_00.xpm  (closed)
base_name_01.xpm  (opening 1/4)
base_name_02.xpm  (opening 2/4)
base_name_03.xpm  (opening 3/4)
base_name_04.xpm  (fully open)
...
base_name_09.xpm  (max 10 frames)
```

### Map Configuration
```
DO ./textures/my_custom_door    # Without _XX.xpm suffix
DA 0.05                          # Animation speed (optional)
```

### Requirements
- All frames must be 64x64 pixels
- All frames must be in XPM format
- Frames must be numbered sequentially from 00
- Missing frames will stop auto-detection

## Example: Custom Sliding Door

1. Create textures:
   ```
   textures/sliding/slide_00.xpm  (door closed)
   textures/sliding/slide_01.xpm  (25% open)
   textures/sliding/slide_02.xpm  (50% open)
   textures/sliding/slide_03.xpm  (75% open)
   textures/sliding/slide_04.xpm  (fully open)
   ```

2. Update map file:
   ```
   DO ./textures/sliding/slide
   DA 0.08
   ```

3. Test in game!

## Performance Notes

- All frames are loaded at startup (no runtime file I/O)
- Animation runs at game frame rate (~60 FPS typical)
- Multiple doors can animate simultaneously
- Door state persists during gameplay (doesn't auto-close)

## Advanced Usage

### Multiple Door Types
Currently, all doors use the same texture set. To use different door types:
1. Create separate map files with different DO configurations
2. Future enhancement: Support multiple door texture sets

### Debugging
Add printf statements in:
- `interact_with_door()` - See when E is pressed
- `update_door_animations()` - Watch progress values
- `is_door_passable()` - Check collision logic

### Memory Check
```bash
valgrind --leak-check=full ./game maps/door_animated_test.cub
```

## File Structure
```
cub3D/
├── textures/
│   └── doors/
│       ├── door_open_00.xpm  (closed)
│       ├── door_open_01.xpm  (opening)
│       ├── door_open_02.xpm  (opening)
│       ├── door_open_03.xpm  (opening)
│       └── door_open_04.xpm  (open)
├── maps/
│   └── door_animated_test.cub
├── srcs/
│   └── execution/
│       └── door_animation.c
└── game (executable)
```

## Next Steps

After testing basic functionality:
1. Try creating custom door textures
2. Experiment with animation speeds
3. Create maps with multiple doors
4. Test edge cases (corner doors, multiple simultaneous openings)

## Feedback

If you encounter issues:
1. Check IMPLEMENTATION_SUMMARY.md for technical details
2. Review ANIMATED_DOORS_README.md for complete documentation
3. Verify all files were committed correctly
4. Check that MLX is properly installed

## Success Criteria

✓ Door loads with frame 0 texture
✓ Pressing E triggers animation
✓ Animation progresses smoothly
✓ Door becomes passable when fully open
✓ No crashes or memory leaks
✓ Frame rate remains stable

Enjoy your animated doors! 🚪✨
