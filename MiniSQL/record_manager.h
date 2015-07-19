#ifndef __RECORD_H__
#define __RECORD_H__

#include "MiniSQL.h"
#include "buffer_manager.h"
#include <list>

#define TRUE 1
#define FALSE 0
#define RECORD_OFFSET 12
#define BLOCK_END 0xffffffff
#define NEW_BLOCK 0x24242424
#define BLANK_NODE 8

typedef int BOOL;

struct RecordBlock
{
	short firstRecord;// ��¼�����ͷ,NULL��ʾ����
	short firstEmpty;// ��λ�����ͷ,NULL��ʾ����
	int recordLength;// ÿ����¼�ĳ���,Ϊʵ�ʼ�¼��2.��ͷ��¼��һ����¼��ƫ����,0xffff��ʾ����
	BOOL eof;// �Ƿ����ļ���β
	byte values[4084];// �չ�4096�ֽ�
};


class CRecordManager : public CBufferManager
{
public:
	CRecordManager(CString tablename){
		m_tablename = tablename;
	}
	~CRecordManager(){}

	/**
	*@brief ���ұ��еļ�¼����ӡ
	*@param recordlen �����ã�Ϊ���ֽӿڲ��������
	*@param recordnum �����ã�Ϊ���ֽӿڲ��������
	*/
	short SelectRecord(condition *conds, column *cols, unsigned int recordlen, unsigned int recordnum);
	/*
	*@brief ɾ�����еļ�¼
	*@param conds ��ɾ���ļ�¼�����е�����
	*@param recordlen �����ã�Ϊ���ֽӿڲ��������
	*@param recordnum �����ã�Ϊ���ֽӿڲ��������
	*/
	short DeleteRecord(condition* conds, unsigned int recordlen, unsigned int recordnum);
	/**
	*@brief �жϱ���value�Ƿ����
	*@param cols ��ʾҪ���ҵ�����
	*@param value ��ʾҪ���ҵ�ֵ
	*@param recordLength ��ʾ������¼�ĳ���
	*@return ��������򷵻�1�����򷵻�0,�������󷵻�-1
	*/
	short IsValueExists(column* cols, char* value, unsigned int recordLength);
	/**
	*@brief ��value�����е�ֵ�����¼
	*@param values ָҪ�������
	*@param recordLength ָ�����뵥����¼�ĳ���
	*@return ���ؼ�¼����ļ������ƫ����,����������ʹ��,���������󷵻�-1
	*/
	int InsertValues(insertvalue* values, unsigned int recordLength);

	
private:
	///@brief ��ֵ�����������бȽϣ���������������TRUE,�����㷵��FALSE
	BOOL ValueCompare(char* valueOfTable, unsigned int type, unsigned int cond, char* compvalue);	  
	///@brief ��ʼ����record��
	void initRecordBlock(RecordBlock* prb, unsigned int recordLength);
	///@brief searchRecord
	short searchRecord(RecordBlock* prb, int coloffset, char* value);
public:
	CString m_tablename;
};

#endif