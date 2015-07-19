#ifndef __catalog_h__
#define __catalog_h__

#include "MiniSQL.h"

typedef struct struct_AttrNode* AttrPointer;
typedef struct struct_AttrNode
{
	char AttrName[NAMELEN];
	unsigned int length;			//length of attribute
	unsigned int offset;			//offset of the attribute in the table
	unsigned int type;				//type of the attribute
	short int IsPrimarykey;
	short int IsUnique;
	AttrPointer Next;
}AttrNode;

typedef struct struct_TableNode* TablePointer;
typedef struct struct_TableNode
{
	char TableName[NAMELEN];
	AttrPointer AllAttrs;
	char primarykey[NAMELEN];		//primary key of the table
	unsigned int number;			//number of the current record
	TablePointer Next;
}TableNode;

typedef struct struct_IndexNode* IndexPointer;
typedef struct struct_IndexNode
{
	char IndexName[NAMELEN];
	char TableName[NAMELEN];
	char AttrName[NAMELEN];
	IndexPointer Next;
}IndexNode;

class CCatalogManager
{
public:
	CCatalogManager(){
		m_tablehead = NULL;	
		m_tabletail = NULL;	
		m_indexhead = NULL;		
		m_indextail = NULL;		
	}
	~CCatalogManager(){}

	void ReadCatalog();
	void UpdateCatalog();

	short int IsTableExists(CString tablename);
	short int DeleteTableInfo(CString tablename);
	short int IsIndexExists(CString indexname);
	short int DeleteIndexInfo(CString indexname);
	short int IsAttrExists(CString tablename, CString attrname);
	short int GetAttrTypeByName(CString tablename, CString attrname);
	short int GetAttrTypeByNum(CString tablename, unsigned int attrnum);
	unsigned int GetAttrOffset(CString tablename, CString attrname);
	unsigned int GetAttrLength(CString tablename, CString attrname);
	unsigned int GetRecordLength(CString tablename);
	short int IsIndexCreated(CString tablename, CString attrname);
	CString GetIndexName(CString tablename, CString attrname);
	unsigned int GetAttrNum(CString tablename);
	short int GetAttrInfo(CString tablename, unsigned int attrnum, column *tempcol);
	short int CreateTableInfo(CString tablename, column *cols);
	short int CreateIndexInfo(CString indexname, CString tablename, column *cols);
	short int IsAttrUnique(CString tablename, CString attrname);
	unsigned int GetRecordNumber(CString tablename);
	void RecordNumDel(CString tablename, unsigned int deleted);
	void RecordNumAdd(CString tablename, unsigned int added);

protected:
	TablePointer m_tablehead;				//pointer to the head of the table_list
	TablePointer m_tabletail;				//pointer to the tail of the table_list
	IndexPointer m_indexhead;				//pointer to the head of the index_list
	IndexPointer m_indextail;				//pointer to the tail of the index_list

};

//CCatalogManager Catalog;

#endif