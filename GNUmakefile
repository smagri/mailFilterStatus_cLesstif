#
#
# ATNF DAS (Data Aquisition System) makefile
#
#

# spc -L$(XFIL) -lX11
# /usr/X11/lib

# note:
# XINCLUDEPATH and XLIBPATH are local to our site (ATNF Epping) login system.

# for LessTif version > 0.87.2
#XINCLUDEPATH =  /usr/local/LessTif/Motif2.0/include -I/usr/X11/include
#XLIBPATH = /usr/local/LessTif/Motif2.0/lib -L/usr/X11/lib

# for LessTif version 0.86
#rglinux XINCLUDEPATH = /usr/local/LessTif/include -I/usr/X11/include -Iinclude
XINCLUDEPATH = /usr/local/LessTif/include -I/usr/X11/include \
-I/usr/include/Xm -Iinclude
#rglinux XLIBPATH = /usr/local/LessTif/lib -L/usr/X11/lib
XLIBPATH = /usr/local/LessTif/lib -L/usr/X11/lib -L/usr/lib

XLIB = /usr/X11R6/lib
XLIBv = -L/usr/local/X11/lib -L/opt/SUNWmotif/lib
XINCv= -I/opt/SUNWmotif/include
XRv = -R/opt/SUNWmotif/lib


APROG = mail_status
AFN = dscomm

# -DDEBUG_DSCOMM
OTHER_ARG_afn =

# -DDEBUG_init_structs -DDEBUG_read_profile -DDEBUG_init_setup_bsb
# -DDEBUG_bsb_entry -DDEBUG_mview_summary_entry -DDEBUG_mview_detail
# -DDEBUG_startmon
# -DDEBUG_startmon_ptr
# -DDEBUG_ftb_calc_dwnload_setup
# -DDEBUG_update_summary_mview
# -DDEBUG_startmon_loop
# -DDEBUG_wait_for_sync
# -DDEBUG_stopmon
# -DDEBUG_auto_setup
# -DDEBUG_auto_setup

OTHER_ARG_app = -lXm -lXt -lX11 -lm \
	-DDEBUG -DDEBUG_bsb_entry -DDEBUG_init_setup_bsb \
	-DDEBUG_mview_summary_entry -DDEBUG_mview_detail \
	-DDEBUG_startmon -DDEBUG_bsb_calc_dwnload_setup \
	-DDEBUG_ftb_calc_dwnload_setup -DDEBUG_status_mview \
	-DDEBUG_update_summary_mview \
	-DDEBUG_stopmon -DDEBUG_auto_setup

OTHER_ARG_aprog = -lXm -lXt -lX11 -DDEBUG

CC = gcc
CFLAGS_fn = -Iinclude -O -Wall -ansi -pedantic -c
CFLAGS = -Iinclude -O -Wall -ansi -pedantic -g



# build application
app:
	$(CC) $(CFLAGS) das.c \
	-L$(XLIBPATH) -I$(XINCLUDEPATH) $(OTHER_ARG_app) dscomm.o -o das
	mv das bin/das

dsapp:
	$(CC) $(CFLAGS) dsapp.c dscomm.o -o dsapp


# To compile a single project program
aprog:
	$(CC) $(CFLAGS) $(APROG).c \
	-L$(XLIBPATH) -I$(XINCLUDEPATH) $(OTHER_ARG_aprog) -o $(APROG)

# To compile a function
afn:
	$(CC) $(CFLAGS_fn)  $(OTHER_ARG_afn) $(AFN).c -o $(AFN).o



# build application on venice
appv:
	$(CC) $(CFLAGS) das.c \
	-L/usr/local/X11/lib -I/opt/SUNWmotif/include -L/opt/SUNWmotif/lib \
	-R/opt/SUNWmotif/lib $(OTHER_ARG_app) -o das -lgen


# To compile a single project program on venice
aprogv:
	$(CC) $(CFLAGS) $(APROG).c \
	-L/usr/local/X11/lib -I/opt/SUNWmotif/include -L/opt/SUNWmotif/lib \
	-R/opt/SUNWmotif/lib $(OTHER_ARG_aprog) -o $(APROG) -lgen


# build application
appw:
	$(CC) $(CFLAGS) dasw.c -L$(XLIB) $(OTHER_ARG_app) -o dasw


# Various cleaning commands
clean:
	\rm -f *~ *.bak *.BAK #*#


rmlog:
	\rm -rf *.log
