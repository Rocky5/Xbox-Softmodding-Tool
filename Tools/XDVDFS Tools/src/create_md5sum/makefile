# $Id: makefile,v 1.1.1.1 2002/07/27 10:00:20 fma Exp $

#######################################
# Include the main configuration file #
#######################################

include ../makefile.prefab

##############################
# Object files and libraries #
##############################

LIBS =
OBJS = $(BINDIR)/create_md5sum.o $(BINDIR)/list.o $(BINDIR)/md5.o $(BINDIR)/xfile.o

##############
# Make rules #
##############

$(BINDIR)/create_md5sums : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

$(BINDIR)/%.o: %.c
	$(CC) -I$(INCDIR) $(CCFLAGS) -c $< -o $@

$(BINDIR)/%.o: ../list/%.c
	$(CC) -I$(INCDIR) $(CCFLAGS) -c $< -o $@

$(BINDIR)/%.o: ../xfile/%.c
	$(CC) -I$(INCDIR) $(CCFLAGS) -c $< -o $@

$(BINDIR)/%.o: ../md5/%.c
	$(CC) -I$(INCDIR) $(CCFLAGS) -c $< -o $@

$(BINDIR)/%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -f $(BINDIR)/*.o
	rm -f $(BINDIR)/create_md5sums

cleanobjs!:
	rm -f $(BINDIR)/*.o
