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
	short firstRecord;// 记录链表表头,NULL表示结束
	short firstEmpty;// 空位链表表头,NULL表示结束
	int recordLength;// 每条记录的长度,为实际记录加2.开头记录下一个记录的偏移量,0xffff表示结束
	BOOL eof;// 是否是文件结尾
	byte values[4084];// 凑够4096字节
};


class CRecordManager : public CBufferManager
{
public:
	CRecordManager(CString tablename){
		m_tablename = tablename;
	}
	~CRecordManager(){}

	/**
	*@brief 查找表中的记录并打印
	*@param recordlen 已弃用，为保持接口不变而保留
	*@param recordnum 已弃用，为保持接口不变而保留
	*/
	short SelectRecord(condition *conds, column *cols, unsigned int recordlen, unsigned int recordnum);
	/*
	*@brief 删除表中的记录
	*@param conds 被删除的纪录所具有的条件
	*@param recordlen 已弃用，为保持接口不变而保留
	*@param recordnum 已弃用，为保持接口不变而保留
	*/
	short DeleteRecord(condition* conds, unsigned int recordlen, unsigned int recordnum);
	/**
	*@brief 判断表中value是否存在
	*@param cols 表示要查找的属性
	*@param value 表示要查找的值
	*@param recordLength 表示单个记录的长度
	*@return 如果存在则返回1，否则返回0,发生错误返回-1
	*/
	short IsValueExists(column* cols, char* value, unsigned int recordLength);
	/**
	*@brief 将value链表中的值插入记录
	*@param values 指要插入的项
	*@param recordLength 指所插入单个记录的长度
	*@return 返回记录相对文件总体的偏移量,供插入索引使用,若发生错误返回-1
	*/
	int InsertValues(insertvalue* values, unsigned int recordLength);

	
private:
	///@brief 对值按照条件进行比较，若满足条件返回TRUE,不满足返回FALSE
	BOOL ValueCompare(char* valueOfTable, unsigned int type, unsigned int cond, char* compvalue);	  
	///@brief 初始化新record块
	void initRecordBlock(RecordBlock* prb, unsigned int recordLength);
	///@brief searchRecord
	short searchRecord(RecordBlock* prb, int coloffset, char* value);
public:
	CString m_tablename;
};

#endif