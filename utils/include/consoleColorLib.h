
extern const char *FOREGROUND_COLOR_CODES[];
extern const char *BACKGROUND_COLOR_CODES[];
extern const char *FONT_STYLE_CODES[];

void setForegroundColor(int color);
void setBackgroundColor(int color);

void setFontStyle(int style);
void resetConsole();

enum ConsoleColor {
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    BRIGHT_BLACK,
    BRIGHT_RED,
    BRIGHT_GREEN,
    BRIGHT_YELLOW,
    BRIGHT_BLUE,
    BRIGHT_MAGENTA,
    BRIGHT_CYAN,
    BRIGHT_WHITE,
};
