#include <stdio.h>
#include <string.h>
#include <string>
#include "string_process.h"

using namespace std;

int main(int argc, char* argv[])
{
  
    string srcFile = argv[1];
    
    FILE *fpSrc = fopen(srcFile.c_str(), "r");
    if (NULL == fpSrc){
        fprintf(stderr, "Error: open %s failed\n", srcFile.c_str());
        return -1;
    }
    const string addStr = ".jpg";
    const int iBufLen = 1000;
    char str[iBufLen]={'\0'};
    while (!feof(fpSrc)){
#if 0
        string lineStr = readLineFromListFile(fpSrc);
	if (!feof(fpSrc)){
	    printf("%s\n", lineStr.c_str());
	}
#endif
    }
    fclose(fpSrc);
}
