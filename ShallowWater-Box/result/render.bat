@echo off
set /a start = 0
set /a stop = 2000
set /a step = 2

echo.rendering...
:begin
if %start% GTR %stop% goto end
if %start% LEQ %stop% goto render

:render
echo.frame %start% ...
"C:/Program Files/Mitsuba/mitsuba.exe" -q -p 24 "obj/frame_%start%.xml" -o "img/frame_%start%.png"
echo.frame %start%/%stop% done!
set /a start = start + step
goto begin

:end
echo.finished rederning.