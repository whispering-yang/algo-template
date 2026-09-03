@echo off

:loop
cls
g++ -std=c++14 -O2 -Wall %1.cpp -o %1

:: 编译错误执行条件
if errorlevel 1 (
    echo [错误] 编译失败！请检查代码。
    timeout /t 20 >nul
    goto loop
)

echo [成功] 编译完成，正在运行...

call %1.exe < "%~2" > %1.out

echo [成功] 运行完毕

type %1.out

timeout /t 20 >nul

goto loop
