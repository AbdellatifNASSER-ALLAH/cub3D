
# ft_libft

<div align="center"><img width="202" alt="image" src="https://github.com/user-attachments/assets/3761b417-8453-46b9-a653-27286f91aa71"></div>
---

## Overview
`ft_libft` is a foundational project in the **42** curriculum, focused on recreating essential functions from the C standard library and additional utility functions. This library is designed to support future projects by providing a custom implementation of frequently used C functions.

## Features
The library is organized into four primary sections: **Libc Functions**, **Additional Functions**, **Bonus Functions**, and **Custom Utility Functions**.

### Libc Functions
Reimplementations of standard C library functions, which include:

- **Memory Functions**  
  `ft_memset`, `ft_bzero`, `ft_memcpy`, `ft_memccpy`, `ft_memmove`, `ft_memchr`, `ft_memcmp`
  
- **String Functions**  
  `ft_strlen`, `ft_strlcpy`, `ft_strlcat`, `ft_strchr`, `ft_strrchr`, `ft_strnstr`, `ft_strncmp`
  
- **Conversion Functions**  
  `ft_atoi`
  
- **Character Checks**  
  `ft_isalpha`, `ft_isdigit`, `ft_isalnum`, `ft_isascii`, `ft_isprint`
  
- **Character Case Conversion**  
  `ft_toupper`, `ft_tolower`

### Additional Functions
Functions not found in the standard library but useful for various programming tasks:

- **String Duplication and Manipulation**  
  `ft_strdup`, `ft_substr`, `ft_strjoin`, `ft_strtrim`, `ft_split`, `ft_itoa`
  
- **String Mapping and Iteration**  
  `ft_strmapi`, `ft_striteri`
  
- **File Descriptor Functions**  
  `ft_putchar_fd`, `ft_putstr_fd`, `ft_putendl_fd`, `ft_putnbr_fd`

### Bonus Functions
These functions enable linked list manipulation, a useful structure for handling dynamic collections of data.

- **Linked List Creation and Manipulation**  
  `ft_lstnew`, `ft_lstadd_front`, `ft_lstsize`, `ft_lstlast`, `ft_lstadd_back`, `ft_lstdelone`, `ft_lstclear`, `ft_lstiter`, `ft_lstmap`

### Custom Utility Functions (Optional)
Additional helper functions may be added here, providing extra functionality beyond the standard requirements.

## Getting Started

### Prerequisites
To compile and use `ft_libft`, you need:
- A Unix-like operating system (Linux, macOS).
- `make` and `gcc` (or another C compiler) installed.

### Compilation
To compile the library, clone the repository and run:

```bash
make
```

This will produce a `libft.a` static library file, which you can link to your own projects.

### Usage
To use `ft_libft` in your project, include the header file in your source files:

```c
#include "libft.h"
```

Then, link the compiled library when compiling your project:

```bash
gcc your_file.c -L. -lft -o your_program
```

## Project Structure
- **src/**: Source files for the implemented functions.
- **includes/**: Header files defining prototypes and macros.
- **Makefile**: For easy compilation and cleaning.

## Goals
- Gain a thorough understanding of fundamental C programming by reimplementing critical library functions.
- Improve proficiency with memory management, string manipulation, and linked list structures.
- Create a robust, reusable library for use in future 42 projects.

## Author
- **Hakki Abdelaziz**  
  1337 (42 Network)
