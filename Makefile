CXX = g++
CXXFLAGS = -g -Wall -Wextra -MMD -MP \
           -I./imgui -I./imgui/backends \
           -I./src/Crud -I./src/Database -I./src/Interface -I./src/Interpreter -I./src/Misc
           
LIBS = -lglfw -lGL

TARGET = Launch.exe
BUILD_DIR = .build

USER_SRCS = $(wildcard src/*/*.cpp) \
            $(wildcard *.cpp)

IMGUI_SRCS = imgui/imgui.cpp \
             imgui/imgui_draw.cpp \
             imgui/imgui_tables.cpp \
             imgui/imgui_widgets.cpp \
             imgui/backends/imgui_impl_glfw.cpp \
             imgui/backends/imgui_impl_opengl3.cpp \
             imgui/misc/cpp/imgui_stdlib.cpp

SRCS = $(USER_SRCS) $(IMGUI_SRCS)

OBJS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking final executable: $(TARGET)..."
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)
	@echo "Build complete! Run with ./$(TARGET)"

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Nuking the build directory and binary..."
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean

-include $(DEPS)