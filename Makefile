LXX = $(CXX)

INCDIR+= -I.


LXXFLAGS+=

CXXFLAGS+= -O2 -g -Wall -Werror $(INCDIR) -DLINUX

OBJS = \
main.o \
cansend.o \
canrec.o \
canwrapper.o \

all:  cantest

# Actual application
cantest: $(OBJS)
	$(LXX) $(CXXFLAGS) $(OBJS) $(LXXFLAGS) -o $@

clean:
	rm -rf cantest *.o .depend
