#pragma once

#include <Core/Application.hpp>

class EngineEditor final : public SW::Application
{
public:
	explicit EngineEditor(const SW::ApplicationSpecification& specification);

public:
	static SW::Eventing::Event<> ProjectLoadedEvent;
	static SW::Eventing::Event<> AssetDirContentChangedEvent;

protected:
	~EngineEditor() override;
};
