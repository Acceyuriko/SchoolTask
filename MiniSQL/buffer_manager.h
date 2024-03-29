#ifndef __buffer_h__
#define __buffer_h__

#include "MiniSQL.h"

class CBufferManager
{
public:
	char* m_address;				//块的首地址
	CString m_name;					//块对应的表或索引名
	unsigned int m_offset_number;	//块对应的表或索引中的页序数
	short int m_index_table;		//块对应的是表，则值为TABLE;若是索引，则值为INDEX;若空为0
	short int m_is_written;			//该块是否被改过，初值为0，改过置1
	short int m_being_used;			//该块当前是否被读或写，若是则值为1
	unsigned int m_count;			//用于实现LRU算法

	CBufferManager(){}
	~CBufferManager(){}

	static void flush_all_blocks();
	static void initiate_blocks();

	unsigned int get_block(short int index_table, CString filename, unsigned int offset_number);
	unsigned int get_blank_block(short int index_table);

	/*标志该块已被改过*/
	void written_block() {
		m_is_written=1;
	}
	/*标志该块已经不在用了*/
	void used_block() {
		m_being_used=0;
	}

protected:
	static CBufferManager *m_ptheblocks;

	static void using_block(unsigned int number);
	static unsigned int max_count_number();

	void flush_block();
	/*为新块做标记*/
	void mark_block(CString filename,unsigned int offset) {
		m_name = filename;
		m_offset_number = offset;
	}
	  
};

#endif