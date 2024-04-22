/*--------------------------------------------------------------------

        RSRC.H
        
        Resource identifier definitions for the SHOWWAVE program
        
  --------------------------------------------------------------------*/

/*--------------------------------------------------------------------
        ICONS
  --------------------------------------------------------------------*/

#define IDI_WAVE                1000

/*--------------------------------------------------------------------
        DIALOG TEMPLATES
  --------------------------------------------------------------------*/

#define DLG_MAIN                1000
#define DLG_ABOUT               1010

/*--------------------------------------------------------------------
        DIALOG ITEM IDs
  --------------------------------------------------------------------*/

/* For DLG_MAIN: */
#define IDD_FILETITLE           200
#define IDD_POSITION            210
#define IDD_SHOWWAVE            220
#define IDD_LENGTH              230
#define IDD_SCROLLBAR           240
#define IDD_PLAY                250
#define IDD_STOP                260
#define IDD_RECORD              270

/*--------------------------------------------------------------------
        MENU TEMPLATE
  --------------------------------------------------------------------*/

#define IDM_MAIN                1000

/*--------------------------------------------------------------------
        MENU ITEMS
  --------------------------------------------------------------------*/

/* main menu item IDs */
#define IDM_NEW                 110
#define IDM_OPEN                120
#define IDM_SAVE                130
#define IDM_SAVEAS              140
#define IDM_EXIT                150

#define IDM_MIXWITHFILE         160

#define IDM_INCREASEVOLUME      170
#define IDM_DECREASEVOLUME      180

#define IDM_ABOUT               190

/*--------------------------------------------------------------------
        STRINGS
  --------------------------------------------------------------------*/

#define IDS_CAPTION             1000        /* window caption */

//#define IDS_CANTOPENFILE        1100        /* MMIO error messages */
#define IDS_NOTWAVEFILE         1110
#define IDS_CORRUPTEDFILE       1120
#define IDS_CANTREADFORMAT      1130
#define IDS_UNSUPPORTEDFORMAT   1140
#define IDS_NOWAVEDATA          1150
#define IDS_OUTOFMEMORY         1160
#define IDS_CANTREADDATA        1170
#define IDS_CANTWRITEWAVE       1180
#define IDS_CANTWRITEFORMAT     1190
#define IDS_CANTWRITEDATA       1200

#define IDS_TIMEFMT             1210        /* program strings */
#define IDS_UNTITLED            1220
#define IDS_CONFIRMCLOSE        1230
#define IDS_OVERWRITE           1240
#define IDS_BADFREQUENCY        1250
#define IDS_BADRECORDFILE       1260

/*--------------------------------------------------------------------
    MAXIMUM LENGTH OF RESOURCE STRINGS
  --------------------------------------------------------------------*/

#define MAX_RSRC_STRING_LEN 80
