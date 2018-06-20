#include "RecordingCommandLoad.h"
#include "RecordingModuleAttorney.h"

void RecordingCommandLoad::EndFrame()
{
	RecordingModuleAttorney::LoadOptions::EndofFrame();
}

void RecordingCommandLoad::ConnectToTimeLine()
{
	RecordingModuleAttorney::LoadOptions::ConnectToTimeLine();
}

void RecordingCommandLoad::SaveTimeLine()
{
	// Does nothing, used in CommandSave
}