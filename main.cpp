#include "appframework/IAppSystem.h"
#include "eiface.h"
#include "icommandline.h"
#include "interface.h"
#include "interfaces/interfaces.h"
#include <cstring>

CreateInterfaceFn g_pfnServerCreateInterface = NULL;

bool(*g_pfnServerConfigConnect)(IAppSystem* appSystem, CreateInterfaceFn factory);
float(*g_pfnServerConfigGetTickInterval)(const ISource2ServerConfig* config);

void (*g_pfnNetworkServerStartupServer)(const GameSessionConfiguration_t &config, ISource2WorldSession *pWorldSession, const char*);

extern ConCommand cvar_unhide;
extern ConCommand cvarlist_md;

template<typename ReturnType, typename ...ArgTypes>
static auto PatchVtable(void* object, size_t index, ReturnType(*hook)(ArgTypes...))
{
	const auto** vtable = *(const void***)object;

	DWORD oldProtect;
	if (!VirtualProtect(vtable, sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect))
	{
		Plat_FatalError("VirtualProtect PAGE_EXECUTE_READWRITE failed: %d", GetLastError());
	}

	const auto original = (decltype(hook))vtable[index];
	vtable[index] = hook;

	if (!VirtualProtect(vtable, sizeof(void*), oldProtect, &oldProtect))
	{
		Plat_FatalError("VirtualProtect restore failed: %d", GetLastError());
	}

	return original;
}

bool Connect(IAppSystem* appSystem, CreateInterfaceFn factory)
{
	const bool result = g_pfnServerConfigConnect(appSystem, factory);

	ConnectInterfaces(&factory, 1);

	ConVar_Register(FCVAR_RELEASE | FCVAR_GAMEDLL);

	return result;
}

float GetTickInterval(const ISource2ServerConfig* config)
{
	if (CommandLine()->CheckParm("-tickrate"))
	{
		const int tickrate = CommandLine()->ParmValue("-tickrate", 0);
		if (tickrate > 10)
			return 1.0f / tickrate;
	}

	return g_pfnServerConfigGetTickInterval(config);
}

#undef CreateInterface

DLL_EXPORT void* CreateInterface(const char* pName, int* pReturnCode)
{
	if (g_pfnServerCreateInterface == NULL)
	{
		auto insecure = CommandLine()->HasParm("-insecure");
		if (!insecure)
		{
			Plat_FatalError("Refusing to load the cvar unlocker in secure mode.\n\nAdd -insecure to Deadlock's launch options and restart the game.");
		}

		// Generate the path to the real server.dll
		CUtlString realServerPath(Plat_GetGameDirectory());
		realServerPath.Append("\\citadel\\bin\\win64\\server.dll");
		realServerPath.FixSlashes();

		HMODULE serverModule = LoadLibrary(realServerPath.GetForModify());
		g_pfnServerCreateInterface = (CreateInterfaceFn)GetProcAddress(serverModule, "CreateInterface");

		if (g_pfnServerCreateInterface == NULL)
		{
			Plat_FatalError("Could not find CreateInterface entrypoint in server.dll: %d", GetLastError());
		}
	}

	auto original = g_pfnServerCreateInterface(pName, pReturnCode);

	// Intercept the first interface requested by the engine
	if (strcmp(pName, INTERFACEVERSION_SERVERCONFIG) == 0)
	{
		g_pfnServerConfigConnect = PatchVtable(original, 0, Connect);
		g_pfnServerConfigGetTickInterval = PatchVtable(original, 13, GetTickInterval);
	}

	return original;
}
