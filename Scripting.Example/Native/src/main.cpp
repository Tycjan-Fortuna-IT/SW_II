#include <Core/ScriptingHost.hpp>

void OnMessage(const char* msg)
{
	std::cout << msg << std::endl;
}

int main(int argc, char** argv)
{
	std::string exeDir = std::filesystem::path(argv[0]).parent_path().string();

	SW::ScriptingHostSettings settings;
	settings.MessageCallback = &OnMessage;
	settings.FXRPath = "C:\\Program Files\\dotnet\\host\\fxr\\8.0.0\\hostfxr.dll";
	settings.ScriptingModulePath = exeDir;

	SW::ScriptingHost host;

	if (!host.Initialize(settings)) {
		return -1;
	}

	host.Shutdown();

	return 0;
}
