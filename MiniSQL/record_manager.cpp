    
#include "stdafx.h"
#include "record_manager.h"

using namespace std;

BOOL CRecordManager::ValueCompare(char *valueOfTable, unsigned int type, unsigned int cond, char *compvalue)
{
	switch (type)
	{
	case INT:
	{
		int intvalue;
		intvalue = atoi(valueOfTable);
		switch (cond)
		{
		case LT:
			if (intvalue < atoi(compvalue))
				return 1;
			break;
		case LE:
			if (intvalue <= atoi(compvalue))
				return 1;
			break;
		case GT:
			if (intvalue > atoi(compvalue))
				return 1;
			break;
		case GE:
			if (intvalue >= atoi(compvalue))
				return 1;
			break;
		case EQ:
			if (intvalue == atoi(compvalue))
				return 1;
			break;
		case NE:
			if (intvalue != atoi(compvalue))
				return 1;
			break;
		}
	}
		break;
	case FLOAT:
	{
		double floatvalue;
		floatvalue = atof(valueOfTable);
		switch (cond)
		{
		case LT:
			if (floatvalue < atof(compvalue))
				return 1;
			break;
		case LE:
			if (floatvalue <= atof(compvalue))
				return 1;
			break;
		case GT:
			if (floatvalue > atof(compvalue))
				return 1;
			break;
		case GE:
			if (floatvalue >= atof(compvalue))
				return 1;
			break;
		case EQ:
			if (floatvalue == atof(compvalue))
				return 1;
			break;
		case NE:
			if (floatvalue != atof(compvalue))
				return 1;
			break;
		}
	}
		break;
	case CHAR:
	{
		switch (cond)
		{
		case LT:
			if (strcmp(valueOfTable, compvalue) < 0)
				return 1;
			break;
		case LE:
			if (strcmp(valueOfTable, compvalue) <= 0)
				return 1;
			break;
		case GT:
			if (strcmp(valueOfTable, compvalue) > 0)
				return 1;
			break;
		case GE:
			if (strcmp(valueOfTable, compvalue) >= 0)
				return 1;
			break;
		case EQ:
			if (strcmp(valueOfTable, compvalue) == 0)
				return 1;
			break;
		case NE:
			if (strcmp(valueOfTable, compvalue) != 0)
				return 1;
			break;
		}
	}
		break;
	}
	return 0;

}

short CRecordManager::SelectRecord(condition* conds, column* cols, unsigned int recordlen, unsigned int recordnum)
{
	CString tableName = m_tablename + ".tab";
	tableName.Format("%s%s", m_tablename, ".tab");

	int currentBlock = get_block(TABLE, tableName, 1);
	m_ptheblocks[currentBlock].m_being_used = 1;

	// 打印表头
	column* tempcols= cols;
	while (tempcols != NULL)
	{
		printf("%s", tempcols->colname);
		if (strlen(tempcols->colname) < tempcols->collength)
		{
			for (unsigned int i = 0; i < tempcols->collength - strlen(tempcols->colname); i++)
			{
				printf(" ");
			}
		}
		tempcols = tempcols->next;
	}
	printf("\n");

	// 查找
	RecordBlock* prb = (RecordBlock*)(m_ptheblocks[currentBlock].m_address);
	for (;;)
	{
		char*p = (char*)prb;
		short currentRecord = prb->firstRecord;
		while (currentRecord != NULL)
		{
			BOOL matchFlag = TRUE;
			condition* tempConds = conds;
			while (tempConds != NULL)
			{
				char* valueOfTable = p+currentRecord+2+tempConds->attroffset;
				if (ValueCompare(valueOfTable, tempConds->type, tempConds->cond, tempConds->value) == FALSE)
				{
					matchFlag = FALSE;
				}
				tempConds = tempConds->next;
			}
			// 打印
			if (matchFlag == TRUE)
			{
				char* value = new char[256];
				tempcols = cols;
				while (tempcols != NULL)
				{
					strcpy(value, p+currentRecord+2+tempcols->coloffset);
					printf("%s", value);
					for (unsigned int i = 0; i < tempcols->collength-strlen(value); i++)
					{
						printf(" ");
					}
					tempcols = tempcols->next;
				}
				printf("\n");
			}
			currentRecord = *(short*)(p+currentRecord);
		}
		m_ptheblocks[currentBlock].used_block();
		if (prb->eof == FALSE)
		{
			currentBlock = get_block(TABLE, tableName, m_ptheblocks[currentBlock].m_offset_number+1);
			prb = (RecordBlock*)m_ptheblocks[currentBlock].m_address;
			m_ptheblocks[currentBlock].m_being_used = 1;
		}
		else
		{
			break;
		}
	}
	return 0;
}


short CRecordManager::DeleteRecord(condition* conds, unsigned int recordlen, unsigned int recordnum)
{
	CString tableName;
	tableName.Format("%s%s", m_tablename, ".tab");
	short recordDeleted = 0;

	int currentBlock = get_block(TABLE, tableName, 1);
	m_ptheblocks[currentBlock].m_being_used = 1;

	// 查找
	RecordBlock* prb = (RecordBlock*)(m_ptheblocks[currentBlock].m_address);
	if (prb->eof == NEW_BLOCK)
	{
		return 0;
	}
	for (;;)
	{
		char*p = (char*)prb;
		short currentRecord = prb->firstRecord;
		short preRecord = 0;// 记下前一个节点的位置,若前一个节点为头节点，其偏移为0
		while (currentRecord != NULL)
		{
			BOOL matchFlag = TRUE;
			condition* tempConds = conds;
			while (tempConds != NULL)
			{
				char* valueOfTable = p+currentRecord+2+tempConds->attroffset;
				if (ValueCompare(valueOfTable, tempConds->type, tempConds->cond, tempConds->value) == FALSE)
				{
					matchFlag = FALSE;
				}
				tempConds = tempConds->next;
			}
			if (matchFlag == TRUE)
			{
				// 调整记录指针
				short temp = currentRecord;
				currentRecord = *(short*)(p+currentRecord);
				*(short*)(p+preRecord) = currentRecord;
				// 删除
				memset(p+temp, 0, prb->recordLength);
				// 调整空记录指针
				*(short*)(p+temp) = prb->firstEmpty;
				prb->firstEmpty = temp;
				recordDeleted++;
			}
			else
			{
				preRecord = currentRecord;
				currentRecord = *(short*)(p+currentRecord);
			}
		}
		m_ptheblocks[currentBlock].used_block();
		m_ptheblocks[currentBlock].written_block();
		if (prb->eof == FALSE)
		{
			currentBlock = get_block(TABLE, tableName, m_ptheblocks[currentBlock].m_offset_number+1);
			prb = (RecordBlock*)m_ptheblocks[currentBlock].m_address;
			m_ptheblocks[currentBlock].m_being_used = 1;
		}
		else
		{
			break;
		}
	}
	return recordDeleted;
}

short CRecordManager::IsValueExists(column* cols, char* value, unsigned int recordLength)		
{
	CString tableName;
	tableName.Format("%s%s", m_tablename, ".tab");

	int currentBlock = get_block(TABLE, tableName, 1);
	m_ptheblocks[currentBlock].m_being_used = 1;

	// 查找
	RecordBlock* prb = (RecordBlock*)(m_ptheblocks[currentBlock].m_address);
	if (prb->eof == NEW_BLOCK)
	{
		initRecordBlock(prb, recordLength);
	}
	for (;;)
	{
		if (searchRecord(prb, cols->coloffset, value) != NULL)
		{
			return 1;
		}
		else
		{
			m_ptheblocks[currentBlock].used_block();
			if (prb->eof == TRUE)
			{
				return 0;
			}
			currentBlock = get_block(TABLE, tableName, m_ptheblocks[currentBlock].m_offset_number+1);
			m_ptheblocks[currentBlock].m_being_used = 1;
			prb = (RecordBlock*)(m_ptheblocks[currentBlock].m_address);
		}
	}
	return 0;
}

short CRecordManager::searchRecord(RecordBlock* prb, int coloffset, char* value)
{
	char*p = (char*)prb;
	short currentRecord = prb->firstRecord;
	while (currentRecord != NULL)
	{
		if ((strcmp(p+currentRecord+2+coloffset, value)) == 0)
		{
			return currentRecord;
		}
		else
		{
			currentRecord = *(short*)(p+currentRecord);
		}
	}
	return 0;
}

int CRecordManager::InsertValues(insertvalue *values, unsigned int recordLength)
{
	CString tableName;
	tableName.Format("%s%s", m_tablename, ".tab");

	int infoBlock = get_block(TABLE, tableName, 0);
	m_ptheblocks[infoBlock].m_being_used = 1;

	RecordBlock* pInfo= (RecordBlock*)(m_ptheblocks[infoBlock].m_address);

	if (pInfo->eof == NEW_BLOCK) // 开头四个字节为"$$$$",表示为新块
	{
		initRecordBlock(pInfo, 10);
		pInfo->firstEmpty = 1;// 指向空块
	}

	int currentBlock = get_block(TABLE, tableName, pInfo->firstEmpty);
	RecordBlock* prb = (RecordBlock*)(m_ptheblocks[currentBlock].m_address);
	if (prb->eof == NEW_BLOCK)
	{
		initRecordBlock(prb, recordLength);
	}
	// 寻找有空位的块
	while (prb->firstEmpty == NULL)
	{
		if (prb->eof == TRUE)
		{
			prb->eof = FALSE;
			m_ptheblocks[currentBlock].used_block();
			pInfo->firstEmpty = m_ptheblocks[currentBlock].m_offset_number+1;
			currentBlock = get_block(TABLE, tableName, m_ptheblocks[currentBlock].m_offset_number+1);
			m_ptheblocks[currentBlock].m_being_used = 1;
			prb = (RecordBlock*)(m_ptheblocks[currentBlock].m_address);
			initRecordBlock(prb, recordLength);
		}
		else
		{
			m_ptheblocks[currentBlock].m_being_used = 0;
			currentBlock = get_block(TABLE, tableName, m_ptheblocks[currentBlock].m_offset_number+1);
			m_ptheblocks[currentBlock].m_being_used = 1;
			prb = (RecordBlock*)(m_ptheblocks[currentBlock].m_address);
		}
	}
	char* p = (char*)prb;
	p += prb->firstEmpty;
	// 调整链表指针
	int recordOffset = prb->firstEmpty;	
	short temp = prb->firstRecord;
	prb->firstRecord = prb->firstEmpty;
	prb->firstEmpty = *(short*)p;
	*(short*)p = temp;
	// 插入
	p += 2;
	insertvalue* currentvalue = values;
	while(currentvalue->next != NULL)
	{
		strcpy(p, currentvalue->value);
		p += currentvalue->length;
		currentvalue = currentvalue->next;
	}
	memcpy(p, currentvalue->value, currentvalue->length);

	// 对块做标记
	m_ptheblocks[currentBlock].written_block();
	m_ptheblocks[currentBlock].used_block();
	m_ptheblocks[infoBlock].written_block();
	m_ptheblocks[infoBlock].used_block();

	// 返回record在table中的偏移量
	return m_ptheblocks[currentBlock].m_offset_number*BLOCK_SIZE+recordOffset;
}

void CRecordManager::initRecordBlock(RecordBlock* prb, unsigned int recordLength)
{
	prb->firstRecord = NULL;
	prb->recordLength = recordLength + 2;
	prb->eof = TRUE;
	prb->firstEmpty = RECORD_OFFSET;

	// 写空位链表
	char* p = (char*)prb;
	for (auto i = RECORD_OFFSET; i < BLOCK_SIZE; i += prb->recordLength)
	{
		// 最后一部分长度不足留空不用
		if (i + 2 * prb->recordLength >= BLOCK_SIZE)
		{
			*(short*)(p + i) = NULL;
		}
		else
		{
			*(short*)(p + i) = i + prb->recordLength;
		}
	}
}






