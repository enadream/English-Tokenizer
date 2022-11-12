#ifndef RECORDER_HPP
#define RECORDER_HPP

#include "datatypes.hpp"

typedef uint16 id_size; // Size of alphabet ID

typedef struct RecordNode // Record struct for Node
{
	uint16 id; // 2 byte 0 to 65,535 
	const char* name;

} RecordNode;


typedef struct Record
{
	id_size id; // 2 byte 0 to 65,535 
	const char* name;
	Record* nextRecord;

} Record;


class RecordList {
private:
	Record* m_firstRecord;
	Record* m_lastRecord;
	id_size lastID;

public:
	RecordList();
	~RecordList();

	bool AddRecordBack(Record* record);
	bool DeleteRecordWithID(id_size id);
	bool DeleteRecordWithName(const char* name);
	char* GetNameFromID(id_size id);
	id_size GetIDfromName(const char* name);
};



#endif