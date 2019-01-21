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

PROTOS_PATH = chirp.proto

vpath %.proto $(PROTOS_PATH)

all: server client servicelayer

server: chirp.pb.o chirp.grpc.pb.o server.o User.o User.h
	$(CXX) $^ $(LDFLAGS) -o $@

client: chirp.pb.o chirp.grpc.pb.o client.o User.o User.h
	$(CXX) $^ $(LDFLAGS) -o $@

servicelayer: chirp.pb.o chirp.grpc.pb.o servicelayer.o User.o User.h
	$(CXX) $^ $(LDFLAGS) -o $@

.PRECIOUS: %.grpc.pb.cc
%.grpc.pb.cc: %.proto
	protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` chirp.proto   

.PRECIOUS: %.pb.cc
%.pb.cc: %.proto
	$(PROTOC) $(PROTOS_PATH) --cpp_out=./

clean:
	rm -f *.o *.pb.cc *.pb.h server client servicelayer
