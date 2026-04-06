SYSTEMC_HOME ?= /usr/local/systemc-2.3.3

CXX = g++
TARGET = soc_sim
SRCS = sc_main.cpp producer.cpp consumer.cpp router.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) -std=c++17 -O2 -I$(SYSTEMC_HOME)/include $(SRCS) -o $(TARGET) -L$(SYSTEMC_HOME)/lib-linux64 -lsystemc -lm

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o output_frame.ppm
