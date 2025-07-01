#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> // For setlocale

#include "terminal.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h> // For _kbhit and _getch
#include <io.h>    // For _setmode
#include <fcntl.h> // For _O_U16TEXT
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>      // For fcntl
#include <sys/select.h> // For select
#endif

// Platform-specific variables for terminal control
#ifdef _WIN32
static HANDLE hConsole;
static DWORD original_console_mode;
#else
static struct termios original_termios;
#endif

// Double buffering variables
static wchar_t **buffer = NULL;
static int buffer_width = 0;
static int buffer_height = 0;

// Sets up the terminal for raw mode (disables echoing, line buffering)
void terminal_setup()
{
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_U16TEXT); // Set stdout to Unicode
    hConsole = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hConsole, &original_console_mode);
    SetConsoleMode(hConsole, original_console_mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
    // Set console output code page to UTF-8 for wide characters
    SetConsoleOutputCP(CP_UTF8);
#else
    tcgetattr(STDIN_FILENO, &original_termios);
    struct termios raw = original_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
#endif
    setlocale(LC_ALL, ""); // Set locale for wide character output
}

// Restores the terminal to its original mode
void terminal_restore()
{
#ifdef _WIN32
    SetConsoleMode(hConsole, original_console_mode);
    SetConsoleOutputCP(437); // Restore default code page
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
    wprintf(L"\033[2J\033[H");
#endif
}

// Moves the cursor to a specific (x, y) position
void terminal_goto_xy(int x, int y)
{
#ifdef _WIN32
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    wprintf(L"\033[%d;%dH", y + 1, x + 1);
#endif
}

// Hides the terminal cursor
void terminal_hide_cursor()
{
    wprintf(L"\033[?25l");
}

// Shows the terminal cursor
void terminal_show_cursor()
{
    wprintf(L"\033[?25h");
}

// Initializes the double buffer
void terminal_init_buffer(int width, int height)
{
    buffer_width = width;
    buffer_height = height;
    buffer = (wchar_t **)malloc(height * sizeof(wchar_t *));
    if (buffer == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory for buffer rows.\n");
        exit(1);
    }

    for (int i = 0; i < height; i++)
    {
        buffer[i] = (wchar_t *)malloc((width + 1) * sizeof(wchar_t)); // +1 for null terminator
        if (buffer[i] == NULL)
        {
            fprintf(stderr, "Error: Failed to allocate memory for buffer columns.\n");
            // Free previously allocated rows
            for (int j = 0; j < i; j++)
            {
                free(buffer[j]);
            }
            free(buffer);
            exit(1);
        }

        for (int j = 0; j < width; j++)
        {
            buffer[i][j] = L' '; // Initialize with wide spaces
        }
        buffer[i][width] = L'\0'; // Null-terminate each row with wide null
    }
}

// Destroys the double buffer and frees memory
void terminal_destroy_buffer()
{
    if (buffer)
    {
        for (int i = 0; i < buffer_height; i++)
        {
            free(buffer[i]);
        }
        free(buffer);
        buffer = NULL;
    }
}

// Writes a character to the buffer at a specific position
void terminal_write_char_to_buffer(int x, int y, wchar_t c)
{
    if (x >= 0 && x < buffer_width && y >= 0 && y < buffer_height)
    {
        buffer[y][x] = c;
    }
}

// Prints the entire buffer to the console
void terminal_print_buffer()
{
    terminal_goto_xy(0, 0); // Move cursor to top-left before printing
    for (int i = 0; i < buffer_height; i++)
    {
        wprintf(L"%ls\n", buffer[i]);
    }
}

// Reads a key press without blocking (returns -1 if no key is pressed)
int terminal_read_key()
{
#ifdef _WIN32
    if (_kbhit())
    {
        return _getch();
    }
#else
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0)
    {
        char c;
        if (read(STDIN_FILENO, &c, 1) == 1)
        {
            return c;
        }
    }
#endif
    return -1; // No key pressed
}
