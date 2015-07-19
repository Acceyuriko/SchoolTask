#ifndef __INDEX_MANAGER__
#define __INDEX_MANAGER__

#include "MiniSQL.h"
#include "buffer_manager.h"
#include <stack>


#define KEY_SIZE 256
#define INDEX_COUNT 16 // ÿ���ڵ�ָ����Ŀ
#define INFO_NODE 0
#define ROOT_NODE 1
#define INTERNAL_NODE 2
#define LEAF_NODE 4
#define BLANK_NODE 8

typedef int KeyType;
typedef int NodeType;

struct Index
{
	int blockNumber;// ���
	int blockOffset;// ����ƫ��,ֵΪ-1��ʾָ�������飬����ָ�����ݿ�
};

struct BNode
{
	char key[INDEX_COUNT - 1][KEY_SIZE];// �ؼ���
	Index index[INDEX_COUNT];// ָ���ӿ�
	KeyType keyType;// key����
	NodeType nodeType;// �ڵ�����
	int keyCount;// �ڵ���key������
	char reserved[116];// ��������Ŀ���Ǵչ�4096�ֽ�
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
		 *@brief ͨ�����������Ҽ�¼�����������̨
		 *@param conds ��ʾ���ҵ�����
		 *@param cols ��ʾҪ�������
		 *@param recordLength �����ã�Ϊ���ֽӿڲ��䱣��
		 */
		short SelectIndex(condition* conds, column* cols, unsigned int recordLength);
		/**
		 *@brief ͨ�����������Ҽ�¼��ɾ��
		 *@param conds ��ʾҪ���ҵ�����
		 *@param recordLength �����ã�Ϊ���ֽӿڲ��䱣��
		 */
		short DeleteIndex(condition* conds, unsigned int recordLength); 
		/**
		 *@brief Ϊ���Խ�������
		 *@param cols ��ʾҪ��������������
		 *@param recordlen �����ã�Ϊ���ֽӿڲ��䱣��
		 *@param recordnum �����ã�Ϊ���ֽӿڲ��䱣��
		 */
		short CreateIndex(column *cols, unsigned int recordlen, unsigned int recordnum);
		/**
		 *@brief ����ֵvalue������
		 *@param	recordPos ��ʾ��ֵ��record�е�λ��
		 *@return ����0��ʾʧ�ܣ�����1��ʾ�ɹ�
		 */
		short InsertIndex(insertvalue* value, unsigned int recordPos); 

	private:
		// �Ƚ������ؼ��ֵĴ�С,��a<b�򷵻�-1,a==b����0��a>b����1
		int keycompare(char *a, char *b, KeyType tag);
		// ���������븸�ڵ�
		void insertInParent(int currentBlock, char* value, int emptyBlock);
		// ɾ������������b+��
		void deleteEntry(int node, char* value, Index pointer);
		// ���뵽�տ�������
		void addToBlankList(int blockNum);
		// �ϲ�������
		void merge(int block1, int block2, char* value, int parentBlock);
		// �ط���������
		void redistribution(int block1, int block2, char* value, int parentBlock);
		// ��������������ֵ
		inline void swapIndex(Index& index1, Index& index2)
		{
			Index temp;
			temp = index1;
			index1 = index2;
			index2 = temp;
		}
		// ��������key��ֵ
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
		std::stack<int> parentNode;// ��¼���ڵ����ļ��е�ƫ����
};

#endif //__INDEX_MANAGER__
