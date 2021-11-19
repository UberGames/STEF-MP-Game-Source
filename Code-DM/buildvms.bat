@echo off
echo.
cd game
if exist game.bat call game
cd ..\cgame
if exist cgame.bat call cgame
cd ..\ui
if exist ui.bat call ui
cd ..
echo"Finished.
echo.
exit
