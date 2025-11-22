# Commands:
# [build] docker build -t bitbishop-windows . -f docker/windows/Dockerfile
# [run] docker run -it --rm bitbishop-windows
# NOTE: Before trying to build this dockerfile, ensure docker engine runs in windows container mode

FROM mcr.microsoft.com/windows/servercore:ltsc2022

# Configure PowerShell as the default shell with strict error handling
# - PowerShell is more powerful than cmd for scripting and package management
# - $ErrorActionPreference = 'Stop' ensures the build fails on any error (critical for CI/CD)
# - $ProgressPreference = 'SilentlyContinue' suppresses progress bars that can clutter build logs
SHELL ["C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe", "-Command", "$ErrorActionPreference = 'Stop'; $ProgressPreference = 'SilentlyContinue';"]

# Install Chocolatey package manager
# - Chocolatey is the standard package manager for Windows, similar to apt/yum on Linux
# - Set-ExecutionPolicy Bypass allows script execution for this process only (security)
# - SecurityProtocol 3072 = TLS 1.2, required for secure HTTPS downloads
# - iex (Invoke-Expression) downloads and executes the Chocolatey install script
RUN Set-ExecutionPolicy Bypass -Scope Process -Force; \
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; \
    iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

# Install core build dependencies via Chocolatey
# - git: version control, required to clone vcpkg and project repositories
# - cmake: cross-platform build system generator, industry standard for C++ projects
# - ninja: fast build system, alternative to MSBuild, preferred for faster compilation
# - zip: archive utility, often needed for packaging and handling compressed files
# - llvm: compiler infrastructure including clang, useful for cross-platform builds
# - -y: automatic yes to prompts (non-interactive installation for Docker builds)
# NOTE: Installing each tool separately ensures proper PATH registration
RUN choco install -y git
RUN choco install -y cmake --installargs 'ADD_CMAKE_TO_PATH=System'
RUN choco install -y ninja
RUN choco install -y zip
RUN choco install -y llvm

# Update PATH environment variable to make installed tools accessible
# CRITICAL: Must use [Environment]::SetEnvironmentVariable to persist changes to the system
# This ensures PATH modifications survive container restarts and are available at runtime
# - Machine scope: system-wide PATH that persists across sessions
RUN [Environment]::SetEnvironmentVariable('PATH', \
    \"C:\\Windows\\System32\\WindowsPowerShell\\v1.0;\" + \
    \"C:\\Program Files\\Git\\cmd;\" + \
    \"C:\\Program Files\\CMake\\bin;\" + \
    \"C:\\ProgramData\\chocolatey\\bin;\" + \
    \"C:\\Tools\\ninja;\" + \
    \"C:\\Program Files\\LLVM\\bin;\" + \
    $env:PATH, \
    [EnvironmentVariableTarget]::Machine)

# Refresh the current session's PATH to include newly installed tools
# This is necessary for subsequent RUN commands in the Dockerfile
RUN refreshenv; \
    $env:PATH = [System.Environment]::GetEnvironmentVariable('PATH','Machine')

# Install Visual Studio Build Tools 2022
# This provides the MSVC compiler toolchain required for building native Windows C++ applications
# - Invoke-WebRequest: downloads the VS Build Tools installer
# - Start-Process with -Wait: ensures installation completes before proceeding
# - --quiet --wait --norestart --nocache: silent, synchronous install without restart or cache
#
# Component selection rationale:
# - Microsoft.VisualStudio.Workload.VCTools:
#   Core C++ build tools workload (compilers, libraries, build tools)
# - Microsoft.VisualStudio.Component.Windows11SDK.22000:
#   Windows 11 SDK (22000 = version 10.0.22000.0), provides Windows API headers and libraries
#   Note: Compatible with Windows 10 projects as well, backward compatible
# - Microsoft.VisualStudio.Component.VC.Tools.x86.x64:
#   MSVC v143 compiler toolset for x86 and x64 (both 32-bit and 64-bit compilation)
# - --includeRecommended:
#   Adds recommended components for selected workloads (debuggers, profilers, etc.)
#
# Remove installer after completion to reduce image size
RUN Invoke-WebRequest -Uri https://aka.ms/vs/17/release/vs_BuildTools.exe -OutFile vs_buildtools.exe; \
    Start-Process -FilePath .\vs_buildtools.exe -ArgumentList '--quiet', '--wait', '--norestart', '--nocache', \
    '--add', 'Microsoft.VisualStudio.Workload.VCTools', \
    '--add', 'Microsoft.VisualStudio.Component.Windows11SDK.22000', \
    '--add', 'Microsoft.VisualStudio.Component.VC.Tools.x86.x64', \
    '--includeRecommended' \
    -NoNewWindow -Wait; \
    Remove-Item vs_buildtools.exe -Force

# Clone the vcpkg C++ package manager
# - vcpkg is Microsoft's cross-platform C++ package manager
# - Cloning from GitHub ensures we get the latest version with all port definitions
# - C:\vcpkg: standard installation location, commonly used in documentation
RUN git clone https://github.com/microsoft/vcpkg.git C:\vcpkg

# Bootstrap vcpkg - this step requires special shell handling
# WHY SHELL SWITCH IS NECESSARY:
# - bootstrap-vcpkg.bat is a Windows batch script that internally calls powershell.exe
# - When executed from PowerShell context, it cannot find powershell.exe in its environment
# - cmd.exe provides the native environment where batch scripts expect to run
# - The batch script will successfully locate and execute powershell.exe from PATH
#
# - /S /C: cmd switches for silent execution and command execution
# - -disableMetrics: opts out of vcpkg telemetry collection
SHELL ["cmd", "/S", "/C"]
RUN C:\vcpkg\bootstrap-vcpkg.bat -disableMetrics

# Switch back to PowerShell for remaining operations
# PowerShell is preferred for its superior scripting capabilities and error handling
SHELL ["C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe", "-Command", "$ErrorActionPreference = 'Stop'; $ProgressPreference = 'SilentlyContinue';"]

# Configure vcpkg environment variables
# - VCPKG_ROOT: tells tools where vcpkg is installed (standard vcpkg convention)
# - VCPKG_FEATURE_FLAGS: enables advanced vcpkg features
#   * manifest: enables vcpkg.json manifest mode for declarative dependency management
#   * binarycaching: enables binary package caching to speed up builds
# - VCPKG_BINARY_SOURCES: configures where binary cache is stored
#   * clear: clears any inherited cache configuration
#   * files: uses local filesystem cache (C:\vcpkg\binarycache)
#   NOTE: x-gha backend has been removed, using local file cache instead
ENV VCPKG_ROOT="C:\\vcpkg"
ENV VCPKG_FEATURE_FLAGS="manifest,binarycaching"
ENV VCPKG_BINARY_SOURCES="clear;files,C:\\vcpkg\\binarycache,readwrite"

# Create binary cache directory
RUN New-Item -ItemType Directory -Path C:\vcpkg\binarycache -Force

# Final PATH configuration with all build tools
# Add VS Build Tools and vcpkg to the system PATH
# This ensures all tools are available when the container starts
# CRITICAL: Include MSVC compiler binaries for ninja/cmake to find cl.exe
RUN $vsPath = (Get-ChildItem 'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC' | Select-Object -Last 1).FullName; \
    [Environment]::SetEnvironmentVariable('PATH', \
    \"C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\Common7\\Tools;\" + \
    \"$vsPath\\bin\\Hostx64\\x64;\" + \
    \"C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\Ninja;\" + \
    \"C:\\vcpkg;\" + \
    [System.Environment]::GetEnvironmentVariable('PATH','Machine'), \
    [EnvironmentVariableTarget]::Machine)

# Set up MSVC environment variables required for compilation
# These environment variables are normally set by vcvarsall.bat or VsDevCmd.bat
# Setting them explicitly ensures cmake/ninja can find and use the MSVC toolchain
RUN $vsPath = (Get-ChildItem 'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC' | Select-Object -Last 1).FullName; \
    $sdkPath = 'C:\Program Files (x86)\Windows Kits\10'; \
    $sdkVersion = (Get-ChildItem \"$sdkPath\Include\" | Select-Object -Last 1).Name; \
    [Environment]::SetEnvironmentVariable('INCLUDE', \
    \"$vsPath\\include;\" + \
    \"$sdkPath\\Include\\$sdkVersion\\ucrt;\" + \
    \"$sdkPath\\Include\\$sdkVersion\\um;\" + \
    \"$sdkPath\\Include\\$sdkVersion\\shared\", \
    [EnvironmentVariableTarget]::Machine); \
    [Environment]::SetEnvironmentVariable('LIB', \
    \"$vsPath\\lib\\x64;\" + \
    \"$sdkPath\\Lib\\$sdkVersion\\ucrt\\x64;\" + \
    \"$sdkPath\\Lib\\$sdkVersion\\um\\x64\", \
    [EnvironmentVariableTarget]::Machine); \
    [Environment]::SetEnvironmentVariable('LIBPATH', \
    \"$vsPath\\lib\\x64;\" + \
    \"$sdkPath\\UnionMetadata\\$sdkVersion;\" + \
    \"$sdkPath\\References\\$sdkVersion\", \
    [EnvironmentVariableTarget]::Machine)

# Set working directory for when container starts
# C:\workspace is a conventional location for build operations
WORKDIR C:\\workspace

COPY . .

# Default command when container starts
# PowerShell provides better interactive experience and scripting capabilities than cmd
CMD ["powershell.exe"]