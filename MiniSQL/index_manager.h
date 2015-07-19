#ifndef __INDEX_MANAGER__
#define __INDEX_MANAGER__

#include "MiniSQL.h"
#include "buffer_manager.h"
#include <stack>


#define KEY_SIZE 256
#define INDEX_COUNT 16 // 每个节点指针数目
#define INFO_NODE 0
#define ROOT_NODE 1
#define INTERNAL_NODE 2
#define LEAF_NODE 4
#define BLANK_NODE 8

typedef int KeyType;
typedef int NodeType;

struct Index
{
	int blockNumber;// 块号
	int blockOffset;// 块内偏移,值为-1表示指向索引块，否则指向数据块
};

struct BNode
{
	char key[INDEX_COUNT - 1][KEY_SIZE];// 关键字
	Index index[INDEX_COUNT];// 指向子块
	KeyType keyType;// key类型
	NodeType nodeType;// 节点类型
	int keyCount;// 节点中key的数量
	char reserved[116];// 保留区域，目的是凑够4096字节
};

class CIndexManager : public CBufferManager
{
	public:
		CIndexManager()
		{
		}
		CIndexManager(CString indexname, CString tablename){
			m_tablename = tablename;
			m_indexname = indexname;
		}
		~CIndexManager(){}

		/**
		 *@brief 通过索引来查找记录并输出到控制台
		 *@param conds 表示查找的条件
		 *@param cols 表示要输出的列
		 *@param recordLength 已弃用，为保持接口不变保留
		 */
		short SelectIndex(condition* conds, column* cols, unsigned int recordLength);
		/**
		 *@brief 通过索引来查找记录并删除
		 *@param conds 表示要查找的条件
		 *@param recordLength 已弃用，为保持接口不变保留
		 */
		short DeleteIndex(condition* conds, unsigned int recordLength); 
		/**
		 *@brief 为属性建立索引
		 *@param cols 表示要建立索引的属性
		 *@param recordlen 已弃用，为保持接口不变保留
		 *@param recordnum 已弃用，为保持接口不变保留
		 */
		short CreateIndex(column *cols, unsigned int recordlen, unsigned int recordnum);
		/**
		 *@brief 插入值value的索引
		 *@param	recordPos 表示该值在record中的位置
		 *@return 返回0表示失败，返回1表示成功
		 */
		short InsertIndex(insertvalue* value, unsigned int recordPos); 

	private:
		// 比较两个关键字的大小,若a<b则返回-1,a==b返回0，a>b返回1
		int keycompare(char *a, char *b, KeyType tag);
		// 将索引插入父节点
		void insertInParent(int currentBlock, char* value, int emptyBlock);
		// 删除索引并调整b+树
		void deleteEntry(int node, char* value, Index pointer);
		// 加入到空块链表中
		void addToBlankList(int blockNum);
		// 合并两个块
		void merge(int block1, int block2, char* value, int parentBlock);
		// 重分配两个块
		void redistribution(int block1, int block2, char* value, int parentBlock);
		// 交换两个索引的值
		inline void swapIndex(Index& index1, Index& index2)
		{
			Index temp;
			temp = index1;
			index1 = index2;
			index2 = temp;
		}
		// 交换两个key的值
		inline void swapKey(char* key1, char* key2)
		{
			char temp[256];
			strcpy(temp, key1);
			strcpy(key1, key2);
			strcpy(key2, temp);
		}
	public:
		CString m_tablename;
		CString m_indexname;
	private:
		std::stack<int> parentNode;// 记录父节点在文件中的偏移量
};

#endif //__INDEX_MANAGER__
