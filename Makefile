include Makeconfig

# Ensure the obj and bin directories exist
$(shell mkdir -p obj bin)

all : $(TARGET)

# Rule to link object files into the final executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile source files into object files
obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf obj/* bin/*

# Setup target (informational)
setup:
	@echo "Setup the build: `date`, starting..."
	@echo "The compiler is $(CC), version $(CC_VERSION)"
	@echo "The flags used for compilation are: $(CFLAGS)"

Test1 :
	@echo "Test1: `date`, starting..."
	./$(TARGET) -l 
	@echo "Test1: `date`, done."