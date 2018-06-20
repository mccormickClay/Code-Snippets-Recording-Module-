#ifndef _RecordingModuleAttorney
#define _RecordingModuleAttorney

#include "RecordingModule.h"

class RecordingModuleAttorney
{
public:
	class EngineOptions
	{
		// Engine name
		friend class Ramen;
	private:

		static void TerminateModule()
		{
			RecordingModule::Terminate();
		}

		// Called at the End of each frame
		static void EndFrame()
		{
			RecordingModule::EndFrame();
		}

		// Connects To TimeLine (if needed)
		static void Connect()
		{
			RecordingModule::Connect();
		}
	};

	class SceneOptions
	{
		friend class Scene;
	private:
		static void SaveScene()
		{
			RecordingModule::Save();
		};
	};
	class KeyOptions
	{
		friend class SingleKeyManager;
	private:
		static void SaveKeyFrame(AZUL_KEY key)
		{
			RecordingModule::SaveKeyToFrame(key);
		}
	};
	class LoadOptions
	{
		friend class RecordingCommandLoad;
	private:
		static void EndofFrame()
		{
			RecordingModule::StopAllKeys();
			RecordingModule::LoadFrame();
		}
		static void ConnectToTimeLine()
		{
			RecordingModule::LoadTimeLine();
		}
	};
	class SaveOptions
	{
		friend class RecordingCommandSave;
	private:
		static void EndofFrame()
		{
			RecordingModule::SaveCurrentFrame();
		}
		static void SaveTimeLine()
		{
			RecordingModule::SaveTimeLine();
		}
	};
};

#endif _RecordingModuleAttorney
