CC = g++ 
CXXFLAGS = -g -Wall -std=c++11 
CXXLINK = $(CC) 
LIBS =
OBJS = Cell.o CellTemplate.o Clockdat.o Clockpath.o Datapath.o FlipFlop.o InputNet.o main.o Net.o OutputNet.o Pin.o Receiver.o Table.o
RM = rm -f

# Creating the executable prog 
prog: $(OBJS)
	$(CXXLINK) -o prog $(OBJS) $(LIBS) 
	
# Creating object files using default rules
Cell.o: Cell.cpp Cell.h enums.h Net.h Receiver.h Clockdat.h \
 CellTemplate.h Table.h
CellTemplate.o: CellTemplate.cpp CellTemplate.h enums.h Table.h
Clockdat.o: Clockdat.cpp
Clockpath.o: Clockpath.cpp enums.h Cell.h Net.h Receiver.h Clockdat.h \
 CellTemplate.h Table.h InputNet.h Flipflop.h
Datapath.o: Datapath.cpp enums.h Cell.h Net.h Receiver.h Clockdat.h \
 CellTemplate.h Table.h Flipflop.h
FlipFlop.o: FlipFlop.cpp Flipflop.h enums.h Cell.h Net.h Receiver.h \
 Clockdat.h CellTemplate.h Table.h
InputNet.o: InputNet.cpp InputNet.h enums.h Net.h Receiver.h Clockdat.h
Net.o: Net.cpp Net.h enums.h Receiver.h Clockdat.h Cell.h CellTemplate.h \
 Table.h
OutputNet.o: OutputNet.cpp
Pin.o: Pin.cpp Net.h enums.h Receiver.h Clockdat.h Pin.h
Receiver.o: Receiver.cpp Receiver.h enums.h Cell.h Net.h Clockdat.h \
 CellTemplate.h Table.h
Table.o: Table.cpp
main.o: main.cpp Cell.h enums.h Net.h Receiver.h Clockdat.h \
 CellTemplate.h Table.h OutputNet.h InputNet.h

# Cleaning old files before new make 
clean:
	$(RM) prog *.o *.bak *~ #* core
