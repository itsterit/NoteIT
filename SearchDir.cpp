#include "SearchDir.h"
using namespace std;

int SerchDir(string* NamePointer, string* OpenDir)
{
    char PatchName[100] = { 0 };
    short PatchCounter = -1;
    DIR* Patch;
    struct dirent* Dir;
    Patch = opendir(OpenDir->c_str());

    if (Patch)
    {
        PatchCounter = 0;
        while ((Dir = readdir(Patch)) != NULL)
        {
            *NamePointer = string(Dir->d_name);
            NamePointer++;
            PatchCounter++;
        }
        closedir(Patch);
    }

    return PatchCounter;
}