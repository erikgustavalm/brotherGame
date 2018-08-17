#include "editor.h"
char* getType(int type)
{
	switch (type) {
	  case 0: 
		  return "wood";
		break;
	  case 1:
		return "city";
		break;
	default:
		break;
	}
}

int main(int argc, char *argv[])
{
    /*if (argc > 1) {
	    printf("editor loaded on: %s\nas: [%s] type level\n", argv[1], argv[2]);
		startEditor(argv[1], argv[2]);
	} else {
	    printf("No file as argument\n");
	}*/

	FILE* f = fopen("levels/listoflevels", "a+");
	if (f == NULL) {
		printf("couldn't find listoflevels\n");
	} else {

		printf("Which level would you like to work on:\n");
		int nroflevels = 0;
		int maxsizearray = 10;
		int* arrayoftypes = NULL;
		arrayoftypes = malloc(sizeof(int)*maxsizearray);
		
		while (!feof(f)) {

			int type = 0;

			if (fscanf(f, "%d\n", &type)) {
				printf("%d: Level%d\n     type: %s\n", nroflevels + 1, nroflevels + 1, getType(type));
				
				if (nroflevels == maxsizearray) {
					maxsizearray *= 2;
					arrayoftypes = realloc(arrayoftypes, sizeof(int) * maxsizearray);
				}
				arrayoftypes[nroflevels] = type;
				nroflevels++;
			}			
		}
		
		int choice = 0;
		printf("enter: ");
		scanf("%d", &choice);
		char level[12];
		sprintf(level, "level%d", choice);

		fclose(f);

		startEditor(level, getType(arrayoftypes[choice-1]));
					
		free(arrayoftypes);
	}
	
	
	return 0;
}
