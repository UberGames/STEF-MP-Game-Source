// Copyright (C) 1999-2000 Id Software, Inc.
//

#include "g_local.h"
#include "g_groups.h"

extern qboolean BG_BorgTransporting( playerState_t *ps );
extern void BG_LoadItemNames(void);

level_locals_t	level;
extern char	races[256];	//this is evil!
extern qboolean levelExiting;
extern int noJoinLimit;

group_list_t	group_list[MAX_GROUP_MEMBERS];
int 			group_count;
int numKilled;

typedef struct {
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;
	int			modificationCount;  // for tracking changes
	qboolean	trackChange;	// track this variable, and announce if changed
} cvarTable_t;

gentity_t		g_entities[MAX_GENTITIES];
gclient_t		g_clients[MAX_CLIENTS];

vmCvar_t	g_pModAssimilation;
vmCvar_t	g_pModDisintegration;
vmCvar_t	g_pModActionHero;
vmCvar_t	g_pModSpecialties;
vmCvar_t	g_pModElimination;

vmCvar_t	g_gametype;
vmCvar_t	g_dmflags;
vmCvar_t	g_fraglimit;
vmCvar_t	g_timelimit;
vmCvar_t	g_timelimitWinningTeam;
vmCvar_t	g_capturelimit;
vmCvar_t	g_friendlyFire;
vmCvar_t	g_password;
vmCvar_t	g_needpass;
vmCvar_t	g_maxclients;
vmCvar_t	g_maxGameClients;
vmCvar_t	g_dedicated;
vmCvar_t	g_speed;
vmCvar_t	g_gravity;
vmCvar_t	g_cheats;
vmCvar_t	g_knockback;
vmCvar_t	g_dmgmult;
vmCvar_t	g_forcerespawn;
vmCvar_t	g_inactivity;
vmCvar_t	g_debugMove;
vmCvar_t	g_debugDamage;
vmCvar_t	g_debugAlloc;
vmCvar_t	g_weaponRespawn;
vmCvar_t	g_adaptRespawn;
vmCvar_t	g_motd;
vmCvar_t	g_synchronousClients;
vmCvar_t	g_warmup;
vmCvar_t	g_doWarmup;
vmCvar_t	g_restarted;
vmCvar_t	g_log;
vmCvar_t	g_logSync;
vmCvar_t	g_podiumDist;
vmCvar_t	g_podiumDrop;
vmCvar_t	g_allowVote;
vmCvar_t	g_teamAutoJoin;
vmCvar_t	g_teamForceBalance;
vmCvar_t	g_banIPs;
vmCvar_t	g_filterBan;
vmCvar_t	g_debugForward;
vmCvar_t	g_debugRight;
vmCvar_t	g_debugUp;
vmCvar_t	g_language;
vmCvar_t	g_holoIntro;
vmCvar_t	g_ghostRespawn;
vmCvar_t    g_intermissionTime;
vmCvar_t	g_team_group_red;
vmCvar_t	g_team_group_blue;
vmCvar_t	g_random_skin_limit;
vmCvar_t	g_noJoinTimeout;
vmCvar_t	g_classChangeDebounceTime;
vmCvar_t	ui_playerclass;

cvarTable_t		gameCvarTable[] = {
	// don't override the cheat state set by the system
	{ &g_cheats, "sv_cheats", "", 0, 0, qfalse },

	// noset vars
	{ NULL, "gamename", GAMEVERSION , CVAR_SERVERINFO | CVAR_ROM, 0, qfalse  },
	{ NULL, "gamedate", __DATE__ , CVAR_ROM, 0, qfalse  },
	{ &g_restarted, "g_restarted", "0", CVAR_ROM, 0, qfalse  },
	{ NULL, "sv_mapname", "", CVAR_SERVERINFO | CVAR_ROM, 0, qfalse  },

	// latched vars
	{ &g_gametype, "g_gametype", "0", CVAR_SERVERINFO | CVAR_LATCH, 0, qfalse  },
	{ &g_pModAssimilation, "g_pModAssimilation", "0", CVAR_SERVERINFO | CVAR_LATCH, 0, qfalse  },
	{ &g_pModDisintegration, "g_pModDisintegration", "0", CVAR_SERVERINFO | CVAR_LATCH, 0, qfalse  },
	{ &g_pModActionHero, "g_pModActionHero", "0", CVAR_SERVERINFO | CVAR_LATCH, 0, qfalse  },
	{ &g_pModSpecialties, "g_pModSpecialties", "0", CVAR_SERVERINFO | CVAR_LATCH, 0, qfalse  },
	{ &g_pModElimination, "g_pModElimination", "0", CVAR_SERVERINFO | CVAR_LATCH, 0, qfalse  },

	{ &g_maxclients, "sv_maxclients", "8", CVAR_SERVERINFO | CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  },
	{ &g_maxGameClients, "g_maxGameClients", "0", CVAR_SERVERINFO | CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  },

	// change anytime vars
	{ &g_dmflags, "dmflags", "0", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue  },
	{ &g_fraglimit, "fraglimit", "20", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_timelimit, "timelimit", "0", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_timelimitWinningTeam, "timelimitWinningTeam", "", CVAR_NORESTART, 0, qtrue },
	{ &g_capturelimit, "capturelimit", "8", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },

	{ &g_synchronousClients, "g_synchronousClients", "0", CVAR_SYSTEMINFO, 0, qfalse  },

	{ &g_friendlyFire, "g_friendlyFire", "0", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue },

	{ &g_teamAutoJoin, "g_teamAutoJoin", "0", CVAR_ARCHIVE, 0, qfalse },
	{ &g_teamForceBalance, "g_teamForceBalance", "1", CVAR_ARCHIVE, 0, qfalse },

	{ &g_intermissionTime, "g_intermissionTime", "20", CVAR_ARCHIVE, 0, qtrue },
	{ &g_warmup, "g_warmup", "20", CVAR_ARCHIVE, 0, qtrue  },
	{ &g_doWarmup, "g_doWarmup", "0", CVAR_ARCHIVE, 0, qtrue  },
	{ &g_log, "g_log", "games.log", CVAR_ARCHIVE, 0, qfalse  },
	{ &g_logSync, "g_logSync", "0", CVAR_ARCHIVE, 0, qfalse  },

	{ &g_password, "g_password", "", CVAR_USERINFO, 0, qfalse  },

	{ &g_banIPs, "g_banIPs", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &g_filterBan, "g_filterBan", "1", CVAR_ARCHIVE, 0, qfalse  },

	{ &g_needpass, "g_needpass", "0", CVAR_SERVERINFO | CVAR_ROM, 0, qfalse },

	{ &g_dedicated, "dedicated", "0", 0, 0, qfalse  },

	{ &g_speed, "g_speed", "250", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue  },				// Quake 3 default was 320.
	{ &g_gravity, "g_gravity", "800", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue  },
	{ &g_knockback, "g_knockback", "500", 0, 0, qtrue  },
	{ &g_dmgmult, "g_dmgmult", "1", 0, 0, qtrue  },
	{ &g_weaponRespawn, "g_weaponrespawn", "5", 0, 0, qtrue  },		// Quake 3 default (with 1 ammo weapons) was 5.
	{ &g_adaptRespawn, "g_adaptrespawn", "1", 0, 0, qtrue  },		// Make weapons respawn faster with a lot of players.
	{ &g_forcerespawn, "g_forcerespawn", "0", 0, 0, qtrue },		// Quake 3 default was 20.  This is more "user friendly".
	{ &g_inactivity, "g_inactivity", "0", 0, 0, qtrue },
	{ &g_debugMove, "g_debugMove", "0", 0, 0, qfalse },
	{ &g_debugDamage, "g_debugDamage", "0", 0, 0, qfalse },
	{ &g_debugAlloc, "g_debugAlloc", "0", 0, 0, qfalse },
	{ &g_motd, "g_motd", "", 0, 0, qfalse },

	{ &g_podiumDist, "g_podiumDist", "80", 0, 0, qfalse },
	{ &g_podiumDrop, "g_podiumDrop", "70", 0, 0, qfalse },

	{ &g_allowVote, "g_allowVote", "1", CVAR_SERVERINFO, 0, qfalse },

#if 0
	{ &g_debugForward, "g_debugForward", "0", 0, 0, qfalse },
	{ &g_debugRight, "g_debugRight", "0", 0, 0, qfalse },
	{ &g_debugUp, "g_debugUp", "0", 0, 0, qfalse },
#endif

	{ &g_language, "g_language", "", CVAR_ARCHIVE, 0, qfalse  },

	{ &g_holoIntro, "g_holoIntro", "1", CVAR_ARCHIVE, 0, qfalse},
	{ &g_ghostRespawn, "g_ghostRespawn", "5", CVAR_ARCHIVE, 0, qfalse},		// How long the player is ghosted, in seconds.
	{ &g_team_group_red, "g_team_group_red", "", CVAR_LATCH, 0, qfalse  },		// Used to have CVAR_ARCHIVE	
	{ &g_team_group_blue, "g_team_group_blue", "", CVAR_LATCH, 0, qfalse  },		// Used to have CVAR_ARCHIVE
	{ &g_random_skin_limit, "g_random_skin_limit", "4", CVAR_ARCHIVE, 0, qfalse },
	{ &g_noJoinTimeout, "g_noJoinTimeout", "120", CVAR_ARCHIVE, 0, qfalse  },
	{ &g_classChangeDebounceTime, "g_classChangeDebounceTime", "180", CVAR_ARCHIVE, 0, qfalse },
		
	{ &ui_playerclass, "ui_playerclass", "", CVAR_ARCHIVE, 0, qfalse },

};

int		gameCvarTableSize = sizeof( gameCvarTable ) / sizeof( gameCvarTable[0] );

void G_InitGame( int levelTime, int randomSeed, int restart );
void G_RunFrame( int levelTime );
void G_ShutdownGame( int restart );
void CheckExitRules( void );

//=============================
//** begin code

/*
================
vmMain

This is the only way control passes into the module.
This MUST be the very first function compiled into the .q3vm file
================
*/
int vmMain( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6 ) {
	switch ( command ) {
	case GAME_INIT:
		G_InitGame( arg0, arg1, arg2 );
		return 0;
	case GAME_SHUTDOWN:
		G_ShutdownGame( arg0 );
		return 0;
	case GAME_CLIENT_CONNECT:
		return (int)ClientConnect( arg0, arg1, arg2 );
	case GAME_CLIENT_THINK:
		ClientThink( arg0 );
		return 0;
	case GAME_CLIENT_USERINFO_CHANGED:
		ClientUserinfoChanged( arg0 );
		return 0;
	case GAME_CLIENT_DISCONNECT:
		ClientDisconnect( arg0 );
		return 0;
	case GAME_CLIENT_BEGIN:
		ClientBegin( arg0, qtrue );
		return 0;
	case GAME_CLIENT_COMMAND:
		ClientCommand( arg0 );
		return 0;
	case GAME_RUN_FRAME:
		G_RunFrame( arg0 );
		return 0;
	case GAME_CONSOLE_COMMAND:
		return ConsoleCommand();
	case BOTAI_START_FRAME:
		return BotAIStartFrame( arg0 );
	}

	return -1;
}


void QDECL G_Printf( const char *fmt, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, fmt);
	vsprintf (text, fmt, argptr);
	va_end (argptr);

	trap_Printf( text );
}

void QDECL G_Error( const char *fmt, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, fmt);
	vsprintf (text, fmt, argptr);
	va_end (argptr);

	trap_Error( text );
}

void G_initGroupsList(void)
{
	char	filename[MAX_QPATH];
	char	dirlist[2048];
	int		i;
	char*	dirptr;
	char*	race_list;
	int		numdirs;
	int		dirlen;

	memset(group_list, 0, sizeof(group_list));
	group_count = 0;

	// search through each and every skin we can find
	numdirs = trap_FS_GetFileList("models/players2", "/", dirlist, 2048 );
	dirptr  = dirlist;
	for (i=0; i<numdirs ; i++,dirptr+=dirlen+1)
	{
		dirlen = strlen(dirptr);
		
		if (dirlen && dirptr[dirlen-1]=='/')
		{
			dirptr[dirlen-1]='\0';
		}

		if (!strcmp(dirptr,".") || !strcmp(dirptr,".."))
		{
			continue;
		}

		if (group_count == MAX_GROUP_MEMBERS)
		{
			G_Printf("Number of possible models larger than group array - limiting to first %d models\n", MAX_GROUP_MEMBERS);
			break;
		}
		// work out racename to 
		Com_sprintf(filename, sizeof(filename), "models/players2/%s/groups.cfg", dirptr);
		race_list = BG_RegisterRace(filename);

		Q_strncpyz( group_list[group_count].name, dirptr , sizeof(group_list[0].name) );
		Q_strncpyz( group_list[group_count++].text, race_list , sizeof(group_list[0].text) );
	}
}



#define MAX_GROUP_FILE_SIZE	5000
char *G_searchGroupList(const char *name)
{
	char	*text_p = NULL, *slash = NULL;
	char	text[MAX_GROUP_FILE_SIZE];
	int		i;
	char	mod_name[200];

	memset (races, 0, sizeof(races));
	memset (text, 0, sizeof(text));

	// check to see if there is a '/' in the name
	Q_strncpyz(mod_name, name, sizeof(mod_name));
	slash = strstr( mod_name, "/" );
	if ( slash != NULL )
	{//drop the slash and everything after it for the purpose of finding the model name in th group
		*slash = 0;
	}

	// find the name in the group list
	for (i=0; i<group_count; i++)
	{
		if (!Q_stricmp(mod_name, group_list[i].name))
		{
			text_p = group_list[i].text;
			break;
		}
	}

	// did we find this group in the list?
	if (i == group_count)
	{
		Com_sprintf(races, sizeof(races), "unknown");
	}
	else
	{
		Com_sprintf(races, sizeof(races), text_p);
	}
	return races;

}


/*
================
G_FindTeams

Chain together all entities with a matching team field.
Entity teams are used for item groups and multi-entity mover groups.

All but the first will have the FL_TEAMSLAVE flag set and teammaster field set
All but the last will have the teamchain field set to the next one
================
*/
void G_FindTeams( void ) {
	gentity_t	*e, *e2;
	int		i, j;
	int		c, c2;

	c = 0;
	c2 = 0;
	for ( i=1, e=g_entities+i ; i < level.num_entities ; i++,e++ ){
		if (!e->inuse)
			continue;
		if (!e->team)
			continue;
		if (e->flags & FL_TEAMSLAVE)
			continue;
		if ( e->classname && Q_stricmp( "func_door", e->classname ) != 0 )
		{//not a door
			if ( Q_stricmp( "1", e->team ) == 0 || Q_stricmp( "2", e->team ) == 0 )
			{//is trying to tell us it belongs to the TEAM_RED or TEAM_BLUE
				continue;
			}
		}
		e->teammaster = e;
		c++;
		c2++;
		for (j=i+1, e2=e+1 ; j < level.num_entities ; j++,e2++)
		{
			if (!e2->inuse)
				continue;
			if (!e2->team)
				continue;
			if (e2->flags & FL_TEAMSLAVE)
				continue;
			if (!strcmp(e->team, e2->team))
			{
				c2++;
				e2->teamchain = e->teamchain;
				e->teamchain = e2;
				e2->teammaster = e;
				e2->flags |= FL_TEAMSLAVE;

				// make sure that targets only point at the master
				if ( e2->targetname ) {
					e->targetname = e2->targetname;
					e2->targetname = NULL;
				}
			}
		}
	}

	G_Printf ("%i teams with %i entities\n", c, c2);
}

/*
=================
G_RegisterCvars
=================
*/
void G_RegisterCvars( void ) {
	int			i;
	cvarTable_t	*cv;

	for ( i = 0, cv = gameCvarTable ; i < gameCvarTableSize ; i++, cv++ ) {
		trap_Cvar_Register( cv->vmCvar, cv->cvarName,
			cv->defaultString, cv->cvarFlags );
		if ( cv->vmCvar )
			cv->modificationCount = cv->vmCvar->modificationCount;
	}

	// check some things

	if ( g_gametype.integer < 0 || g_gametype.integer >= GT_MAX_GAME_TYPE ) {
		G_Printf( "g_gametype %i is out of range, defaulting to 0\n", g_gametype.integer );
		trap_Cvar_Set( "g_gametype", "0" );
	}

	level.warmupModificationCount = g_warmup.modificationCount;
}

/*
=================
G_UpdateCvars
=================
*/
void G_UpdateCvars( void ) {
	int			i;
	cvarTable_t	*cv;

	for ( i = 0, cv = gameCvarTable ; i < gameCvarTableSize ; i++, cv++ ) {
		if ( cv->vmCvar ) {
			trap_Cvar_Update( cv->vmCvar );

			if ( cv->modificationCount != cv->vmCvar->modificationCount ) {
				cv->modificationCount = cv->vmCvar->modificationCount;

				if ( cv->trackChange ) {
					if ( !levelExiting )
					{//no need to do this during level changes
						trap_SendServerCommand( -1, va("print \"Server: %s changed to %s\n\"", 
							cv->cvarName, cv->vmCvar->string ) );
					}
				}
			}
		}
	}
}

extern int altAmmoUsage[];
extern team_t	borgTeam;
extern team_t	initialBorgTeam;
void G_InitModRules( void )
{
	numKilled = 0;

	if ( g_pModDisintegration.integer != 0 )
	{//don't use up ammo in disintegration mode
		altAmmoUsage[WP_COMPRESSION_RIFLE] = 0;
	}
	if ( g_pModSpecialties.integer != 0 )
	{//tripwires use more ammo
		altAmmoUsage[WP_GRENADE_LAUNCHER] = 3;
	}
	if ( g_pModAssimilation.integer != 0 )
	{
		borgTeam = initialBorgTeam;
	}
}

/*
============
G_InitGame

============
*/
extern int lastKillTime[];
void G_InitGame( int levelTime, int randomSeed, int restart ) {
	int					i;

	G_Printf ("------- Game Initialization -------\n");
	G_Printf ("gamename: %s\n", GAMEVERSION);
	G_Printf ("gamedate: %s\n", __DATE__);

	srand( randomSeed );

	noJoinLimit = 0;//allow players to join the game still
	level.numObjectives = 0;//no objectives by default
	for ( i = 0; i < MAX_CLIENTS; i++ )
	{
		lastKillTime[i] = level.time-30000;
	}

	G_RegisterCvars();

	G_ProcessIPBans();

	G_InitMemory();

	G_initGroupsList();

	BG_LoadItemNames();

	// set some level globals
	memset( &level, 0, sizeof( level ) );
	level.time = levelTime;
	level.startTime = levelTime;
	level.restarted = restart;

	level.snd_fry = G_SoundIndex("sound/player/fry.wav");	// FIXME standing in lava / slime

	if ( g_gametype.integer != GT_SINGLE_PLAYER && g_log.string[0] ) {
		if ( g_logSync.integer ) {
			trap_FS_FOpenFile( g_log.string, &level.logFile, FS_APPEND_SYNC );
		} else {
			trap_FS_FOpenFile( g_log.string, &level.logFile, FS_APPEND );
		}
		if ( !level.logFile ) {
			G_Printf( "WARNING: Couldn't open logfile: %s\n", g_log.string );
		} else {
			char	serverinfo[MAX_INFO_STRING];

			trap_GetServerinfo( serverinfo, sizeof( serverinfo ) );

			G_LogPrintf("------------------------------------------------------------\n" );
			G_LogPrintf("InitGame: %s\n", serverinfo );
		}
	} else {
		G_Printf( "Not logging to disk.\n" );
	}


	G_LogWeaponInit();

	G_InitWorldSession();

	// initialize all entities for this game
	memset( g_entities, 0, MAX_GENTITIES * sizeof(g_entities[0]) );
	level.gentities = g_entities;

	// initialize all clients for this game
	level.maxclients = g_maxclients.integer;
	memset( g_clients, 0, MAX_CLIENTS * sizeof(g_clients[0]) );
	level.clients = g_clients;

	// set client fields on player ents
	for ( i=0 ; i<level.maxclients ; i++ ) {
		g_entities[i].client = level.clients + i;
	}

	// always leave room for the max number of clients,
	// even if they aren't all used, so numbers inside that
	// range are NEVER anything but clients
	level.num_entities = MAX_CLIENTS;

	// let the server system know where the entites are
	trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ), 
		&level.clients[0].ps, sizeof( level.clients[0] ) );

	// reserve some spots for dead player bodies
	InitBodyQue();

	ClearRegisteredItems();

	// parse the key/value pairs and spawn gentities
	G_SpawnEntitiesFromString();

	// general initialization
	G_FindTeams();

	// make sure we have flags for CTF, etc
	G_CheckTeamItems();

	SaveRegisteredItems();

	G_Printf ("-----------------------------------\n");

	if( g_gametype.integer == GT_SINGLE_PLAYER || trap_Cvar_VariableIntegerValue( "com_buildScript" ) ) {
		G_ModelIndex( SP_PODIUM_MODEL );
		G_SoundIndex( "sound/player/gurp1.wav" );
		G_SoundIndex( "sound/player/gurp2.wav" );
	}
	if (g_gametype.integer >= GT_TEAM || trap_Cvar_VariableIntegerValue( "com_buildScript" ) )
	{
		G_ModelIndex( TEAM_PODIUM_MODEL );
	}

	if ( trap_Cvar_VariableIntegerValue( "bot_enable" ) ) {
		BotAISetup( restart );
		BotAILoadMap( restart );
		G_InitBots( restart );
	}

	G_InitModRules();
	
	levelExiting = qfalse;
}

extern void G_RestoreClientInitialStatus( gentity_t *ent );
extern int	borgQueenClientNum;
void G_CheckResetAssimilationClients( void )
{
	if ( g_pModAssimilation.integer != 0 )
	{
		gentity_t *ent;
		int			i;
		//clear current queen
		borgQueenClientNum = -1;
		//put the assimilated players back on their original team and class
		for ( i = 0; i < level.maxclients; i++ )
		{
			ent = &g_entities[i];
			if ( ent->client && ent->inuse )
			{
				G_RestoreClientInitialStatus( ent );
			}
		}
		//clear borg team
		borgTeam = initialBorgTeam;
	}
}
void EliminationRespawn( gentity_t *ent, char *team );
void G_CheckResetEliminationClients( void )
{
	if ( g_pModElimination.integer != 0 )
	{//no players respawn when in elimination
		gentity_t	*client;
		int			i;

		for ( i = 0; i < level.numConnectedClients; i++ )
		{
			client = &g_entities[&level.clients[level.sortedClients[i]] - level.clients];
			if ( client->team && client->client->sess.sessionTeam == TEAM_SPECTATOR )
			{
				EliminationRespawn( client, client->team );
			}
		}
	}
}

/*
=================
G_ShutdownGame
=================
*/
void G_ShutdownGame( int restart ) {
	G_CheckResetAssimilationClients();
	G_CheckResetEliminationClients();

	G_Printf ("==== ShutdownGame ====\n");

#if 0	// kef -- Pat sez this is causing some trouble these days
	G_LogWeaponOutput();
#endif
	if ( level.logFile ) {
		G_LogPrintf("ShutdownGame:\n" );
		G_LogPrintf("------------------------------------------------------------\n" );
		trap_FS_FCloseFile( level.logFile );
	}

	// write all the client session data so we can get it back
	G_WriteSessionData();

	if ( trap_Cvar_VariableIntegerValue( "bot_enable" ) ) {
		BotAIShutdown( restart );
	}
}



//===================================================================

#ifndef GAME_HARD_LINKED
// this is only here so the functions in q_shared.c and bg_*.c can link

void QDECL Com_Error ( int level, const char *error, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, error);
	vsprintf (text, error, argptr);
	va_end (argptr);

	G_Error( "%s", text);
}

void QDECL Com_Printf( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	vsprintf (text, msg, argptr);
	va_end (argptr);

	G_Printf ("%s", text);
}

#endif

/*
========================================================================

PLAYER COUNTING / SCORE SORTING

========================================================================
*/

/*
=============
AddTournamentPlayer

If there are less than two tournament players, put a
spectator in the game and restart
=============
*/
void AddTournamentPlayer( void ) {
	int			i;
	gclient_t	*client;
	gclient_t	*nextInLine;

	if ( level.numPlayingClients >= 2 ) {
		return;
	}

	// never change during intermission
	if ( level.intermissiontime ) {
		return;
	}

	nextInLine = NULL;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		client = &level.clients[i];
		if ( client->pers.connected != CON_CONNECTED ) {
			continue;
		}
		if ( client->sess.sessionTeam != TEAM_SPECTATOR ) {
			continue;
		}
		// never select the dedicated follow or scoreboard clients
		if ( client->sess.spectatorState == SPECTATOR_SCOREBOARD || 
			client->sess.spectatorClient < 0  ) {
			continue;
		}

		if ( !nextInLine || client->sess.spectatorTime < nextInLine->sess.spectatorTime ) {
			nextInLine = client;
		}
	}

	if ( !nextInLine ) {
		return;
	}

	level.warmupTime = -1;

	// set them to free-for-all team
	SetTeam( &g_entities[ nextInLine - level.clients ], "f" );
}


/*
=======================
RemoveTournamentLoser

Make the loser a spectator at the back of the line
=======================
*/
void RemoveTournamentLoser( void ) {
	int			clientNum;

	if ( level.numPlayingClients != 2 ) {
		return;
	}

	clientNum = level.sortedClients[1];

	if ( level.clients[ clientNum ].pers.connected != CON_CONNECTED ) {
		return;
	}

	// make them a spectator
	SetTeam( &g_entities[ clientNum ], "s" );
}


/*
=======================
AdjustTournamentScores

=======================
*/
void AdjustTournamentScores( void ) {
	int			clientNum;

	clientNum = level.sortedClients[0];
	if ( level.clients[ clientNum ].pers.connected == CON_CONNECTED ) {
		level.clients[ clientNum ].sess.wins++;
		ClientUserinfoChanged( clientNum );
	}

	clientNum = level.sortedClients[1];
	if ( level.clients[ clientNum ].pers.connected == CON_CONNECTED ) {
		level.clients[ clientNum ].sess.losses++;
		ClientUserinfoChanged( clientNum );
	}

}



/*
=============
SortRanks

=============
*/
int QDECL SortRanks( const void *a, const void *b ) {
	gclient_t	*ca, *cb;

	ca = &level.clients[*(int *)a];
	cb = &level.clients[*(int *)b];

	// sort special clients last
	if ( ca->sess.spectatorState == SPECTATOR_SCOREBOARD || ca->sess.spectatorClient < 0 ) {
		return 1;
	}
	if ( cb->sess.spectatorState == SPECTATOR_SCOREBOARD || cb->sess.spectatorClient < 0  ) {
		return -1;
	}

	// then connecting clients
	if ( ca->pers.connected == CON_CONNECTING ) {
		return 1;
	}
	if ( cb->pers.connected == CON_CONNECTING ) {
		return -1;
	}


	// then spectators
	if ( ca->sess.sessionTeam == TEAM_SPECTATOR && cb->sess.sessionTeam == TEAM_SPECTATOR ) {
		if ( ca->sess.spectatorTime < cb->sess.spectatorTime ) {
			return -1;
		}
		if ( ca->sess.spectatorTime > cb->sess.spectatorTime ) {
			return 1;
		}
		return 0;
	}
	if ( ca->sess.sessionTeam == TEAM_SPECTATOR ) {
		return 1;
	}
	if ( cb->sess.sessionTeam == TEAM_SPECTATOR ) {
		return -1;
	}

	// then sort by score & number of times killed
	if ( ca->ps.persistant[PERS_SCORE]
		> cb->ps.persistant[PERS_SCORE] ) {
		return -1;
	}
	if ((ca->ps.persistant[PERS_SCORE] == cb->ps.persistant[PERS_SCORE]) &&
		(ca->ps.persistant[PERS_KILLED] < cb->ps.persistant[PERS_KILLED])   )
	{	return -1;}

	if ( ca->ps.persistant[PERS_SCORE]
		< cb->ps.persistant[PERS_SCORE] ) {
		return 1;
	}
	if ((ca->ps.persistant[PERS_SCORE] == cb->ps.persistant[PERS_SCORE]) &&
		(ca->ps.persistant[PERS_KILLED] > cb->ps.persistant[PERS_KILLED])   )
	{	return 1;}

	return 0;
}

/*
============
CalculateRanks

Recalculates the score ranks of all players
This will be called on every client connect, begin, disconnect, death,
and team change.

FIXME: for elimination, the last man standing must be ranked first
============
*/
void CalculateRanks( qboolean fromExit ) {
	int		i;
	int		rank;
	int		score;
	int		newScore;
	gclient_t	*cl;

	level.follow1 = -1;
	level.follow2 = -1;
	level.numConnectedClients = 0;
	level.numNonSpectatorClients = 0;
	level.numPlayingClients = 0;
	level.numVotingClients = 0;		// don't count bots
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].pers.connected != CON_DISCONNECTED ) {
			level.sortedClients[level.numConnectedClients] = i;
			level.numConnectedClients++;

			if ( level.clients[i].sess.sessionTeam != TEAM_SPECTATOR ) {
				level.numNonSpectatorClients++;
			
				// decide if this should be auto-followed
				if ( level.clients[i].pers.connected == CON_CONNECTED ) {
					level.numPlayingClients++;
					if ( !(g_entities[i].r.svFlags & SVF_BOT) ) {
						level.numVotingClients++;
					}
					if ( level.follow1 == -1 ) {
						level.follow1 = i;
					} else if ( level.follow2 == -1 ) {
						level.follow2 = i;
					}
				}
			}
		}
	}

	qsort( level.sortedClients, level.numConnectedClients, 
		sizeof(level.sortedClients[0]), SortRanks );

	// set the rank value for all clients that are connected and not spectators
	if ( g_gametype.integer >= GT_TEAM ) {
		// in team games, rank is just the order of the teams, 0=red, 1=blue, 2=tied
		for ( i = 0;  i < level.numConnectedClients; i++ ) {
			cl = &level.clients[ level.sortedClients[i] ];
			if ( level.teamScores[TEAM_RED] == level.teamScores[TEAM_BLUE] ) {
				cl->ps.persistant[PERS_RANK] = 2;
			} else if ( level.teamScores[TEAM_RED] > level.teamScores[TEAM_BLUE] ) {
				cl->ps.persistant[PERS_RANK] = 0;
			} else {
				cl->ps.persistant[PERS_RANK] = 1;
			}
		}
	} else {	
		rank = -1;
		score = 0;
		for ( i = 0;  i < level.numPlayingClients; i++ ) {
			cl = &level.clients[ level.sortedClients[i] ];
			newScore = cl->ps.persistant[PERS_SCORE];
			if ( i == 0 || newScore != score ) {
				rank = i;
				// assume we aren't tied until the next client is checked
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank;
			} else {
				// we are tied with the previous client
				level.clients[ level.sortedClients[i-1] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
			}
			score = newScore;
			if ( g_gametype.integer == GT_SINGLE_PLAYER && level.numPlayingClients == 1 ) {
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
			}
		}
	}

	// set the CS_SCORES1/2 configstrings, which will be visible to everyone
	if ( g_gametype.integer >= GT_TEAM ) {
		trap_SetConfigstring( CS_SCORES1, va("%i", level.teamScores[TEAM_RED] ) );
		trap_SetConfigstring( CS_SCORES2, va("%i", level.teamScores[TEAM_BLUE] ) );
	} else {
		if ( level.numConnectedClients == 0 ) {
			trap_SetConfigstring( CS_SCORES1, va("%i", SCORE_NOT_PRESENT) );
			trap_SetConfigstring( CS_SCORES2, va("%i", SCORE_NOT_PRESENT) );
		} else if ( level.numConnectedClients == 1 ) {
			trap_SetConfigstring( CS_SCORES1, va("%i", level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE] ) );
			trap_SetConfigstring( CS_SCORES2, va("%i", SCORE_NOT_PRESENT) );
		} else {
			trap_SetConfigstring( CS_SCORES1, va("%i", level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE] ) );
			trap_SetConfigstring( CS_SCORES2, va("%i", level.clients[ level.sortedClients[1] ].ps.persistant[PERS_SCORE] ) );
		}
	}

	// see if it is time to end the level
	if ( !fromExit )
	{//not coming this from the CheckExitRules func
		CheckExitRules();
	}

	// if we are at the intermission, send the new info to everyone
	if ( level.intermissiontime ) {
		SendScoreboardMessageToAllClients();
	}
}


/*
========================================================================

MAP CHANGING

========================================================================
*/

/*
========================
SendScoreboardMessageToAllClients

Do this at BeginIntermission time and whenever ranks are recalculated
due to enters/exits/forced team changes
========================
*/
void SendScoreboardMessageToAllClients( void ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[ i ].pers.connected == CON_CONNECTED ) {
			DeathmatchScoreboardMessage( g_entities + i );
		}
	}
}

/*
========================
MoveClientToIntermission

When the intermission starts, this will be called for all players.
If a new client connects, this will be called after the spawn function.
========================
*/
void MoveClientToIntermission( gentity_t *ent ) {
	// take out of follow mode if needed
	if ( ent->client->sess.spectatorState == SPECTATOR_FOLLOW ) {
		StopFollowing( ent );
	}


	// move to the spot
	VectorCopy( level.intermission_origin, ent->s.origin );
	VectorCopy( level.intermission_origin, ent->client->ps.origin );
	VectorCopy (level.intermission_angle, ent->client->ps.viewangles);
	ent->client->ps.pm_type = PM_INTERMISSION;
	ent->client->ps.eFlags ^= EF_TELEPORT_BIT;

	// clean up powerup info
	memset( ent->client->ps.powerups, 0, sizeof(ent->client->ps.powerups) );

	ent->client->ps.eFlags = 0;
	ent->s.eFlags = 0;
	ent->s.eType = ET_GENERAL;
	ent->s.modelindex = 0;
	ent->s.loopSound = 0;
	ent->s.event = 0;
	ent->r.contents = 0;
}

/*
==================
FindIntermissionPoint

This is also used for spectator spawns
==================
*/
void FindIntermissionPoint( void ) {
	gentity_t	*ent, *target;
	vec3_t		dir;

	// find the intermission spot
	ent = G_Find (NULL, FOFS(classname), "info_player_intermission");
	if ( !ent ) {	// the map creator forgot to put in an intermission point...
		SelectSpawnPoint ( vec3_origin, level.intermission_origin, level.intermission_angle );
	} else {
		VectorCopy (ent->s.origin, level.intermission_origin);
		VectorCopy (ent->s.angles, level.intermission_angle);
		// if it has a target, look towards it
		if ( ent->target ) {
			target = G_PickTarget( ent->target );
			if ( target ) {
				VectorSubtract( target->s.origin, level.intermission_origin, dir );
				vectoangles( dir, level.intermission_angle );
			}
		}
	}

}

/*
==================
ClearFiringFlags
==================
*/
void ClearFiringFlags()
{
	int i = 0;
	gentity_t	*ent = NULL;

	for (i=0 ; i< level.maxclients ; i++) 
	{
		ent = g_entities + i;
		if (!ent->inuse)
			continue;
		// clear the firing flag
		if (ent->client)
		{
			ent->client->ps.eFlags &= ~EF_FIRING;
		}
	}
}

/*
==================
BeginIntermission
==================
*/
void BeginIntermission( void ) {
	int			i;
	gentity_t	*client;
	qboolean	doingLevelshot;

	if (level.intermissiontime == -1)
		doingLevelshot = qtrue;
	else
		doingLevelshot = qfalse;

	if ( level.intermissiontime && level.intermissiontime != -1 ) {
		return;		// already active
	}

	G_CheckResetEliminationClients();

	// if in tournament mode, change the wins / losses
	if ( g_gametype.integer == GT_TOURNAMENT ) {
		AdjustTournamentScores();
	}

	level.intermissiontime = level.time;
	FindIntermissionPoint();

	// kef -- make sure none of the players are still firing (cuz we don't want weapons fx going off while
	//they're on the podium)
	ClearFiringFlags();


	// cdr - Want to generate victory pads for all game types  - except level shots (gametype 10)
	UpdateTournamentInfo();
	if (!doingLevelshot)
		SpawnModelsOnVictoryPads();


	// move all clients to the intermission point
	for (i=0 ; i< level.maxclients ; i++) {
		client = g_entities + i;
		if (!client->inuse)
			continue;
		// respawn if dead
		if ( BG_BorgTransporting( &client->client->ps ) )
		{//in borg teleport fly around mode, turn it off
			client->client->ps.stats[STAT_USEABLE_PLACED] = 0;
			client->client->ps.stats[STAT_HOLDABLE_ITEM] = 0;
			client->client->ps.eFlags &= ~EF_NODRAW;
			client->s.eFlags &= ~EF_NODRAW;
			client->flags &= ~FL_NOTARGET;
		}
		if (client->health <= 0) {
			respawn(client);
		}
		MoveClientToIntermission( client );

	}

	// send the current scoring to all clients
	SendScoreboardMessageToAllClients();
}


void G_ClearObjectives( void )
{
	gentity_t *tent;

	tent = G_TempEntity( vec3_origin, EV_OBJECTIVE_COMPLETE );
	//Be sure to send the event to everyone
	tent->r.svFlags |= SVF_BROADCAST;
	tent->s.eventParm = 0;//tells it to clear all
}
/*
=============
ExitLevel

When the intermission has been exited, the server is either killed
or moved to a new level based on the "nextmap" cvar 

=============
*/
qboolean levelExiting = qfalse;
void ExitLevel (void) {
	int		i;

	levelExiting = qtrue;

	//bot interbreeding
	BotInterbreedEndMatch();

	G_ClearObjectives();
	// if we are running a tournement map, kick the loser to spectator status,
	// which will automatically grab the next spectator and restart
	if ( g_gametype.integer == GT_TOURNAMENT ) {
		if ( !level.restarted ) {
			RemoveTournamentLoser();
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			level.changemap = NULL;
			level.intermissiontime = 0;
		}
		return;	
	}


	trap_SendConsoleCommand( EXEC_APPEND, "vstr nextmap\n" );
	level.changemap = NULL;
	level.intermissiontime = 0;

	//don't reset the bots until after level.intermission is off so that it doesn't send 5 billion score updates
	G_CheckResetAssimilationClients();

	// we need to do this here before chaning to CON_CONNECTING
	G_WriteSessionData();

	// change all client states to connecting, so the early players into the
	// next level will know the others aren't done reconnecting
	for (i=0 ; i< g_maxclients.integer ; i++) {
		if ( level.clients[i].pers.connected == CON_CONNECTED ) {
			level.clients[i].pers.connected = CON_CONNECTING;
		}
	}

}


/*
=================
CheckIntermissionExit

The level will stay at the intermission for a minimum of 5 seconds
If all players wish to continue, the level will then exit.
If one or more players have not acknowledged the continue, the game will
wait 20 seconds before going on.
=================
*/
void CheckIntermissionExit( void ) {
	int			ready, notReady;
	int			i;
	gclient_t	*cl;
	int			readyMask;


	if ( levelExiting )
	{//already on our way out, skip the check
		return;
	}

	if ( level.time < level.intermissiontime + 5000 )
	{
			// bring up the scoreboard after 5 seconds
	}


	// Single player exit does not happen until menu event
	if ( g_gametype.integer == GT_SINGLE_PLAYER ) {
		return;
	}

	// see which players are ready
	ready = 0;
	notReady = 0;
	readyMask = 0;
	for (i=0 ; i< g_maxclients.integer ; i++) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
		if ( g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT ) {
			continue;
		}

		if ( cl->readyToExit ) {
			ready++;
			if ( i < 16 ) {
				readyMask |= 1 << i;
			}
		} else {
			notReady++;
		}
	}

	// copy the readyMask to each player's stats so
	// it can be displayed on the scoreboard
	for (i=0 ; i< g_maxclients.integer ; i++) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
		cl->ps.stats[STAT_CLIENTS_READY] = readyMask;
	}

	// never exit in less than five seconds
	if ( level.time < level.intermissiontime + 5000 ) {
		return;
	}

	// if nobody wants to go, clear timer
	if ( !ready ) {
		level.readyToExit = qfalse;
		return;
	}

	// if everyone wants to go, go now
	if ( !notReady ) {
		ExitLevel();
		return;
	}

	// the first person to ready starts the ten second timeout
	if ( !level.readyToExit ) {
		level.readyToExit = qtrue;
		level.exitTime = level.time;
	}

	// if we have waited g_intermissionTime seconds since at least one player
	// wanted to exit, go ahead
	if ( level.time < level.exitTime + (1000 * g_intermissionTime.integer) ) {
		return;
	}

	ExitLevel();
}

/*
=============
ScoreIsTied
=============
*/
qboolean ScoreIsTied( void ) {
	int		a, b;

	if ( level.numPlayingClients < 2 ) {
		return qfalse;
	}
	
	if ( g_gametype.integer >= GT_TEAM ) {
		return level.teamScores[TEAM_RED] == level.teamScores[TEAM_BLUE];
	}

	a = level.clients[level.sortedClients[0]].ps.persistant[PERS_SCORE];
	b = level.clients[level.sortedClients[1]].ps.persistant[PERS_SCORE];

	return a == b;
}

/*
=================
CheckExitRules

There will be a delay between the time the exit is qualified for
and the time everyone is moved to the intermission spot, so you
can see the last frag.
=================
*/
extern int	borgQueenClientNum;
void CheckExitRules( void ) {
	int			i;
	gclient_t	*cl;

	// if at the intermission, wait for all non-bots to
	// signal ready, then go to next level
	if ( level.intermissiontime ) {
		CheckIntermissionExit ();
		return;
	}

	if ( level.intermissionQueued ) {
		if ( level.time - level.intermissionQueued >= INTERMISSION_DELAY_TIME ) {
			level.intermissionQueued = 0;
			BeginIntermission();
		}
		return;
	}

	if ( g_doWarmup.integer )
	{
		if ( level.warmupTime != 0 )
		{
			if ( level.warmupTime < 0 || level.time - level.startTime <= level.warmupTime )
			{//don't win until warmup is done
				return;
			}
		}
	}

	if ( level.numPlayingClients < 2 ) {
		//not enough players
		return;
	}

	if ( g_pModAssimilation.integer != 0 )
	{//check assimilation rules
		if ( level.numConnectedClients > 0 && numKilled > 0 )
		{
			gclient_t *survivor = NULL;

			if ( borgQueenClientNum != -1 )
			{//see if borg queen is dead first
				if ( g_entities[borgQueenClientNum].health <= 0 )
				{//the queen is dead!
					//FIXME: What if the queen disconnects somehow...?  Shouldn't be possible
					G_LogExit( "The Borg Queen has been killed!" );
					return;
				}
			}

			//See if only one player remains alive, if so, end it.
			for ( i = 0; i < level.maxclients; i++ )
			{
				cl = &level.clients[i];
				if ( cl->pers.connected == CON_CONNECTED && cl->sess.sessionTeam != TEAM_SPECTATOR && cl->sess.sessionClass != PC_BORG )
				{
					survivor = cl;
					break;
				}
			}
			if ( survivor == NULL )
			{
				G_LogExit( "Assimilation Complete." );
				return;
			}
		}
		//don't check anything else
		return;
	}

	if ( g_pModElimination.integer != 0 )
	{//check elimination rules
		gclient_t *survivor = NULL;
		//See if only one player remains alive, if so, end it.
		if ( level.numConnectedClients > 1 && numKilled > 0 )
		{
			for ( i = 0; i < level.numConnectedClients; i++ )
			{
				cl = &level.clients[ level.sortedClients[i] ];
				if ( cl->sess.sessionTeam != TEAM_SPECTATOR && !(cl->ps.eFlags&EF_ELIMINATED) )
				{
					if ( survivor != NULL )
					{
						if ( g_gametype.integer < GT_TEAM || cl->sess.sessionTeam != survivor->sess.sessionTeam )
						{//not in a team game or survivor is on same team as previously found survivor, keep looking
							survivor = NULL;
							break;
						}
					}
					else
					{
						survivor = cl;
					}
				}
			}
		}
		if ( survivor != NULL )
		{
			G_LogExit( "Last Man Standing." );
		}
		//don't check anything else
		return;
	}

	if ( g_timelimit.integer && !level.warmupTime ) 
	{//check timelimit
		if ( level.time - level.startTime >= g_timelimit.integer*60000 ) {
			// check for sudden death 
			if ( g_gametype.integer != GT_CTF && ScoreIsTied() ) {
				// score is tied, so don't end the game
				return;
			}
			//if timelimit causes a team to win, we need to fudge the score
			if ( g_timelimitWinningTeam.string[0] == 'r' || g_timelimitWinningTeam.string[0] == 'R' )
			{
				if ( g_capturelimit.integer > level.teamScores[TEAM_BLUE])
				{
					level.teamScores[TEAM_RED] = g_capturelimit.integer;
				}
				else
				{
					level.teamScores[TEAM_RED] = level.teamScores[TEAM_BLUE] + 1;
				}
				CalculateRanks( qtrue );
			}
			else if ( g_timelimitWinningTeam.string[0] == 'b' || g_timelimitWinningTeam.string[0] == 'B' )
			{
				if ( g_capturelimit.integer > level.teamScores[TEAM_RED])
				{
					level.teamScores[TEAM_BLUE] = g_capturelimit.integer;
				}
				else
				{
					level.teamScores[TEAM_BLUE] = level.teamScores[TEAM_RED] + 1;
				}
				CalculateRanks( qtrue );
			}
			trap_SendServerCommand( -1, "print \"Timelimit hit.\n\"");
			G_LogExit( "Timelimit hit." );
			return;
		}
	}

	if ( g_gametype.integer != GT_CTF && g_fraglimit.integer ) 
	{//check fraglimit
		if ( level.teamScores[TEAM_RED] >= g_fraglimit.integer ) {
			trap_SendServerCommand( -1, "print \"Red hit the point limit.\n\"" );
			G_LogExit( "Fraglimit hit." );
			return;
		}

		if ( level.teamScores[TEAM_BLUE] >= g_fraglimit.integer ) {
			trap_SendServerCommand( -1, "print \"Blue hit the point limit.\n\"" );
			G_LogExit( "Fraglimit hit." );
			return;
		}

		for ( i=0 ; i< g_maxclients.integer ; i++ ) {
			cl = level.clients + i;
			if ( cl->pers.connected != CON_CONNECTED ) {
				continue;
			}
			if ( cl->sess.sessionTeam != TEAM_FREE ) {
				continue;
			}

			if ( cl->ps.persistant[PERS_SCORE] >= g_fraglimit.integer ) {
				G_LogExit( "Fraglimit hit." );
				trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " hit the point limit.\n\"",
					cl->pers.netname ) );
				return;
			}
		}
	}

	if ( g_gametype.integer == GT_CTF && g_capturelimit.integer ) 
	{//check CTF
		if ( level.teamScores[TEAM_RED] >= g_capturelimit.integer ) {
			trap_SendServerCommand( -1, "print \"Red hit the capturelimit.\n\"" );
			G_LogExit( "Capturelimit hit." );
			return;
		}

		if ( level.teamScores[TEAM_BLUE] >= g_capturelimit.integer ) {
			trap_SendServerCommand( -1, "print \"Blue hit the capturelimit.\n\"" );
			G_LogExit( "Capturelimit hit." );
			return;
		}
	}
}



/*
========================================================================

FUNCTIONS CALLED EVERY FRAME

========================================================================
*/


/*
=============
CheckTournement

Once a frame, check for changes in tournement player state
=============
*/
void CheckTournement( void ) {
	if ( level.numPlayingClients == 0 ) {
		return;
	}

	if ( g_gametype.integer == GT_TOURNAMENT ) {

		// pull in a spectator if needed
		if ( level.numPlayingClients < 2 ) {
			AddTournamentPlayer();
		}

		// if we don't have two players, go back to "waiting for players"
		if ( level.numPlayingClients != 2 ) {
			if ( level.warmupTime != -1 ) {
				level.warmupTime = -1;
				trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
				G_LogPrintf( "Warmup:\n" );
			}
			return;
		}

		if ( level.warmupTime == 0 ) {
			return;
		}

		// if the warmup is changed at the console, restart it
		if ( g_warmup.modificationCount != level.warmupModificationCount ) {
			level.warmupModificationCount = g_warmup.modificationCount;
			level.warmupTime = -1;
		}

		// if all players have arrived, start the countdown
		if ( level.warmupTime < 0 ) {
			if ( level.numPlayingClients == 2 ) {
				// fudge by -1 to account for extra delays
				level.warmupTime = level.time + ( g_warmup.integer - 1 ) * 1000;
				trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
			}
			return;
		}

		// if the warmup time has counted down, restart
		if ( level.time > level.warmupTime ) {
			level.warmupTime += 10000;
			trap_Cvar_Set( "g_restarted", "1" );
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			return;
		}
	} else if ( g_gametype.integer != GT_SINGLE_PLAYER && g_doWarmup.integer ) {
		int		counts[TEAM_NUM_TEAMS];
		qboolean	notEnough = qfalse;

		if ( g_gametype.integer > GT_TEAM ) {
			counts[TEAM_BLUE] = TeamCount( -1, TEAM_BLUE );
			counts[TEAM_RED] = TeamCount( -1, TEAM_RED );

			if (counts[TEAM_RED] < 1 || counts[TEAM_BLUE] < 1) {
				notEnough = qtrue;
			}
		} else if ( level.numPlayingClients < 2 ) {
			notEnough = qtrue;
		}

		if ( notEnough ) {
			if ( level.warmupTime != -1 ) {
				level.warmupTime = -1;
				trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
				G_LogPrintf( "Warmup:\n" );
			}
			return; // still waiting for team members
		}

		if ( level.warmupTime == 0 ) {
			return;
		}

		// if the warmup is changed at the console, restart it
		if ( g_warmup.modificationCount != level.warmupModificationCount ) {
			level.warmupModificationCount = g_warmup.modificationCount;
			level.warmupTime = -1;
		}

		// if all players have arrived, start the countdown
		if ( level.warmupTime < 0 ) {
			// fudge by -1 to account for extra delays
			level.warmupTime = level.time + ( g_warmup.integer - 1 ) * 1000;
			trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
			return;
		}

		// if the warmup time has counted down, restart
		if ( level.time > level.warmupTime ) {
			level.warmupTime += 10000;
			trap_Cvar_Set( "g_restarted", "1" );
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			return;
		}
	}
}


/*
==================
CheckVote
==================
*/
void CheckVote( void ) {
	if ( !level.voteTime ) {
		return;
	}
	if ( level.time - level.voteTime >= VOTE_TIME ) {
		trap_SendServerCommand( -1, "print \"Vote failed.\n\"" );
	} else {
		if ( level.voteYes > level.numVotingClients/2 ) {
			// execute the command, then remove the vote
			trap_SendServerCommand( -1, "print \"Vote passed.\n\"" );
			trap_SendConsoleCommand( EXEC_APPEND, va("%s\n", level.voteString ) );
		} else if ( level.voteNo >= level.numVotingClients/2 ) {
			// same behavior as a timeout
			trap_SendServerCommand( -1, "print \"Vote failed.\n\"" );
		} else {
			// still waiting for a majority
			return;
		}
	}
	level.voteTime = 0;
	trap_SetConfigstring( CS_VOTE_TIME, "" );

}


/*
==================
CheckCvars
==================
*/
void CheckCvars( void ) {
	static int lastMod = -1;

	if ( g_password.modificationCount != lastMod ) {
		lastMod = g_password.modificationCount;
		if ( *g_password.string && Q_stricmp( g_password.string, "none" ) ) {
			trap_Cvar_Set( "g_needpass", "1" );
		} else {
			trap_Cvar_Set( "g_needpass", "0" );
		}
	}
}

/*
=============
G_RunThink

Runs thinking code for this frame if necessary
=============
*/
void G_RunThink (gentity_t *ent) {
	float	thinktime;

	thinktime = ent->nextthink;
	if (thinktime <= 0) {
		return;
	}
	if (thinktime > level.time) {
		return;
	}
	
	ent->nextthink = 0;
	if (!ent->think) {
		G_Error ( "NULL ent->think");
	}
	ent->think (ent);
}

/*
================
G_RunFrame

Advances the non-player objects in the world
================
*/
void CheckHealthInfoMessage( void );
extern void G_PickBorgQueen( void );
extern void INeedAHero( void );
extern qboolean levelExiting;
extern int actionHeroClientNum;
void G_RunFrame( int levelTime ) {
	int			i;
	gentity_t	*ent;
	int			msec;
int start, end;

	// if we are waiting for the level to restart, do nothing
	if ( level.restarted ) {
		return;
	}

	level.framenum++;
	level.previousTime = level.time;
	level.time = levelTime;
	msec = level.time - level.previousTime;

	// get any cvar changes
	G_UpdateCvars();

	//
	// go through all allocated objects
	//
start = trap_Milliseconds();
	ent = &g_entities[0];
	for (i=0 ; i<level.num_entities ; i++, ent++) {
		if ( !ent->inuse ) {
			continue;
		}

		// clear events that are too old
		if ( level.time - ent->eventTime > EVENT_VALID_MSEC ) {
			if ( ent->s.event ) {
				ent->s.event = 0;	// &= EV_EVENT_BITS;
				if ( ent->client ) {
					ent->client->ps.externalEvent = 0;
					ent->client->ps.events[0] = 0;
					ent->client->ps.events[1] = 0;
					ent->client->ps.events[2] = 0;
					ent->client->ps.events[3] = 0;
				}
			}
			if ( ent->freeAfterEvent ) {
				// tempEntities or dropped items completely go away after their event
				G_FreeEntity( ent );
				continue;
			} else if ( ent->unlinkAfterEvent ) {
				// items that will respawn will hide themselves after their pickup event
				ent->unlinkAfterEvent = qfalse;
				trap_UnlinkEntity( ent );
			}
		}

		// temporary entities don't think
		if ( ent->freeAfterEvent ) {
			continue;
		}

		if ( !ent->r.linked && ent->neverFree ) {
			continue;
		}

		if ( !ent->client )
		{
			if ( ent->s.eFlags & EF_ANIM_ONCE )
			{//this must be capped render-side
				ent->s.frame++;
			}
		}

		if ( (ent->s.eType == ET_MISSILE) || (ent->s.eType == ET_ALT_MISSILE) ) {
			G_RunMissile( ent );
			continue;
		}

		if ( ent->s.eType == ET_ITEM || ent->physicsObject ) {
			G_RunItem( ent );
			continue;
		}

		if ( ent->s.eType == ET_MOVER ) {
			G_RunMover( ent );
			continue;
		}

		if ( i < MAX_CLIENTS ) {
			G_RunClient( ent );
			continue;
		}

		G_RunThink( ent );
	}
end = trap_Milliseconds();

start = trap_Milliseconds();
	// perform final fixups on the players
	ent = &g_entities[0];
	for (i=0 ; i < level.maxclients ; i++, ent++ ) {
		if ( ent->inuse ) {
			ClientEndFrame( ent );
		}
	}
end = trap_Milliseconds();

	// see if it is time to do a tournement restart
	CheckTournement();

	// see if it is time to end the level
	CheckExitRules();

	// update to team status?
	CheckTeamStatus();

	// update to health status
	//CheckHealthInfoMessage();//done from inside CheckTeamStatus now

	// cancel vote if timed out
	CheckVote();

	// for tracking changes
	CheckCvars();

	if ( !levelExiting )
	{
		//keep looking for a borgQueen if we don't have one yet
		if ( borgQueenClientNum == -1 )
		{
			G_PickBorgQueen();
		}
		//keep looking for an actionHero if we don't have one yet
		if ( actionHeroClientNum == -1 )
		{
			INeedAHero();
		}
	}
}

