PLANCK = planck

VB = @ 	# verbose

SRCDIR = src
OBJDIR = obj
BINDIR = bin

VPATH = src

CCFLAGS = -c -std=c11
LDFLAGS = -lreadline

OBJS = $(OBJDIR)/planck.o

all : create_dir $(OBJS)
	$(VB)echo "Linking.."
	$(VB)gcc -o $(BINDIR)/$(PLANCK) $(OBJS) $(LDFLAGS)
	
clean : create_dir
	$(VB)rm -f $(OBJDIR)/*
	$(VB)rm -f $(BINDIR)/*

create_dir :
	$(VB)mkdir -p $(OBJDIR)
	$(VB)mkdir -p $(BINDIR)

$(OBJDIR)/%.o : %.c
	$(VB)echo "compile " $<
	$(VB)gcc $(CCFLAGS) -o $@ $<