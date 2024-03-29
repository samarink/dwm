/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 15;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 15;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 22;       /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]          = { "firacode:size=11" };
static const char dmenufont[]       = "firacode:size=11";
static const char col_gray1[]       = "#2e3440";
static const char col_gray2[]       = "#4c566a";
static const char col_gray3[]       = "#d8dee9";
static const char col_gray4[]       = "#eeeeee";
static const char col_gray5[]       = "#3b4252";
static const char col_cyan[]        = "#88c0d0";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_gray5, col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class          instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "kitty",            NULL,  NULL, 0,         0,          1,          -1,        -1 },
	{ "Thunar",						NULL,  NULL, 0,         1,          0,          -1,        -1 },
	{ "Pavucontrol",      NULL,  NULL, 0,         1,          0,          -1,        -1 },
	{ "copyq",            NULL,  NULL, 0,         1,          0,          -1,        -1 },
	{ "TelegramDesktop",  NULL,  NULL, 0,         1,          0,          -1,        -1 },
	{ "Transmission-gtk", NULL,  NULL, 0,         1,          0,          -1,        -1 },
	{ "Rocket.Chat",			NULL,  NULL, 0,         1,          0,          -1,        -1 },
	{ "Anki",							NULL,  NULL, 0,         1,          0,          -1,        -1 },
	{ "discord",					NULL,  NULL, 0,         1,          0,          -1,        -1 },
	{ "syncplay",					NULL,  NULL, 0,         1,          0,          -1,        -1 },
	{ "zoom",							NULL,  NULL, 0,         1,          0,          -1,        -1 },
	{                     NULL,  NULL, "Event Tester", 0,         1,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },                    /* Default: Master on left, slaves on right */
  { "TTT",	    bstack },                  /* Master on top, slaves on bottom */

 	{ "[@]",      spiral },                  /* Fibonacci spiral */
 	{ "[\\]",     dwindle },                 /* Decreasing in size right and leftward */

	{ "[M]",      monocle },                 /* All windows on top of eachother */
  { "H[]",	    deck },                    /* Master on left, slaves in monocle-like mode on right */

  { "|M|",	    centeredmaster },		       /* Master in middle, slaves on sides */
	{ ">M>",	    centeredfloatingmaster },  /* Same but master floats */

	{ "><>",      NULL },                    /* no layout function means floating behavior */
  { NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, XK_q,     ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
	{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
	{ MOD, XK_w,     ACTION##stack, {.i = -1 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { "kitty", NULL };

/* commands spawned when clicking statusbar, the mouse button pressed is exported as BUTTON */
static char *statuscmds[] = { "dwm-volume" };
static char *statuscmd[] = { "/bin/sh", "-c", NULL, NULL };

static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "kitty", "-T", scratchpadname, NULL };

#include "shiftview.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	STACKKEYS(MODKEY,                                     focus)
	STACKKEYS(MODKEY|ShiftMask,                           push)
	{ MODKEY,                       XK_p,                 spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,            spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_u,                 togglescratch,  {.v = scratchpadcmd } },
  { MODKEY,                       XK_y,                 spawn,          SHCMD("copyq toggle") },
  { MODKEY,                       XK_n,                 spawn,          SHCMD("chromium") },
  { MODKEY|ShiftMask,             XK_n,                 spawn,          SHCMD("chromium --incognito") },
  { MODKEY|Mod1Mask,							XK_n,                 spawn,          SHCMD("tor-browser") },
  { MODKEY,                       XK_v,                 spawn,          SHCMD("kitty -e nvim") },
  { MODKEY,                       XK_e,                 spawn,          SHCMD("kitty -e lf") },
  { MODKEY|ShiftMask,             XK_e,                 spawn,          SHCMD("thunar") },
  { MODKEY,                       XK_F1,                spawn,          SHCMD("pavucontrol") },
  { 0,                            XK_Print,             spawn,          SHCMD("flameshot gui") },
  { MODKEY,                       XK_F2,                spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%") },
  { MODKEY,                       XK_F3,                spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%") },
  { MODKEY,                       XK_F4,                spawn,          SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle") },
  { MODKEY|ShiftMask,             XK_grave,             spawn,          SHCMD("sudo systemctl suspend") },
  { MODKEY|Mod1Mask,              XK_grave,             spawn,          SHCMD("sudo systemctl hibernate") },
  { MODKEY|ControlMask,						XK_grave,             spawn,          SHCMD("sudo shutdown now") },
  { MODKEY|Mod1Mask,              XK_o,                 spawn,          SHCMD("telegram-desktop") },
  { MODKEY|Mod1Mask,              XK_k,                 spawn,          SHCMD("transmission-gtk") },
  { MODKEY|Mod1Mask,              XK_a,                 spawn,          SHCMD("anki") },
  { MODKEY|Mod1Mask,              XK_p,                 spawn,          SHCMD("thunderbird") },
  { MODKEY|Mod1Mask,              XK_s,                 spawn,          SHCMD("syncplay") },
  { MODKEY|Mod1Mask,              XK_f,                 spawn,          SHCMD("firefox") },
  { MODKEY|Mod1Mask,              XK_d,                 spawn,          SHCMD("discord") },
	{ MODKEY,                       XK_b,                 togglebar,      {0} },
	{ MODKEY,                       XK_i,                 incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,                 incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                 setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                 setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_equal,             incrgaps,       {.i = +3 } },
	{ MODKEY,                       XK_minus,             incrgaps,       {.i = -3 } },
	{ MODKEY|ShiftMask,             XK_BackSpace,         togglegaps,     {0} },
	{ MODKEY,                       XK_BackSpace,         defaultgaps,    {0} },
	{ MODKEY,                       XK_g,                 zoom,           {0} },
	{ MODKEY,                       XK_Tab,               view,           {0} },
	{ MODKEY,                       XK_x,                 killclient,     {0} },
	{ MODKEY,                       XK_t,                 setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_t,                 setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_r,                 setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_r,                 setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_m,                 setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_m,                 setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_c,                 setlayout,      {.v = &layouts[6]} },
	{ MODKEY|ShiftMask,             XK_c,                 setlayout,      {.v = &layouts[7]} },
	{ MODKEY|ShiftMask,             XK_s,                 setlayout,      {.v = &layouts[8]} },
	{ MODKEY|ShiftMask,             XK_space,             setlayout,      {0} },
	{ MODKEY,                       XK_s,                 togglefloating, {0} },
	{ MODKEY,                       XK_f,                 togglefullscr,  {0} },
	{ MODKEY,                       XK_0,                 view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                 tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,             focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,            focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,             tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,            tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_semicolon,         shiftview,      {.i = -1 } },
	{ MODKEY,                       XK_apostrophe,        shiftview,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_semicolon,         shifttag,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_apostrophe,        shifttag,       {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        spawn,          {.v = statuscmd } },
	{ ClkStatusText,        0,              Button4,        spawn,          {.v = statuscmd } },
	{ ClkStatusText,        0,              Button5,        spawn,          {.v = statuscmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,		        0,		          Button4,	      shiftview,	    {.i = 1} },
	{ ClkTagBar,		        0,		          Button5,	      shiftview,	    {.i = -1} },
};

