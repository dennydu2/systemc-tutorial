# Change this path if your SystemC install is somewhere else.
SYSTEMC_HOME ?= /usr/local/systemc-2.3.3

# Compiler and source list.
CXX = g++
TARGET = soc_sim
SRCS = sc_main.cpp producer.cpp consumer.cpp router.cpp

# Build default target.
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) -std=c++17 -O2 -I$(SYSTEMC_HOME)/include $(SRCS) -o $(TARGET) -L$(SYSTEMC_HOME)/lib-linux64 -lsystemc -lm

# Run simulation.
run: $(TARGET)
	./$(TARGET)

# Remove binary and generated image.
clean:
	rm -f $(TARGET) *.o output_frame.ppm
