#ifndef TERMINAL_H
#define TERMINAL_H

// Function declarations for terminal control
void terminal_setup();
void terminal_restore();
void terminal_clear_screen();
void terminal_goto_xy(int x, int y);

#endif // TERMINAL_H
