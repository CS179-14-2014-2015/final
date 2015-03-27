#Finals  (deadline: 2015-03-XX)

Submit your finals here. Here are a couple of reminders:

1. Do not commit compiler or IDE-generated files (the *.o, *.pdb among others)
2. Documentation, especially on how to build and play the game, is appreciated.
3. Also commit a PDF version of the [Certificate of Authorship for groups](http://www.admu.edu.ph/ls/sose/iscs/downloads).




This project requires SDL_image.h, SDL2, SDL2gfx, and SDL_ttf.h.

1. Download SDL2-devel-2.0.3-VC.zip in the development library section in http://libsdl.org/download-2.0.php

2. Extract the file into the computer. 

3. Download SDL2_gfx-1.0.1.tar.gz in http://cms.ferzkopp.net/index.php/software/13-sdl-gfx

4. Extract the file into the computer.

5. Create an empty project in Visual Studios (any name can do).

6. Download the main.cpp for the homework.

7. Copy and paste the main.cpp file into the source files at the solution explorer pane of Visual studios.

8. Right click on the project name at the solution explorer and select properties.

9. Under configuration properties, go to VC++ Directories, select the drop down button on include directories and click <edit>.

10. Click the ... button in the white space and select the path location on where the include folder of SDL2-2.0.3 is. (ex. C:\Users\Marty\Documents\SDL\SDL2(1)\SDL2-2.0.3\include). Then click ok.

11. Click another ... button and select the path location on where the SDL2_gfx-1.0.1 file location is. (ex. C:\Users\Marty\Documents\SDL\SDL2_gfx-1.0.1). Then click ok.

12. Click the drop down button on the library directories and select <edit>.

13. Click the ... button then look for the directory for the x86 folder in SDL2-2.0.3. (ex. C:\Users\Marty\Documents\SDL\SDL2(1)\SDL2-2.0.3\lib\x86). Then click ok. Click another ... button then look for the directory for the x86 folder in SDL2_ttf-2.0.12. (ex. C:\Users\Marty\Documents\SDL\SDL2_ttf-2.0.12\VisualC\external\lib\x86). Then click ok. 

14. Under the configuration properties, click the arrow beside linker.

15. Under linker, click input.

16. Click the drop down on additional dependencies and select <edit>.

17. Type in the white space SDL2.lib and SDL2main.lib. Then click ok. 

18. Now back to linker, click system. Click the drop down button on SubSystem and select Console (/SUBSYSTEMS:CONSOLE).

19. Click Apply then ok.

20. Change the path of the image in the .cpp file. 

