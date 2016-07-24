
LLVM_CONFIG=llvm-config

SRC_DIR=$(PWD)
LDFLAGS+=$(shell $(LLVM_CONFIG) --ldflags)
COMMON_FLAGS=-Wall -Wextra -Werror -std=c++14 -g -pedantic
#CXXFLAGS+=$(shell $(LLVM_CONFIG) --cxxflags)
CXXFLAGS+=-I/usr/include -march=x86-64 -mtune=generic -O2 -pipe -fstack-protector-strong  -fPIC -fvisibility-inlines-hidden -Wall -W -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wno-missing-field-initializers -pedantic -Wno-long-long -Wno-uninitialized -Wdelete-non-virtual-dtor -Wno-comment -std=c++11 -ffunction-sections -fdata-sections -O3 -DNDEBUG  -fno-exceptions -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
CXXFLAGS+=$(COMMON_FLAGS)

CPPFLAGS+=$(shell $(LLVM_CONFIG) --cppflags)
CPPFLAGS+=-I$(SRC_DIR)

LIBS=$(shell $(LLVM_CONFIG) --libs bitreader core support --system-libs)

HELLO=helloworld
HELLO_OBJECTS=hello.o

default: $(HELLO)

%.o : $(SRC_DIR)/%.cpp
	@echo Compiling $*.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

%.bs : $(SRC_DIR)/%.cpp
	@echo Emmiting bitcode for $*.cpp
	$(CXX) -c -emit-llvm $(CPPFLAGS) $(CXXFLAGS) $<

$(HELLO) : $(HELLO_OBJECTS)
	@echo Linking $<
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS)

clean:
	rm -f $(HELLO) $(HELLO_OBJECTS)
