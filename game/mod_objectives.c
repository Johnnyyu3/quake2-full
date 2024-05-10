#include "f_mod.h"

int objective_type = -1;


void ED_CallSpawn(edict_t* ent);
char quest[200] = "Press 5 to init loadout\n Press 9 to start an Objective.";
char quest_secondary[200] = "LCtrl for Stratagem/ConfigLoadout\nArrowKeys to nagivate both.";
char* le_objectif[3] = {"Kill Enemies", "Complete Stratagem Code", "Complete Stratagem Code and Survive"};

void UpdateQuest()
{
	char amt[10];
	if (objective_type == 0)
	{
		itoa(level.killed_monsters, amt, 10);
		strcpy(quest, "Kill 100 Monsters. ");
		strcat(quest, amt);
		strcat(quest, "killed.");
	}
	else if (objective_type == 1)
	{
		strcpy(quest, "Complete the given \nStratagem code: ");
		strcpy(quest_secondary, strStratagem());
	}
	else if (objective_type == 2)
	{
		itoa(time_left, amt, 10);
		strcpy(quest, " ");
		if (obj_strat_done == false)
		{
			strcpy(quest, "Complete the given \nStratagem code and ");
			strcpy(quest_secondary, strStratagem());
		}
		strcat(quest, "survive for ");
		strcat(quest, amt);
		strcat(quest, " seconds.");
	}
}

void KillEnemies(edict_t* ent)
{

	level.killed_monsters = 0;
	objective_type = 0;
	UpdateQuest();
}
void DoStratagem(edict_t* ent)
{
	GenerateStratagem();
	obj_strat_done = false;
	obj_left = 3;
	UpdateQuest();
}
void DoStratagemAndSurvive(edict_t* ent)
{
	GenerateStratagem();
	time_left = 120;
	obj_strat_done = false;
	obj_left = 1;
	UpdateQuest();
}

int gen_randint(qboolean abs, int range, int offset)
{
	int ret;
	float val;
	val = crandom();
	if (val < 0 && abs)
		val *= -1;
	val *= range;
	val += offset;
	ret = (int)val;
	return ret;
}

char* strStratagem()
{
	if (obj_stratagem == NULL)
		return "";
	char* ret;
	ret = malloc(sizeof(char) * 160);
	if (ret == NULL)
		return "";
	strcpy(ret, " ");
	for (int i = 0; i < obj_len; i++)
	{
		switch (obj_stratagem[i])
		{
		case UP:
			strcat(ret, "UP ");
			break;
		case LEFT:
			strcat(ret, "LEFT ");
			break;
		case RIGHT:
			strcat(ret, "RIGHT ");
			break;
		case DOWN:
			strcat(ret, "DOWN ");
			break;
		}
		if (i == 4)
			strcat(ret, "\n");
	}
	return ret;
}

void GenerateStratagem()
{
	obj_len = gen_randint(true, 5,5);
	obj_stratagem = malloc(sizeof(enum Arrows)*obj_len);
	if (obj_stratagem == NULL)
		return;

	for (int i = 0; i < obj_len; i++)
	{
		int a = gen_randint(true, 4, 0);
		switch (a)
		{
		case 0:
			obj_stratagem[i] = UP;
			break;
		case 1:
			obj_stratagem[i] = LEFT;
			break;
		case 2:
			obj_stratagem[i] = RIGHT;
			break;
		case 3:
			obj_stratagem[i] = DOWN;
			break;
		}
	}
}

char* units[] = {
		"monster_berserk","monster_gladiator","monster_gunner","monster_infantry", "monster_soldier_light", "monster_soldier","monster_soldier_ss", "monster_tank",
		"monster_tank_commander", "monster_medic", "monster_flipper", "monster_chick", "monster_parasite", "monster_flyer", "monster_brain", "monster_floater",
		"monster_hover", "monster_mutant", "monster_supertank", "monster_jorg"
};


void SpawnPatrolCluster(edict_t* self)
{
	last_patrol = level.time;
	int v;
	for (int i = 0; i < 4; i++)
	{
		v = gen_randint(true, 20, 0);
		SpawnPatrolUnit(self, units[v]);
	}

}

void SpawnPatrolUnit(edict_t* ent, char* classname)
{
	edict_t* sp_ent;
	vec3_t vt;
	vec3_t	offset;
	vec3_t	forward, right;
	vec3_t	start;

	VectorSet(offset, 8, 8, ent->viewheight - 8);
	AngleVectors(ent->client->v_angle, forward, right, NULL);

	for (int i = 0; i < 3; i++)
		vt[i] = ent->s.origin[i] + right[i] + offset[i];
	vt[0] += 100* gen_randint(false, 3, 1); vt[1] += 100 * gen_randint(false, 3, 1);
	sp_ent = G_Spawn();

	VectorCopy(vt, sp_ent->s.origin);
	VectorCopy(ent->s.angles, sp_ent->s.angles);

	sp_ent->classname = classname;
	ED_CallSpawn(sp_ent);
	gi.linkentity(sp_ent);
}

void MissionComplete(edict_t* ent)
{
	gi.cprintf(ent, PRINT_HIGH, "Mission complete.\n");
	objective_type = -1;
	objective = false;
	char quest[200] = "Press 5 to init loadout\n Press 9 to start an Objective.";
	char quest_secondary[200] = "LCtrl for Stratagem/ConfigLoadout\nArrowKeys to nagivate both.";
}