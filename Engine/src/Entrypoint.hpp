#pragma once

#include "Core/Application.hpp"

extern SW::Application* CreateApplication();

int main()
{
    SW::Application* instance = CreateApplication();

    instance->Run();

    delete instance;
}
