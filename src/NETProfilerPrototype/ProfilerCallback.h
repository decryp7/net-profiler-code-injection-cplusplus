#pragma once

#include "cor.h"
#include "corprof.h"
#include "framework.h"
#include "NETProfilerPrototype_i.h"
#include "Utility.h"

/// <summary>A list of opcodes</summary>
/// <remarks>Used, amongst other things, to build the <c>OperationDetails</c> structures 
/// when pulling in "opcode.def"</remarks>
enum CanonicalName {
#define OPDEF(name, str, decs, incs, args, optp, stdlen, stdop1, stdop2, flow) name,
#include "opcode.def"
#undef OPDEF
};

class ATL_NO_VTABLE ProfilerCallback :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<ProfilerCallback, &CLSID_NetProfiler>,
	public ICorProfilerCallback6
{
public:
	ProfilerCallback();

	DECLARE_REGISTRY_RESOURCEID(IDR_PROFILER)
	BEGIN_COM_MAP(ProfilerCallback)
		COM_INTERFACE_ENTRY(ICorProfilerCallback)
		COM_INTERFACE_ENTRY(ICorProfilerCallback2)
		COM_INTERFACE_ENTRY(ICorProfilerCallback3)
		COM_INTERFACE_ENTRY(ICorProfilerCallback4)
		COM_INTERFACE_ENTRY(ICorProfilerCallback5)
		COM_INTERFACE_ENTRY(ICorProfilerCallback6)
	END_COM_MAP()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease();

	// ICorProfilerCallback interface implementation
	virtual HRESULT __stdcall Initialize(IUnknown* pICorProfilerInfoUnk);
	virtual HRESULT __stdcall Shutdown();
	virtual HRESULT __stdcall AppDomainCreationStarted(AppDomainID appDomainID);
	virtual HRESULT __stdcall AppDomainCreationFinished(AppDomainID appDomainID, HRESULT hrStatus);
	virtual HRESULT __stdcall AppDomainShutdownStarted(AppDomainID appDomainID);
	virtual HRESULT __stdcall AppDomainShutdownFinished(AppDomainID appDomainID, HRESULT hrStatus);
	virtual HRESULT __stdcall AssemblyLoadStarted(AssemblyID assemblyID);
	virtual HRESULT __stdcall AssemblyLoadFinished(AssemblyID assemblyID, HRESULT hrStatus);
	virtual HRESULT __stdcall AssemblyUnloadStarted(AssemblyID assemblyID);
	virtual HRESULT __stdcall AssemblyUnloadFinished(AssemblyID assemblyID, HRESULT hrStatus);
	virtual HRESULT __stdcall ModuleLoadStarted(ModuleID moduleID);
	virtual HRESULT __stdcall ModuleLoadFinished(ModuleID moduleID, HRESULT hrStatus);
	virtual HRESULT __stdcall ModuleUnloadStarted(ModuleID moduleID);
	virtual HRESULT __stdcall ModuleUnloadFinished(ModuleID moduleID, HRESULT hrStatus);
	virtual HRESULT __stdcall ModuleAttachedToAssembly(ModuleID moduleID, AssemblyID assemblyID);
	virtual HRESULT __stdcall ClassLoadStarted(ClassID classID);
	virtual HRESULT __stdcall ClassLoadFinished(ClassID classID, HRESULT hrStatus);
	virtual HRESULT __stdcall ClassUnloadStarted(ClassID classID);
	virtual HRESULT __stdcall ClassUnloadFinished(ClassID classID, HRESULT hrStatus);
	virtual HRESULT __stdcall FunctionUnloadStarted(FunctionID functionID);
	virtual HRESULT __stdcall JITCompilationStarted(FunctionID functionID, BOOL fIsSafeToBlock);
	virtual HRESULT __stdcall JITCompilationFinished(FunctionID functionID, HRESULT hrStatus, BOOL fIsSafeToBlock);
	virtual HRESULT __stdcall JITCachedFunctionSearchStarted(FunctionID functionID, BOOL* pbUseCachedFunction);
	virtual HRESULT __stdcall JITCachedFunctionSearchFinished(FunctionID functionID, COR_PRF_JIT_CACHE result);
	virtual HRESULT __stdcall JITFunctionPitched(FunctionID functionID);
	virtual HRESULT __stdcall JITInlining(FunctionID callerID, FunctionID calleeID, BOOL* pfShouldInline);
	virtual HRESULT __stdcall ThreadCreated(ThreadID threadID);
	virtual HRESULT __stdcall ThreadDestroyed(ThreadID threadID);
	virtual HRESULT __stdcall ThreadAssignedToOSThread(ThreadID managedThreadID, DWORD osThreadID);
	virtual HRESULT __stdcall RemotingClientInvocationStarted();
	virtual HRESULT __stdcall RemotingClientSendingMessage(GUID* pCookie, BOOL fIsAsync);
	virtual HRESULT __stdcall RemotingClientReceivingReply(GUID* pCookie, BOOL fIsAsync);
	virtual HRESULT __stdcall RemotingClientInvocationFinished();
	virtual HRESULT __stdcall RemotingServerReceivingMessage(GUID* pCookie, BOOL fIsAsync);
	virtual HRESULT __stdcall RemotingServerInvocationStarted();
	virtual HRESULT __stdcall RemotingServerInvocationReturned();
	virtual HRESULT __stdcall RemotingServerSendingReply(GUID* pCookie, BOOL fIsAsync);
	virtual HRESULT __stdcall UnmanagedToManagedTransition(FunctionID functionID, COR_PRF_TRANSITION_REASON reason);
	virtual HRESULT __stdcall ManagedToUnmanagedTransition(FunctionID functionID, COR_PRF_TRANSITION_REASON reason);
	virtual HRESULT __stdcall RuntimeSuspendStarted(COR_PRF_SUSPEND_REASON suspendReason);
	virtual HRESULT __stdcall RuntimeSuspendFinished();
	virtual HRESULT __stdcall RuntimeSuspendAborted();
	virtual HRESULT __stdcall RuntimeResumeStarted();
	virtual HRESULT __stdcall RuntimeResumeFinished();
	virtual HRESULT __stdcall RuntimeThreadSuspended(ThreadID threadid);
	virtual HRESULT __stdcall RuntimeThreadResumed(ThreadID threadid);
	virtual HRESULT __stdcall MovedReferences(ULONG cmovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], ULONG cObjectIDRangeLength[]);
	virtual HRESULT __stdcall ObjectAllocated(ObjectID objectID, ClassID classID);
	virtual HRESULT __stdcall ObjectsAllocatedByClass(ULONG classCount, ClassID classIDs[], ULONG objects[]);
	virtual HRESULT __stdcall ObjectReferences(ObjectID objectID, ClassID classID, ULONG cObjectRefs, ObjectID objectRefIDs[]);
	virtual HRESULT __stdcall RootReferences(ULONG cRootRefs, ObjectID rootRefIDs[]);
	virtual HRESULT __stdcall ExceptionThrown(ObjectID thrownObjectID);
	virtual HRESULT __stdcall ExceptionSearchFunctionEnter(FunctionID functionID);
	virtual HRESULT __stdcall ExceptionSearchFunctionLeave();
	virtual HRESULT __stdcall ExceptionSearchFilterEnter(FunctionID functionID);
	virtual HRESULT __stdcall ExceptionSearchFilterLeave();
	virtual HRESULT __stdcall ExceptionSearchCatcherFound(FunctionID functionID);
	virtual HRESULT __stdcall ExceptionCLRCatcherFound();
	virtual HRESULT __stdcall ExceptionCLRCatcherExecute();
	virtual HRESULT __stdcall ExceptionOSHandlerEnter(FunctionID functionID);
	virtual HRESULT __stdcall ExceptionOSHandlerLeave(FunctionID functionID);
	virtual HRESULT __stdcall ExceptionUnwindFunctionEnter(FunctionID functionID);
	virtual HRESULT __stdcall ExceptionUnwindFunctionLeave();
	virtual HRESULT __stdcall ExceptionUnwindFinallyEnter(FunctionID functionID);
	virtual HRESULT __stdcall ExceptionUnwindFinallyLeave();
	virtual HRESULT __stdcall ExceptionCatcherEnter(FunctionID functionID, ObjectID objectID);
	virtual HRESULT __stdcall ExceptionCatcherLeave();
	virtual HRESULT __stdcall COMClassicVTableCreated(ClassID wrappedClassID, REFGUID implementedIID, void* pVTable, ULONG cSlots);
	virtual HRESULT __stdcall COMClassicVTableDestroyed(ClassID wrappedClassID, REFGUID implementedIID, void* pVTable);

	// ICorProfilerCallback2 interface implementation
	virtual HRESULT __stdcall ThreadNameChanged(ThreadID threadId, ULONG cchName, WCHAR name[]);
	virtual HRESULT __stdcall GarbageCollectionStarted(int cGenerations, BOOL generationCollected[], COR_PRF_GC_REASON reason);
	virtual HRESULT __stdcall SurvivingReferences(ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[], ULONG cObjectIDRangeLength[]);
	virtual HRESULT __stdcall GarbageCollectionFinished();
	virtual HRESULT __stdcall FinalizeableObjectQueued(DWORD finalizerFlags, ObjectID objectID);
	virtual HRESULT __stdcall RootReferences2(ULONG cRootRefs, ObjectID rootRefIds[], COR_PRF_GC_ROOT_KIND rootKinds[], COR_PRF_GC_ROOT_FLAGS rootFlags[], UINT_PTR rootIds[]);
	virtual HRESULT __stdcall HandleCreated(GCHandleID handleId, ObjectID initialObjectId);
	virtual HRESULT __stdcall HandleDestroyed(GCHandleID handleId);

	// ICorProfilerCallback3 interface implementation
	virtual HRESULT __stdcall InitializeForAttach(
		/* [in] */ IUnknown *pCorProfilerInfoUnk,
		/* [in] */ void *pvClientData,
		/* [in] */ UINT cbClientData);
	virtual HRESULT __stdcall ProfilerAttachComplete();
	virtual HRESULT __stdcall ProfilerDetachSucceeded();

	// ICorProfilerCallback4 interface implementation
	virtual HRESULT __stdcall ReJITCompilationStarted(
		/* [in] */ FunctionID functionId,
		/* [in] */ ReJITID rejitId,
		/* [in] */ BOOL fIsSafeToBlock);
	virtual HRESULT __stdcall GetReJITParameters(
		/* [in] */ ModuleID moduleId,
		/* [in] */ mdMethodDef methodId,
		/* [in] */ ICorProfilerFunctionControl *pFunctionControl);
	virtual HRESULT __stdcall ReJITCompilationFinished(
		/* [in] */ FunctionID functionId,
		/* [in] */ ReJITID rejitId,
		/* [in] */ HRESULT hrStatus,
		/* [in] */ BOOL fIsSafeToBlock);
	virtual HRESULT __stdcall ReJITError(
		/* [in] */ ModuleID moduleId,
		/* [in] */ mdMethodDef methodId,
		/* [in] */ FunctionID functionId,
		/* [in] */ HRESULT hrStatus);
	virtual HRESULT __stdcall MovedReferences2(
		/* [in] */ ULONG cMovedObjectIDRanges,
		/* [size_is][in] */ ObjectID oldObjectIDRangeStart[],
		/* [size_is][in] */ ObjectID newObjectIDRangeStart[],
		/* [size_is][in] */ SIZE_T cObjectIDRangeLength[]);
	virtual HRESULT __stdcall SurvivingReferences2(
		/* [in] */ ULONG cSurvivingObjectIDRanges,
		/* [size_is][in] */ ObjectID objectIDRangeStart[],
		/* [size_is][in] */ SIZE_T cObjectIDRangeLength[]);

	// ICorProfilerCallback5 interface implementation
	virtual HRESULT __stdcall ConditionalWeakTableElementReferences(
		/* [in] */ ULONG cRootRefs,
		/* [size_is][in] */ ObjectID keyRefIds[],
		/* [size_is][in] */ ObjectID valueRefIds[],
		/* [size_is][in] */ GCHandleID rootIds[]);

	// ICorProfilerCallback6 interface implementation
	virtual HRESULT __stdcall GetAssemblyReferences(
		/* [string][in] */ const WCHAR *wszAssemblyPath,
		/* [in] */ ICorProfilerAssemblyReferenceProvider *pAsmRefProvider);
private:
	Utility* utility;
};

OBJECT_ENTRY_AUTO(__uuidof(NetProfiler), ProfilerCallback)
