SYSTEMC_HOME ?= /usr/local/systemc-2.3.3

CXX := g++
CXXFLAGS := -std=c++17 -O2 -I$(SYSTEMC_HOME)/include $(shell pkg-config --cflags opencv4)
LDFLAGS_SYS := -L$(SYSTEMC_HOME)/lib-linux64 -Wl,-rpath,$(SYSTEMC_HOME)/lib-linux64 -lsystemc -lm -pthread
LDFLAGS_CV := $(shell pkg-config --libs opencv4)

TARGET_HELLO := hello_opencv
TARGET_EDGE := edge_detect
TARGET_SOC := soc_line

SOC_SRCS := sc_main.cpp producer.cpp memory.cpp line_processor.cpp

all: $(TARGET_HELLO) $(TARGET_EDGE) $(TARGET_SOC)

$(TARGET_HELLO): hello_opencv.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS_CV)

$(TARGET_EDGE): edge_detect.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS_CV)

$(TARGET_SOC): $(SOC_SRCS)
	$(CXX) $(CXXFLAGS) $(SOC_SRCS) -o $@ $(LDFLAGS_SYS) $(LDFLAGS_CV)

clean:
	rm -f $(TARGET_HELLO) $(TARGET_EDGE) $(TARGET_SOC) *.o hello_copy.jpg edges.jpg line_output.jpg
