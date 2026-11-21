param([string]$QtPrefix='C:/Qt/6.11.2/mingw_64')
$ErrorActionPreference='Stop'
$projectPath=Split-Path $PSScriptRoot -Parent
$repoPath=Split-Path $projectPath -Parent
$candidates=@((Join-Path $repoPath 'build/FocusGarden.exe'),(Join-Path $repoPath 'build-ascii/FocusGarden.exe'),(Join-Path $projectPath '../../02_Local/build/FocusGarden.exe'))
$exe=$candidates | Where-Object { Test-Path -LiteralPath $_ } | Select-Object -First 1
if (!$exe) { throw 'Build the project first. See README.md.' }
$env:PATH="$QtPrefix/bin;C:/Qt/Tools/mingw1310_64/bin;"+$env:PATH
Start-Process -FilePath $exe -WorkingDirectory (Split-Path $exe -Parent)
