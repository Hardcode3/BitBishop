# Commands:
# [build] docker build -t bitbishop-windows . -f docker/windows/Dockerfile
# [run] docker run -it --rm bitbishop-windows
# NOTE: Before trying to build this dockerfile, ensure docker engine runs in windows container mode

# Use the official GitHub Actions Windows runner image
# This image already has most development tools pre-installed:
# - Chocolatey
# - Git
# - Visual Studio Build Tools
# - Windows SDK
# - PowerShell
# - And many more tools commonly used in CI/CD
FROM ghcr.io/actions/actions-runner:windows-2025

# Switch to PowerShell with strict error handling
SHELL ["powershell", "-Command", "$ErrorActionPreference = 'Stop'; $ProgressPreference = 'SilentlyContinue';"]

# Install additional tools not included in the base image
# CMake, Ninja, and LLVM are needed for your C++ builds
RUN choco install -y cmake --installargs 'ADD_CMAKE_TO_PATH=System'
RUN choco install -y ninja
RUN choco install -y llvm

# Refresh environment to ensure newly installed tools are in PATH
RUN refreshenv

# Clone and bootstrap vcpkg
# vcpkg is Microsoft's C++ package manager
RUN git clone https://github.com/microsoft/vcpkg.git C:\vcpkg

# Bootstrap vcpkg using cmd shell (required for batch script)
SHELL ["cmd", "/S", "/C"]
RUN C:\vcpkg\bootstrap-vcpkg.bat -disableMetrics

# Switch back to PowerShell
SHELL ["powershell", "-Command", "$ErrorActionPreference = 'Stop'; $ProgressPreference = 'SilentlyContinue';"]

# Configure vcpkg environment variables
ENV VCPKG_ROOT="C:\\vcpkg"
ENV VCPKG_FEATURE_FLAGS="manifest,binarycaching"
ENV VCPKG_BINARY_SOURCES="clear;files,C:\\vcpkg\\binarycache,readwrite"

# Create binary cache directory
RUN New-Item -ItemType Directory -Path C:\vcpkg\binarycache -Force

# Create custom vcpkg triplet that uses MSBuild instead of Ninja
# This avoids rc.exe access violations in Windows Server Core containers
RUN New-Item -ItemType Directory -Path C:\vcpkg\triplets\community -Force
RUN $content = @(); \
    $content += 'set(VCPKG_TARGET_ARCHITECTURE x64)'; \
    $content += 'set(VCPKG_CRT_LINKAGE dynamic)'; \
    $content += 'set(VCPKG_LIBRARY_LINKAGE dynamic)'; \
    $content += 'set(VCPKG_CMAKE_GENERATOR \"Visual Studio 17 2022\")'; \
    $content += 'set(VCPKG_PLATFORM_TOOLSET v143)'; \
    $content | Out-File -FilePath C:\vcpkg\triplets\community\x64-windows-msbuild.cmake -Encoding ASCII

# Set the custom triplet as default
ENV VCPKG_DEFAULT_TRIPLET="x64-windows-msbuild"

# Add vcpkg to PATH
RUN [Environment]::SetEnvironmentVariable('PATH', \
    'C:\vcpkg;' + [System.Environment]::GetEnvironmentVariable('PATH','Machine'), \
    [EnvironmentVariableTarget]::Machine)

# Set working directory
WORKDIR C:\\workspace

# Default command
CMD ["powershell.exe"]