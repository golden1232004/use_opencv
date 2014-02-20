#ifndef __STRING_PROCESS_HPP__
#define __STRING_PROCESS_HPP__

#include <string>
#include <stdio.h>

using namespace std;

#ifdef __cplusplus
extern "C"{
#endif

string dirName(const string& pathname, bool withLastSep);

string readLineFromListFile(FILE* fp);
  /*
{
    const int iBufLen = 1000;
    char str[iBufLen]={'\0'};
    if (!feof(fp)){
	if (fgets(str, iBufLen, fp)){
	    int len = strlen(str);
	    if (!isprint(str[len-1])){
	        str[len-1] = '\0';
	    }
	}
    }
    string lineStr = str;
    return lineStr;
}
  */

#ifdef __cplusplus
}
#endif

#endif // __STRING_PROCESS_HPP__
