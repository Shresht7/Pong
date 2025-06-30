#include "terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

// Platform-specific variables for terminal control
#ifdef _WIN32
static HANDLE hConsole;
static DWORD original_console_mode;
#else
static struct termios original_termios;
#endif

// Double buffering variables
static char** buffer = NULL;
static int buffer_width = 0;
static int buffer_height = 0;

// Sets up the terminal for raw mode (disables echoing, line buffering)
void terminal_setup() {
#ifdef _WIN32
    hConsole = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hConsole, &original_console_mode);
    SetConsoleMode(hConsole, original_console_mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
#else
    tcgetattr(STDIN_FILENO, &original_termios);
    struct termios raw = original_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
#endif
}

// Restores the terminal to its original mode
void terminal_restore() {
#ifdef _WIN32
    SetConsoleMode(hConsole, original_console_mode);
#else
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
#endif
}

// Clears the terminal screen
void terminal_clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J\033[H");
#endif
}

// Moves the cursor to a specific (x, y) position
void terminal_goto_xy(int x, int y) {
#ifdef _WIN32
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    printf("\033[%d;%dH", y + 1, x + 1);
#endif
}

// Hides the terminal cursor
void terminal_hide_cursor() {
    printf("\033[?25l"); // ANSI escape code to hide cursor
}

// Shows the terminal cursor
void terminal_show_cursor() {
    printf("\033[?25h"); // ANSI escape code to show cursor
}

// Initializes the double buffer
void terminal_init_buffer(int width, int height) {
    buffer_width = width;
    buffer_height = height;
    buffer = (char**)malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        buffer[i] = (char*)malloc((width + 1) * sizeof(char)); // +1 for null terminator
        memset(buffer[i], ' ', width); // Initialize with spaces
        buffer[i][width] = '\0'; // Null-terminate each row
    }
}

// Destroys the double buffer and frees memory
void terminal_destroy_buffer() {
    if (buffer) {
        for (int i = 0; i < buffer_height; i++) {
            free(buffer[i]);
        }
        free(buffer);
        buffer = NULL;
    }
}

// Writes a character to the buffer at a specific position
void terminal_write_char_to_buffer(int x, int y, char c) {
    if (x >= 0 && x < buffer_width && y >= 0 && y < buffer_height) {
        buffer[y][x] = c;
    }
}

// Prints the entire buffer to the console
void terminal_print_buffer() {
    terminal_goto_xy(0, 0); // Move cursor to top-left before printing
    for (int i = 0; i < buffer_height; i++) {
        printf("%s\n", buffer[i]);
    }
}