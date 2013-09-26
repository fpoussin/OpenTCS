
INC = ./inc
SRC = ./src
PB = ./protobuf

.PHONY: all

all: clean $(SRC)/messages.pb $(SRC)/nanopb.pb finish

# Build rule for the protocol
$(SRC)/%.pb: $(PB)/%.proto
	protoc -I/usr/include -I. -o $@ $^
	python nanopb_generator.py $@ 
  
finish:
	mv -vf $(SRC)/*.pb.h $(INC)/
	rm -vf $(SRC)/*.pb

clean:
	rm -f $(SRC)/*.pb*
	rm -f $(INC)/*.pb*
