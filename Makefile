# === Compiler Setup ===
CXX_LINUX = g++
CXX_WINDOWS = x86_64-w64-mingw32-g++

# === Project Paths ===
VCPKG_ROOT = /home/blaze/vcpkg
SRC_DIR = src
GLAD_SRC = glad/src/gl.c
SRC = $(wildcard $(SRC_DIR)/*.cpp) $(GLAD_SRC)

TARGET_DIR = build
TARGET_LINUX = $(TARGET_DIR)/mainx64li
TARGET_WINDOWS = $(TARGET_DIR)/mainx64win.exe

# === Flags for Linux ===
CXXFLAGS_LINUX = -std=c++17 -I$(SRC_DIR)/include -Iglad/include \
	-I$(VCPKG_ROOT)/installed/x64-linux/include

LDFLAGS_LINUX = -L$(VCPKG_ROOT)/installed/x64-linux/lib \
	-lglfw3 -lGL -ldl -lpthread -lm -lX11

# === Flags for Windows ===
CXXFLAGS_WINDOWS = -std=c++17 -I$(SRC_DIR)/include -Iglad/include \
	-I$(VCPKG_ROOT)/installed/x64-mingw-static/include

LDFLAGS_WINDOWS = -L$(VCPKG_ROOT)/installed/x64-mingw-static/lib \
	-lglfw3 -lopengl32 -lwinmm -lgdi32 -lkernel32 -luser32 \
	-static -static-libgcc -static-libstdc++ 

# === Rules ===
.PHONY: all linux windows clean run_linux run_windows

all: linux 

linux: $(TARGET_LINUX)

windows: $(TARGET_WINDOWS)

# === Linking Rules ===
$(TARGET_LINUX): $(SRC)
	@mkdir -p $(TARGET_DIR)
	$(CXX_LINUX) $(CXXFLAGS_LINUX) $^ $(LDFLAGS_LINUX) -o $@

$(TARGET_WINDOWS): $(SRC)
	@mkdir -p $(TARGET_DIR)
	$(CXX_WINDOWS) $(CXXFLAGS_WINDOWS) $^ $(LDFLAGS_WINDOWS) -o $@

# === Run Targets ===
run_linux: $(TARGET_LINUX)
	./$(TARGET_LINUX)

run_windows: $(TARGET_WINDOWS)
	./$(TARGET_WINDOWS)

# === Clean ===
clean:
	rm -rf $(TARGET_DIR)

