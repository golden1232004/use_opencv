#include <string.h>

#include "string_process.h"

/*
string dirName(const string& pathname, bool withLastSep)
{
    size_t pos;
    pos = pathname.rfind("/");
    if (pos == string::npos)
        return string("");
    else if (pos == 0) {
        if (withLastSep)
            return string("/");
        else
            return string("");
    }
    return pathname.substr(0, withLastSep ? pos+1 : pos);
}

string readLineFromListFile(FILE* fp)
{
    const int iBufLen = 1000;
    char str[iBufLen]={'\0'};
    if (!feof(fp)){
        if (fgets(str, iBufLen, fp)){ // std::getline()
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
