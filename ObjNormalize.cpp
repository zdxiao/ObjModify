#include "ObjNormalize.h"

bool ObjNormalize::copyData()
{
	if(!firstVerticesLine)
	{
		getInfo();
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
		    line = nor_v(line);
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
			outfile << fstr;
			if(r_count != triangles)
            {
                outfile << endl;
            }
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
