CXX = clang++
CXXFLAGS = -I./include $(shell pkg-config --cflags freetype2)
LIBS = -lGL -ldl -lglfw -lassimp $(shell pkg-config --libs freetype2)

SRC_DIR = src
OBJ_DIR = obj
BUILD_DIR = build
RES_DIR = assets
SHADERS_DIR = shaders

SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
GLAD_SRC = $(SRC_DIR)/glad.c

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
GLAD_OBJ = $(OBJ_DIR)/glad.o

TARGET = Rake

all: $(TARGET)

$(TARGET): $(OBJS) $(GLAD_OBJ)
	@echo "Linking objects $(TARGET)"
	$(CXX) $(OBJS) $(GLAD_OBJ) $(LIBS) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(GLAD_OBJ): $(GLAD_SRC)
	@mkdir -p $(dir $@)
	clang $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

build: all
	@echo "Creating build directory..."
	mkdir -p $(BUILD_DIR)
	
	@echo "Copying resources..."
	cp $(TARGET) $(BUILD_DIR)/
	cp -r $(RES_DIR) $(BUILD_DIR)/
	cp -r $(SHADERS_DIR) $(BUILD_DIR)/
	@echo "Build complete! Check the '$(BUILD_DIR)' folder."

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR) $(TARGET)