#include <Core/ScriptingHost.hpp>

void OnMessage(const char* msg)
{
	std::cout << msg << std::endl;
}

int main()
{
	SW::ScriptingHostSettings settings;
	settings.MessageCallback = &OnMessage;
	settings.FXRPath = "C:\\Program Files\\dotnet\\host\\fxr\\8.0.0\\hostfxr.dll";
	settings.ScriptingModulePath = "C:\\Users\\tycja\\Desktop\\SW_II\\Scripting.Managed";

	SW::ScriptingHost host;

	if (!host.Initialize(settings)) {
		return -1;
	}



	host.Shutdown();

	return 0;
}
