
#include "stdafx.h"
#include "MiniSQL.h"
#include "catalog_manager.h"

using namespace std;

//The following function ReadCatalog is used to construct the table_list and the index_list
//	according to the catalog files

void CCatalogManager::ReadCatalog()
{
	char buffer[100];
	FILE *fp;
	int tag = 0;								//tag of the table list to show whether it is 
												//	the first time to create node
	int tagA = 0;								//tag of the attribute list
	int tagI = 0;								//tag of the index list
	int attrtype = 0;
	int length = 0;								//total length of the existing items in the 
												//	table node
	
	TablePointer ptrTable = NULL;				//the moving pointer in the table_list	
	AttrPointer ptrAttr = NULL;					//the moving pointer in the attr_list

	IndexPointer ptrIndex = NULL;				//the moving pointer in the index_list

	m_tablehead = m_tabletail = NULL;
	m_indextail = m_indextail = NULL;

	//create the table list

	if ((fp = fopen("tables.catlog","r")) == NULL)//file does not exist
	{
		fp = fopen("tables.catlog","w");			//create the file
		fprintf(fp,"-1\n-1\n");
	}

	fclose(fp);

	fp = fopen("tables.catlog","r");
	fscanf(fp,"%s",buffer);
	while(strcmp(buffer,"-1")!=0)				//file is not blank
	{
		if(tag == 0)							//the first time to create the table node
		{
			m_tablehead = new TableNode;

			ptrTable = m_tablehead;
			ptrTable->AllAttrs = NULL;
			ptrTable->Next = NULL;
			m_tabletail = m_tablehead;			

			tag = 1;
		}
		else
		{
			ptrTable->Next = new TableNode;
			ptrTable = ptrTable->Next;
			ptrTable->Next = NULL;
			m_tabletail = ptrTable;
		}

		strcpy(ptrTable->TableName,buffer);

		fscanf(fp,"%s",buffer);
		strcpy(ptrTable->primarykey,buffer);

		fscanf(fp,"%s",buffer);
		ptrTable->number = atoi(buffer);
		
		fscanf(fp,"%s",buffer);

		while(strcmp(buffer,"-1")!=0)
		{
			if (tagA == 0)
			{
				ptrTable->AllAttrs = new AttrNode;
				ptrAttr = ptrTable->AllAttrs;
				tagA = 1;
			}
			else 
			{
				ptrAttr->Next = new AttrNode;
				ptrAttr = ptrAttr->Next;
			}

			strcpy(ptrAttr->AttrName,buffer);

			ptrAttr->offset = length;				//for the first while loop,it is the first
													//	attribute,so the offset of the attribute
													//	is the original value of length,0

			if(strcmp((ptrAttr->AttrName),(ptrTable->primarykey)) == 0)
				ptrAttr->IsPrimarykey = 1;
			else 
				ptrAttr->IsPrimarykey = 0;

			fscanf(fp,"%s",buffer);
			if((strcmp(buffer,"INT"))==0)
			{
				ptrAttr->type = INT;
				ptrAttr->length = 10;
				length = length + 10;
			}	
			else if((strcmp(buffer,"FLOAT"))==0)
			{
				ptrAttr->type = FLOAT;
				ptrAttr->length = 20;
				length = length + 20;
			}
			else
			{
				fscanf(fp,"%d",&attrtype);
				ptrAttr->type = CHAR;
				ptrAttr->length = attrtype;
				length = length + attrtype;
			}

			fscanf(fp,"%d",&attrtype);
			ptrAttr->IsUnique = attrtype;

			fscanf(fp,"%s",buffer);
		}
		ptrAttr->Next = NULL;
		tagA = 0;
		fscanf(fp,"%s",buffer);
		length = 0;
	}
	fclose(fp);

	//create the index list
	
	if ((fp = fopen("index.catlog","r")) == NULL)	//file does not exist
	{
		fp = fopen("index.catlog","w");			//create the file
		fprintf(fp,"-1\n");
	}

	fclose(fp);

	fp = fopen("index.catlog","r");
	fscanf(fp,"%s",buffer);
	while((strcmp(buffer,"-1")) != 0)					//the item is not equal to -1
	{
		if(tagI==0)
		{
			m_indextail = new IndexNode;

			ptrIndex = m_indextail;		
			ptrIndex->Next = NULL;
			m_indextail = m_indextail;			

			tagI = 1;
		}
		else
		{
			ptrIndex->Next = new IndexNode;
			ptrIndex = ptrIndex->Next;
			ptrIndex->Next = NULL;
			m_indextail = ptrIndex;
		}

		strcpy(ptrIndex->IndexName,buffer);
		fscanf(fp,"%s",buffer);

		strcpy(ptrIndex->TableName,buffer);
		fscanf(fp,"%s",buffer);

		strcpy(ptrIndex->AttrName,buffer);
		fscanf(fp,"%s",buffer);
	}

	fclose(fp);
}

//The following function UpdateCatalog is used to write back the content of 
//the table_list and the index_list to the according catalog file

void CCatalogManager::UpdateCatalog()
{
	char buffer[100];
	FILE* fp;
		
	if(m_tablehead != NULL){							//if NULL,.cat file has just been 
														//	constructed in readcatalog,so 
														//	no use to update 

	TablePointer ptrTable = m_tablehead;				//the moving pointer in the table list
	AttrPointer ptrAttr = ptrTable->AllAttrs;			//the moving pointer in the attrlist
	
	IndexPointer ptrIndex = m_indextail;
	
	TablePointer tempT = ptrTable;						//the pointers used for the free of the
	AttrPointer tempA = ptrAttr;						//	table_list
	
	IndexPointer temp = ptrIndex;						//the pointer used for the free of the 
														//	index_list
	int attrtype = 0;
	unsigned int len = 0;
	int tagP = 0;										

	//write back the table_list to the according catalog file

	if ((fp = fopen("tables.catlog","r")) == NULL)				//file does not exist
	{	
		fp = fopen("tables.catlog","w");						//create the file
	}	
	fclose(fp);

	fp = fopen("tables.catlog","w");							//begin to write file
	
	while(ptrTable != NULL)
	{
		strcpy(buffer,ptrTable->TableName);				//table name
		fprintf(fp,buffer);
		fprintf(fp,"\n");
		
		strcpy(buffer,ptrTable->primarykey);			//table primary key
		fprintf(fp,buffer);
	
		fprintf(fp,"\n");
		
		_itoa(ptrTable->number,buffer,10);
		fprintf(fp,buffer);
		fprintf(fp,"\n");
		
		
		ptrAttr = ptrTable ->AllAttrs;
		while(ptrAttr != NULL)							//the table is not blank
		{
			strcpy(buffer,ptrAttr->AttrName);			//first attribute name
			fprintf(fp,buffer);
			fprintf(fp,"\n");

			if(ptrAttr->type == INT)
			{
				fprintf(fp,"INT");
				fprintf(fp,"\n");
			}
			else if(ptrAttr->type == FLOAT)
			{
				fprintf(fp,"FLOAT");
				fprintf(fp,"\n");
			}
			else 
			{
				fprintf(fp,"CHAR");
				fprintf(fp,"\n");
				len = ptrAttr->length;
				fprintf(fp,"%u",len);
				fprintf(fp,"\n");
			}

			tagP = ptrAttr->IsUnique;
			fprintf(fp,"%d",tagP);
			fprintf(fp,"\n");

			ptrAttr = ptrAttr->Next;
		}
		fprintf(fp,"-1\n");
		ptrTable = ptrTable->Next;

	}
	fprintf(fp,"-1\n");

	fclose(fp);

	//free the table_list memory

	ptrTable = m_tablehead;
	ptrAttr = ptrTable->AllAttrs;

	while(ptrTable != NULL)
	{
		while(ptrAttr != NULL)
		{
			ptrAttr = ptrAttr->Next;
			delete tempA;
			tempA = ptrAttr;
		}
		delete ptrAttr;
		
		ptrTable = ptrTable->Next;
		delete tempT;
		tempT = ptrTable;
	}

	//write back the index_list to the according catalog file

	if ((fp = fopen("index.catlog","r")) == NULL)				//file does not exist
	{	
		fp = fopen("index.catlog","w");						//create the file
	}	
	fclose(fp);

	fp = fopen("index.catlog","w");							//begin to write file
	
	while(ptrIndex != NULL)
	{
		strcpy(buffer,ptrIndex->IndexName);
		fprintf(fp,buffer);
		fprintf(fp,"\n");

		strcpy(buffer,ptrIndex->TableName);
		fprintf(fp,buffer);
		fprintf(fp,"\n");

		strcpy(buffer,ptrIndex->AttrName);
		fprintf(fp,buffer);
		fprintf(fp,"\n");

		ptrIndex = ptrIndex->Next;
	}

	fprintf(fp,"-1\n");

	fclose(fp);

	//free the index_list memory

	ptrIndex = m_indextail;

	while(ptrIndex != NULL)
	{
		ptrIndex = ptrIndex->Next;
		delete temp;
		temp = ptrIndex;
	}
	}
}

short int CCatalogManager::IsTableExists(CString tablename)
{
	TablePointer ptrTable  = m_tablehead;
	
	while(ptrTable != NULL)
	{
		if(strcmp(ptrTable->TableName,tablename) == 0)
			return 1;
		else
			ptrTable = ptrTable->Next;
	}
	return 0;
}

short int CCatalogManager::DeleteTableInfo(CString tablename)
{
	int i = 0;
	TablePointer ptrTable  = m_tablehead;
	TablePointer temp = m_tablehead;
	while(ptrTable != NULL)
	{
		if(strcmp(ptrTable->TableName,tablename) == 0)
		{
			if(ptrTable == m_tablehead)
			{
				m_tablehead = ptrTable->Next;
				delete ptrTable;
				return 1;
			}
			else if(ptrTable == m_tabletail)
			{
				temp->Next = NULL;
				m_tabletail = temp;
				delete ptrTable;
				return 1;
			}
			else
			{
				temp->Next = ptrTable->Next;
				delete ptrTable;
				return 1;
			}
		}

		else
		{
			temp = ptrTable;
			ptrTable = ptrTable->Next;
		}
	
	}
	return 0;
}

short int CCatalogManager::IsIndexExists(CString indexname)
{
	IndexPointer ptrIndex = m_indextail;

	while(ptrIndex != NULL)
	{
		if(strcmp(ptrIndex->IndexName,indexname) == 0)
			return 1;
		else
			ptrIndex = ptrIndex->Next;
	}
	return 0;
}

short int CCatalogManager::DeleteIndexInfo(CString indexname)
{
	IndexPointer ptrIndex  = m_indextail;
	IndexPointer temp = m_indextail;
	while(ptrIndex != NULL)
	{
		if(strcmp(ptrIndex->IndexName,indexname) == 0)
		{
			if(ptrIndex == m_indextail)
			{
				m_indextail = ptrIndex->Next;
				delete ptrIndex;
				return 1;
			}
			else if(ptrIndex == m_indextail)
			{
				temp->Next = NULL;
				m_indextail = temp;
				delete ptrIndex;
				return 1;
			}
			else
			{
				temp->Next = ptrIndex->Next;
				delete ptrIndex;
				return 1;
			}
		}
		else
		{
			temp = ptrIndex;
			ptrIndex = ptrIndex->Next;
		}
	}
	return 0;
}

short int CCatalogManager::IsAttrExists(CString tablename, CString attrname)
{
	if(IsTableExists(tablename) == 1)
	{
		TablePointer ptrTable  = m_tablehead;
		TablePointer temp = NULL;
		AttrPointer tempattr = NULL;
		
		while(ptrTable != NULL)
		{
			if(strcmp(ptrTable->TableName,tablename) == 0)
			{
				temp = ptrTable;								//temp指向参数给定的表
				ptrTable = NULL;
			}
			else
				ptrTable = ptrTable->Next;
		}
		tempattr = temp -> AllAttrs;
		while(tempattr != NULL)
		{
			if(strcmp(tempattr -> AttrName,attrname) == 0)
				return 1;
			else
				tempattr = tempattr -> Next;
		}
	}
	return 0;
}

short int CCatalogManager::GetAttrTypeByName(CString tablename, CString attrname)
{
	TablePointer ptrTable  = m_tablehead;
	TablePointer temp = NULL;
	AttrPointer tempattr; 

	if(ptrTable != NULL)									//table list at least has one node
	{
		while(ptrTable != NULL)
		{
			if(strcmp(ptrTable->TableName,tablename) == 0)
			{
				temp = ptrTable;								//temp指向参数给定的表
				ptrTable = NULL;
			}
			else
				ptrTable = ptrTable->Next;
		}
		tempattr = temp -> AllAttrs;
		while(tempattr != NULL)
		{
			if(strcmp(tempattr->AttrName,attrname) == 0)
			{
				return tempattr->type;
			}
			else
			{
				tempattr = tempattr->Next;
			}
		}
	}
	return 0;
}

short int CCatalogManager::GetAttrTypeByNum(CString tablename, unsigned int attrnum)
{
	int i = 0;
	TablePointer ptrTable  = m_tablehead;
	TablePointer temp = NULL;
	AttrPointer tempattr;

	if(ptrTable != NULL)
	{
		while(ptrTable != NULL)
		{
			if(strcmp(ptrTable->TableName,tablename) == 0)
			{
				temp = ptrTable;								//temp指向参数给定的表
				ptrTable = NULL;
			}
			else
			{
				ptrTable = ptrTable->Next;
			}
		}
		tempattr=temp->AllAttrs;
		while(tempattr != NULL)
		{
			for(i=0;i<(int)attrnum;i++)
			{
				tempattr = tempattr->Next;
			}
			return tempattr->type;
		}
	}
	return 0;
}

unsigned int CCatalogManager::GetAttrOffset(CString tablename, CString attrname)
{
	TablePointer ptrTable  = m_tablehead;
	TablePointer temp = NULL;
	AttrPointer tempattr;

	if(ptrTable != NULL)
	{
		while(ptrTable != NULL)
		{
			if(strcmp(ptrTable->TableName,tablename) == 0)
			{
				temp = ptrTable;								//temp指向参数给定的表
				ptrTable = NULL;
			}
			else
				ptrTable = ptrTable->Next;
		}
		tempattr=temp->AllAttrs;
		while(tempattr != NULL)
		{
			if(strcmp(tempattr->AttrName,attrname) == 0)
			{
				return tempattr->offset;
			}
			else
			{
				tempattr= tempattr->Next;
			}
		}
	}
	return 0;
}

unsigned int CCatalogManager::GetAttrLength(CString tablename, CString attrname)
{
	TablePointer ptrTable  = m_tablehead;
	TablePointer temp = NULL;
	AttrPointer tempattr;

	if(ptrTable != NULL)
	{
		while(ptrTable != NULL)
		{
			if(strcmp(ptrTable->TableName,tablename) == 0)
			{
				temp = ptrTable;								//temp指向参数给定的表
				ptrTable = NULL;
			}
			else
				ptrTable = ptrTable->Next;
		}
		tempattr=temp->AllAttrs;
		while(tempattr != NULL)
		{
			if(strcmp(tempattr->AttrName,attrname) == 0)
			{
				return tempattr->length;
			}
			else
			{
				tempattr = tempattr->Next;
			}
		}
	}
	return 0;
}

unsigned int CCatalogManager::GetRecordLength(CString tablename)
{
	int len = 0;
	TablePointer ptrTable  = m_tablehead;
	TablePointer temp = NULL;
	AttrPointer tempattr;

	if(ptrTable != NULL)
	{
		if(IsTableExists(tablename) == 1)
		{
			while(ptrTable != NULL)
			{
				if(strcmp(ptrTable->TableName,tablename) == 0)
				{
					temp = ptrTable;							//temp指向参数给定的表
					ptrTable = NULL;
				}
				else
					ptrTable = ptrTable->Next;
			}
			tempattr=temp->AllAttrs;
			while(tempattr!= NULL)
			{
				len += tempattr->length;
				tempattr = tempattr->Next;
			}
		
			return len;
		}
	}
	return 0;
}

short int CCatalogManager::IsIndexCreated(CString tablename, CString attrname)
{
	IndexPointer ptrIndex = m_indextail;
	
	while(ptrIndex != NULL)
	{
		if(strcmp(ptrIndex->TableName,tablename) == 0)
			if(strcmp(ptrIndex->AttrName,attrname) == 0)
				return 1;
		ptrIndex = ptrIndex->Next;
	}

	return 0;
}

CString CCatalogManager::GetIndexName(CString tablename, CString attrname)
{
	IndexPointer ptrIndex = m_indextail;

	while(ptrIndex != NULL)
	{
		if(strcmp(ptrIndex->TableName,tablename) == 0)
		{
			if(strcmp(ptrIndex->AttrName,attrname) == 0)
				return ptrIndex->IndexName;
			else 
				ptrIndex = ptrIndex->Next;
		}
		else
			ptrIndex = ptrIndex->Next;
	}

	return CString("");
}

unsigned int CCatalogManager::GetAttrNum(CString tablename)
{
	int num = 0;
	TablePointer ptrTable  = m_tablehead;
	AttrPointer ptrAttr;
	TablePointer temp = NULL;
	
	if(IsTableExists(tablename) == 1)
	{
		while(ptrTable != NULL)
		{
			if(strcmp(ptrTable->TableName,tablename) == 0)
			{
				temp = ptrTable;								//temp指向参数给定的表
				ptrTable = NULL;
			}
			else
				ptrTable = ptrTable->Next;
		}
		ptrAttr = temp ->AllAttrs;
		while(ptrAttr!= NULL)
		{
			num ++;
			ptrAttr = ptrAttr->Next;
		}		
		return num;
	}
	return 0;
}

short int CCatalogManager::GetAttrInfo(CString tablename, unsigned int attrnum, column* tempcol)
{
	int i = 0;
	TablePointer ptrTable  = m_tablehead;
	TablePointer temp = NULL;
	AttrPointer tempattr;
	if(ptrTable != NULL)
	{
		while(ptrTable != NULL)
		{
			if(strcmp(ptrTable->TableName,tablename) == 0)
			{
				temp = ptrTable;								//temp指向参数给定的表
				ptrTable = NULL;
			}
			else
			{
				ptrTable = ptrTable->Next;
			}
		}
		tempattr=temp->AllAttrs;
		if(tempattr != NULL)
		{
			for(i=0;i<(int)attrnum;i++)
			{
				tempattr = tempattr->Next;
			}
			
			tempcol->collength = tempattr ->length;
			strcpy(tempcol->colname,tempattr->AttrName);
			tempcol->coloffset =tempattr->offset;
			tempcol->IsPrimary = tempattr->IsPrimarykey;
			tempcol->IsUnique = tempattr->IsUnique;
			tempcol->type = tempattr->type;

			return 1;
		}
	}
	return 0;
}

short int CCatalogManager::CreateTableInfo(CString tablename, column* cols)
{
	AttrPointer ptrAttr = NULL;	
	column* temp = cols;									//save the initial colpointer
	int tagA = 0;
	int len = 0;

	TablePointer ptrTable = new TableNode;
	TablePointer ptr = m_tablehead;
	if(m_tabletail != NULL)
	{
		m_tabletail->Next = ptrTable;
		m_tabletail = ptrTable;
		ptrTable->Next = NULL;
	}
	else
	{
		m_tabletail = ptrTable;
		m_tablehead = ptrTable;
		ptrTable->Next = NULL;
	}
	
	strcpy(ptrTable->TableName,tablename);					//copy the tablename
		
	ptrTable->number = 0;
	while((cols->IsPrimary == 0) && (cols->next != NULL))	//copy the primarykey
	{
		cols = cols->next;
	}
	if((cols->IsPrimary == 0) && (cols->next == NULL))
	{
		strcpy(ptrTable->primarykey,"NULL");
	}
	else
	{
		strcpy(ptrTable->primarykey,cols->colname);
	}
	cols = temp;

	while(cols != NULL)										//copy the attrbutes
	{
		if(tagA == 0)
		{
			ptrTable->AllAttrs = new AttrNode;
			ptrAttr = ptrTable->AllAttrs;
			tagA = 1;
		}
		else
		{
			ptrAttr->Next = new AttrNode;
			ptrAttr = ptrAttr->Next;
		}

		strcpy(ptrAttr->AttrName,cols->colname);
		ptrAttr->IsPrimarykey = cols->IsPrimary;
		ptrAttr->IsUnique = cols->IsUnique;
		ptrAttr->length = cols->collength;
		ptrAttr->type = cols->type;

		if(cols->type == INT)
		{
			ptrAttr->offset = len;
			len = len + 10;
		}
		else if (cols->type == FLOAT)
		{
			ptrAttr->offset = len;
			len = len + 20;
		}
		else
		{
			ptrAttr->offset = len;
			len = len + cols->collength;
		}

		cols = cols->next;
	}
	ptrAttr->Next = NULL;
	cols = temp;

	return 1;
}

short int CCatalogManager::CreateIndexInfo(CString indexname, CString tablename, column* cols)
{
	IndexPointer ptrIndex = new IndexNode;
	
	if(m_indextail != NULL)
	{
		m_indextail->Next = ptrIndex;
		m_indextail = ptrIndex;
		ptrIndex->Next = NULL;
	}
	else
	{
		m_indextail = ptrIndex;
		m_indextail = ptrIndex;
		ptrIndex->Next = NULL;
	}
	strcpy(ptrIndex->IndexName,indexname);
	strcpy(ptrIndex->TableName,tablename);
	strcpy(ptrIndex->AttrName,cols->colname);

	return 1;
}

short int CCatalogManager::IsAttrUnique(CString tablename, CString attrname)
{
	TablePointer ptrTable = m_tablehead;
	TablePointer temp = NULL;
	AttrPointer tempattr;
	if(ptrTable != NULL)
	{
		while(ptrTable != NULL)
		{
			if(strcmp(ptrTable->TableName,tablename) == 0)
			{
				temp = ptrTable;								//temp指向参数给定的表
				ptrTable = NULL;
			}
			else
				ptrTable = ptrTable->Next;
		}
		tempattr=temp->AllAttrs;
		while(tempattr != NULL)
		{
			if(strcmp(tempattr->AttrName,attrname) == 0)
			{
				if(tempattr->IsUnique == 1)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				tempattr = tempattr->Next;
			}
		}
	}
	return 0;
}

unsigned int CCatalogManager::GetRecordNumber(CString tablename)
{
	TablePointer ptrTable  = m_tablehead;
	TablePointer temp = NULL;

	if(ptrTable != NULL)
	{
		while(ptrTable != NULL)
		{
			if(strcmp(ptrTable->TableName,tablename) == 0)
			{
				temp = ptrTable;								//temp指向参数给定的表
				ptrTable = NULL;
			}	
			else
				ptrTable = ptrTable->Next;
		}

		return temp->number;
	}
	return 0;
}

void CCatalogManager::RecordNumDel(CString tablename, unsigned int deleted)
{
	TablePointer ptrTable  = m_tablehead;
	TablePointer temp = NULL;

	if(ptrTable != NULL)
	{
		while(ptrTable != NULL)
		{
			if(strcmp(ptrTable->TableName,tablename) == 0)
			{
				temp = ptrTable;								//temp指向参数给定的表
				ptrTable = NULL;
			}	
			else
				ptrTable = ptrTable->Next;
		}

		temp->number = temp->number - deleted;
	}
}

void CCatalogManager::RecordNumAdd(CString tablename, unsigned int added)
{
	TablePointer ptrTable  = m_tablehead;
	TablePointer temp = NULL;

	if(ptrTable != NULL)
	{
		while(ptrTable != NULL)
		{
			if(strcmp(ptrTable->TableName,tablename) == 0)
			{
				temp = ptrTable;								//temp指向参数给定的表
				ptrTable = NULL;
			}	
			else
				ptrTable = ptrTable->Next;
		}

		temp->number = temp->number + added;
	}
}