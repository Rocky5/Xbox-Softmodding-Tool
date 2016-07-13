# $Id: makefile,v 1.1.1.1 2002/07/27 10:00:20 fma Exp $

#######################################
# Include the main configuration file #
#######################################

include ../makefile.prefab

##############################
# Object files and libraries #
##############################

LIBS =
OBJS = $(BINDIR)/dumper.o $(BINDIR)/buffered_io.o $(BINDIR)/xdvdfs.o \
		$(BINDIR)/list.o $(BINDIR)/xfile.o

##############
# Make rules #
##############

$(BINDIR)/xdvdfs_dumper : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

$(BINDIR)/%.o: %.c
	$(CC) -I$(INCDIR) $(CCFLAGS) -c $< -o $@

$(BINDIR)/%.o: ../xdvdfs/%.c
	$(CC) -I$(INCDIR) $(CCFLAGS) -c $< -o $@

$(BINDIR)/%.o: ../list/%.c
	$(CC) -I$(INCDIR) $(CCFLAGS) -c $< -o $@

$(BINDIR)/%.o: ../xfile/%.c
	$(CC) -I$(INCDIR) $(CCFLAGS) -c $< -o $@

$(BINDIR)/%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -f $(BINDIR)/*.o
	rm -f $(BINDIR)/xdvdfs_dumper

cleanobjs!:
	rm -f $(BINDIR)/*.o


