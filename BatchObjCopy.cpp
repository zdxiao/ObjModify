#include "BatchObjCopy.h"

bool BatchObjCopy::read_listfile()
{
    ifstream listfile_stream;
    string listfilePathName = inputPath + "\\" + listFilename;
    cout << listfilePathName << endl;

    open_file(listfile_stream, listfilePathName.c_str());

    if(!listfile_stream)
    {
        cerr << "Can not open list file!" << endl;
        return false;
    }
    string line;
    fileList.clear();
    while(getline(listfile_stream, line))
    {
        fileList.push_back(line);
    }
    listfile_stream.close();
    return true;
}

bool BatchObjCopy::batchCopy(ObjModify::State s)
{
    if(!read_listfile())
    {
        cerr << "Reading list file goes wrong!" << endl;
        return false;
    }
    if(fileList.empty())
    {
        cerr << "File list is empty!";
        return false;
    }
    for(auto iter = fileList.begin(); iter != fileList.end(); ++iter)
    {
        cout << *iter << endl;
        ObjModify* file;
        if(s == ObjModify::modify)
        {
            file = new ObjModify(*iter, inputPath, outputPath);
        }
        else if(s == ObjModify::normalize)
        {
            file = new ObjNormalize(*iter, inputPath, outputPath);
        }
        if(!(file->copyData()))
        {
            return false;
        };
    }
    return true;
}
