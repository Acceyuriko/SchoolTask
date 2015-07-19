#ifndef __API_h__
#define __API_h__
#include "catalog_manager.h"
#include "interpret.h"

class API
{
public:
	API(){}
	~API(){}
	unsigned int m_operation;		//Ҫִ�еĲ�����������,�ú��ʾ
	CString m_tabname;		//Ҫ�����ı����
	CString m_indname;		//Ҫ������������
	CString m_filename;		//Ҫ�������ļ���,execfile�õ�
	column *m_cols;				//Ҫ��������������
	condition *m_conds;			//Ҫ�Ƚϵ�where�־������
	insertvalue *m_values;		//Ҫ�����ֵ����

	void Exefrominterprt(CInterpret parsetree);
	void ExecSelect();
	void ExecDelete();
	void ExecDropTable();
	void ExecDropIndex();
	void ExecCreateTable();
	void ExecCreateIndex();
	void ExecInsert();
	void ExecFile();
	void Execute();
	void initcol(column *p);
	void DumpTree();
	short int IsInt(const char *);
	short int IsFloat(char *input);
};

#endif