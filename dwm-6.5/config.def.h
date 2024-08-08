/* See LICENSE file for copyright and license details. */

#define ICONSIZE 20   /* icon size */
#define ICONSPACING 5 /* space between icon and title */

/* appearance */
static unsigned int borderpx  	    	 = 1;        /* border pixel of windows */
static const unsigned int fborderpx      = 2;        /* border pixel of floating windows */
static unsigned int snap      	    	 = 32;       /* snap pixel */
static unsigned int gappih    		 = 20;       /* horiz inner gap between windows */
static unsigned int gappiv    		 = 10;       /* vert inner gap between windows */
static unsigned int gappoh    		 = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    		 = 30;       /* vert outer gap between windows and screen edge */
static int smartgaps          		 = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;        /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 5;        /* systray spacing */
static const int systraypinningfailfirst = 1;        /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;        /* 0 means no systray */
static int showbar           		 = 1;        /* 0 means no bar */
static int topbar            		 = 1;        /* 0 means bottom bar */
static int user_bh                       = 10;        /* 2 is the default spacing around the bar's font */
static char dmenufont[]      		 = "IosevkaMayukaiCodepro:size=10";
static const char *fonts[]         	 = { "IosevkaMayukaiCodepro:size=10" ,"Symbols Nerd Font:size=12", "Noto Sans CJK JP:style=bold:size=10" };
static char normbgcolor[]          	 = "#222222";
static char normbordercolor[]      	 = "#444444";
static char normfgcolor[]          	 = "#bbbbbb";
static char selfgcolor[]           	 = "#eeeeee";
static char selbordercolor[]       	 = "#770000";
static char selbgcolor[]           	 = "#005577";
static char termcol0[]                   = "#282828"; // background
static char termcol1[]                   = "#cc241d"; // red
static char termcol2[]                   = "#98971a"; // green
static char termcol3[]                   = "#d79921"; // yellow
static char termcol4[]                   = "#458588"; // blue
static char termcol5[]                   = "#b16286"; // purple
static char termcol6[]                   = "#689d6a"; // aqua
static char termcol7[]                   = "#a89984"; // gray
static char termcol8[]                   = "#928374"; // bright black
static char termcol9[]                   = "#fb4934"; // bright red
static char termcol10[]                  = "#b8bb26"; // bright green
static char termcol11[]                  = "#fabd2f"; // bright yellow
static char termcol12[]                  = "#83a598"; // bright blue
static char termcol13[]                  = "#d3869b"; // bright purple
static char termcol14[]                  = "#8ec07c"; // bright aqua
static char termcol15[]                  = "#ebdbb2"; // white
static char *termcolor[] 		 = {
   termcol0,
   termcol1,
   termcol2,
   termcol3,
   termcol4,
   termcol5,
   termcol6,
   termcol7,
   termcol8,
   termcol9,
   termcol10,
   termcol11,
   termcol12,
   termcol13,
   termcol14,
   termcol15,
};

static char *colors[][3] 		 = {
       /*               	fg           bg            border   	*/
       [SchemeNorm] 	  = { normfgcolor, normbgcolor,  normbordercolor },
       [SchemeSel]  	  = { selfgcolor,  selbgcolor,   selbordercolor  },
       [SchemeTitle]      = { selfgcolor,  selbgcolor,   selbordercolor  },
       [SchemeLayout]     = { termcol13,   normbgcolor,  normbordercolor },
};




/* sticky indicator icon */
static const XPoint stickyicon[]    = { {0,0}, {4,0}, {4,8}, {2,6}, {0,8}, {0,0} }; /* represents the icon as an array of vertices */
static const XPoint stickyiconbb    = {4,8};	/* defines the bottom right corner of the polygon's bounding box (speeds up scaling) */

/* scratchpads */
typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm",  "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spfm",    "-g", "144x41", "-e", "lfrun", NULL };
const char *spcmd3[] = {"st", "-n", "spmusic", "-g", "160x20", "-e", "ncmpcpp", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"splf",    	spcmd2},
	{"spncmpcpp",   spcmd3},
};

/* tagging */
static const char *tags[]         = { "一", "二", "三", "四", "五", "六", "七", "八", "九" };
static const char *tagsalt[] 	  = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const int momentaryalttags = 0; /* 1 means alttags will show only when key is held down*/


static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      	  instance    title       	tags mask     isfloating   monitor */
	{ "Gimp",	  NULL,	      NULL,		0,	      1,	   -1 },
	{ "firefox",      NULL,	      NULL,		1 << 2,	      0,	   -1 },
	{ NULL,		  "spterm",   NULL,		SPTAG(0),     1,	   -1 },
	{ NULL,		  "spfm",     NULL,		SPTAG(1),     1,	   -1 },
	{ NULL,		  "spmusic",  NULL,		SPTAG(2),     1,	   -1 },
};

/* window swallowing */
static const int swaldecay = 3;
static const int swalretroactive = 1;
static const char swalsymbol[] = "👅";

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

/* extra include */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
#define STATUSBAR "dwmblocks"
#include "shiftview.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "color0",		STRING,	 &normbordercolor },
		{ "color1",		STRING,	 &selbordercolor },
		{ "color0",		STRING,	 &normbgcolor },
		{ "color4",		STRING,	 &normfgcolor },
		{ "color0",		STRING,	 &selfgcolor },
		{ "color4",		STRING,	 &selbgcolor },
		{ "color0",             STRING,  &termcol0 },
		{ "color1",             STRING,  &termcol1 },
		{ "color2",             STRING,  &termcol2 },
		{ "color3",             STRING,  &termcol3 },
		{ "color4",             STRING,  &termcol4 },
		{ "color5",             STRING,  &termcol5 },
		{ "color6",             STRING,  &termcol6 },
		{ "color7",             STRING,  &termcol7 },
		{ "color8",             STRING,  &termcol8 },
		{ "color9",             STRING,  &termcol9 },
		{ "color10",            STRING,  &termcol10 },
		{ "color11",            STRING,  &termcol11 },
		{ "color12",            STRING,  &termcol12 },
		{ "color13",            STRING,  &termcol13 },
		{ "color14",            STRING,  &termcol14 },
		{ "color15",            STRING,  &termcol15 },
		{ "borderpx",		INTEGER, &borderpx },
		{ "snap",		INTEGER, &snap },
		{ "showbar",		INTEGER, &showbar },
		{ "topbar",		INTEGER, &topbar },
		{ "nmaster",		INTEGER, &nmaster },
		{ "resizehints",	INTEGER, &resizehints },
		{ "mfact",		FLOAT,	 &mfact },
		{ "gappih",		INTEGER, &gappih },
		{ "gappiv",		INTEGER, &gappiv },
		{ "gappoh",		INTEGER, &gappoh },
		{ "gappov",		INTEGER, &gappov },
		{ "smartgaps",		INTEGER, &smartgaps },
};

/* key definitions */
#define MODKEY Mod4Mask
#include <X11/XF86keysym.h> /* including function keys */
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* stacking keys */
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_grave, ACTION##stack, {.i = 0 } }, \
/*	{ MOD, XK_q,     ACTION##stack, {.i = PREVSEL } }, \ */
/*	{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
/*	{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
/*	{ MOD, XK_x,     ACTION##stack, {.i = -1 } }, */




/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *pmenucmd[] = { "pmenu", NULL };

/* volume control */
static const char *vol_up[]   = { "volume", "up",   NULL };
static const char *vol_down[] = { "volume", "down", NULL };
static const char *vol_mute[] = { "volume", "mute", NULL };
static const char *mic_mute[] = { "audiomicmute",   NULL };

/* brightness control */
static const char *light_up[]   = { "backlight", "up",   NULL };
static const char *light_down[] = { "backlight", "down", NULL };

/* dmenuemojicmd */
static const char *emojicmd[]   = { "dmenuunicode",   NULL };

/* torrentcmd */
static const char *torrentcmd[] = { "td-toggle",   NULL };

/* screenshotcmd */
static const char *sscmd[]         = { "maimshot",  NULL };

/* {mount/unmount}cmd */
static const char *mountcmd[]      = { "mounter",  NULL };
static const char *unmountcmd[]    = { "unmounter",  NULL };

/* networking-cmds */
static const char *netmancmd[]     = { "dmenu-netman",  NULL };
static const char *bluemancmd[]    = { "dmenu-bluetooth",  NULL };

/* jgmenucmd */
static const char *jgmenucmd[] = {"jgmenu_run", NULL};

static const Key keys[] = {
	/* modifier                     key        	function        argument */
        TAGKEYS(                        XK_1,                      	0)
        TAGKEYS(                        XK_2,                      	1)
        TAGKEYS(                        XK_3,                      	2)
        TAGKEYS(                        XK_4,                      	3)
        TAGKEYS(                        XK_5,                      	4)
        TAGKEYS(                        XK_6,                     	5)
        TAGKEYS(                        XK_7,                      	6)
        TAGKEYS(                        XK_8,                      	7)
        TAGKEYS(                        XK_9,                      	8)
        STACKKEYS(MODKEY,                          		   	focus)
        STACKKEYS(MODKEY|ShiftMask,                		   	push)
	{ MODKEY,                       XK_0,           view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,           tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_Tab,         view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,           quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,           quit,           {1} },
	{ MODKEY,                       XK_e,           spawn,          {.v = emojicmd } },
	{ MODKEY,                       XK_t,           setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ControlMask,           XK_t,           spawn,          {.v = torrentcmd } },
	{ MODKEY,                       XK_u,           swalstopsel,    {0} },
	{ MODKEY,                       XK_i,           incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,           setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_p,           spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_a,           togglegaps,     {0} },
	{ MODKEY|ControlMask,           XK_a,           defaultgaps,    {0} },
	{ MODKEY|ShiftMask,             XK_a,           togglescratch,  {.ui = 0 } },
	{ MODKEY,                       XK_s,           togglesticky,   {0} },
	{ MODKEY|ControlMask,           XK_s,           spawn,          {.v = sscmd } },
	{ MODKEY|ShiftMask,             XK_s,           togglescratch,  {.ui = 1 } },
	{ MODKEY,                       XK_d,           incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_d,           togglescratch,  {.ui = 2 } },
	{ MODKEY,                       XK_f,           setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_f,           togglefullscr,  {0} },
	{ MODKEY,                       XK_g,           shiftview,      { .i = -1 } },
	{ MODKEY|ShiftMask,             XK_g,           shifttag,       { .i = -1 } },
	{ MODKEY,                       XK_h,           setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_h,           setcfact,       {.f = +0.25} },
	{ MODKEY,                       XK_l,           setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_l,           setcfact,       {.f = -0.25} },
	{ MODKEY,                       XK_semicolon,   shiftview,      { .i = 0 } },
	{ MODKEY|ShiftMask,             XK_semicolon,   shifttag,       { .i = 1 } },
	{ MODKEY,                       XK_Return,      zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_Return,      spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_z,           incrgaps,       {.i = +3 } },
	{ MODKEY,                       XK_x,           incrgaps,       {.i = -3 } },
	{ MODKEY|Mod1Mask,              XK_x,           spawn,          {.v = pmenucmd } },
	{ MODKEY|ShiftMask,             XK_c,           killclient,     {0} },
	{ MODKEY,                       XK_b,           togglebar,      {0} },
	{ MODKEY,                       XK_n,           togglealttag,   {0} },
	{ MODKEY,                       XK_m,           setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_comma,       focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_comma,       tagmon,         {.i = -1 } },
	{ MODKEY|ControlMask,           XK_comma,       cyclelayout,    {.i = -1 } },
	{ MODKEY,                       XK_period,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_period,      tagmon,         {.i = +1 } },
	{ MODKEY|ControlMask,           XK_period,      cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_space,       setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,       togglefloating, {0} },
	{ MODKEY,                       XK_F1,          xrdb,           {.v = NULL } },
	{ MODKEY,			XK_F2,          spawn,          {.v = mountcmd } },
	{ MODKEY,			XK_F3,          spawn,          {.v = unmountcmd } },
	{ MODKEY,			XK_F4,          spawn,          {.v = netmancmd } },
	{ MODKEY,			XK_F5,          spawn,          {.v = bluemancmd } },
	{ 0,         XF86XK_AudioMute,	          	spawn,	        {.v = vol_mute } },
	{ 0,         XF86XK_AudioRaiseVolume,	  	spawn,	        {.v = vol_up } },
	{ 0,         XF86XK_AudioLowerVolume,	  	spawn,	        {.v = vol_down } },
	{ 0,         XF86XK_AudioMicMute,	  	spawn,	        {.v = mic_mute } },
        { 0,         XF86XK_MonBrightnessUp,      	spawn,          {.v = light_up } },
        { 0,         XF86XK_MonBrightnessDown,    	spawn,          {.v = light_down } }, 
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      	button          function        argument */
	{ ClkLtSymbol,          0,              	Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              	Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              	Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              	Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              	Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              	Button3,        sigstatusbar,   {.i = 3} },
        { ClkStatusText,        0,             		Button4,        sigstatusbar,   {.i = 4} },
        { ClkStatusText,        0,              	Button5,        sigstatusbar,   {.i = 5} },
        { ClkStatusText,        ShiftMask,      	Button1,        sigstatusbar,   {.i = 6} },
	{ ClkClientWin,         MODKEY,         	Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         	Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         	Button3,        resizemouse,    {0} },
  	{ ClkClientWin,         MODKEY|ControlMask, 	Button1,      	swalmouse,      {0} },
	{ ClkTagBar,            0,              	Button1,        view,           {0} },
	{ ClkTagBar,            0,              	Button3,        toggleview,     {0} },
	{ ClkTagBar,		0,		        Button4,	shiftview,	{.i = -1} },
	{ ClkTagBar,		0,			Button5,	shiftview,	{.i = 1} },
	{ClkRootWin, 		0, 			Button3, 	spawn, 		{.v = jgmenucmd} },
};
