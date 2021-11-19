@echo off
if not exist vm\nul mkdir vm
cd vm
if not exist ..\..\..\bin_nt\lcc.exe goto nolcc
echo"Creating the UI ASM files...
echo. 
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_main.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_cdkey.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_ingame.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_confirm.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../../game/bg_misc.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../../game/bg_lib.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../../game/q_math.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../../game/q_shared.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_gameinfo.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_atoms.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_connect.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_controls2.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_demo2.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_fonts.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_mfield.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_menu.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_sound.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_network.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_playermodel.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_players.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_playersettings.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_preferences.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_qmenu.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_serverinfo.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_servers2.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_sparena.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_specifyserver.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_splevel.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_sppostgame.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_startserver.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_syscalls.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_team.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_video.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_spskill.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_addbots.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_removebots.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_teamorders.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_mods.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ui_cvars.c
@if errorlevel 1 goto quit

if not exist ..\..\..\bin_nt\q3asm.exe goto noq3asm
echo"Creating the UI.QVM file...
echo.
..\..\..\bin_nt\q3asm -f ../ui
goto done
:noq3asm
echo"ERROR:  Q3ASM.EXE was not found in the ..\..\..\bin_nt subdirectory.
exit
:nolcc
echo"ERROR:  LCC.EXE was not found in the ..\..\..\bin_nt subdirectory.
exit
:quit
echo"Error building the UI ASM files with LCC.EXE!
exit
:done
echo.
cd ..
