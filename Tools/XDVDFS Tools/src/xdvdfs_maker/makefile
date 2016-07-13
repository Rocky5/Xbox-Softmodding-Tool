# $Id: makefile,v 1.1.1.1 2002/07/27 10:00:20 fma Exp $

#######################################
# Include the main configuration file #
#######################################

include ../makefile.prefab

##############################
# Object files and libraries #
##############################

LIBS =
OBJS = $(BINDIR)/xdvdfs_maker.o \
		$(BINDIR)/BuildDirectoryTable.o \
		$(BINDIR)/BuildDirectoryTree.o \
		$(BINDIR)/CalcFilePlacement.o \
		$(BINDIR)/CreateISOVolumeDescriptors.o \
		$(BINDIR)/CreateXDVDFSVolumeDescriptor.o \
		$(BINDIR)/EvaluateFileSizes.o \
		$(BINDIR)/GetListFromDirectory.o \
		$(BINDIR)/GetListFromFile.o \
		$(BINDIR)/ListEnumFiles.o \
		$(BINDIR)/Tools.o \
		$(BINDIR)/list.o $(BINDIR)/xfile.o

##############
# Make rules #
##############

$(BINDIR)/xdvdfs_maker : $(OBJS)
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
	rm -f $(BINDIR)/xdvdfs_maker

cleanobjs!:
	rm -f $(BINDIR)/*.o


