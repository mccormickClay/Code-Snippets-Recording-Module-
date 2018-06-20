#include "RecordingModule.h"

#include <fstream>
#include <string>
#include "WordInput.h"
#include "TimeManager.h"
#include "RandomGeneratorAttorney.h"

#include <ctime>

#include "RecordingCommandBase.h"
#include "RecordingCommandLoad.h"
#include "RecordingCommandSave.h"
#include "RecordingCommandNull.h"

RecordingModule* RecordingModule::ptrInstance = nullptr;

void RecordingModule::privStart()
{
	commandLoad = new RecordingCommandLoad();
	commandSave = new RecordingCommandSave();
	commandNull = new RecordingCommandNull();
	commandBase = commandNull;
	ip = nullptr;
}

void RecordingModule::privSetRecordedFileName(std::string filename)
{
	RecordedFile = filename;
}

std::string RecordingModule::privGetRecordedFileName()
{
	return(RecordedFile);
}

void RecordingModule::privSaveKeyToFrame(AZUL_KEY key)
{
	frameToSave.insert(frameToSave.end(), key);
}

void RecordingModule::privSaveCurrentFrame()
{
	timelineToSave.insert(timelineToSave.end(), std::pair<Frame,int>(frameToSave,frameNumberInEngine));
	frameToSave.clear();
}

void RecordingModule::privLoadFrame()
{
	frameNumberInEngine++;
	if (timeLine.size() > 0 && frameNumberInEngine == timeLine.front().second)
	{
		DebugMsg::out("\nExecuting Loaded Frame...%i", frameNumberInEngine);

		currentFrame = timeLine.front().first;
		Frame usingFrame = currentFrame;
		timeLine.pop_front();

		Frame::iterator it;
		it = usingFrame.begin();
		numOfKeys = usingFrame.size();

		ip = new INPUT[numOfKeys];

		// Force Keyboard to read from list
		for (int i = 0; it != usingFrame.end(); i++)
		{
			ip[i].type = INPUT_KEYBOARD;
			ip[i].ki.wScan = 0;
			ip[i].ki.time = 0;
			ip[i].ki.dwExtraInfo = 0;

			WORD vKey = WordInput::ConvertAZULtoWORD(usingFrame.front());

			ip[i].ki.wVk = vKey; 
			ip[i].ki.dwFlags = 0; 
			usingFrame.erase(it++);
		}
		SendInput(numOfKeys, &ip[0], sizeof(INPUT));
	}

}

void RecordingModule::privStopAllKeys()
{
	if(numOfKeys > 0)
	{
		for (int i = 0; i < numOfKeys; i++)
		{
			// force all inputs from keyboard to stop
			ip[i].type = INPUT_KEYBOARD;
			ip[i].ki.wScan = 0; 
			ip[i].ki.time = 0;
			ip[i].ki.dwExtraInfo = 0;

			// Tells specific input value from the loaded frame to stop
			ip[i].ki.wVk = WordInput::ConvertAZULtoWORD(currentFrame.front()); 
			ip[i].ki.dwFlags = KEYEVENTF_KEYUP; 
			currentFrame.pop_front();
		}
		SendInput(numOfKeys, &ip[0], sizeof(INPUT));
		delete ip;
		numOfKeys = 0;
	}
}

void RecordingModule::privLoadTimeLine()
{
	std::ifstream inFile;
	std::string skip;
	int currentKeyValue;

	int framesize;
	int frameCounter = 0;

	std::string filepath = "Recordings\\";
	if (!GetRecordedFileName().empty())
	{
		std::string filename = filepath + GetRecordedFileName() + ".txt";
		inFile.open(filename); 
		if (!inFile)
		{
			DebugMsg::out("NOT ABLE TO FIND TEXT FILE");
			assert(0);
		}
		else //if files was found
		{
			inFile >> currentKeyValue;
			RandomGeneratorAttorney::RecordingOptions::SetSceneSeed(currentKeyValue);

			while (inFile >> frameCounter)
			{
				inFile >> framesize;
				for (int i = 0; i < framesize; i++)
				{
					inFile >> currentKeyValue;

					AZUL_KEY currentKey = AZUL_KEY(currentKeyValue);

					currentFrame.insert(currentFrame.end(), currentKey);
				}
				timeLine.insert(timeLine.end(), std::pair<Frame, int>(currentFrame, frameCounter));
				currentFrame.clear();
			}
			inFile.close();
		}
	}

}

void RecordingModule::privSaveTimeLine()
{
 
	if (RecordingType == State::RECORD && timelineToSave.size() > 1)
	{
		std::ofstream outFile;
		int currentKeyValue;
		TimeLine::iterator it;
		it = timelineToSave.begin();
		Frame::iterator it2;
		Frame framebeingSaved;

		int framesize;
		int frameCounter = 0;

		char timeline[80];

		time_t rawtime = time(0);
		struct tm timeinfo;
		localtime_s(&timeinfo, &rawtime);

		// saves recording as "year-month-day-hour-minute.txt"
		strftime(timeline, sizeof(timeline), "Recordings\\%Y-%m-%d-%I-%M.txt", &timeinfo);

		outFile.open(timeline); //default parameter should be in as a ifstream

		if (!outFile)
		{
			DebugMsg::out("NOT ABLE TO FIND TEXT FILE");
			assert(0);
		}
		else //if file was found
		{
			// Get Seed to out into txt file
			outFile << RandomGeneratorAttorney::RecordingOptions::GetSeed() << std::endl;

			while (it != timelineToSave.end())
			{
				framebeingSaved = (*it++).first;
				it2 = framebeingSaved.begin();
				frameCounter++;

				if (framebeingSaved.size() != 0)
				{
					
					framesize = framebeingSaved.size();
					// Add frame number to txt file
					outFile << frameCounter << std::endl;

					// Add how many inputs were used in
					// frame into the txt file
					outFile << framesize << std::endl;

					while (it2 != framebeingSaved.end())
					{
						currentKeyValue = framebeingSaved.front();
						outFile << currentKeyValue << std::endl;
						framebeingSaved.erase(it2++);
					}

				}
			}
			outFile.close();
		}
	}
}

void RecordingModule::privEndFrame()
{
	commandBase->EndFrame();
}

void RecordingModule::privConnect()
{
	commandBase->ConnectToTimeLine();
}

void RecordingModule::privSave()
{
	commandBase->SaveTimeLine();
}

void RecordingModule::privSetRecordingState(State choice)
{
	Start();
	RecordingType = choice;
	switch (choice)
	{
	case State::LOAD:
		commandBase = commandLoad;
		break;
	case State::RECORD:
		commandBase = commandSave;
		break;
	case State::NONE:
		commandBase = commandNull;
		break;
	default:
		break;
	}
}

void RecordingModule::privTerminate()
{
	delete ptrInstance;
	ptrInstance = nullptr;
}

RecordingModule::~RecordingModule()
{
	delete commandLoad;
	delete commandSave;
	delete commandNull;
	commandLoad = nullptr;
	commandSave	= nullptr;
	commandNull	= nullptr;
	ip = nullptr;
	timeLine.clear();
	currentFrame.clear();
	frameToSave.clear();
	timelineToSave.clear();
}