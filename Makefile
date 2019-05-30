PLANCK = planck

VB = @ 	# verbose

SRCDIR = src
OBJDIR = obj
BINDIR = bin

VPATH = src

INCDIR = -Isrc

CCFLAGS = -c -std=c11
LDFLAGS = -lreadline

OBJS  =  $(OBJDIR)/planck.o
OBJS +=  $(OBJDIR)/code_gen.o
OBJS +=  $(OBJDIR)/virtual_machine.o
OBJS +=  $(OBJDIR)/lex.yy.o
OBJS +=  $(OBJDIR)/y.tab.o
	 

all : create_dir $(OBJS)
	$(VB)echo "Linking.."
	$(VB)gcc -o $(BINDIR)/$(PLANCK) $(OBJS) $(INCDIR) $(LDFLAGS)
	
clean : create_dir
	$(VB)rm -f $(OBJDIR)/*
	$(VB)rm -f $(BINDIR)/*

create_dir :
	$(VB)mkdir -p $(OBJDIR)
	$(VB)mkdir -p $(BINDIR)

$(OBJDIR)/%.o : %.c
	$(VB)echo "compile " $<
	$(VB)gcc $(CCFLAGS) $(INCDIR) -o $@ $<