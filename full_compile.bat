@echo off
echo Borrando rom
del *.sms
echo Reexportar graficos 
.\bmp2tile\bmp2tile.exe .\gfx-source\systemfont.png -savetiles .\assets\systemfont.psgcompr -exit
if %errorlevel% neq 0 exit
.\bmp2tile\bmp2tile.exe .\gfx-source\levelbackground.png -mirror -removedupes -savetiles .\assets\levelbackgroundtiles.psgcompr -savepalette .\assets\levelbackgroundpalette.bin -savetilemap .\assets\levelbackgroundtilemap.bin -exit
if %errorlevel% neq 0 exit
.\bmp2tile\bmp2tile.exe .\gfx-source\fnac_logo.png -mirror -removedupes -savetiles .\assets\fnaclogotiles.psgcompr -savepalette .\assets\fnaclogopalette.bin -savetilemap .\assets\fnaclogotilemap.bin -exit
if %errorlevel% neq 0 exit

echo Exportar spritesheets
python ./tools/spriter-tiles.py -w 4 -e 4 -m True -c ./tools/player.cfg -o ./ -b ./assets ./gfx-source/player_spritesheet.png
if %errorlevel% neq 0 exit
echo Convertir ficheros de graficos a C
assets2banks assets
if %errorlevel% neq 0 exit
echo Compilar ficheros de graficos C
sdcc -c -mz80 --peep-file peep-rules.txt --constseg BANK2 bank2.c
if %errorlevel% neq 0 exit
sdcc -c -mz80 --peep-file peep-rules.txt --constseg BANK3 bank3.c
if %errorlevel% neq 0 exit
echo Compilar Librerias
sdcc -c -mz80 --peep-file peep-rules.txt libs/GSLib.c
if %errorlevel% neq 0 exit
sdcc -c -mz80 --peep-file peep-rules.txt gamelogic.c
if %errorlevel% neq 0 exit
sdcc -c -mz80 --peep-file peep-rules.txt montylib.c
if %errorlevel% neq 0 exit
sdcc -c -mz80 --peep-file peep-rules.txt resources.c
if %errorlevel% neq 0 exit
sdcc -c -mz80 --peep-file peep-rules.txt spriteengine.c
if %errorlevel% neq 0 exit
sdcc -c -mz80 --peep-file peep-rules.txt player.c
if %errorlevel% neq 0 exit
sdcc -c -mz80 --peep-file peep-rules.txt soundengine.c
if %errorlevel% neq 0 exit
echo Compilar Juego
sdcc -c -mz80 --peep-file peep-rules.txt game.c
if %errorlevel% neq 0 exit
echo Linkar Juego
sdcc -o msb.ihx -mz80 --no-std-crt0 --data-loc 0xC000 -Wl-b_BANK2=0x8000 -Wl-b_BANK3=0x8000 crt0_sms.rel game.rel montylib.rel gamelogic.rel resources.rel spriteengine.rel player.rel soundengine.rel SMSlib.lib ./libs/PSGLib.rel GSLib.rel bank2.rel bank3.rel
if %errorlevel% neq 0 exit
echo Convertir de binario a rom
ihx2sms msb.ihx msb.sms
echo Ejecutar emulador
java -jar c:\Users\JordiM\Desktop\emuladores\mastersystem\Emulicious\Emulicious.jar msb.sms
del *.asm
del *.ihx
del *.lst
del *.sym

