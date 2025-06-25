# Target platform
set(CMAKE_SYSTEM_NAME      Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Prefix used by MinGW-w64 binaries
set(TOOLCHAIN_PREFIX       x86_64-w64-mingw32)

# Cross-compilers
set(CMAKE_C_COMPILER       "${TOOLCHAIN_PREFIX}-gcc")
set(CMAKE_CXX_COMPILER     "${TOOLCHAIN_PREFIX}-g++")

# Where libc++ / libstdc++ and Windows import libs live
set(CMAKE_FIND_ROOT_PATH   "/usr/local/opt/mingw-w64")

# Strip useless host paths while searching for headers and libs
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)