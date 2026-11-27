[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$BuildDirectory,
    [string]$OutputDirectory = (Join-Path (Get-Location) 'deploy-output')
)

$ErrorActionPreference = 'Stop'
$windeployqtPath = 'C:\Qt\6.11.2\mingw_64\bin\windeployqt.exe'
$qtBinPath = 'C:\Qt\6.11.2\mingw_64\bin'
$qtPluginPath = 'C:\Qt\6.11.2\mingw_64\plugins'
$applicationPath = Join-Path $BuildDirectory 'FocusGarden.exe'
$libraryPath = Join-Path $BuildDirectory 'libfg_ui.dll'
$sqlLibraryPath = Join-Path $qtBinPath 'Qt6Sql.dll'
$sqlitePluginPath = Join-Path $qtPluginPath 'sqldrivers/qsqlite.dll'

if (-not (Test-Path -LiteralPath $windeployqtPath -PathType Leaf)) {
    throw "Qt 6.11.2 windeployqt not found: $windeployqtPath"
}
if (-not (Test-Path -LiteralPath $applicationPath -PathType Leaf)) {
    throw "Built application not found: $applicationPath"
}
if (-not (Test-Path -LiteralPath $libraryPath -PathType Leaf)) {
    throw "Built fg_ui library not found: $libraryPath"
}
if (-not (Test-Path -LiteralPath $sqlLibraryPath -PathType Leaf)) {
    throw "Qt SQL runtime not found: $sqlLibraryPath"
}
if (-not (Test-Path -LiteralPath $sqlitePluginPath -PathType Leaf)) {
    throw "Qt SQLite driver not found: $sqlitePluginPath"
}

New-Item -ItemType Directory -Force -Path $OutputDirectory | Out-Null
Copy-Item -LiteralPath $applicationPath -Destination (Join-Path $OutputDirectory 'FocusGarden.exe') -Force
Copy-Item -LiteralPath $libraryPath -Destination (Join-Path $OutputDirectory 'libfg_ui.dll') -Force
Copy-Item -LiteralPath $sqlLibraryPath -Destination (Join-Path $OutputDirectory 'Qt6Sql.dll') -Force
$sqlOutputDirectory = Join-Path $OutputDirectory 'sqldrivers'
New-Item -ItemType Directory -Force -Path $sqlOutputDirectory | Out-Null
Copy-Item -LiteralPath $sqlitePluginPath -Destination (Join-Path $sqlOutputDirectory 'qsqlite.dll') -Force

& $windeployqtPath --release --no-translations --dir $OutputDirectory (Join-Path $OutputDirectory 'FocusGarden.exe')
if ($LASTEXITCODE -ne 0) {
    throw "windeployqt failed with exit code $LASTEXITCODE"
}
& $windeployqtPath --release --no-translations --dir $OutputDirectory (Join-Path $OutputDirectory 'libfg_ui.dll')
if ($LASTEXITCODE -ne 0) {
    throw "windeployqt dependency pass failed with exit code $LASTEXITCODE"
}

Write-Output "Deployment output: $OutputDirectory"
