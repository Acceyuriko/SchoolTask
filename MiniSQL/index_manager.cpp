#include "stdafx.h"
#include "index_manager.h"
#include "record_manager.h"
#include <iostream>
#include <math.h>
using namespace std;

int CIndexManager::keycompare(char* a, char* b, KeyType tag)
{
	switch (tag)
	{
	case CHAR:
		if (strcmp(a, b) < 0)
		{
			return -1;
		}
		else if (strcmp(a, b) == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	case INT:
		int ikey1, ikey2;
		ikey1 = atoi(a);
		ikey2 = atoi(b);
		if (ikey1 < ikey2)
		{
			return -1;
		}
		else if (ikey1 == ikey2)
		{
			return 0;
		}
		else
		{
			return 1;
		}

	case FLOAT:
		double fkey1, fkey2;
		fkey1 = atof(a);
		fkey2 = atof(b);
		if (fkey1 < fkey2)
		{
			return -1;
		}
		else if (fkey1 == fkey2)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	case NOTCHAR:
		fkey1 = atof(a);
		fkey2 = atof(b);
		if (fkey1 < fkey2)
		{
			return -1;
		}
		else if (fkey1 == fkey2)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	default:
		return 2;
	}
}

short CIndexManager::CreateIndex(column* cols, unsigned int recordlen, unsigned int recordnum)
{
	CString this_index, this_table;
	this_table.Format("%s%s", m_tablename, ".tab");

	int currentBlock = get_block(TABLE, this_table, 1);
	m_ptheblocks[currentBlock].m_being_used = 1;
	RecordBlock* prb = (RecordBlock*)m_ptheblocks[currentBlock].m_address;

	for (;;)
	{
		insertvalue value;
		value.type = cols->type;
		value.length = cols->collength;
		value.next = NULL;

		// 获得record
		short record = prb->firstRecord;
		while (record != NULL)
		{
			char* p = (char*)prb;
			strcpy(value.value, p + record + 2 + cols->coloffset);
			InsertIndex(&value, m_ptheblocks[currentBlock].m_offset_number*BLOCK_SIZE + record);

			// 取下一条record
			record = *(short*)(p + record);
		}
		// 取下一块
		if (prb->eof == TRUE)
		{
			break;
		}
		else
		{
			m_ptheblocks[currentBlock].used_block();
			currentBlock = get_block(TABLE, this_table, m_ptheblocks[currentBlock].m_offset_number + 1);
			m_ptheblocks[currentBlock].m_being_used = 1;
			prb = (RecordBlock*)m_ptheblocks[currentBlock].m_address;
		}
	}
	return 0;
}

short CIndexManager::InsertIndex(insertvalue* value, unsigned int recordPos)
{
	CString this_index;
	this_index.Format("%s%s", m_indexname, ".idx");

	int currentBlock = get_block(INDEX, this_index, 0);
	m_ptheblocks[currentBlock].m_being_used = 1;
	BNode* pbn = (BNode*)m_ptheblocks[currentBlock].m_address;
	// 若索引中没有数据则初始化，第0块不存索引，维持一个空块链表，只起找到根节点的作用，默认根节点为第1块
	int nextBlock;
	if (pbn->nodeType == NEW_BLOCK)
	{
		pbn->index[0].blockNumber = 1;// 用于找到根节点
		pbn->index[INDEX_COUNT - 1].blockNumber = 2;// 用于维持空块链表,这里是链表头
		pbn->nodeType = INFO_NODE;
		nextBlock = 1;
		m_ptheblocks[currentBlock].used_block();
		m_ptheblocks[currentBlock].written_block();
	}
	else
	{
		nextBlock = pbn->index[0].blockNumber;
		m_ptheblocks[currentBlock].used_block();
	}

	// 找到根节点
	currentBlock = get_block(INDEX, this_index, nextBlock);
	m_ptheblocks[currentBlock].m_being_used = 1;
	pbn = (BNode*)m_ptheblocks[currentBlock].m_address;
	if (pbn->nodeType == NEW_BLOCK)
	{
		// 根节点是空的
		memset((char*)pbn, 0, BLOCK_SIZE);
		pbn->nodeType = ROOT_NODE | LEAF_NODE;
		pbn->keyType = value->type;
		pbn->keyCount = 1;
		strcpy(pbn->key[0], value->value);
		pbn->index[0].blockNumber = recordPos / BLOCK_SIZE;
		pbn->index[0].blockOffset = recordPos % BLOCK_SIZE;
		pbn->index[INDEX_COUNT - 1].blockNumber = NULL;
		m_ptheblocks[currentBlock].used_block();
		m_ptheblocks[currentBlock].written_block();

		return 0;
	}
	else
	{
		// 找到key所应插入的叶节点
		while ((pbn->nodeType&LEAF_NODE) != LEAF_NODE)
		{
			parentNode.push(m_ptheblocks[currentBlock].m_offset_number);// 将父节点压栈
			int nextBlock;
			int i;
			for (i = 0; i < pbn->keyCount; i++)
			{
				if (keycompare(value->value, pbn->key[i], value->type) != 1)
				{
					break;
				}
			}
			if (i == pbn->keyCount)
			{
				nextBlock = pbn->index[i].blockNumber;
			}
			else if (keycompare(value->value, pbn->key[i], value->type) == 0)
			{
				nextBlock = pbn->index[i + 1].blockNumber;
			}
			else
			{
				nextBlock = pbn->index[i].blockNumber;
			}
			m_ptheblocks[currentBlock].used_block();
			currentBlock = get_block(INDEX, this_index, nextBlock);
			m_ptheblocks[currentBlock].m_being_used = 1;
			pbn = (BNode*)m_ptheblocks[currentBlock].m_address;
		}
		// 叶节点未满,则直接插入
		if (pbn->keyCount < INDEX_COUNT - 1)
		{
			strcpy(pbn->key[pbn->keyCount], value->value);
			pbn->index[pbn->keyCount].blockNumber = recordPos/BLOCK_SIZE;
			pbn->index[pbn->keyCount].blockOffset = recordPos % BLOCK_SIZE;
			for (auto i = pbn->keyCount; i > 0; i--)
			{
				if (keycompare(pbn->key[i], pbn->key[i - 1], value->type) == -1)
				{
					swapIndex(pbn->index[i], pbn->index[i - 1]);
					swapKey(pbn->key[i], pbn->key[i - 1]);
				}
				else
				{
					break;
				}
			}
			pbn->keyCount++;
		}
		else
		{
			// 先插入数据到一个临时结点中,然后再分裂
			struct tempNode
			{
				char key[INDEX_COUNT][KEY_SIZE];
				Index index[INDEX_COUNT];
			}temp;
			for (auto i = 0; i < pbn->keyCount; i++)
			{
				strcpy(temp.key[i], pbn->key[i]);
				temp.index[i] = pbn->index[i];
			}
			strcpy(temp.key[pbn->keyCount], value->value);
			temp.index[pbn->keyCount].blockNumber = recordPos/BLOCK_SIZE;
			temp.index[pbn->keyCount].blockOffset = recordPos % BLOCK_SIZE;
			for (auto i = pbn->keyCount; i > 0; i--)
			{
				if (keycompare(temp.key[i], temp.key[i - 1], value->type) == -1)
				{
					swapIndex(temp.index[i], temp.index[i - 1]);
					swapKey(temp.key[i], temp.key[i - 1]);
				}
				else
				{
					break;
				}
			}
			// 找到空块用于分裂结点
			int infoBlock = get_block(INDEX, this_index, 0);
			m_ptheblocks[infoBlock].m_being_used = 1;
			BNode* pInfo = (BNode*)m_ptheblocks[infoBlock].m_address;

			int emptyBlock = pInfo->index[INDEX_COUNT - 1].blockNumber;
			emptyBlock = get_block(INDEX, this_index, emptyBlock);
			m_ptheblocks[emptyBlock].m_being_used = 1;
			BNode* pEmpty = (BNode*)m_ptheblocks[emptyBlock].m_address;
			// 调整空块链表，并对空块进行初始化
			if (pEmpty->nodeType != BLANK_NODE)
			{
				pInfo->index[INDEX_COUNT - 1].blockNumber++;
			}
			else
			{
				pInfo->index[INDEX_COUNT - 1].blockNumber = pEmpty->index[INDEX_COUNT - 1].blockNumber;
			}
			m_ptheblocks[infoBlock].used_block();
			m_ptheblocks[infoBlock].written_block();

			memset((char*)pEmpty, 0, BLOCK_SIZE);
			pEmpty->nodeType = LEAF_NODE;
			pEmpty->keyType = value->type;

			pEmpty->index[INDEX_COUNT - 1].blockNumber = pbn->index[INDEX_COUNT - 1].blockNumber;
			NodeType tempNodeType = pbn->nodeType;
			memset((char*)pbn, 0, BLOCK_SIZE);
			pbn->index[INDEX_COUNT - 1].blockNumber = m_ptheblocks[emptyBlock].m_offset_number;
			pbn->keyType = value->type;
			pbn->nodeType = tempNodeType;
			// 分裂
			int mid = (int)ceil(INDEX_COUNT / 2.0);
			for (auto i = 0; i < mid; i++)
			{
				pbn->index[i] = temp.index[i];
				strcpy(pbn->key[i], temp.key[i]);
				pbn->keyCount++;

				pEmpty->index[i]= temp.index[i + mid];
				strcpy(pEmpty->key[i], temp.key[i + mid]);
				pEmpty->keyCount++;
			}

			// 调整父节点
			insertInParent(currentBlock, pEmpty->key[0], emptyBlock);

			m_ptheblocks[emptyBlock].used_block();
			m_ptheblocks[emptyBlock].written_block();
		}
		m_ptheblocks[currentBlock].used_block();
		m_ptheblocks[currentBlock].written_block();
	}
	return 0;
}


void CIndexManager::insertInParent(int currentBlock, char* value, int emptyBlock)
{
	CString this_index;
	this_index.Format("%s%s", m_indexname, ".idx");

	BNode* pbn = (BNode*)m_ptheblocks[currentBlock].m_address;
	BNode* pEmpty = (BNode*)m_ptheblocks[emptyBlock].m_address;
	// 如果是根节点，则增加树高
	if ((pbn->nodeType&ROOT_NODE) == ROOT_NODE)
	{
		if ((pbn->nodeType&LEAF_NODE) == LEAF_NODE)
		{
			pbn->nodeType = LEAF_NODE;
		}
		else
		{
			pbn->nodeType = INTERNAL_NODE;
		}
		int infoBlock = get_block(INDEX, this_index, 0);
		m_ptheblocks[infoBlock].m_being_used = 1;
		BNode* pInfo = (BNode*)m_ptheblocks[infoBlock].m_address;
		// 申请空块并调整空块链表
		int newBlock = pInfo->index[INDEX_COUNT - 1].blockNumber;
		newBlock = get_block(INDEX, this_index, newBlock);
		m_ptheblocks[newBlock].m_being_used = 1;
		BNode* pNew = (BNode*)m_ptheblocks[newBlock].m_address;

		if (pNew->nodeType != BLANK_NODE)
		{
			pInfo->index[INDEX_COUNT - 1].blockNumber++;
		}
		else
		{
			pInfo->index[INDEX_COUNT - 1].blockNumber = pNew->index[INDEX_COUNT - 1].blockNumber;
		}
		pInfo->index[0].blockNumber = m_ptheblocks[newBlock].m_offset_number;
		m_ptheblocks[infoBlock].used_block();
		m_ptheblocks[infoBlock].written_block();

		memset((char*)pNew, 0, BLOCK_SIZE);
		pNew->nodeType = ROOT_NODE;
		pNew->keyType = pbn->keyType;
		strcpy(pNew->key[0], value);
		pNew->index[0].blockNumber = m_ptheblocks[currentBlock].m_offset_number;
		pNew->index[1].blockNumber = m_ptheblocks[emptyBlock].m_offset_number;
		pNew->keyCount++;

		m_ptheblocks[newBlock].used_block();
		m_ptheblocks[newBlock].written_block();

		return;
	}
	else
	{
		int parentBlock = parentNode.top();
		parentBlock = get_block(INDEX, this_index, parentBlock);
		m_ptheblocks[parentBlock].m_being_used = 1;
		parentNode.pop();

		BNode* ppb = (BNode*)m_ptheblocks[parentBlock].m_address;
		// 如果父节点有空位,直接插入
		if (ppb->keyCount < INDEX_COUNT - 1)
		{
			strcpy(ppb->key[ppb->keyCount], value);
			ppb->index[ppb->keyCount + 1].blockNumber = m_ptheblocks[emptyBlock].m_offset_number;
			for (auto i = ppb->keyCount; i > 0; i--)
			{
				if (keycompare(ppb->key[i], ppb->key[i - 1], ppb->keyType) == 1)
				{
					break;
				}
				swapKey(ppb->key[i], ppb->key[i - 1]);
				swapIndex(ppb->index[i + 1], ppb->index[i]);
			}
			ppb->keyCount++;
		}
		else
		{
			struct tempNode
			{
				char key[INDEX_COUNT][KEY_SIZE];
				Index index[INDEX_COUNT + 1];
			}temp;
			// 复制
			for (auto i = 0; i < ppb->keyCount; i++)
			{
				strcpy(temp.key[i], ppb->key[i]);
				temp.index[i].blockNumber = ppb->index[i].blockNumber;
			}
			temp.index[INDEX_COUNT-1].blockNumber = ppb->index[INDEX_COUNT-1].blockNumber;
			// 插入
			strcpy(temp.key[INDEX_COUNT - 1], value);
			temp.index[INDEX_COUNT].blockNumber = m_ptheblocks[emptyBlock].m_offset_number;
			for (auto i = INDEX_COUNT - 1; i > 0; i--)
			{
				if (keycompare(temp.key[i], temp.key[i - 1], ppb->keyType) == 1)
				{
					break;
				}
				swapKey(temp.key[i], temp.key[i - 1]);
				swapIndex(temp.index[i + 1], temp.index[i]);
			}

			// 找到空块进行分裂
			int infoBlock = get_block(INDEX, this_index, 0);
			m_ptheblocks[infoBlock].m_being_used = 1;
			BNode* pInfo = (BNode*)m_ptheblocks[infoBlock].m_address;

			emptyBlock = pInfo->index[INDEX_COUNT - 1].blockNumber;
			emptyBlock = get_block(INDEX, this_index, emptyBlock);
			m_ptheblocks[emptyBlock].m_being_used = 1;
			BNode* pEmpty = (BNode*)m_ptheblocks[emptyBlock].m_address;
			// 调整空块链表，并对空块进行初始化
			if (pEmpty->nodeType != BLANK_NODE)
			{
				pInfo->index[INDEX_COUNT - 1].blockNumber++;
			}
			else
			{
				pInfo->index[INDEX_COUNT - 1].blockNumber = pEmpty->index[INDEX_COUNT - 1].blockNumber;
			}
			m_ptheblocks[infoBlock].used_block();
			m_ptheblocks[infoBlock].written_block();

			memset((char*)pEmpty, 0, BLOCK_SIZE);
			pEmpty->nodeType = INTERNAL_NODE;
			pEmpty->keyType = ppb->keyType;

			NodeType tempNodeType = ppb->nodeType;
			memset((char*)ppb, 0, BLOCK_SIZE);
			ppb->keyType = pEmpty->keyType;
			ppb->nodeType = tempNodeType;
			// 分裂
			int mid = INDEX_COUNT / 2.0;
			for (auto i = 0; i < mid + 1; i++)
			{
				ppb->index[i].blockNumber = temp.index[i].blockNumber;
			}
			for (auto i = mid + 1; i < INDEX_COUNT + 1; i++)
			{
				pEmpty->index[i - mid - 1].blockNumber = temp.index[i].blockNumber;
			}
			for (auto i = 0; i < mid; i++)
			{
				strcpy(ppb->key[i], temp.key[i]);
				ppb->keyCount++;
			}
			for (auto i = mid + 1; i < INDEX_COUNT; i++)
			{
				strcpy(pEmpty->key[i - mid-1], temp.key[i]);
				pEmpty->keyCount++;
			}

			insertInParent(parentBlock, temp.key[mid], emptyBlock);

			m_ptheblocks[emptyBlock].used_block();
			m_ptheblocks[emptyBlock].written_block();
		}
		m_ptheblocks[parentBlock].used_block();
		m_ptheblocks[parentBlock].written_block();
	}
}

short CIndexManager::SelectIndex(condition* conds, column* cols, unsigned int recordLength)
{
	CString this_index, this_table;
	this_index.Format("%s%s", m_indexname, ".idx");
	this_table.Format("%s%s", m_tablename, ".tab");

	int currentBlock = get_block(INDEX, this_index, 0);
	m_ptheblocks[currentBlock].m_being_used = 1;
	BNode* pbn = (BNode*)m_ptheblocks[currentBlock].m_address;
	m_ptheblocks[currentBlock].used_block();
	// 第一块只存根节点的块号,来获得根节点
	currentBlock = get_block(INDEX, this_index, pbn->index[0].blockNumber);
	m_ptheblocks[currentBlock].m_being_used = 1;
	pbn = (BNode*)m_ptheblocks[currentBlock].m_address;

	while ((pbn->nodeType&LEAF_NODE) != LEAF_NODE)
	{
		int nextBlock = 0;
		int i;
		// 找到不大于value的最大的key[i]
		for (i = 0; i < pbn->keyCount; i++)
		{
			if (keycompare(conds->value, pbn->key[i], conds->type) != 1)
			{
				break;
			}
		}
		if (i == pbn->keyCount)
		{
			nextBlock = pbn->index[i].blockNumber;
		}
		else if (keycompare(conds->value, pbn->key[i], conds->type) == 0)
		{
			nextBlock = pbn->index[i + 1].blockNumber;
		}
		else
		{
			nextBlock = pbn->index[i].blockNumber;
		}
		m_ptheblocks[currentBlock].used_block();
		currentBlock = get_block(INDEX, this_index, nextBlock);
		m_ptheblocks[currentBlock].m_being_used = 1;
		pbn = (BNode*)m_ptheblocks[currentBlock].m_address;
	}
	int i;
	for (i = 0; i < pbn->keyCount; i++)
	{
		if (keycompare(conds->value, pbn->key[i], conds->type) == 0)
		{
			break;
		}
	}
	if (i == pbn->keyCount)
	{
		printf("cannot find record\n");
	}
	else
	{
		column* tempCols = cols;
		while (tempCols != NULL)
		{
			printf("%s", tempCols->colname);
			if (strlen(tempCols->colname) < tempCols->collength)
			{
				for (unsigned int j = 0; j < tempCols->collength - strlen(tempCols->colname); j++)
				{
					printf(" ");
				}
			}
			else
			{
				printf(" ");
			}
			tempCols = tempCols->next;
		}
		printf("\n");
		m_ptheblocks[currentBlock].used_block();
		currentBlock = get_block(TABLE, this_table, pbn->index[i].blockNumber);
		m_ptheblocks[currentBlock].m_being_used = 1;
		RecordBlock* prb = (RecordBlock*)m_ptheblocks[currentBlock].m_address;
		char value[256];
		tempCols = cols;
		// 打印
		while (tempCols != NULL)
		{
			strcpy(value, (char*)prb + 2 + pbn->index[i].blockOffset + tempCols->coloffset);
			printf("%s", value);
			for (unsigned int j = 0; j < tempCols->collength - strlen(value); j++)
			{
				printf(" ");
			}
			tempCols = tempCols->next;
		}
		printf("\n");
		m_ptheblocks[currentBlock].used_block();
	}
	return 0;
}

short CIndexManager::DeleteIndex(condition* conds, unsigned int recordLength)
{
	CString this_index, this_table;
	this_index.Format("%s%s", m_indexname, ".idx");
	this_table.Format("%s%s", m_tablename, ".tab");

	int currentBlock = get_block(INDEX, this_index, 0);
	m_ptheblocks[currentBlock].m_being_used = 1;
	BNode* pbn = (BNode*)m_ptheblocks[currentBlock].m_address;
	m_ptheblocks[currentBlock].used_block();
	// 第一块只存根节点的块号,来获得根节点
	currentBlock = get_block(INDEX, this_index, pbn->index[0].blockNumber);
	m_ptheblocks[currentBlock].m_being_used = 1;
	pbn = (BNode*)m_ptheblocks[currentBlock].m_address;

	while ((pbn->nodeType&LEAF_NODE) != LEAF_NODE)
	{
		parentNode.push(m_ptheblocks[currentBlock].m_offset_number);
		int nextBlock = 0;
		int i;
		// 找到不大于value的最大的key[i]
		for (i = 0; i < pbn->keyCount; i++)
		{
			if (keycompare(conds->value, pbn->key[i], conds->type) != 1)
			{
				break;
			}
		}
		if (i == pbn->keyCount)
		{
			nextBlock = pbn->index[i].blockNumber;
		}
		else if (keycompare(conds->value, pbn->key[i], conds->type) == 0)
		{
			nextBlock = pbn->index[i + 1].blockNumber;
		}
		else
		{
			nextBlock = pbn->index[i].blockNumber;
		}
		m_ptheblocks[currentBlock].used_block();
		currentBlock = get_block(INDEX, this_index, nextBlock);
		m_ptheblocks[currentBlock].m_being_used = 1;
		pbn = (BNode*)m_ptheblocks[currentBlock].m_address;
	}
	// 删除record
	Index pointer;
	for (auto i = 0; i < pbn->keyCount; i++)
	{
		if (keycompare(pbn->key[i], conds->value, conds->type) == 0)
		{
			int recordBlock = get_block(TABLE, this_table, pbn->index[i].blockNumber);
			pointer = pbn->index[i];
			m_ptheblocks[recordBlock].m_being_used = 1;
			RecordBlock* prb = (RecordBlock*)m_ptheblocks[recordBlock].m_address;
			char* p = (char*)prb;
			// 判断是否满足条件
			if (keycompare(conds->value, p + pbn->index[i].blockOffset + 2 + conds->attroffset, conds->type) != 0)
			{
				return 0;
			}
			// 要找到前一条记录，来调整链表
			short preRecord = 0;
			while (*(short*)(p + preRecord) != pbn->index[i].blockOffset)
			{
				preRecord = *(short*)(p + preRecord);
			}
			*(short*)(p + preRecord) = *(short*)(p + pbn->index[i].blockOffset);
			// 删除记录，并调整空链表
			memset(p + pbn->index[i].blockOffset, 0, prb->recordLength);
			*(short*)(p + pbn->index[i].blockOffset) = prb->firstEmpty;
			prb->firstEmpty = pbn->index[i].blockOffset;
		}
	}

	deleteEntry(currentBlock, conds->value, pointer);

	m_ptheblocks[currentBlock].used_block();
	m_ptheblocks[currentBlock].written_block();

	return 0;
}

void CIndexManager::deleteEntry(int node, char* value, Index pointer)
{
	CString this_index;
	this_index.Format("%s%s", m_indexname, ".idx");

	BNode* pbn = (BNode*)m_ptheblocks[node].m_address;
	// 删除索引信息
	for (auto i = 0; i < pbn->keyCount - 1; i++)
	{
		if (keycompare(pbn->key[i], value, pbn->keyType) == 0)
		{
			for (auto j = i; j < pbn->keyCount - 1; j++)
			{
				swapKey(pbn->key[j], pbn->key[j + 1]);
			}
		}
		if ((pbn->nodeType&LEAF_NODE) == LEAF_NODE) 
		{
			if (pbn->index[i].blockNumber == pointer.blockNumber 
				&& pbn->index[i].blockOffset == pointer.blockOffset)
			{
				for (auto j = i; j < pbn->keyCount - 1; j++)
				{
					swapIndex(pbn->index[j], pbn->index[j + 1]);
				}
			}
		}
		else 
		{
			if (pbn->index[i].blockNumber == pointer.blockNumber)
			{
				for (auto j = i; j < pbn->keyCount; j++)
				{
					swapIndex(pbn->index[j], pbn->index[j + 1]);
				}
			}
		}
	}
	strcpy(pbn->key[pbn->keyCount-1], "");
	if ((pbn->nodeType&LEAF_NODE) == LEAF_NODE)
	{
		pbn->index[pbn->keyCount-1].blockNumber = 0;
		pbn->index[pbn->keyCount-1].blockOffset = 0;
	}
	else
	{
		pbn->index[pbn->keyCount].blockNumber = 0;
		pbn->index[pbn->keyCount].blockOffset = 0;
	}

	pbn->keyCount--;

	if (((pbn->nodeType&ROOT_NODE) == ROOT_NODE)
		&& (pbn->keyCount == 0))
	{
		if ((pbn->nodeType&LEAF_NODE) != LEAF_NODE)
		{
			int infoBlock = get_block(INDEX, this_index, 0);
			m_ptheblocks[infoBlock].m_being_used = 1;
			BNode* pInfo = (BNode*)m_ptheblocks[infoBlock].m_address;

			// 删除当前的根节点
			int root = pbn->index[0].blockNumber;
			pInfo->index[0].blockNumber = root;

			m_ptheblocks[infoBlock].used_block();
			m_ptheblocks[infoBlock].written_block();

			addToBlankList(node);
		}
	}
	else if (((pbn->nodeType&ROOT_NODE) != ROOT_NODE))
	{
		// 若子节点太少则与兄弟节点合并
		if ((((pbn->nodeType&LEAF_NODE) == LEAF_NODE) && (pbn->keyCount < (int)floor(INDEX_COUNT / 2.0) - 1))
			|| ((pbn->nodeType&INTERNAL_NODE) == INTERNAL_NODE) && (pbn->keyCount < (int)ceil(INDEX_COUNT / 2.0) - 1))
		{
			int parentBlock = parentNode.top();
			parentBlock = get_block(INDEX, this_index, parentBlock);
			m_ptheblocks[parentBlock].m_being_used = 1;
			parentNode.pop();

			BNode* pParent = (BNode*)m_ptheblocks[parentBlock].m_address;
			// 找到兄弟节点
			for (auto i = 0; i < pParent->keyCount + 1; i++)
			{
				if (pParent->index[i].blockNumber == m_ptheblocks[node].m_offset_number)
				{
					if (i > 0)
					{
						int preSibling = 0;
						char value[KEY_SIZE];
						preSibling = pParent->index[i - 1].blockNumber;
						strcpy(value, pParent->key[i - 1]);

						preSibling = get_block(INDEX, this_index, preSibling);
						m_ptheblocks[preSibling].m_being_used = 1;

						merge(preSibling, node, value, parentBlock);

						m_ptheblocks[preSibling].used_block();
						m_ptheblocks[preSibling].written_block();
					}
					else if (i < pParent->keyCount)
					{
						int nextSibling = 0;
						char value[KEY_SIZE];
						nextSibling = pParent->index[i + 1].blockNumber;
						strcpy(value, pParent->key[i]);

						nextSibling = get_block(INDEX, this_index, nextSibling);
						m_ptheblocks[nextSibling].m_being_used = 1;

						merge(node, nextSibling, value, parentBlock);

						m_ptheblocks[nextSibling].used_block();
						m_ptheblocks[nextSibling].written_block();
					}
				}
			}
			m_ptheblocks[parentBlock].used_block();
			m_ptheblocks[parentBlock].written_block();
		}
	}
}

void CIndexManager::addToBlankList(int blockNum)
{
	CString this_index;
	this_index.Format("%s%s", m_indexname, ".idx");

	int infoBlock = get_block(INDEX, this_index, 0);
	m_ptheblocks[infoBlock].m_being_used = 1;
	BNode* pInfo = (BNode*)m_ptheblocks[infoBlock].m_address;
	BNode* pbn = (BNode*)m_ptheblocks[blockNum].m_address;

	memset((char*)pbn, 0, BLOCK_SIZE);
	pbn->nodeType = BLANK_NODE;
	// 调整空块指针
	pbn->index[INDEX_COUNT - 1].blockNumber = pInfo->index[INDEX_COUNT - 1].blockNumber;
	pInfo->index[INDEX_COUNT - 1].blockNumber = m_ptheblocks[blockNum].m_offset_number;

	m_ptheblocks[infoBlock].used_block();
	m_ptheblocks[infoBlock].written_block();
}

void CIndexManager::merge(int block1, int block2, char* value, int parentBlock)
{
	// block1总是在block2之前
	BNode* pPre = (BNode*)m_ptheblocks[block1].m_address;
	BNode* pbn = (BNode*)m_ptheblocks[block2].m_address;

	// 判断能否合并,不能则重新分配
	if ((pPre->nodeType&LEAF_NODE) == LEAF_NODE)
	{
		if (pPre->keyCount + pbn->keyCount > INDEX_COUNT - 1)
		{
			redistribution(block1, block2, value, parentBlock);
			return;
		}
	}
	else
	{
		if (pPre->keyCount + pbn->keyCount >= INDEX_COUNT - 1)
		{
			redistribution(block1, block2, value, parentBlock);
			return;
		}
	}
	// 合并
	if ((pbn->nodeType&LEAF_NODE) != LEAF_NODE)
	{
		strcpy(pPre->key[pPre->keyCount], value);
		pPre->keyCount++;
		for (auto i = 0; i < pbn->keyCount; i++)
		{
			strcpy(pPre->key[i + pPre->keyCount], pbn->key[i]);
			pPre->index[i + pPre->keyCount] = pbn->index[i];
		}
		pPre->keyCount += pbn->keyCount;
		pPre->index[pPre->keyCount] = pbn->index[pbn->keyCount];
	}
	else
	{
		for (auto i = 0; i < pbn->keyCount; i++)
		{
			strcpy(pPre->key[i + pPre->keyCount], pbn->key[i]);
			pPre->index[i + pPre->keyCount] = pbn->index[i];
		}
		pPre->keyCount += pbn->keyCount;
		pPre->index[INDEX_COUNT - 1] = pbn->index[INDEX_COUNT - 1];
	}
	Index pointer;
	pointer.blockNumber = m_ptheblocks[block2].m_offset_number;
	pointer.blockOffset = 0;
	deleteEntry(parentBlock, value, pointer);
	addToBlankList(block2);
	return;
}

void CIndexManager::redistribution(int block1, int block2, char* value, int parentBlock)
{
	BNode* pPre = (BNode*)m_ptheblocks[block1].m_address;
	BNode* pbn = (BNode*)m_ptheblocks[block2].m_address;
	BNode* pParent = (BNode*)m_ptheblocks[parentBlock].m_address;

	if (pPre->keyCount < pbn->keyCount)
	{
		if ((pbn->nodeType&LEAF_NODE) != LEAF_NODE)
		{
			// 调整block1
			strcpy(pPre->key[pbn->keyCount], value);
			pPre->index[pPre->keyCount + 1] = pbn->index[0];
			pPre->keyCount++;
			// 调整父节点
			for (auto i = 0; i < pParent->keyCount; i++)
			{
				if (pParent->index[i].blockNumber == m_ptheblocks[block1].m_offset_number)
				{
					strcpy(pParent->key[i], pbn->key[0]);
					break;
				}
			}
			// 调整block2
			for (auto i = 0; i < pbn->keyCount - 1; i++)
			{
				swapKey(pbn->key[i], pbn->key[i + 1]);
				swapIndex(pbn->index[i], pbn->index[i + 1]);
			}
			swapIndex(pbn->index[pbn->keyCount - 1], pbn->index[pbn->keyCount]);

			strcpy(pbn->key[pbn->keyCount - 1], "");
			pbn->index[pbn->keyCount].blockNumber = 0;
			pbn->index[pbn->keyCount].blockOffset = 0;
			pbn->keyCount--;
		}
		else
		{
			// 调整block1
			strcpy(pPre->key[pbn->keyCount], pbn->key[0]);
			pPre->index[pPre->keyCount] = pbn->index[0];
			pPre->keyCount++;
			// 调整父节点
			for (auto i = 0; i < pParent->keyCount; i++)
			{
				if (pParent->index[i].blockNumber == m_ptheblocks[block1].m_offset_number)
				{
					strcpy(pParent->key[i], pbn->key[1]);
					break;
				}
			}
			// 调整block2
			for (auto i = 0; i < pbn->keyCount - 1; i++)
			{
				swapKey(pbn->key[i], pbn->key[i + 1]);
				swapIndex(pbn->index[i], pbn->index[i + 1]);
			}
			strcpy(pbn->key[pbn->keyCount - 1], "");
			pbn->index[pbn->keyCount - 1].blockNumber = 0;
			pbn->index[pbn->keyCount - 1].blockOffset = 0;
			pbn->keyCount--;
		}
	}
	else
	{
		if ((pbn->nodeType&LEAF_NODE) != LEAF_NODE)
		{
			// 调整block2
			strcpy(pbn->key[pbn->keyCount], value);
			pbn->index[pbn->keyCount + 1] = pPre->index[pPre->keyCount];
			pbn->keyCount++;
			for (auto i = pbn->keyCount - 1; i > 0; i--)
			{
				swapKey(pbn->key[i], pbn->key[i - 1]);
				swapIndex(pbn->index[i + 1], pbn->index[i]);
			}
			swapIndex(pbn->index[0], pbn->index[1]);
			// 调整父节点
			for (auto i = 0; i < pParent->keyCount; i++)
			{
				if (pParent->index[i].blockNumber == m_ptheblocks[block1].m_offset_number)
				{
					strcpy(pParent->key[i], pPre->key[pPre->keyCount - 1]);
					break;
				}
			}
			// 调整block1
			strcpy(pPre->key[pPre->keyCount - 1], "");
			pPre->index[pPre->keyCount].blockNumber = 0;
			pPre->index[pPre->keyCount].blockOffset = 0;
			pPre->keyCount--;
		}
		else
		{
			// 调整block2
			strcpy(pbn->key[pbn->keyCount], pPre->key[pPre->keyCount - 1]);
			pbn->index[pbn->keyCount] = pPre->index[pPre->keyCount - 1];
			pbn->keyCount++;
			for (auto i = pbn->keyCount-1; i > 0; i--)
			{
				swapKey(pbn->key[i], pbn->key[i - 1]);
				swapIndex(pbn->index[i], pbn->index[i - 1]);
			}
			// 调整父节点
			for (auto i = 0; i < pParent->keyCount; i++)
			{
				if (pParent->index[i].blockNumber == m_ptheblocks[block1].m_offset_number)
				{
					strcpy(pParent->key[i], pbn->key[0]);
				}
			}
			// 调整block1
			strcpy(pPre->key[pPre->keyCount - 1], "");
			pPre->index[pPre->keyCount - 1].blockNumber = 0;
			pPre->index[pPre->keyCount-1].blockOffset = 0;
			pPre->keyCount--;
		}
	}
}