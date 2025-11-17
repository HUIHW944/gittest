@echo off
setlocal enabledelayedexpansion

echo Current Directory : %CD%
echo.

IF NOT EXIST Code.exe (
    echo ERROR: Code.exe not found.
    pause
    goto :EOF
)
IF NOT EXIST Replace.exe (
    echo ERROR: Replace.exe not found.
    pause
    goto :EOF
)

set "extensions=c h py md txt"

:: Temporary directory
set "temp_root=%~dp0._work"
if exist "%temp_root%" rd /s /q "%temp_root%"
mkdir "%temp_root%"

for %%e in (%extensions%) do (
    echo === Processing %%e files ===

    set "workdir=%temp_root%\type_%%e"
    mkdir "!workdir!"

    :: 复制当前目录所有 .%%e 文件到工作目录（保留原名）
    xcopy /y /q ".\*.%%e" "!workdir!\" >nul 2>&1

    :: 进入工作目录处理
    pushd "!workdir!"
    ..\..\Code.exe %%e
    ..\..\Replace.exe %%e

    ::把处理后的 .%%e 文件复制回原目录
    xcopy /y /q ".\*.%%e" "..\..\" >nul

    popd

    echo Done processing %%e.
    echo.
)

:: 清理临时目录
rd /s /q "%temp_root%" 2>nul
echo All operations completed.
pause