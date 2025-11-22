# Commands:
# [build] docker build -t bitbishop-windows . -f docker/windows/Dockerfile
# [run] docker run -it --rm bitbishop-windows
# [configure cmake] cmake --preset=clang_release
# [build cmake] cmake --build --preset=clang_release

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
# - --installargs 'ADD_CMAKE_TO_PATH=System': ensures CMake is accessible system-wide
# - -y: automatic yes to prompts (non-interactive installation for Docker builds)
RUN choco install -y git cmake ninja zip llvm --installargs 'ADD_CMAKE_TO_PATH=System'

# Update PATH environment variable to make installed tools accessible
# CRITICAL: PowerShell directory must be in PATH for vcpkg's bootstrap script
# - C:\Windows\System32\WindowsPowerShell\v1.0: PowerShell executable location
#   vcpkg's bootstrap-vcpkg.bat internally calls powershell.exe and expects it in PATH
# - C:\Program Files\Git\cmd: Git command-line tools
# - C:\ProgramData\chocolatey\bin: Chocolatey-installed binaries
# - ${PATH}: preserves existing PATH entries (inheritance)
ENV PATH="C:\\Windows\\System32\\WindowsPowerShell\\v1.0;C:\\Program Files\\Git\\cmd;C:\\ProgramData\\chocolatey\\bin;${PATH}"

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
#   * x-gha,readwrite: uses GitHub Actions cache with read/write permissions
#     (particularly useful when running in GitHub Actions runners)
ENV VCPKG_ROOT="C:\\vcpkg"
ENV VCPKG_FEATURE_FLAGS="manifest,binarycaching"
ENV VCPKG_BINARY_SOURCES="clear;x-gha,readwrite"

# Final PATH configuration with all build tools
# - C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools:
#   VS Developer Command Prompt tools (VsDevCmd.bat location for setting up build environment)
# - C:\vcpkg: vcpkg.exe location for package management
# - ${PATH}: inherits previously set PATH entries
ENV PATH="C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\Common7\\Tools;C:\\vcpkg;${PATH}"

# Set working directory for when container starts
# C:\workspace is a conventional location for build operations
WORKDIR C:\\workspace

COPY . .

# Default command when container starts
# PowerShell provides better interactive experience and scripting capabilities than cmd
CMD ["powershell.exe"]
