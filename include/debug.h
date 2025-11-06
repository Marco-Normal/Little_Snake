#ifndef DEBUG_
#define DEBUG_

#include <ncurses.h>
#include <stdarg.h> // For variable arguments

// Global debug window (create at bottom of screen)
void init_debug_console(int height);
void debug_log(const char *format, ...);
void toggle_debug_console();
#endif