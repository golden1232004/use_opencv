#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* argv[])
{

    char* listName = argv[1];
    char* infoName = argv[2];

    FILE* listFile = fopen(listName, "r");
    if (NULL == listFile){
        fprintf(stderr, "Error: load %s failed\n", listName);
	return -1;
    }
    FILE* infoFile = fopen(infoName, "a");
    if (NULL == infoFile){
        fprintf(stderr, "Error: open %s failed\n", infoName);
	return -1;
    }
    const int iBufLen = 1000;
    char tmpName[iBufLen]={'\0'};
    int cnt = 0;
    while(!feof(listFile)){
        cnt++;
        if (fgets(tmpName, iBufLen, listFile)){
	    int len = strlen(tmpName);
	    if (!isprint(tmpName[len-1])){
		tmpName[len-1] = '\0';
	    }
        }
        fprintf(infoFile, "%s ", tmpName);
        const int count = 1;
	fprintf(infoFile, "%d ", count);
	int left = 0;
	int top = 0;
	int width = 24;
	int height = 24;
	fprintf(infoFile, "%d %d %d %d\n", left, top, width, height);
    }
    fclose(listFile);
    fclose(infoFile);
    return 1;
}
