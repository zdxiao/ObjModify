/**
 * 文件名 OBJMODIFY.CPP
 * 版本： v1.0
 * 创建日期： 2015/4/8
 * 作者：肖泽东
 * 功能：ObjModify 类成员函数的定义
 */

#include "ObjModify.h"

/**
 * @brief 获取obj文件除去.obj后缀的文件名
 * @param 空
 * @return string 类型的文件名（去.obj后缀）
 */
string ObjModify::getName() const
{
	string name;
	for(auto iter = filename.begin(); iter != filename.end() - 4; ++iter)
	{
		name.push_back(*iter);
	}
	return name;
}

/**
 * @brief 设置输出的obj文件文件头，包含obj基本信息和注释
 * @param 输出文件文件流
 * @return 空
 */
void ObjModify::setHeader(ofstream &objfile)
{
	if(!firstVerticesLine)
	{
		getInfo();
	}
	objfile << "# This is a obj file only include 'v' and 'f'" << endl;
	objfile << "# Email: zd.xiao@qq.com" << endl;
	objfile << "####" << endl;
	objfile << "# " << vertices << " vertices" << endl;
	objfile << "# " << triangles << " triangles" << endl;
	//objfile << "###" << endl;
	return;
}


/**
 * @brief 复制obj文件数据，只复制v部分和只含点信息的f
 * @param 空
 * @return 非零代表复制成功，0代表复制失败
 */
bool ObjModify::copyData()
{
	if(!firstVerticesLine)
	{
		if(!getInfo())
        {
            return false;
        }
	}
    string filePathName = inputPath + "\\" + filename;
    cout << filePathName << endl;
	ifstream infile;
	open_file(infile, filePathName.c_str());
	string newFilename = outputPath + "\\" + getName() + fileSuffix;
	ofstream outfile(newFilename.c_str(), ofstream::out);//如果没有对应文件，在根目录下创建一个
    for(int i = 0; !infile && i != 10; ++i)
    {
        open_file(infile, filePathName.c_str());
    }
	if(!infile)
	{
		cerr << "Try 10 times, but still can not open input file： " << filePathName << endl;
		return false;
	}
	if(!outfile)
	{
		cerr << "Can not open output file： " << newFilename << endl;
		return false;
	}
    ///写入obj文件头
	setHeader(outfile);

	string line;
	for(int i = 0; i != firstVerticesLine - 1;  ++i)
	{
		getline(infile, line);
		//cout << line << endl;
	}
	int v_count = 1, v_percent = 0;
	int r_count = 1, r_percent = 0;
	while(getline(infile, line))
	{
		if(line[0] == 'v' && line[1] == ' ')
		{
			outfile << line << endl;
			//显示已处理百分比
			if((v_count++ * 10 / vertices) != v_percent)
			{
				cout << "vertices processing " << 10 * v_percent << "%" << endl;
				v_percent = v_count * 10 / vertices;
			}
		}
		else if(line[0] == 'f' && line[1] == ' ')
		{
			string fstr;
			fstr = cov_f(line);
			outfile << fstr << endl;
			//显示已处理百分比
			if((r_count++ * 10 / triangles ) != r_percent)
			{
				cout << "triangles processing " << 10 * r_percent << "%" << endl;
				r_percent = r_count * 10 / triangles;
			}
		}
	}
	outfile.close();
	infile.close();
	return true;
}
