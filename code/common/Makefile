
INC = ./inc
SRC = ./src
PB = ./protobuf

all: clean $(SRC)/status.pb.c $(SRC)/sensors.pb.c $(SRC)/settings.pb.c finish

# Build rule for the protocol
$(SRC)/status.pb.c: $(PB)/status.proto
	protoc -o $(PB)/status.pb $(PB)/status.proto
	python nanopb_generator.py $(PB)/status.pb

$(SRC)/sensors.pb.c: $(PB)/sensors.proto
	protoc -o $(PB)/sensors.pb $(PB)/sensors.proto
	python nanopb_generator.py $(PB)/sensors.pb

$(SRC)/settings.pb.c: $(PB)/settings.proto
	protoc -o $(PB)/settings.pb $(PB)/settings.proto
	python nanopb_generator.py $(PB)/settings.pb
  
finish:
	mv -vf $(PB)/*.pb.h $(INC)/
	mv -vf $(PB)/*.pb.c $(SRC)/
	rm -vf $(PB)/*.pb

clean:
	rm -f $(SRC)/*.pb.c $(SRC)/*.pb.h
