@echo off
echo Borrando rom
del *.sms
echo Compilar ficheros de graficos C
sdcc -c -mz80 --peep-file peep-rules.txt --constseg BANK2 bank2.c
if %errorlevel% neq 0 exit
sdcc -c -mz80 --peep-file peep-rules.txt --constseg BANK3 bank3.c
if %errorlevel% neq 0 exit
echo Compilar Librerias
sdcc -c -mz80 --peep-file peep-rules.txt libs/GSLib.c
echo Compilar Tester
sdcc -c -mz80 --peep-file peep-rules.txt scrolltester.c
echo Compilar Enemys
sdcc -c -mz80 --peep-file peep-rules.txt enemy.c
if %errorlevel% neq 0 exit
echo Linkar Tester
sdcc -o scrolltester.ihx -mz80 --no-std-crt0 --data-loc 0xC000 -Wl-b_BANK2=0x8000 -Wl-b_BANK3=0x8000 crt0_sms.rel scrolltester.rel SMSlib.lib GSLib.rel enemy.rel bank2.rel bank3.rel
if %errorlevel% neq 0 exit
echo Convertir de binario a rom
ihx2sms scrolltester.ihx scrolltester.sms
echo Ejecutar emulador
java -jar c:\Users\JordiM\Desktop\emuladores\mastersystem\Emulicious\Emulicious.jar scrolltester.sms
del *.asm
del *.ihx
del *.lst
del *.sym

