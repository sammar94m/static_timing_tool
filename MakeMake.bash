var=`ls *"$3"| sed "s/"$3"/.o/g"`
if [ "$1" == "gcc" ]; then
echo "CC = "$1" 
CFLAGS = -g -Wall 
CCLINK = \$(CC) 
LIBS =
OBJS = "$var"
RM = rm -f

# Creating the executable "$2" 
"$2": \$(OBJS)
	\$(CCLINK) -o "$2" \$(OBJS) \$(LIBS) 
	
# Creating object files using default rules">makefile
else
echo "CC = "$1" 
CXXFLAGS = -g -Wall -std=c++11 
CXXLINK = \$(CC) 
LIBS =
OBJS = "$var"
RM = rm -f

# Creating the executable "$2" 
"$2": \$(OBJS)
	\$(CXXLINK) -o "$2" \$(OBJS) \$(LIBS) 
	
# Creating object files using default rules">makefile
fi
"$1" -g -std=c++11 -MM *"$3">>makefile
echo "
# Cleaning old files before new make 
clean:
	\$(RM) "$2" *.o *.bak *~ "#"* core">>makefile
