#pragma once
#include "g_local.h"
#include "m_player.h"
typedef enum Arrows {
	UP,
	LEFT,
	RIGHT,
	DOWN
};

typedef struct node {
	char* string;
	qboolean cmd;
	struct node* left;
	struct node* right;
	struct node* down;
	struct node* up;

} node;

node* root;

char* loadout[7];
char* primaries[4];
char* secondaries[2];
char* stratagems[5];
qboolean enable_stratagems[5];
qboolean primary_wep[4];
qboolean secondary_wep[2];
qboolean stratagem_wep[4];
void Reload(edict_t* ent);
void Give_Reload(edict_t* ent);
void change_ammo_count(edict_t* ent, int g_ammo);

void Init_Loadout(edict_t* ent, char* s);
void Use_Loadout(edict_t* ent, char* s);
void Edit_Loadout(edict_t* ent, char* s, int target);

void give_Weap(edict_t* ent, char* name);
void Allocate_Stratagem_Tree(edict_t* ent);
node* traverse_strat_tree(int* arrows, int len);