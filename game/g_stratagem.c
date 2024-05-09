
#include "f_mod.h"
struct node* createNode(char* value, qboolean cmd) {
	struct node* newNode = malloc(sizeof(struct node));
	newNode->string = value;
	newNode->cmd = cmd;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->down = NULL;
	newNode->up = NULL;
	return newNode;
}

void insert_stratagem(node* root, enum Arrows* dir, int len, char* item_name)
{
	qboolean cmd;
	char* str;
	node* current = root;
	node* temp;
	enum Arrows a;

	for (int i = 0; i < len; i++) 
	{
		str = malloc(sizeof(char) * 80);
		strcpy(str, current->string);
		cmd = false;
		if (i == len - 1) 
		{
			cmd = true;
		}
		a = dir[i];
		switch (a)
		{
		case UP:
			strcat(str, "UP ");
			if(current->up == NULL)
				current->up = createNode(str, cmd);
			current = current->up;
			break;
		case LEFT:
			strcat(str, "LEFT ");
			if(current->left == NULL)
				current->left = createNode(str, cmd);
			current = current->left;
			break;
		case RIGHT:
			strcat(str, "RIGHT ");
			if (current->right == NULL)
				current->right = createNode(str, cmd);
			current = current->right;
			break;
		case DOWN:
			strcat(str, "DOWN ");
			if (current->down == NULL)
				current->down = createNode(str, cmd);
			current = current->down;
			break;
		}
		if (i == len - 1)
		{
			current->string = item_name;
		}
	}
	return ;
}

void Allocate_Stratagem_Tree(edict_t* ent) {
	root = createNode("Stratagem: ", false);

	if (enable_stratagems[0])
	{
		enum Arrows GL_strat[] = { DOWN,LEFT,UP,LEFT,DOWN };
		insert_stratagem(root, GL_strat, 5, "Grenade Launcher");
	}

	if (enable_stratagems[1])
	{
		enum Arrows AC_strat[] = { DOWN,LEFT,DOWN,UP,UP,RIGHT };
		insert_stratagem(root, AC_strat, 6, "AutoCannon");
	}

	if (enable_stratagems[2])
	{
		enum Arrows RG_strat[] = { DOWN,RIGHT,DOWN,UP,LEFT,RIGHT };
		insert_stratagem(root, RG_strat, 6, "Railgunv2");
	}

	if (enable_stratagems[3])
	{
		enum Arrows QC_strat[] = { DOWN,DOWN,UP,LEFT,RIGHT };
		insert_stratagem(root, QC_strat, 5, "Quasar Cannon");
	}

	if (enable_stratagems[4])
	{
		enum Arrows OPS_strat[] = { RIGHT,RIGHT,UP };
		insert_stratagem(root, OPS_strat, 3, "Orbital Precision Strike");
	}



}

node* traverse_strat_tree(int* arrows, int len) {
	node* current = root;
	enum Arrows dir;
	for (int i = 0; i < len; i++)
	{
		dir = arrows[i] - 1;
		switch (dir)
		{
		case UP:
			if (current->up == NULL)
				return NULL;
			current = current->up;
			break;
		case LEFT:
			if (current->left == NULL)
				return NULL;
			current = current->left;
			break;
		case RIGHT:
			if (current->right == NULL)
				return NULL;
			current = current->right;
			break;
		case DOWN:
			if (current->down == NULL)
				return NULL;
			current = current->down;
			break;
		}
	}
	return current;
}


void Give_Reload(edict_t* ent)
{
	int* rds = &ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))];
	int ammo = 0;
	gitem_t* wep;
	wep = ent->client->pers.weapon;
	if (Q_stricmp(wep->classname, "weapon_senator") == 0)
	{
		if (*rds >= 6)
			return;
		ammo = 6;
		change_ammo_count(ent, ammo);
		ammo = *rds;
	}
	else if (Q_stricmp(wep->classname, "weapon_slugger") == 0)
	{
		if (*rds >= 16)
			return;
		ammo = 16;
		change_ammo_count(ent, ammo);
		ammo = *rds;
	}
	else if (Q_stricmp(wep->classname, "weapon_las-99cannon") == 0)
	{
		ammo = -1;
		ent->client->pers.inventory[ITEM_INDEX(FindItem("Energy"))] = 100;
	}
	else if (Q_stricmp(wep->classname, "weapon_LAS16_Sickle") == 0)
	{
		ammo = -1;
		ent->client->pers.inventory[ITEM_INDEX(FindItem("Sickle Mag"))] = 6;
		ent->client->pers.inventory[ITEM_INDEX(FindItem("EnergySickle"))] = 88;
	}
	rds = ammo;
	Reload(ent);
}

void give_Weap(edict_t* ent, char* name) {
	edict_t* it_ent;
	gitem_t* it;
	int			index;
	it = FindItem(name);
	index = ITEM_INDEX(it);
	if (it->flags & IT_AMMO)
	{
		Add_Ammo(ent, it, it->quantity);
	}
	else
	{
		if (ent->client->pers.inventory[index] == 1)
			return;
		it_ent = G_Spawn();
		it_ent->classname = it->classname;
		SpawnItem(it_ent, it);
		Touch_Item(it_ent, ent, NULL, NULL);
		if (it_ent->inuse)
			G_FreeEdict(it_ent);
	}
	if (Q_stricmp(name, "Orbital Precision Strike") == 0)
	{
		it->use(ent, it);
		return;
	}

	Give_Reload(ent);

}


char* loadout[7] = {"Primary","Secondary", "Stratagem","Stratagem","Stratagem", "Stratagem", "Stratagem"};

char* primaries[4] = {"Sickle","Breaker","Breaker Incendiary","Slugger"};
char* secondaries[2] = {"Redeemer","Senator"};
char* stratagems[5] = {"Grenade Launcher","AutoCannon","Railgunv2","Quasar Cannon", "Orbital Precision Strike"};
qboolean enable_stratagems[5] = {false, false, false, false, false}; //GL_strat, AC_strat, RG_strat, QC_strat, OPS_strat
qboolean primary_wep[4] = { false, false, false, false };
qboolean secondary_wep[2] = { false, false };
qboolean stratagem_wep[4] = { false, false, false, false};

void Init_Loadout(edict_t* ent, char* s)
{
	int i;
	if (Q_stricmp(s, "Primary")==0)
	{
		for (i = 0; i < 4; i++)
		{
			if (primary_wep[i])
			{
				give_Weap(ent, primaries[i]);
				return;
			}
		}
		give_Weap(ent, primaries[0]);
		primary_wep[0] = true;
	}
	else if (Q_stricmp(s, "Secondary")==0)
	{
		for ( i = 0; i < 2; i++)
		{
			if (secondary_wep[i])
			{
				give_Weap(ent, secondaries[i]);
				return;
			}
		}
		give_Weap(ent, secondaries[0]);
		secondary_wep[0] = true;
	}
	else if (Q_stricmp(s, "Stratagem")==0)
	{
			return;
	}
}

void Use_Loadout(edict_t* ent, char* s)
{
	int			index;
	gitem_t*	it;
	int			i;
	char* name;

	if (Q_stricmp(s, "Primary")==0)
	{
		for (i = 0; i < 4; i++)
		{
			if (primary_wep[i])
			{
				s = primaries[i];
				break;
			}
		}
	}
	else if (Q_stricmp(s, "Secondary")==0)
	{
		for (i = 0; i < 2; i++)
		{
			if (secondary_wep[i])
			{
				s = secondaries[i];
				break;
			}
		}
	}
	else if (Q_stricmp(s, "Stratagem")==0)
	{
		for (i = 0; i < 4; i++)
		{
			if (stratagem_wep[i])
			{
				s= stratagems[i];
				break;
			}
		}
	}
	it = FindItem(s);
	if (!it)
	{
		gi.cprintf(ent, PRINT_HIGH, "unknown item: %s\n", s);
		return;
	}
	if (!it->use)
	{
		gi.cprintf(ent, PRINT_HIGH, "Item is not usable.\n");
		return;
	}
	index = ITEM_INDEX(it);
	if (!ent->client->pers.inventory[index])
	{
		gi.cprintf(ent, PRINT_HIGH, "Out of item: %s\n", s);
		return;
	}

	it->use(ent, it);
}

void Edit_Loadout(edict_t* ent, char* s, int target)
{
	int i;
	if (Q_stricmp(s, "Primary")==0)
	{
		for (i = 0; i < 4; i++)
			primary_wep[i] = false;
		primary_wep[target] = true;
		Init_Loadout(ent, s);
		return;
	}
	else if (Q_stricmp(s, "Secondary")==0)
	{
		for (i = 0; i < 2; i++)
			secondary_wep[i] = false;
		secondary_wep[target] = true;
		Init_Loadout(ent, s);
	}
	else if (Q_stricmp(s, "Stratagem")==0)
	{
		if (enable_stratagems[target])
		{
			enable_stratagems[target] = false;
			gi.cprintf(ent, PRINT_HIGH, "Stratagem %s Disnabled\n", stratagems[target]);
			if (target < 3)
			{
				stratagem_wep[target] = false;
				Init_Loadout(ent, s);
			}
		}
		else
		{
			int active = 0;
			for (i = 0; i < 5; i++)
			{
				if (enable_stratagems[i]== true)
					active++;
			}
			if (active >= 3) 
			{
				gi.cprintf(ent, PRINT_HIGH, "Disable Other Stratagems to enable Selected Stratagem. (MAX: 3)\n");
				return;
			}
			gi.cprintf(ent, PRINT_HIGH, "Stratagem %s Enabled\n", stratagems[target]);
			enable_stratagems[target] = true;
		}
		
	}
}