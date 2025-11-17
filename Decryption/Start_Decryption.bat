@echo off
setlocal

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

set "FILE_TYPES=c h md py txt xlsx"
FOR %%t IN (%FILE_TYPES%) DO (
    echo ---------------------------------------------------------
    echo == Processing File Type: %%t ==
    
    echo   -> Calling Code.exe for decryption...
    .\Code.exe %%t

    IF ERRORLEVEL 1 (
        echo     !! WARNING/ERROR: Code.exe returned a non-zero exit code when processing %%t.
    )
    echo.
    echo   -> Calling Replace.exe for replacement...
    .\Replace.exe %%t
    
    IF ERRORLEVEL 1 (
        echo     !! WARNING/ERROR: Replace.exe returned a non-zero exit code when processing %%t.
    )
    echo.
    echo == File Type %%t Processing Complete ==
)
echo ---------------------------------------------------------
echo.

IF EXIST Calling (
    DEL Calling
)

echo All tasks completed.

endlocal
pause