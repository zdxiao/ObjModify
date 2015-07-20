/**
 * �ļ��� OBJMODIFY.CPP
 * �汾�� v1.0
 * �������ڣ� 2015/4/8
 * ���ߣ�Ф��
 * ���ܣ�ObjModify ���Ա�����Ķ���
 */

#include "ObjModify.h"

/**
 * @brief ��ȡobj�ļ���ȥ.obj��׺���ļ���
 * @param ��
 * @return string ���͵��ļ�����ȥ.obj��׺��
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
 * @brief ���������obj�ļ��ļ�ͷ������obj������Ϣ��ע��
 * @param ����ļ��ļ���
 * @return ��
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
 * @brief ����obj�ļ����ݣ�ֻ����v���ֺ�ֻ������Ϣ��f
 * @param ��
 * @return ��������Ƴɹ���0������ʧ��
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
	ofstream outfile(newFilename.c_str(), ofstream::out);//���û�ж�Ӧ�ļ����ڸ�Ŀ¼�´���һ��
    for(int i = 0; !infile && i != 10; ++i)
    {
        open_file(infile, filePathName.c_str());
    }
	if(!infile)
	{
		cerr << "Try 10 times, but still can not open input file�� " << filePathName << endl;
		return false;
	}
	if(!outfile)
	{
		cerr << "Can not open output file�� " << newFilename << endl;
		return false;
	}
    ///д��obj�ļ�ͷ
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
			//��ʾ�Ѵ���ٷֱ�
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
			//��ʾ�Ѵ���ٷֱ�
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
