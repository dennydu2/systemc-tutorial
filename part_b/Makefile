# Update SYSTEMC_HOME to your SystemC installation path.
SYSTEMC_HOME ?= /usr/local/systemc-2.3.3

CXX      := g++
CXXFLAGS := -std=c++17 -O2 -I$(SYSTEMC_HOME)/include
LDFLAGS  := -L$(SYSTEMC_HOME)/lib-linux64 -lsystemc -lm

TARGET := soc_sim
SRCS   := sc_main.cpp producer.cpp consumer.cpp router.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o output_frame.ppm
