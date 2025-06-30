#ifndef TERMINAL_H
#define TERMINAL_H

// Function declarations for terminal control
void terminal_setup();
void terminal_restore();
void terminal_clear_screen();
void terminal_goto_xy(int x, int y);

// Cursor visibility functions
void terminal_hide_cursor();
void terminal_show_cursor();

// Double buffering functions
void terminal_init_buffer(int width, int height);
void terminal_destroy_buffer();
void terminal_write_char_to_buffer(int x, int y, char c);
void terminal_print_buffer();

#endif // TERMINAL_H
