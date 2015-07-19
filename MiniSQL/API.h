#ifndef __API_h__
#define __API_h__
#include "catalog_manager.h"
#include "interpret.h"

class API
{
public:
	API(){}
	~API(){}
	unsigned int m_operation;		//要执行的操作或错误代码,用宏表示
	CString m_tabname;		//要操作的表格名
	CString m_indname;		//要操作的索引名
	CString m_filename;		//要操作的文件名,execfile用到
	column *m_cols;				//要操作的属性链表
	condition *m_conds;			//要比较的where字句的链表
	insertvalue *m_values;		//要插入的值链表

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