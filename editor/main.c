#include "editor.h"

int main(int argc, char *argv[])
{
  if (argc > 1) {
    printf("editor loaded on: %s\nas: [%s] type level\n", argv[1], argv[2]);
    startEditor(argv[1], argv[2]);
  } else {
    printf("No file as argument\n");
  }
  return 0;
}
