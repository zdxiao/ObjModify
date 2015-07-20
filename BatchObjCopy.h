#ifndef BATCHOBJCOPY_H
#define BATCHOBJCOPY_H

#include "ObjModify.h"
#include "ObjNormalize.h"

using namespace std;

class BatchObjCopy
{
public:
    string inputPath;
    string outputPath;
    string listFilename;

    BatchObjCopy(
         string iP = "E:\\Research\\EVA data\\Evadata Obj",
         string oP = "E:\\Research\\EVA data\\Normal",
         string lF = "fileList.txt")
         :inputPath(iP), outputPath(oP), listFilename(lF){}

    bool batchCopy(ObjModify::State);

private:
    vector<string>  fileList;
    bool            read_listfile();
    ifstream&       open_file(ifstream&, const string&);
};

inline ifstream& BatchObjCopy::open_file(ifstream &in, const string &file)
{
    in.close();
    in.clear();
    in.open(file.c_str());
    return in;
}
#endif // BATCHOBJCOPY_H
