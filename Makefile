CXX = clang++
CXXFLAGS = -I./include
LIBS = -lGL -ldl -lglfw -lassimp

SRC_DIR = src
BUILD_DIR = build
RES_DIR = textures
SHADERS_DIR = shaders

SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/core/*.cpp $(SRC_DIR)/lib/*.cpp $(SRC_DIR)/glad.c
TARGET = $(BUILD_DIR)/Rake

run:
	$(CXX) $(SRCS) $(CXXFLAGS) $(LIBS) -o Rake && ./Rake

build: clean
	@echo "Creating build directory..."
	mkdir -p $(BUILD_DIR)
	
	@echo "Compiling..."
	$(CXX) $(SRCS) $(CXXFLAGS) $(LIBS) -o $(TARGET)
	
	@echo "Copying resources..."
	cp -r $(RES_DIR) $(BUILD_DIR)/
	cp -r $(SHADERS_DIR) $(BUILD_DIR)/
	
	@echo "Build complete! Check the '$(BUILD_DIR)' folder."

clean:
	rm -rf $(BUILD_DIR)
	rm -f Rake
	