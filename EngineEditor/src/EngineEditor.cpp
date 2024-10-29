#include "EngineEditor.hpp"

#include "EditorLayer.hpp"

SW::Eventing::Event<> EngineEditor::ProjectLoadedEvent;
SW::Eventing::Event<> EngineEditor::AssetDirContentChangedEvent;

EngineEditor::EngineEditor(const SW::ApplicationSpecification& specification) : SW::Application(specification)
{
	this->PushLayer(new SW::EditorLayer("Editor Layer"));
}

EngineEditor::~EngineEditor()
{
	this->PopLayer();
}
