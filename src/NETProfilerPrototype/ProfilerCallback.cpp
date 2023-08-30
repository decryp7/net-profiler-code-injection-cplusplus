#include "pch.h"
#include "ProfilerCallback.h"
#include <iostream>
#include <corprof.h>
#include <string>
#include <corhlpr.h>
#include <vector>

const ULONG TINY_HEADER_SIZE = 0x1;

CComQIPtr<ICorProfilerInfo2> iCorProfilerInfo;

ProfilerCallback::ProfilerCallback() {
	//std::cout << "constructor" << std::endl;
}

HRESULT ProfilerCallback::FinalConstruct()
{
	return S_OK;
}

void ProfilerCallback::FinalRelease()
{
}

HRESULT __stdcall ProfilerCallback::Initialize(IUnknown* pICorProfilerInfoUnk)
{
	//request for instance of ICorProfilerInfo
	pICorProfilerInfoUnk->QueryInterface(IID_ICorProfilerInfo2, (LPVOID*)&iCorProfilerInfo);
	//set flags for events that we want
	iCorProfilerInfo->SetEventMask(COR_PRF_MONITOR_MODULE_LOADS);
	utility = new Utility(iCorProfilerInfo);
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::Shutdown()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::AppDomainCreationStarted(AppDomainID appDomainID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::AppDomainCreationFinished(AppDomainID appDomainID, HRESULT hrStatus)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::AppDomainShutdownStarted(AppDomainID appDomainID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::AppDomainShutdownFinished(AppDomainID appDomainID, HRESULT hrStatus)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::AssemblyLoadStarted(AssemblyID assemblyID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::AssemblyLoadFinished(AssemblyID assemblyID, HRESULT hrStatus)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::AssemblyUnloadStarted(AssemblyID assemblyID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::AssemblyUnloadFinished(AssemblyID assemblyID, HRESULT hrStatus)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ModuleLoadStarted(ModuleID moduleID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ModuleLoadFinished(ModuleID moduleID, HRESULT hrStatus)
{
	//get the module name
	char* moduleName = new char[100];
	utility->GetModuleNameByModuleId(moduleID, moduleName, 100);
	const std::string moduleN = moduleName;
	delete[] moduleName;

	//looking for target module
	if (moduleN.find("TestApp.exe") > moduleN.length())
	{
		return S_OK;
	}

	std::cout << "ModuleName: " << moduleN << std::endl;

	std::cout << "Getting IMetaDataEmit interface..." << std::endl;
	//get interface to create method
	IMetaDataEmit* metadataEmit;
	if(!SUCCEEDED(iCorProfilerInfo->GetModuleMetaData(moduleID, ofRead | ofWrite, IID_IMetaDataEmit, (IUnknown**)&metadataEmit)))
	{
		std::cout << "Unable to get IMetaDataEmit!" << std::endl;
		return S_OK;
	}

	std::cout << "Getting IMetaDataImport interface..." << std::endl;
	//get interface to get type information
	IMetaDataImport* metadataImport;
	if (!SUCCEEDED(iCorProfilerInfo->GetModuleMetaData(moduleID, ofRead | ofWrite, IID_IMetaDataImport, (IUnknown**)&metadataImport)))
	{
		std::cout << "Unable to get IMetaDataImport!" << std::endl;
		return S_OK;
	}
	
	std::cout << "Looking for TestApp.DerivedClass type..." << std::endl;
	//get type token
	mdTypeDef typeDef;
	if (!SUCCEEDED(metadataImport->FindTypeDefByName(L"TestApp.DerivedClass", NULL, &typeDef)))
	{
		std::cout << "Unable to find TestApp.DerivedClass!" << std::endl;
		return S_OK;
	}

	//mdMethodDef methodDef;
	//if(!SUCCEEDED(metadataImport->FindMethod(typeDef, L"SayHello", NULL, NULL, &methodDef)))
	//{
	//	std::cout << "Unable to find method in TestApp.DerivedClass!" << std::endl;
	//	return S_OK;
	//}

	//wchar_t methodName[512];
	//DWORD size = 512;
	//PCCOR_SIGNATURE methodSignature;
	//ULONG methodSignatureSize;
	//DWORD methodAttributes;
	//DWORD methodImplFlags;
	//if (!SUCCEEDED(metadataImport->GetMethodProps(methodDef, &typeDef, methodName, size, &size, &methodAttributes, &methodSignature, &methodSignatureSize, NULL, &methodImplFlags)))
	//{
	//	std::cout << "Unable to get method props in TestApp.DerivedClass!" << std::endl;
	//	return S_OK;
	//}
	////print the signature of the function
	//fprintf(stdout, "il:");
	//for (ULONG i = 0; i < methodSignatureSize; i++) {
	//	fprintf(stdout, " %02x", methodSignature[i]);
	//}
	//fprintf(stdout, "\n");

	//LPCBYTE pMethodBytes;
	//ULONG cbMethodSize;
	//if (!SUCCEEDED(iCorProfilerInfo->GetILFunctionBody(moduleID, methodDef, &pMethodBytes, &cbMethodSize)))
	//{
	//	std::cout << "Unable to get method IL function body in TestApp.DerivedClass!" << std::endl;
	//	return S_OK;
	//}

	////print the il code of the function
	//fprintf(stdout, "il:");
	//for (ULONG i = 0; i < cbMethodSize; i++) {
	//	fprintf(stdout, " %02x", pMethodBytes[i]);
	//}
	//fprintf(stdout, "\n");


	std::cout << "Creating SayHello() in TestApp.DerivedClass type..." << std::endl;
	//create method in class
	COR_SIGNATURE newMethodSignature[] = { IMAGE_CEE_CS_CALLCONV_HASTHIS,   
		0,                               
		ELEMENT_TYPE_STRING       
	};
	mdMethodDef methodDef;
	if (!SUCCEEDED(metadataEmit->DefineMethod(typeDef, L"SayHello",
		mdPublic | mdHideBySig | mdVirtual | mdReuseSlot,
		newMethodSignature, 
		sizeof(newMethodSignature), 
		0x00,
		miIL,
		&methodDef)))
	{
		std::cout << "Unable to define SayHello!" << std::endl;
		return S_OK;
	}

	//il instructions reverse engineered from actual method implementation
	//0x13 header type, flags and header size
	//0x30	Read in reverse order (Intel's reversed byte-order)
	//		0x3013 (0011 0000000100 11)
	//		0011 - First 4 bits hold header size in DWORDs
	//		0000000100 - Next 10 bits hold the Flags value(0x4), which means that local variables must be initialized
	//		11 - The lower 2 bits(11) indicate the header type (Fat)
	//------------------------------------
	//0x01 MaxStack(WORD) Maximum stack size in slots (items)
	//0x00
	//0x0b CodeSize(DWORD) IL code size in bytes (without method header)
	//0x00
	//0x00
	//0x00
	//0x01
	//0x00
	//0x00
	//0x11
	//0x00
	//0x72 ldstr Push a string object for the literal string.
	//0x2d
	//0x00
	//0x00
	//0x70
	//0x0a stloc.0 Pop a value from stack into local variable 0.
	//0x2b br.s Branch to target, short form.
	//0x00
	//0x06 ldloc.0 Load local variable 0 onto stack.
	//0x2a ret

	//create method body
	LPCWSTR szString = L"Hello from Overridden SayHello in Derived Class!";
	mdString msg;
	if(!SUCCEEDED(metadataEmit->DefineUserString(szString, wcslen(szString), &msg)))
	{
		std::cout << "Unable to define string!" << std::endl;
		return S_OK;
	}

	unsigned instructionOffset = 0;
	BYTE * instructions = new BYTE[12];
	instructions[instructionOffset++] = CEE_LDSTR; //ltdstr
	//msg token
	for (ULONG i = 0; i < sizeof(msg); i++) {
		instructions[instructionOffset++] = ((char*)&msg)[i];
	}
	instructions[instructionOffset++] = CEE_RET; //ret

	std::cout << "Writing SayHello() body in TestApp.DerivedClass type..." << std::endl;
	IMethodMalloc* allocator = nullptr;
	if (!SUCCEEDED(iCorProfilerInfo->GetILFunctionBodyAllocator(moduleID, &allocator)) || allocator == nullptr)
	{
		std::cout << "Unable to get IL Function Body Allocator!" << std::endl;
		return S_OK;
	}

	BYTE* pMethodBody = (BYTE*)allocator->Alloc(sizeof(IMAGE_COR_ILMETHOD_FAT) + instructionOffset);
	allocator->Release();

	BYTE * pCurrent = pMethodBody; //use this pointer to move

	//Write method header
	IMAGE_COR_ILMETHOD_FAT *pHeader = (IMAGE_COR_ILMETHOD_FAT *)pCurrent;
	pHeader->Flags = CorILMethod_FatFormat;
	pHeader->Size = sizeof(IMAGE_COR_ILMETHOD_FAT) / sizeof(DWORD);
	pHeader->MaxStack = 0x1;
	pHeader->CodeSize = instructionOffset;
	pHeader->LocalVarSigTok = 0x0;

	pCurrent = (BYTE*)(pHeader + 1);

	//write the instruction
	CopyMemory(pCurrent, instructions, instructionOffset);

	//set the method body 
	if (!SUCCEEDED(iCorProfilerInfo->SetILFunctionBody(moduleID, methodDef, pMethodBody)))
	{
		std::cout << "Unable to set IL Function Body!" << std::endl;
		return S_OK;
	}

	//check the method body
	//LPCBYTE pMethodBytes;
	//ULONG cbMethodSize;
	//if (!SUCCEEDED(iCorProfilerInfo->GetILFunctionBody(moduleID, methodDef, &pMethodBytes, &cbMethodSize)))
	//{
	//	std::cout << "Unable to get method IL function body in TestApp.DerivedClass!" << std::endl;
	//	return S_OK;
	//}

	//print the code of the function
	//fprintf(stdout, "il:");
	//for (ULONG i = 0; i < cbMethodSize; i++) {
	//	fprintf(stdout, " %02x", pMethodBytes[i]);
	//}
	//fprintf(stdout, "\n");

	std::cout << "------------Completed injecting method into TestApp.DerivedClass----------------" << std::endl << std::endl;
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ModuleUnloadStarted(ModuleID moduleID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ModuleUnloadFinished(ModuleID moduleID, HRESULT hrStatus)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ModuleAttachedToAssembly(ModuleID moduleID, AssemblyID assemblyID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ClassLoadStarted(ClassID classID)
{
	//char* className = new char[100];
	//utility->GetClassNameByClassId(classID, className, 100);
	//std::cout << "Classname: "<< className << std::endl;
	//delete[] className;
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ClassLoadFinished(ClassID classID, HRESULT hrStatus)
{
	//char* className = new char[100];
	//utility->GetClassNameByClassId(classID, className, 100);
	//std::cout << className << std::endl;
	//delete[] className;
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ClassUnloadStarted(ClassID classID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ClassUnloadFinished(ClassID classID, HRESULT hrStatus)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::FunctionUnloadStarted(FunctionID functionID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::JITCompilationStarted(FunctionID functionID, BOOL fIsSafeToBlock)
{
	//char* functionName = new char[100];
	//utility->GetFunctionNameById(functionID, functionName, 100);
	//std::cout << "Function Name: "<< functionName << std::endl;
	//delete[] functionName;
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::JITCompilationFinished(FunctionID functionID, HRESULT hrStatus, BOOL fIsSafeToBlock)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::JITCachedFunctionSearchStarted(FunctionID functionID, BOOL* pbUseCachedFunction)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::JITCachedFunctionSearchFinished(FunctionID functionID, COR_PRF_JIT_CACHE result)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::JITFunctionPitched(FunctionID functionID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::JITInlining(FunctionID callerID, FunctionID calleeID, BOOL* pfShouldInline)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::UnmanagedToManagedTransition(FunctionID functionID, COR_PRF_TRANSITION_REASON reason)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ManagedToUnmanagedTransition(FunctionID functionID, COR_PRF_TRANSITION_REASON reason)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ThreadCreated(ThreadID threadID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ThreadDestroyed(ThreadID threadID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ThreadAssignedToOSThread(ThreadID managedThreadID, DWORD osThreadID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RemotingClientInvocationStarted()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RemotingClientSendingMessage(GUID* pCookie, BOOL fIsAsync)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RemotingClientReceivingReply(GUID* pCookie, BOOL fIsAsync)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RemotingClientInvocationFinished()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RemotingServerReceivingMessage(GUID* pCookie, BOOL fIsAsync)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RemotingServerInvocationStarted()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RemotingServerInvocationReturned()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RemotingServerSendingReply(GUID* pCookie, BOOL fIsAsync)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RuntimeSuspendStarted(COR_PRF_SUSPEND_REASON suspendReason)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RuntimeSuspendFinished()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RuntimeSuspendAborted()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RuntimeResumeStarted()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RuntimeResumeFinished()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RuntimeThreadSuspended(ThreadID threadID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RuntimeThreadResumed(ThreadID threadID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::MovedReferences(ULONG cmovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], ULONG cObjectIDRangeLength[])
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ObjectAllocated(ObjectID objectID, ClassID classID)
{
	//char* className = new char[1000];
	//if (utility->GetClassNameByClassId(classID, className, 1000)) {
	//	std::cout << "\t\nfrom profiler: class allocated: " << className << "\r\n";
	//}
	//delete[] className;
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ObjectsAllocatedByClass(ULONG classCount, ClassID classIDs[], ULONG objects[])
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ObjectReferences(ObjectID objectID, ClassID classID, ULONG objectRefs, ObjectID objectRefIDs[])
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RootReferences(ULONG rootRefs, ObjectID rootRefIDs[])
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ExceptionThrown(ObjectID thrownObjectID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ExceptionUnwindFunctionEnter(FunctionID functionID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ExceptionUnwindFunctionLeave()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ExceptionSearchFunctionEnter(FunctionID functionID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ExceptionSearchFunctionLeave()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ExceptionSearchFilterEnter(FunctionID functionID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ExceptionSearchFilterLeave()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ExceptionSearchCatcherFound(FunctionID functionID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ExceptionCLRCatcherFound()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ExceptionCLRCatcherExecute()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ExceptionOSHandlerEnter(FunctionID functionID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ExceptionOSHandlerLeave(FunctionID functionID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ExceptionUnwindFinallyEnter(FunctionID functionID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ExceptionUnwindFinallyLeave()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ExceptionCatcherEnter(FunctionID functionID,
	ObjectID objectID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ExceptionCatcherLeave()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::COMClassicVTableCreated(ClassID wrappedClassID, REFGUID implementedIID, void* pVTable, ULONG cSlots)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::COMClassicVTableDestroyed(ClassID wrappedClassID, REFGUID implementedIID, void* pVTable)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::ThreadNameChanged(ThreadID threadID, ULONG cchName, WCHAR name[])
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::GarbageCollectionStarted(int cGenerations, BOOL generationCollected[], COR_PRF_GC_REASON reason)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::SurvivingReferences(ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[], ULONG cObjectIDRangeLength[])
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::GarbageCollectionFinished()
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::FinalizeableObjectQueued(DWORD finalizerFlags, ObjectID objectID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::RootReferences2(ULONG cRootRefs, ObjectID rootRefIDs[], COR_PRF_GC_ROOT_KIND rootKinds[], COR_PRF_GC_ROOT_FLAGS rootFlags[], UINT_PTR rootIDs[])
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::HandleCreated(GCHandleID handleID, ObjectID initialObjectID)
{
	return S_OK;
}

HRESULT __stdcall ProfilerCallback::HandleDestroyed(GCHandleID handleID)
{
	return S_OK;
}