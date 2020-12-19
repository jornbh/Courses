chcp 1252
@py.exe C:\Users\Public\Documents\makeInput.py %*





    setlocal enableextensions disabledelayedexpansion
    for %%a in ("C:\Users\Public\Documents/input*.txt") do (
        <"%%~a" "C:\Python27\python.exe" "C:\Users\Public\Documents\tull.py"
    )