#include "RecordingCommandSave.h"
#include "RecordingModuleAttorney.h"

void RecordingCommandSave::EndFrame()
{
	RecordingModuleAttorney::SaveOptions::EndofFrame();
}

void RecordingCommandSave::ConnectToTimeLine()
{
	// Does nothing, used in CommandLoad
}

void RecordingCommandSave::SaveTimeLine()
{
	RecordingModuleAttorney::SaveOptions::SaveTimeLine();
}