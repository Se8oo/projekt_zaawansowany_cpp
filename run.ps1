$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot

$sources = @(
    "src\main.cpp",
    "src\models\Author.cpp",
    "src\models\Category.cpp",
    "src\models\Item.cpp",
    "src\core\Library.cpp",
    "src\utils\FileStorage.cpp"
)

function Build-WithGpp {
    $gpp = Get-Command "g++" -ErrorAction SilentlyContinue
    if (-not $gpp) {
        $gppPath = "C:\MinGW\bin\g++.exe"
        if (Test-Path $gppPath) {
            $gpp = @{ Source = $gppPath }
        }
    }

    if (-not $gpp) {
        throw "Nie znaleziono CMake ani g++. Sprawdz instalacje CMake albo MinGW."
    }

    New-Item -ItemType Directory -Force -Path "build" | Out-Null
    & $gpp.Source -std=c++17 -Wall -Wextra -Wpedantic -Isrc @sources -o "build\HomeLibrary.exe"
    if ($LASTEXITCODE -ne 0) {
        throw "Kompilacja przez g++ nie powiodla sie."
    }
}

$cmake = Get-Command "cmake" -ErrorAction SilentlyContinue
$cmakeCandidates = @(
    "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
    "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
    "C:\Program Files\GNU Octave\Octave-10.1.0\mingw64\bin\cmake.exe",
    "C:\Program Files\MATLAB\R2025a\bin\win64\cmake\bin\cmake.exe"
)

if (-not $cmake) {
    foreach ($candidate in $cmakeCandidates) {
        if (Test-Path $candidate) {
            $cmake = @{ Source = $candidate }
            break
        }
    }
}

$built = $false
if ($cmake) {
    & $cmake.Source -B build
    if ($LASTEXITCODE -eq 0) {
        & $cmake.Source --build build
        $built = ($LASTEXITCODE -eq 0)
    }
}

if (-not $built) {
    Write-Host "CMake nie zadzialal, uzywam g++."
    Build-WithGpp
}

& ".\build\HomeLibrary.exe"
