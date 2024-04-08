#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_TEXT_LENGTH 16

int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <text> <target_ip>\n", argv[0]);
    return 1;
  }

  char* text = argv[1];
  char* target = argv[2];

  if (strlen(text) > MAX_TEXT_LENGTH) {
    printf("Text too long!\n");
    return 1;
  }
  
  char command[1024]; 
  int command_len = snprintf(command, sizeof(command), "ping -p ");

  
  
  if (command_len >= sizeof(command) - 3) {
    fprintf(stderr, "Command string too long\n");
    return 1;
  }

  
  for (int i = 0; text[i] != '\0'; i++) {
    command_len += snprintf(command + command_len, sizeof(command) - command_len, "%02X", (unsigned char)text[i]);
    if (command_len >= sizeof(command) - 3) {
      fprintf(stderr, "Command string too long\n");
      return 1;
    }
  }

  
  
  snprintf(command + command_len, sizeof(command) - command_len, " -c 1 %s", target);

  int ping_count = 10;

  for (int i = 0; i < ping_count; i++) {
    FILE* pipe = popen(command, "r");
    if (!pipe) {
      perror("popen");
      return 1;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
      printf("%s", buffer);
    }

    pclose(pipe);
  }

  return 0;
}
