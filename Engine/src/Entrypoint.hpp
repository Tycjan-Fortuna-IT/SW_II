#pragma once

#include "Core/Application.hpp"

extern SW::Application* CreateApplication();

int main(void)
{
	SW::LogSystem::Initialize();

    SW::Application* instance = CreateApplication();

    instance->Run();

    delete instance;

	SW::LogSystem::Shutdown();

	return 0;
}
