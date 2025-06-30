#include "terminal.h"
#include <stdio.h>

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

// Sets up the terminal for raw mode (disables echoing, line buffering)
void terminal_setup()
{
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
void terminal_restore()
{
#ifdef _WIN32
    SetConsoleMode(hConsole, original_console_mode);
#else
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
#endif
}

// Clears the terminal screen
void terminal_clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J\033[H");
#endif
}

// Moves the cursor to a specific (x, y) position
void terminal_goto_xy(int x, int y)
{
#ifdef _WIN32
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    printf("\033[%d;%dH", y + 1, x + 1);
#endif
}

// Hides the terminal cursor
void terminal_hide_cursor()
{
    printf("\033[?25l"); // ANSI escape code to hide cursor
}

// Shows the terminal cursor
void terminal_show_cursor()
{
    printf("\033[?25h"); // ANSI escape code to show cursor
}
