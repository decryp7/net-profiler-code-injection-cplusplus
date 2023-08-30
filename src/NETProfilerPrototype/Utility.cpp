#include "pch.h"
#include "cor.h"
#include "corprof.h"
#include "Utility.h"

Utility::Utility(ICorProfilerInfo2* info) {
	this->iCorProfilerInfo = info;
}

bool Utility::GetClassNameByObjectId(ObjectID objectId, char* output, ULONG outputLength) {
	ClassID classId;
	iCorProfilerInfo->GetClassFromObject(objectId, &classId);
	return this->GetClassNameByClassId(classId, output, outputLength);
}

bool Utility::GetClassNameByClassId(ClassID classId, char* output, ULONG outputLength) {
	ModuleID moduleId;
	mdTypeDef typeDefToken;
	IMetaDataImport* metadata;
	wchar_t* className = new wchar_t[outputLength];
	ULONG read = 0;

	HRESULT hresult = iCorProfilerInfo->GetClassIDInfo(classId, &moduleId, &typeDefToken);

	if (hresult < 0 || moduleId == 0) {
		return false;
	}

	hresult = iCorProfilerInfo->GetModuleMetaData(moduleId, ofRead, IID_IMetaDataImport, (IUnknown**)&metadata);
	hresult = metadata->GetTypeDefProps(typeDefToken, className, outputLength, &read, NULL, NULL);
	metadata->Release();

	memset(output, 0, outputLength);
	wcstombs(output, className, outputLength);
	delete[] className;
	return true;
}

bool Utility::GetFunctionNameById(FunctionID functionId, char* output, ULONG outputLength) {
	IMetaDataImport* metadata;
	mdMethodDef methodToken;
	mdTypeDef typeDefToken;
	wchar_t* functionName = new wchar_t[1000];
	ULONG wcbCount;
	memset(functionName, 0, 1000);
	iCorProfilerInfo->GetTokenAndMetaDataFromFunction(functionId, IID_IMetaDataImport, (IUnknown**)&metadata, &methodToken);
	metadata->GetMethodProps(methodToken, &typeDefToken, functionName, 1000, &wcbCount, NULL, NULL, NULL, NULL, NULL);
	wcstombs(output, functionName, outputLength);
	metadata->Release();
	delete[] functionName;
	return true;
}

bool Utility::GetModuleNameByModuleId(ModuleID moduleId, char* output, ULONG outputLength)
{
	LPCBYTE loadAddress;
	ULONG charLength;
	AssemblyID assemblyId;
	WCHAR* moduleName = new WCHAR[1000];

	HRESULT hresult = iCorProfilerInfo->GetModuleInfo(moduleId, &loadAddress, outputLength, &charLength, moduleName, &assemblyId);

	if (hresult < 0) {
		return false;
	}

	wcstombs(output, moduleName, outputLength);
	delete[] moduleName;
	return true;
}
