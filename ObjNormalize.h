#ifndef OBJNORMALIZE_H
#define OBJNORMALIZE_H

#include "ObjModify.h"

class ObjNormalize: public ObjModify
{
public:
    ObjNormalize(string file, string iP = ".", string oP = ".", string suffix = "_nor.obj", double f = 0.005):
        ObjModify(file, iP, oP, suffix), factor(f){};

    bool copyData();

private:
    string nor_v(const string& ) const;
    const double factor;
};

inline string ObjNormalize::nor_v(const string &line) const
{
    istringstream is(line);
    ostringstream os;
    string dump;
    is >> dump;
    os << dump << " ";
    double num;
    while(is >> num)
    {
        num *= factor;
        os << num << " ";
    }
    return os.str();
}
#endif // OBJNORMALIZE_H

