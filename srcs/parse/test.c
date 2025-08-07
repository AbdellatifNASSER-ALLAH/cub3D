#include "cub3d.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define TEX_COUNT 4

static void exit_err(const char *msg, int code)
{
    perror(msg);
    exit(code);
}

// Utility: skip whitespace chars
static char *skip_ws(char *s)
{
    while (*s && (*s == ' ' || *s == '\t'))
        s++;
    return s;
}

// Checks if line is a map line (begins with 1, 0, or space)
static int is_map_line(const char *line)
{
    while (*line && (*line == ' ' || *line == '\t'))
        line++;
    return (*line == '1' || *line == '0');
}

// Count lines in file
static int count_lines(const char *path)
{
    int fd = open(path, O_RDONLY);
    int count = 0;
    char *line;

    if (fd < 0)
        exit_err("count_lines: open failed", 1);
    while ((line = get_next_line(fd)))
    {
        count++;
        free(line);
    }
    if (close(fd) == -1)
        exit_err("count_lines: close failed", 1);
    return count;
}

// Reads all lines from file
static char **read_file(const char *path)
{
    int total = count_lines(path);
    char **lines = malloc(sizeof(char *) * (total + 1));
    int fd = open(path, O_RDONLY);
    int i = 0;
    if (!lines)
        exit_err("read_file: malloc failed", 1);
    if (fd < 0)
        exit_err("read_file: open failed", 1);
    while (i < total)
    {
        lines[i] = get_next_line(fd);
        i++;
    }
    lines[i] = NULL;
    if (close(fd) == -1)
        exit_err("read_file: close failed", 1);
    return lines;
}

// Returns index of first map line
static int find_map_start(char **lines)
{
    int i = 0;
    while (lines[i])
    {
        char *trim = skip_ws(lines[i]);
        if (*trim && is_map_line(trim))
            return i;
        i++;
    }
    exit_err("No map found", 1);
    return -1;
}

// Checks if string starts with given prefix
static int starts_with(const char *s, const char *prefix)
{
    while (*s && *prefix && *s == *prefix)
    {
        s++;
        prefix++;
    }
    return *prefix == 0;
}

// Parses RGB from string (e.g., "220,100,0")
static int parse_rgb(const char *s, int *rgb)
{
    int r, g, b;
    if (sscanf(s, "%d,%d,%d", &r, &g, &b) != 3)
        return 0;
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
        return 0;
    rgb[0] = r; rgb[1] = g; rgb[2] = b;
    return 1;
}

// Structure for internal tracking of config
typedef struct s_parseconf {
    char *tex[TEX_COUNT];
    int f_rgb[3], c_rgb[3];
    int tex_found[TEX_COUNT];
    int f_found, c_found;
} t_parseconf;

// Helper: free lines
static void free_lines(char **lines)
{
    int i = 0;
    while (lines && lines[i])
        free(lines[i++]);
    free(lines);
}

// Extract and validate configs (NO,SO,WE,EA,F,C)
static void validate_config_section(char **lines, int map_start, t_parseconf *pcfg)
{
    int i;
    for (i = 0; i < TEX_COUNT; i++) pcfg->tex_found[i] = 0;
    pcfg->f_found = pcfg->c_found = 0;
    for (i = 0; i < map_start; i++)
    {
        char *l = skip_ws(lines[i]);
        if (!*l) continue; // empty line
        if (starts_with(l, "NO "))
        {
            if (pcfg->tex_found[0]++)
                exit_err("Duplicate NO", 1);
            pcfg->tex[0] = strdup(skip_ws(l + 2));
        }
        else if (starts_with(l, "SO "))
        {
            if (pcfg->tex_found[1]++)
                exit_err("Duplicate SO", 1);
            pcfg->tex[1] = strdup(skip_ws(l + 2));
        }
        else if (starts_with(l, "WE "))
        {
            if (pcfg->tex_found[2]++)
                exit_err("Duplicate WE", 1);
            pcfg->tex[2] = strdup(skip_ws(l + 2));
        }
        else if (starts_with(l, "EA "))
        {
            if (pcfg->tex_found[3]++)
                exit_err("Duplicate EA", 1);
            pcfg->tex[3] = strdup(skip_ws(l + 2));
        }
        else if (starts_with(l, "F "))
        {
            if (pcfg->f_found++)
                exit_err("Duplicate F", 1);
            if (!parse_rgb(skip_ws(l + 1), pcfg->f_rgb))
                exit_err("Invalid F color", 1);
        }
        else if (starts_with(l, "C "))
        {
            if (pcfg->c_found++)
                exit_err("Duplicate C", 1);
            if (!parse_rgb(skip_ws(l + 1), pcfg->c_rgb))
                exit_err("Invalid C color", 1);
        }
        else
            exit_err("Unknown config line", 1);
    }
    for (i = 0; i < TEX_COUNT; i++)
        if (!pcfg->tex_found[i])
            exit_err("Missing texture config", 1);
    if (!pcfg->f_found || !pcfg->c_found)
        exit_err("Missing F or C config", 1);
}

// Validate map: only allowed chars, only one player, etc.
static void validate_and_extract_map(char **lines, t_config *cfg)
{
    int i = 0, player_count = 0;
    while (lines[i])
    {
        char *l = skip_ws(lines[i]);
        int j = 0;
        while (l[j])
        {
            if (l[j] == 'N' || l[j] == 'S' || l[j] == 'E' || l[j] == 'W')
                player_count++;
            else if (l[j] != '1' && l[j] != '0' && l[j] != ' ')
                exit_err("Invalid map character", 1);
            j++;
        }
        i++;
    }
    if (player_count != 1)
        exit_err("Map must have exactly one player start", 1);
    // TODO: check map closed by walls, etc. (for brevity, omitted here)
    // Store lines as map in cfg as needed by your project
    // e.g., cfg->map = map_copy(lines);
}

// Main parse function
void parse(t_config *cfg, char *path)
{
    char **lines;
    int map_start;
    t_parseconf pcfg;

    // valid_file(path, ".cub"); // implement as needed
    lines = read_file(path);
    map_start = find_map_start(lines);
    validate_config_section(lines, map_start, &pcfg);
    validate_and_extract_map(lines + map_start, cfg);

    // Copy extracted values to your t_config struct as needed
    // Example:
    // cfg->no_tex = pcfg.tex[0]; ... etc.
    // cfg->f_rgb[0] = pcfg.f_rgb[0]; ... etc.

    free_lines(lines);
    // Free pcfg.tex[] if you copy them elsewhere
}
