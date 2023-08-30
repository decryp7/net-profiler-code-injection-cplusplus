// dllmain.h : Declaration of module class.

class CNETProfilerPrototypeModule : public ATL::CAtlDllModuleT< CNETProfilerPrototypeModule >
{
public :
	DECLARE_LIBID(LIBID_NETProfilerPrototypeLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_NETPROFILERPROTOTYPE, "{295009ca-eb3b-4f1a-a23b-343ce68c7c19}")
};

extern class CNETProfilerPrototypeModule _AtlModule;
