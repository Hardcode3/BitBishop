# Use Windows Server Core with LTSC 2022
FROM mcr.microsoft.com/windows/servercore:ltsc2022

SHELL ["powershell", "-Command", "$ErrorActionPreference = 'Stop';"]

ENV VCPKG_ROOT="C:\\vcpkg"
ENV VCPKG_FEATURE_FLAGS="manifest,binarycaching"
ENV VCPKG_BINARY_SOURCES="clear;x-gha,readwrite"
ENV PATH="C:\\vcpkg;C:\\cmake\\bin;$env:PATH"

# Install Chocolatey
RUN Set-ExecutionPolicy Bypass -Scope Process -Force; \
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.SecurityProtocolType]::Tls12; \
    iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

# Install core build dependencies
RUN choco install -y git cmake --installargs "ADD_CMAKE_TO_PATH=System" ninja zip unzip llvm

# Install Visual Studio Build Tools (C++ toolchain)
RUN Invoke-WebRequest -Uri https://aka.ms/vs/17/release/vs_BuildTools.exe -OutFile vs_buildtools.exe ; \
    Start-Process -FilePath .\\vs_buildtools.exe -ArgumentList '--quiet --wait --norestart --nocache \
    --add Microsoft.VisualStudio.Workload.VCTools \
    --add Microsoft.VisualStudio.Component.Windows10SDK.19041' -NoNewWindow -Wait ; \
    Remove-Item vs_buildtools.exe

# Clone and bootstrap vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git C:\\vcpkg ; \
    cd C:\\vcpkg ; \
    .\\bootstrap-vcpkg.bat

WORKDIR C:\\workspace

CMD ["cmd"]
