typedef unsigned short vga_atrributes;

// Define color
#define VGA_COLOR_BLACK             0
#define VGA_COLOR_BLUE              1
#define VGA_COLOR_GREEN             2
#define VGA_COLOR_CYAN              3
#define VGA_COLOR_RED               4
#define VGA_COLOR_MAGENTA           5
#define VGA_COLOR_BROWN             6
#define VGA_COLOR_DARK_GREY         8
#define VGA_COLOR_LIGHT_GREY        7
#define VGA_COLOR_LIGHT_BLUE        9
#define VGA_COLOR_LIGHT_GREEN       10
#define VGA_COLOR_LIGHT_CYAN        11
#define VGA_COLOR_LIGHT_RED         12
#define VGA_COLOR_LIGHT_MAGENTA     13
#define VGA_COLOR_LIGHT_BROWN       14
#define VGA_COLOR_WHITE             15

void tty_set_theme(vga_atrributes fg, vga_atrributes bg); // Set theme
void tty_put_char(char chr); // Store character enable to print characters
void tty_put_str(char* str); // Store string enable to print string
void tty_scroll_up(); // Scroll up one line
void tty_clear(); // clear screen