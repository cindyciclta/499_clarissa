HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
CXX = g++
CPPFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++11 
ifeq ($(SYSTEM),Darwin)
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc`\
           -lgrpc++_reflection\
           -ldl
else
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc`\
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
           -ldl
endif
PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)` 

PROTOS_PATH = *.proto

vpath %.proto $(PROTOS_PATH)

all: keyvaluestoreinstance keyvaluestoretest test backend clientcommandline servicelayer

backend: chirp.pb.o chirp.grpc.pb.o backend.o keyvaluestoreserver.cc keyvaluestoreserver.h
	$(CXX) $^ $(LDFLAGS) -o $@ -lgtest

clientcommandline: chirp.pb.o chirp.grpc.pb.o clientcommandline.o clientfunctionalities.o clientfunctionalities.h
	$(CXX) $^ $(LDFLAGS) -o $@ -lgtest -lgflags 

servicelayer: chirp.pb.o chirp.grpc.pb.o servicelayer.o servicelayerfunctionalities.o servicelayerfunctionalities.h
	$(CXX) $^ $(LDFLAGS) -o $@ -lgtest 
test: chirp.pb.o chirp.grpc.pb.o test.o clientfunctionalities.o clientfunctionalities.h servicelayerfunctionalities.o servicelayerfunctionalities.h
	$(CXX) $^ $(LDFLAGS) -o $@ -lgtest 

keyvaluestoreinstance: keyvaluestoreinstance.o keyvaluestoreinstance.h keyvaluestorevirtual.h 
	g++ -std=c++11 -c -o keyvaluestoreinstance.o keyvaluestoreinstance.cc

keyvaluestoretest: keyvaluestoretest.o keyvaluestoreinstance.o keyvaluestoreinstance.h
	$(CXX) $^ $(LDFLAGS) -o $@ -lgtest

.PRECIOUS: %.grpc.pb.cc
%.grpc.pb.cc: %.proto
	protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` chirp.proto

.PRECIOUS: %.pb.cc
%.pb.cc: %.proto
	$(PROTOC) $(PROTOS_PATH) --cpp_out=./

clean:
	rm -f *.o *.pb.cc *.pb.h backend clientcommandline servicelayer test keyvaluestoreinstance keyvaluestoretest
