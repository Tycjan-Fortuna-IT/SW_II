#pragma once

#include "Core/Application.hpp"

extern SW::Application* CreateApplication();

int main(void)
{
	SW::Logger::LogSystem::Initialize();

	SW::Application* instance = CreateApplication();

	instance->Run();

	delete instance;

	SW::Logger::LogSystem::Shutdown();

	return 0;
}
