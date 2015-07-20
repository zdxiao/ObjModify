
#ifndef OBJMODIFY_H
#define OBJMODIFY_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class ObjModify
{
public:

	ObjModify(string file, string iP = ".", string oP = ".", string suffix = "_vf.obj"):
	        filename(file), inputPath(iP), outputPath(oP), fileSuffix(suffix)
	{
		vertices  = 0;
		triangles = 0;
		firstVerticesLine = 0;
	}
	bool                getInfo();
	virtual bool        copyData();

	static const int    InfoMaxLine = 20;

	string	            filename;
	string              inputPath;
	string              outputPath;
	const string        fileSuffix;
	int		            vertices;
	int		            triangles;
	int                 firstVerticesLine;
    enum                State{modify = 0, normalize = 1};
	virtual             ~ObjModify(){};

protected:
	string              getName() const;
	void                setHeader(ofstream& );
	string              cov_f(const string&) const;
    ifstream&           open_file(ifstream &in, const string &file);
};

/**
	* @brief 将包含f的行（面片信息）转换成只包含点坐标索引信息，如 将f 2/73 4/99 5/33.5 转换为 f 2 4 5
	* @param 输入的f行字符串
	* @return 转换后的字符串
	*/
inline string ObjModify::cov_f(const string &fstr) const
{
	string cov_fstr;
	for(auto iter = fstr.begin(); iter < fstr.end(); )
	{
		while(iter != fstr.end() && *iter != '/')
		{
			cov_fstr.push_back(*iter++);
		}
		cov_fstr.push_back(' ');
		while(iter != fstr.end() && *iter++ != ' ');
	}
	return cov_fstr;
}

/**
 * @brief 获取obj文件基本信息，如点个数vertices,和三角面片个数triangles
 * @param 空
 * @return 非零代表获取信息成功，0代表获取信息失败
 */
inline bool ObjModify::getInfo()
{
	ifstream infile;
	string filePathName = inputPath + "\\" + filename;
	open_file(infile, filePathName.c_str());
    for(int i = 0; !infile && i != 10; ++i)
    {
        open_file(infile, filePathName.c_str());
    }
	//open_file(infile, filename.c_str());

	if(!infile)
	{
		cerr << "Try 10 times, but still cannot open file: " << filePathName << endl;
		return false;
	}
	string line, word;
	for(int i = 0; getline(infile, line) && i != InfoMaxLine; ++i)
	{
		//cout << line << endl;
		istringstream lineStream(line);
		while(lineStream >> word)
		{
			if(word == "vertices")
			{
				string dump;
				istringstream verticesStream(line);
				verticesStream >> dump >> vertices;
				if(!verticesStream)/// 检查输入错误
                {
                    vertices = 0;
                    cerr << "Reading vertices number gets wrong!" << endl;
                    return false;
                }
			}
			else if(word == "triangles" || word == "triangular")
			{
				string dump;
				istringstream triangularsStream(line);
				triangularsStream >> dump >> triangles;
				if(!triangularsStream)/// 检查输入错误
                {
                    triangles = 0;
                    cerr << "Reading triangles number gets wrong!" << endl;
                    return false;
                }
			}
			else if(word == "v")
			{
				firstVerticesLine = i + 1;
			}
		}
		// 如果找到了第一个点坐标数据，退出
		if(firstVerticesLine)
		{
			break;
		}
	}
	infile.close();
	if(triangles < 1 || vertices < 1)
	{
		cerr << "Cannot get the obj files information." << endl
			<< "Please check the OBJ input file formate" << endl;
		return false;
	}
	return true;
}

inline ifstream& ObjModify::open_file(ifstream &in, const string &file)
{
    in.close();
    in.clear();
    in.open(file.c_str());
    return in;
}

#endif
