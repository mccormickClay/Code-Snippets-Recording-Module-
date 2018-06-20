# Code-Snippets-Recording-Module-

These are only code samples that will not run on their own. They are used on an Engine I made and use OpenGL and other
customized key values to work.

What it does:
   Takes inputs on a specific frame (if any) and adds them to a text file. This text file can be read later to load in
   player input and the scene Seed.
   
   The text file will only have the frames that have keyboard input to save on file memory.
   
Reading Text Files:
  The files are created by time and date (in RecordingModule.cpp/privSaveTimeLine()). This filename can be used in
  the UserStartScene.cpp to find the correct file to load. 
