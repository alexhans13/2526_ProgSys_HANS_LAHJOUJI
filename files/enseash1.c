#include <unistd.h>
#include <string.h>

int main (void) {

const char *message =

"Bienvenue dans le Shell ENSEA.\n"
"Pour quitter, tapez 'exit'.\n"
"enseash %";

write(STDOUT_FILENO, message, strlen(message));

return 0;

}

