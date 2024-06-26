#pragma warning (disable : 4996)

#pragma once

#include "cor.h"
#include "corprof.h"

class Utility {
public:
	Utility(ICorProfilerInfo5* info);
	bool GetClassNameByObjectId(ObjectID objectId, char* output, ULONG outputLength);
	bool GetClassNameByClassId(ClassID classId, char* output, ULONG outputLength);
	bool GetFunctionNameById(FunctionID functionId, char* output, ULONG outputLength);
	bool GetModuleNameByModuleId(ModuleID moduleId, char* output, ULONG outputLength);
private:
	ICorProfilerInfo5* iCorProfilerInfo;
};