// See the file "COPYING" in the main distribution directory for copyright.

#ifndef INPUTMGR_H
#define INPUTMGR_H

#include "InputReader.h"
#include "BroString.h"

#include "Val.h"
#include "EventHandler.h"
#include "RemoteSerializer.h"
#include "LogMgr.h" // for the LogVal and LogType data types

#include <vector>

class InputReader;


class InputMgr {
public:
	InputMgr();
    
    	InputReader* CreateStream(EnumVal* id, RecordVal* description);
	bool ForceUpdate(const EnumVal* id);
	bool RemoveStream(const EnumVal* id);	

	bool AddFilter(EnumVal *id, RecordVal* filter);
	bool RemoveFilter(EnumVal* id, const string &name);

	
protected:
	friend class InputReader;
	
	// Reports an error for the given reader.
	void Error(InputReader* reader, const char* msg);

	// for readers to write to input stream in direct mode (reporting new/deleted values directly)
	void Put(const InputReader* reader, int id. const LogVal* const *vals);
	void Clear(const InputReader* reader, int id);
	bool Delete(const InputReader* reader, int id, const LogVal* const *vals);

	// for readers to write to input stream in indirect mode (manager is monitoring new/deleted values)
	void SendEntry(const InputReader* reader, int id, const LogVal* const *vals);
	void EndCurrentSend(const InputReader* reader, int id);
	
private:
	struct ReaderInfo;

	bool IsCompatibleType(BroType* t, bool atomic_only=false);

	bool UnrollRecordType(vector<LogField*> *fields, const RecordType *rec, const string& nameprepend);
	void SendEvent(const string& name, EnumVal* event, Val* left, Val* right);	

	HashKey* HashLogVals(const int num_elements, const LogVal* const *vals);
	int GetLogValLength(const LogVal* val);
	int CopyLogVal(char *data, const int startpos, const LogVal* val);

	Val* LogValToVal(const LogVal* val, BroType* request_type);
	Val* LogValToIndexVal(int num_fields, const RecordType* type, const LogVal* const *vals);
	Val* LogValToRecordVal(const LogVal* const *vals, RecordType *request_type, int* position);	

	//void SendEvent(const string& name, const int num_vals, const LogVal* const *vals);
	ReaderInfo* FindReader(const InputReader* reader);
	ReaderInfo* FindReader(const EnumVal* id);

	vector<ReaderInfo*> readers;

	string Hash(const string &input);	

	struct Filter;

};

extern InputMgr* input_mgr;


#endif /* INPUTMGR_H */