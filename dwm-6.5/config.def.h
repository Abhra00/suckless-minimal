/* See LICENSE file for copyright and license details. */

/* fore restoring dwm */
#define SESSION_FILE "/tmp/dwm-session"

#define ICONSIZE 16   /* icon size */
#define ICONSPACING 5 /* space between icon and title */


/* appearance */
static unsigned int borderpx             = 3;        /* border pixel of windows */
static unsigned int fborderpx            = 5;        /* border pixel of floating windows */
static unsigned int snap                 = 32;       /* snap pixel */
static unsigned int gappih               = 20;       /* horiz inner gap between windows */
static unsigned int gappiv               = 10;       /* vert inner gap between windows */
static unsigned int gappoh               = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov               = 30;       /* vert outer gap between windows and screen edge */
static int smartgaps                     = 0;        /* 1 means no outer gap when there is only one window */
static int swallowfloating               = 0;        /* 1 means swallow floating windows by default */
static const unsigned int systraypinning = 0;        /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft  = 0;        /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;        /* systray spacing */
static const int systraypinningfailfirst = 1;        /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;        /* 0 means no systray */
static int showbar                       = 1;        /* 0 means no bar */
static int topbar                        = 1;        /* 0 means bottom bar */
static char font[]                       = "monospace:size=10";
static char dmenufont[]                  = "monospace:size=10";
static const char *fonts[]               = { font, "Noto Sans CJK JP:size=10", "Symbols Nerd Font:size=12" };
static char normbgcolor[]                = "#222222";
static char normbordercolor[]            = "#444444";
static char normfgcolor[]                = "#bbbbbb";
static char selfgcolor[]                 = "#eeeeee";
static char selbordercolor[]             = "#770000";
static char selbgcolor[]                 = "#005577";
static char *colors[][3] = {
       /*                 fg           bg           border   */
       [SchemeNorm]   = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]    = { selfgcolor,  selbgcolor,  selbordercolor  },
       [SchemeTitle]  = { selfgcolor,  selbgcolor,  selbordercolor  },
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
const char *spcmd2[] = {"st", "-n", "spfm",    "-g", "144x41", "-e", "ranger", NULL };
const char *spcmd3[] = {"st", "-n", "spmusic", "-g", "144x41", "-e", "ncmpcpp", NULL };
static Sp scratchpads[] = {
       /* name          cmd  */
       {"spterm",      spcmd1},
       {"spranger",    spcmd2},
       {"spmusic",     spcmd3},
};



/* tagging */
static const char *tags[]    = { "一", "二", "三", "四", "五", "六", "七", "八", "九" };
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const int momentaryalttags = 0; /* 1 means alttags will show only when key is held down*/

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance   title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,      NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,      NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "St",      NULL,      NULL,           0,         0,          1,           0,        -1 },
        { NULL,      "spterm",  NULL,           SPTAG(0),  1,          1,           0,        -1 },
        { NULL,      "spfm",    NULL,           SPTAG(1),  1,          1,           0,        -1 },
        { NULL,      "spmusic", NULL,           SPTAG(2),  1,          1,           0,        -1 },
	{ NULL,      NULL,      "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

/* extra includes */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
#include "shift-tools.c"

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
                { "font",               STRING,  &font },
                { "font",               STRING,  &dmenufont },
		{ "color0",		STRING,	 &normbordercolor },
		{ "color10",		STRING,	 &selbordercolor },
		{ "color0",		STRING,	 &normbgcolor },
		{ "color15",		STRING,	 &normfgcolor },
		{ "color0",		STRING,	 &selfgcolor },
		{ "color6",		STRING,	 &selbgcolor },
		{ "borderpx",		INTEGER, &borderpx },
		{ "snap",		INTEGER, &snap },
		{ "showbar",		INTEGER, &showbar },
		{ "topbar",		INTEGER, &topbar },
		{ "nmaster",		INTEGER, &nmaster },
		{ "resizehints",	INTEGER, &resizehints },
		{ "mfact",		FLOAT,	&mfact },
		{ "gappih",		INTEGER, &gappih },
		{ "gappiv",		INTEGER, &gappiv },
		{ "gappoh",		INTEGER, &gappoh },
		{ "gappov",		INTEGER, &gappov },
		{ "swallowfloating",	INTEGER, &swallowfloating },
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, "-p", "Run:->", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *pmenucmd[] = { "pmenu", NULL };

/* volume control */
static const char *vol_up[]   = { "volume-dwm", "up",   NULL };
static const char *vol_down[] = { "volume-dwm", "down", NULL };
static const char *vol_mute[] = { "volume-dwm", "mute", NULL };
static const char *mic_mute[] = { "audiomicmute-dwm",   NULL };

/* brightness control */
static const char *light_up[]   = { "backlight-dwm", "up",   NULL };
static const char *light_down[] = { "backlight-dwm", "down", NULL };

/* dmenuemojicmd */
static const char *emojicmd[]   = { "dmenuunicode",   NULL };

/* torrentcmd */
static const char *torrentcmd[] = { "td-toggle",   NULL };

/* screenshotcmd */
static const char *sscmd[]      = { "flameshot", "gui", NULL };

static const Key keys[] = {
	/* modifier                     key        function        	argument */
	STACKKEYS(MODKEY,                          		   	focus)
	STACKKEYS(MODKEY|ShiftMask,                		   	push)
        TAGKEYS(                        XK_1,                      	0)
        TAGKEYS(                        XK_2,                      	1)
        TAGKEYS(                        XK_3,                      	2)
        TAGKEYS(                        XK_4,                      	3)
        TAGKEYS(                        XK_5,                      	4)
        TAGKEYS(                        XK_6,                      	5)
        TAGKEYS(                        XK_7,                      	6)
        TAGKEYS(                        XK_8,                      	7)
	TAGKEYS(                        XK_9,                      	8)
	{ MODKEY,                       XK_p,      spawn,               {.v = dmenucmd } },
        { MODKEY|Mod1Mask,              XK_x,      spawn,               {.v = pmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,               {.v = termcmd } },
	{ MODKEY,			XK_e,      spawn,               {.v = emojicmd } },
	{ MODKEY|ControlMask,		XK_t,      spawn,               {.v = torrentcmd } },
	{ MODKEY|ControlMask,		XK_s,      spawn,               {.v = sscmd } },
	{ MODKEY,                       XK_b,      togglebar,      	{0} },
	{ MODKEY,                       XK_i,      incnmaster,     	{.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     	{.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       	{.f = -0.05} },
	{ MODKEY|ShiftMask,		XK_h,      shiftboth,      	{ .i = -1 }},
	{ MODKEY|ControlMask,		XK_h,      shiftswaptags,  	{ .i = -1 } },
	{ MODKEY|ControlMask,		XK_l,      shiftswaptags,  	{ .i = +1 } },
	{ MODKEY|ShiftMask,             XK_l,      shiftboth,      	{ .i = +1 } },
	{ MODKEY,                       XK_l,      setmfact,       	{.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       	{.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       	{.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       	{.f =  0.00} },
	{ MODKEY,                       XK_Return, zoom,           	{0} },
	{ MODKEY|Mod1Mask,              XK_u,      incrgaps,       	{.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       	{.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_i,      incrigaps,      	{.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      	{.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrogaps,      	{.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      	{.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_6,      incrihgaps,     	{.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     	{.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_7,      incrivgaps,     	{.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     	{.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,      incrohgaps,     	{.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     	{.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_9,      incrovgaps,     	{.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     	{.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     	{0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    	{0} },
	{ MODKEY,                       XK_Tab,    view,           	{0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     	{0} },
        { MODKEY|ControlMask,           XK_comma,  cyclelayout,    	{.i = -1 } },
        { MODKEY|ControlMask,           XK_period, cyclelayout,    	{.i = +1 } },
	{ MODKEY,                       XK_t,      setlayout,      	{.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      	{.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      	{.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      	{0} },
        { MODKEY,                       XK_Up,     shiftviewclients,    { .i = +1 } },
        { MODKEY|ShiftMask,             XK_Up,     shiftview,           { .i = +1 } },
        { MODKEY|ShiftMask,             XK_Down,   shiftview,           { .i = -1 } },
        { MODKEY,                       XK_Down,   shiftviewclients,    { .i = -1 } },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, 	{0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  	{0} },
	{ MODKEY,                       XK_0,      view,           	{.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            	{.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       	{.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       	{.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         	{.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         	{.i = +1 } },
	{ MODKEY,                       XK_n,      togglealttag,   	{0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  	{0} },
	{ MODKEY,            		XK_y,  	   togglescratch,  	{.ui = 0 } },
	{ MODKEY,            		XK_u,	   togglescratch,  	{.ui = 1 } },
	{ MODKEY,            		XK_x,	   togglescratch,  	{.ui = 2 } },
	{ MODKEY,                       XK_s,      togglesticky,   	{0} },
	{ MODKEY,			XK_F5,	   xrdb,		{.v = NULL } },
	{ MODKEY|ShiftMask,             XK_q,      quit,           	{0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           	{1} }, 
	{ 0,         XF86XK_AudioMute,	           spawn,	        {.v = vol_mute } },
	{ 0,         XF86XK_AudioRaiseVolume,	   spawn,	        {.v = vol_up } },
	{ 0,         XF86XK_AudioLowerVolume,	   spawn,	        {.v = vol_down } },
	{ 0,         XF86XK_AudioMicMute,	   spawn,	        {.v = mic_mute } },
        { 0,         XF86XK_MonBrightnessUp,       spawn,               {.v = light_up } },
        { 0,         XF86XK_MonBrightnessDown,     spawn,               {.v = light_down } }, 
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
        { ClkTagBar,            0,              Button4,        shiftview,      {.i = -1} },
        { ClkTagBar,            0,              Button5,        shiftview,      {.i = 1} },
};
