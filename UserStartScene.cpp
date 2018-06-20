#include "..\src\SceneManager.h"
#include "..\src\RecordingTool.h"

#include "AlarmsDemo.h" //This is inside the src folder to allow use for Doxygen
#include "SpriteDemo.h" //This is inside the src folder to allow use for Doxygen
#include "CollisionDemoScene.h"
#include "SubmissionDemoScene.h"

#include "TerrainScene.h"
#include "TitleScreen.h"


Scene* SceneManager::StartScene()
{
	RecordingTool::RecordingState(RecordingModule::NONE);
	RecordingTool::SetRecordedFileName("2018-06-02-01-51");
	return(new TitleScreen);
}