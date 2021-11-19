@echo off
if not exist vm\nul mkdir vm
cd vm
if not exist ..\..\..\bin_nt\lcc.exe goto nolcc
echo"Creating the CGAME ASM files...
echo.
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../../game/bg_misc.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../../game/bg_pmove.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../../game/bg_slidemove.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../../game/bg_lib.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../../game/q_math.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../../game/q_shared.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_consolecmds.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_draw.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_drawtools.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_effects.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_ents.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_env.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_event.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_info.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_localents.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_main.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_marks.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_players.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_playerstate.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_predict.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_scoreboard.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_screenfx.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_servercmds.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_snapshot.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_view.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../cg_weapons.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../fx_compression.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../fx_imod.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../fx_lib.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../fx_misc.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../fx_phaser.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../fx_scavenger.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../fx_tetrion.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../fx_transporter.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../fx_grenade.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../fx_quantum.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../fx_stasis.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../fx_item.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../fx_dreadnought.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../fx_borg.c
@if errorlevel 1 goto quit

if not exist ..\..\..\bin_nt\q3asm.exe goto noq3asm
echo"Creating the CGAME.QVM file...
echo.
..\..\..\bin_nt\q3asm -f ../cgame
goto done
:noq3asm
echo"ERROR:  Q3ASM.EXE was not found in the ..\..\..\bin_nt subdirectory.
exit
:nolcc
echo"ERROR:  LCC.EXE was not found in the ..\..\..\bin_nt subdirectory.
exit
:quit
echo"Error building the CGAME ASM files with LCC.EXE!
exit
:done
echo.
cd ..
