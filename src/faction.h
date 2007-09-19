/*
 * See Licensing and Copyright notice in naev.h
 */



#ifndef FACTION_H
#  define FACTION_H


typedef struct Faction_ {

	char* name;

	struct Faction_** enemies;
	int nenemies;
	struct Faction_** allies;
	int nallies;

} Faction;


Faction* faction_get( const char* name );

int areEnemies( Faction* a, Faction* b );
int areAllies( Faction* a, Faction* b );

int factions_load (void);
void factions_free (void);


#endif /* FACTION_H */
