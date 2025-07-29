# Understanding Ray Casting

# Implementation Details
- Setting Up the Environment
- The Math Behind Ray Casting
    - Step 1: Calculating the Ray Direction
    - Step 2: Calculating the Delta Distance
    - Step 3: Calculating the Step and Initial Side Distances
    - Step 4: Performing Digital Differential Analysis
    - Step 5: Calculating the Wall Height
- Handling the Textures
- The Pixel Map
- Optimising Performance



```c
    int is_intersection(float x, float y, t_game *game)
    {
        static int old_x;
        static int old_y;
        int calc_x;
        int calc_y;
        int res = 0;
        
        calc_x = (old_x - 1) / MINI_BLOCK; 
        calc_y = (old_y - 1) / MINI_BLOCK;
        if (calc_x > 19 || calc_y > 19 || old_x == 0 || old_y == 0)
        {
            printf("%d %d %f %f\n", calc_x, calc_y, x, y);
            old_x = x;
            old_y = y;
            return 0;
        }
        if (calc_x < 0 || calc_y < 0)
        {
            old_x = x;
            old_y = y;
            return 0;
        }
        int xx = old_x / MINI_BLOCK;
        int yy = old_y / MINI_BLOCK;
        if (game->map[calc_y][xx] == '1' && game->map[yy][calc_x] == '1')
            res = 1;
        old_x = x;
        old_y = y;
        return res;
    }
```