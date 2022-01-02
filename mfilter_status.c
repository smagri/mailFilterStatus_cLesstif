/* Determines the state of each of the mail filters and reflects the
   state on the GUI.  Essentially, if the mail filter file has a size
   greater than 0, the gui line for that filter's state turns
   turqoise.  Thus, NOT_EMPTY_MFILTER_COLOUR => 0 size for the filter
   file and EMPTY_MFILTER_COLOUR => >0 size for the filter file.  A
   check of the size of the mail filter files is done ever
   CHECK_MFILTERS_INTERVAL_MS.  The mail filter files to be checked in
   the MFILER_ROOT_DIR are given as command line input; from the file
   MFILTER_ROOT_DIR/mfilter_files. */


/*-----------------------------------------------------------------------------
 History:

 07-Aug-1998    Simone Magri    Original routines
 25-May-2001	Simone Magri    gave it a more appropriate default geometry

-----------------------------------------------------------------------------*/


#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/Xm.h>	/* Standard Motif Widget set header file. */


/* Symbolic Constants */
#define EMPTY_MFILTER_COLOUR "#300030"
#define NOT_EMPTY_MFILTER_COLOUR "turquoise4"
#define MAX_FILTERS 10

/* can't use ~smagri in path, stat doesn't like it */
#define MFILTER_ROOT_DIR "/lu1/smagri/mfilter/"
#define CHECK_MFILTERS_INTERVAL_MS (1000) /* every 1 ms */


struct stat filename_stats; /* allocate storage for my local stat structure */
char *filename; /* mail filter file name, wrt MFILTER_ROOT_DIR */
Widget filter_file_id[MAX_FILTERS];
const unsigned long timout_interval_ms = CHECK_MFILTERS_INTERVAL_MS; 

struct main_arg_struct{
  int argc;
  char *argv[MAX_FILTERS];
};

Widget lastChild;
XmString cmpd_str;      /* generic pointer to a compound string */

XtAppContext app_context;	/* For maximum OS portability. */
String fallback_resources[] = 
{
  "mfilter_status*fontList:	lucidasans-bold-10",
  "mfilter_status.geometry:    129x97+0-0",
  "mfilter_status*foreground:	white",
  "mfilter_status*background:   MidnightBlue",
  (char *) NULL
};


void process_filters_cbk_proc(XtPointer client_data, XtIntervalId *id);


int main(int argc, char *argv[])

{
  Widget topLevel, topForm, titleLabel;
  struct main_arg_struct main_arg;
  int i;
  int str_height;         /* generic int, height of string */
  XmFontList fontlist;    /* default FontList used in application */


  main_arg.argc = argc;
  for (i=0; i<argc; i++)
    main_arg.argv[i] = argv[i];


   /* Register the default language procedure; this started in X11R5 */
   XtSetLanguageProc(NULL,NULL,NULL);

  /* XtVaAppInitialize initalizes the Toolkit (Xt Intrinsics library +
     'a' widget library); creates the topLevel shell (parent of all
     other widgets, applicationShellWidgetClass). */
  /* That is, determines and uses application: display, screen, name
     and local(language string); parses command line arguments;
     generates resource database; as well as the creating the topLevel
     shell. */
  topLevel = XtVaAppInitialize (&app_context,
                                "mfilter_status", /* Application Class. */
                                NULL, 0,
                                &argc, argv,
                                fallback_resources,
                                NULL, 0);

  /* can create a managed Manager widget here as geometry management
     is negotated only after widgets are relaized */
  topForm =
    XtVaCreateManagedWidget ("topForm", /* arbitrary widget instance name */
			     xmFormWidgetClass, /* widget class-header file */
			     topLevel,          /* widget parent name */
			     NULL); 

  cmpd_str = XmStringCreateLocalized("Mail Filter Status Tool");
  titleLabel =
    XtVaCreateManagedWidget("titleLabel",
			    xmLabelWidgetClass, topForm,
			    XmNlabelString, cmpd_str,
			    XtVaTypedArg, XmNbackground,
			    XmRString, "magenta4", strlen("magenta4")+1,
			    XtVaTypedArg, XmNforeground,
			    XmRString, "Khaki", strlen("Khaki")+1,
			    XmNleftAttachment,XmATTACH_FORM,
			    XmNrightAttachment, XmATTACH_FORM,
			    XmNtopAttachment, XmATTACH_FORM,
			    NULL);

  XtVaGetValues(titleLabel,
                XmNfontList, &fontlist,   /* resource, storage address */
                NULL);
  str_height = XmStringHeight(fontlist, cmpd_str);
  XtVaSetValues(titleLabel,
		XmNheight, (str_height*2),
		NULL);

  for (i=1; i<argc; i++){
    cmpd_str = XmStringCreateLocalized(argv[i]);
    if (i == 1){
      /* first filter file */
      lastChild = filter_file_id[i] =
	XtVaCreateManagedWidget(argv[i],
				xmLabelWidgetClass, topForm,
				XmNlabelString, cmpd_str,
				XmNleftAttachment,XmATTACH_FORM,
				XmNrightAttachment, XmATTACH_FORM,
				XmNtopAttachment, XmATTACH_WIDGET,
				XmNtopWidget, titleLabel,
				NULL);
    }
    else if (i == argc){
      /* last filter file */
      lastChild = filter_file_id[i] =
	XtVaCreateManagedWidget(argv[i],
				xmLabelWidgetClass, topForm,
				XmNlabelString, cmpd_str,
				XmNleftAttachment,XmATTACH_FORM,
				XmNrightAttachment, XmATTACH_FORM,
				XmNbottomAttachment, XmATTACH_FORM,
				XmNtopAttachment, XmATTACH_WIDGET,
				NULL);
    }
    else{
      /* all the files in between the fist and the last */
      lastChild = filter_file_id[i] =
	XtVaCreateManagedWidget(argv[i],
				xmLabelWidgetClass, topForm,
				XmNlabelString, cmpd_str,
				XmNtopAttachment, XmATTACH_WIDGET,
				XmNtopWidget, lastChild,
				XmNleftAttachment,XmATTACH_FORM,
				XmNrightAttachment, XmATTACH_FORM,
				NULL);

    } /* for: all command line arguments */
  } 
  XmStringFree(cmpd_str);
    

  XtAppAddTimeOut(app_context,
		  timout_interval_ms, process_filters_cbk_proc,
		  (XtPointer) &main_arg);


  /* Create 'windows for widgets' and _maps_ (X concept) them onto the
     screen. */
  XtRealizeWidget (topLevel);
  
  /* Start event(user action) loop. */
  XtAppMainLoop (app_context);
  return 0;
}


void process_filters_cbk_proc(XtPointer client_data, XtIntervalId *id)

{
  void process_filters(struct main_arg_struct *, int);
  struct main_arg_struct *main_arg = (struct main_arg_struct *) client_data;

  process_filters(main_arg, main_arg->argc);

  /* Re-register the XtTimerCallbackProc again, Xt automatically
     unregisters it as soon as the callback procedure executes. */
  XtAppAddTimeOut(app_context,
		  timout_interval_ms, process_filters_cbk_proc,
		  (XtPointer) main_arg);
  return;
}



void process_filters(struct main_arg_struct *main_arg, int max_num_files)

/* Determines the status of the filter files and indicates their
   status. Filter files are given on the command line, with their root
   path being MFILTER_ROOT_DIR. */

{
  int i, retval;
  extern int errno;
  char plain_stray[80];

  for (i=1; i<max_num_files; i++){
    strcpy (&plain_stray[0], MFILTER_ROOT_DIR);
    filename = strcat(&plain_stray[0], main_arg->argv[i]);
    retval = stat(filename, &filename_stats);

    if (retval == -1){
      XtVaSetValues(filter_file_id[i],
		    XtVaTypedArg, XmNbackground,
		    XmRString, EMPTY_MFILTER_COLOUR,
		    strlen(EMPTY_MFILTER_COLOUR)+1,
		    NULL);
      continue; /* continue to next itteration of for loop file
		   doesn't exist */
    }

    /* If file exists and contains new mail; or, doesn't exist or
       exists yet doesn't contain mail; indicate on user interface
       with NOT_EMPTY_MFILTER_COLOUR and EMPTY_MFILTER_COLOUR. */
    if (filename_stats.st_size > 0){
      XtVaSetValues(filter_file_id[i],
		    XtVaTypedArg, XmNbackground,
		    XmRString, NOT_EMPTY_MFILTER_COLOUR,
		    strlen(NOT_EMPTY_MFILTER_COLOUR)+1,
		    NULL);
    }
    else{
      XtVaSetValues(filter_file_id[i],
		    XtVaTypedArg, XmNbackground,
		    XmRString, EMPTY_MFILTER_COLOUR,
		    strlen(EMPTY_MFILTER_COLOUR)+1,
		    NULL);
    }

  } /* for: all filenames given on the command line */

  return;
}
