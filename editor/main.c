#include "editor.h"

int main(int argc, char *argv[])
{
  if (argc > 1) {
    printf("editor loaded on level: %s\n", argv[1]);
    startEditor(argv[1]);
  } else {
    printf("No file as argument\n");
  }
  return 0;
}
