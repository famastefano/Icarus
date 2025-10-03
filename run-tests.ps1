#!/usr/bin/env pwsh
param(
    [Parameter(Mandatory=$true)]
    [string]$TestName
)

# Map test names to their executable paths
$TestExecutables = @{
    "Test Framework" = "build\bootstrap\exe\test_framework.exe"
    "All Tests" = @("build\bootstrap\exe\test_framework.exe")
}

function Write-TestHeader {
    param([string]$Name)
    Write-Host ""
    Write-Host "================================" -ForegroundColor Cyan
    Write-Host "Running: $Name" -ForegroundColor Cyan
    Write-Host "================================" -ForegroundColor Cyan
    Write-Host ""
}

function Write-TestResult {
    param([string]$Name, [bool]$Passed)
    if ($Passed) {
        Write-Host "$Name PASSED" -ForegroundColor Green
    } else {
        Write-Host "$Name FAILED" -ForegroundColor Red
    }
}

# Check if the test name exists in our mapping
if (-not $TestExecutables.ContainsKey($TestName)) {
    Write-Host "Unknown test: $TestName" -ForegroundColor Red
    Write-Host "Available tests:" -ForegroundColor Yellow
    foreach ($key in $TestExecutables.Keys) {
        Write-Host "  - $key" -ForegroundColor Yellow
    }
    exit 1
}

$executables = $TestExecutables[$TestName]
$workspaceRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$allPassed = $true

# Handle single executable or array of executables
if ($executables -is [array]) {
    Write-TestHeader "Running All Tests"
    
    foreach ($exe in $executables) {
        $fullPath = Join-Path $workspaceRoot $exe
        
        if (-not (Test-Path $fullPath)) {
            Write-Host "Executable not found: $fullPath" -ForegroundColor Red
            Write-Host "Did you run bootstrap first?" -ForegroundColor Yellow
            $allPassed = $false
            continue
        }
        
        $testName = [System.IO.Path]::GetFileNameWithoutExtension($exe)
        Write-Host "Running $testName..." -ForegroundColor White
        
        & $fullPath
        $testPassed = $LASTEXITCODE -eq 0
        Write-TestResult $testName $testPassed
        
        if (-not $testPassed) {
            $allPassed = $false
        }
    }
    
    Write-Host ""
    Write-TestResult "All Tests" $allPassed
} else {
    # Single executable
    $fullPath = Join-Path $workspaceRoot $executables
    
    if (-not (Test-Path $fullPath)) {
        Write-Host "Executable not found: $fullPath" -ForegroundColor Red
        Write-Host "Did you run bootstrap first?" -ForegroundColor Yellow
        exit 1
    }
    
    Write-TestHeader $TestName
    & $fullPath
    $testPassed = $LASTEXITCODE -eq 0
    Write-TestResult $TestName $testPassed
    $allPassed = $testPassed
}

# Exit with appropriate code
if ($allPassed) {
    exit 0
} else {
    exit 1
}