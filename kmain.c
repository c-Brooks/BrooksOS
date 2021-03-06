#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/* The output colours */
#define FB_GREEN     2
#define FB_DARK_GREY 8


/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}


/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the FrameBuffer.
 *
 *  16-bit FrameBuffer data has this form:
 * Bit:     | 15 14 13 12 11 10 9 8 | 7 6 5 4 | 3 2 1 0 |
 * Content: | ASCII                 | FG      | BG      |
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    char *fb = (char *) 0x000B8000;

    fb[i] = c;
    fb[i+1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

/** write:
 *  Writes an array of chars of length len at position cursorStart
 *
 *  @param buf         The array of characters
 *  @param len         The length of the array
 *  @param cursorStart The starting position of the cursor
 *  @return            The end position of the cursor
 */
unsigned int write(char *buf, unsigned int len, unsigned int cursorStart) {
  unsigned int cursorPos = cursorStart;
  unsigned int i = 0;
  fb_move_cursor(cursorPos);
  for ( ; i < len; i++) {
    if (!buf[i]) {
      break;
    }
    fb_write_cell((cursorPos)*2, buf[i], FB_GREEN, FB_DARK_GREY);
    fb_move_cursor(cursorPos);
    cursorPos++;
  }
  fb_move_cursor(cursorPos);
  return cursorPos;
}

/** kmain:
 *  Driver function.
*/
int kmain() {
    // Write a message to the FrameBuffer in pos. 0x01E0 (row 6 col 1)
    char msg[] = "Hello, BrooksOS!";
    write(msg, 16, 0x01E0);
    return 0;
}
