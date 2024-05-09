#include "f_mod.h"


static qboolean	is_quad;
static byte		is_silenced;

typedef enum WeaponCodes_HD {
	WEP_SICKLE,
	WEP_REDEEMER,
	WEP_BREAKER,
	WEP_RAILGUNV2,
	WEP_AUTOCANNON,
	WEP_QUASARCANNON,
	WEP_GRENADELAUNCHERV2,
	WEP_BREAKERINCENDIARY,
	WEP_SLUGGER,
	WEP_SENATOR
};

time_t p_time = 0;

void P_ProjectSource(gclient_t* client, vec3_t point, vec3_t distance, vec3_t forward, vec3_t right, vec3_t result)
{
	vec3_t	_distance;

	VectorCopy(distance, _distance);
	if (client->pers.hand == LEFT_HANDED)
		_distance[1] *= -1;
	else if (client->pers.hand == CENTER_HANDED)
		_distance[1] = 0;
	G_ProjectSource(point, _distance, forward, right, result);
}
void Blaster_Fire(edict_t* ent, vec3_t g_offset, int damage, qboolean hyper, int effect);
void Weapon_Generic(edict_t* ent, int FRAME_ACTIVATE_LAST, int FRAME_FIRE_LAST, int FRAME_IDLE_LAST, int FRAME_DEACTIVATE_LAST, int* pause_frames, int* fire_frames, void (*fire)(edict_t* ent));
void NoAmmoWeaponChange(edict_t* ent);

void change_ammo_count(edict_t* ent, int g_ammo)
{
	int			ammo_index;
	gitem_t* ammo_item;

	ammo_item = FindItem(ent->client->pers.weapon->ammo);
	ammo_index = ITEM_INDEX(ammo_item);

		ent->client->pers.inventory[ammo_index] -= g_ammo;
		ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))] += g_ammo;
	
}

void Reload(edict_t* ent)
{
	int* rds = &ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))];
	int ammo = 0;
	gitem_t* wep;
	wep = ent->client->pers.weapon;
	if (Q_stricmp(wep->classname, "weapon_LAS16_Sickle") == 0)
	{
		if (ent->client->pers.inventory[ITEM_INDEX(FindItem("Sickle Mag"))] == 0) \
		{
			NoAmmoWeaponChange(ent);
			return;
		}
		ent->client->pers.inventory[ITEM_INDEX(FindItem("EnergySickle"))] = 88;
		ent->client->pers.inventory[ITEM_INDEX(FindItem("Sickle Mag"))]--;
		ent->client->pers.inventory[ITEM_INDEX(FindItem("Overheat"))] = 0;
	}
	else if (Q_stricmp(wep->classname, "weapon_Redeemer") == 0)
	{
		if (*rds >= 31)
			return;
		ammo = 31;
		change_ammo_count(ent, ammo);
	}
	else if (Q_stricmp(wep->classname, "weapon_Breaker") == 0)
	{
		if (*rds >= 13)
			return;
		ammo = 13;
		change_ammo_count(ent, ammo);
	}
	else if (Q_stricmp(wep->classname, "weapon_railgunv2") == 0)
	{
		if (*rds == 1)
			return;
		ammo = 1;
		change_ammo_count(ent, ammo);
	}
	else if (Q_stricmp(wep->classname, "weapon_AutoCannon") == 0)
	{
		if (*rds > 5)
			return;
		ammo = 5;
		change_ammo_count(ent, ammo);
		ammo = *rds;
	}
	else if (Q_stricmp(wep->classname, "weapon_grenadelauncherv2") == 0)
	{
		if (*rds >= 10)
			return;
		ammo = 10;
		change_ammo_count(ent, ammo);
	}
	else if (Q_stricmp(wep->classname, "weapon_las-99cannon") == 0)
	{
		ammo = -1;
	}
	else if (Q_stricmp(wep->classname, "weapon_breakerincen") == 0)
	{
		if (*rds >= 25)
			return;
		ammo = 25;
		change_ammo_count(ent, ammo);
	}
	else if (Q_stricmp(wep->classname, "weapon_slugger") == 0)
	{
		if (*rds >= 16)
			return;
		ammo = 1;
		change_ammo_count(ent, ammo);
		ammo = *rds;
	}
	else if (Q_stricmp(wep->classname, "weapon_senator") == 0)
	{
		if (*rds >= 6)
			return;
		ammo = 1;
		change_ammo_count(ent, ammo);
		ammo = *rds;
	}
	ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))] = ammo;
}



//void load_Ammo(edict_t* ent, enum WeaponCodes_HD weap)
//{
//	int current_ammo;
//	switch (weap)
//	{
//	case WEP_SICKLE:
//		current_ammo = ent->client->pers.inventory[ITEM_INDEX(FindItem("Sickle Mag"))];
//		break;
//	case WEP_REDEEMER:
//		current_ammo = 31;
//		change_ammo_count(ent, current_ammo);
//		break;
//	case WEP_BREAKER:
//		current_ammo = 13;
//		change_ammo_count(ent, current_ammo);
//		break;
//	case WEP_RAILGUNV2:
//		current_ammo = 1;
//		change_ammo_count(ent, current_ammo);
//		break;
//	case WEP_AUTOCANNON:
//		current_ammo = 10;
//		change_ammo_count(ent, current_ammo);
//		break;
//	case WEP_QUASARCANNON:
//		current_ammo = -1;
//		break;
//	case WEP_GRENADELAUNCHERV2:
//		current_ammo = 10;
//		change_ammo_count(ent, current_ammo);
//		break;
//	case WEP_BREAKERINCENDIARY:
//		current_ammo = 25;
//		change_ammo_count(ent, current_ammo);
//		break;
//	case WEP_SLUGGER:
//		current_ammo = 16;
//		change_ammo_count(ent, current_ammo);
//		break;
//	case WEP_SENATOR:
//		current_ammo = 6;
//		change_ammo_count(ent, current_ammo);
//		break;
//	default:
//		current_ammo = -1;
//	}
//	ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))] = current_ammo;
//}


void Weapon_Generic3(edict_t* ent, int FRAME_ACTIVATE_LAST, int FRAME_FIRE_LAST, int FRAME_IDLE_LAST, int FRAME_DEACTIVATE_LAST, int* pause_frames, int* fire_frames, void (*fire)(edict_t* ent), enum WeaponCodes_HD weap)
{
	//if (ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))] == 0)
	//{
	//	gi.cprintf(ent, PRINT_HIGH, "Out of Ammo,Press r to reload.\n");
	//	return;
	//}

	Weapon_Generic(ent,FRAME_ACTIVATE_LAST, FRAME_FIRE_LAST, FRAME_IDLE_LAST, FRAME_DEACTIVATE_LAST, pause_frames, fire_frames, *fire );
}


void Weapon_Sickle_Fire(edict_t* ent)
{
	float	rotation;
	vec3_t	offset;
	int		effect;
	int		damage;
	time_t current_time;

	ent->client->weapon_sound = gi.soundindex("weapons/hyprbl1a.wav");


	if (!(ent->client->buttons & BUTTON_ATTACK))
	{
		ent->client->ps.gunframe++;
		return;
	}
	else
	{
		if (ent->client->pers.inventory[ITEM_INDEX(FindItem("EnergySickle"))] == 1)
		{
			gi.cprintf(ent, PRINT_HIGH, "Overheated, Press r to reload.\n");
			return;
		}

		if (p_time == 0) {
			p_time = time(NULL);
		}
		current_time = time(NULL);

		if (!ent->client->pers.inventory[ent->client->ammo_index])
		{
			if (level.time >= ent->pain_debounce_time)
			{
				gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
				ent->pain_debounce_time = level.time + 1;
			}
			NoAmmoWeaponChange(ent);
		}
		else
		{

			
			rotation = (ent->client->ps.gunframe - 5) * 2 * M_PI / 6;
			offset[0] = -4 * sin(rotation);
			offset[1] = 0;
			offset[2] = 4 * cos(rotation);

			if ((ent->client->ps.gunframe == 6) || (ent->client->ps.gunframe == 9))
				effect = EF_HYPERBLASTER;
			else
				effect = 0;
			if (deathmatch->value)
				damage = 15;
			else
				damage = 20;
			if (p_time != current_time)
			{
				Blaster_Fire(ent, offset, damage, true, effect);
				if (!((int)dmflags->value & DF_INFINITE_AMMO))
					ent->client->pers.inventory[ent->client->ammo_index]--;

				ent->client->anim_priority = ANIM_ATTACK;
				if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
				{
					ent->s.frame = FRAME_crattak1 - 1;
					ent->client->anim_end = FRAME_crattak9;
				}
				else
				{
					ent->s.frame = FRAME_attack1 - 1;
					ent->client->anim_end = FRAME_attack8;
				}
			}
			
		}

		ent->client->ps.gunframe++;
		if (ent->client->ps.gunframe == 12 && ent->client->pers.inventory[ent->client->ammo_index])
			ent->client->ps.gunframe = 6;
	}

	if (ent->client->ps.gunframe == 12 && p_time != current_time)
	{
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/hyprbd1a.wav"), 1, ATTN_NORM, 0);
		ent->client->weapon_sound = 0;
	}

}

void Weapon_Sickle(edict_t* ent)
{
	static int	pause_frames[] = { 0 };
	static int	fire_frames[] = { 6, 7, 8, 9, 10, 11, 0 };

	Weapon_Generic3(ent, 5, 20, 49, 53, pause_frames, fire_frames, Weapon_Sickle_Fire, WEP_SICKLE);
}



void Weapon_Redeemer_Fire(edict_t* ent)
{
	int i,r,u;
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		angles;
	int			damage = 8;
	int			kick = 2;
	vec3_t		offset;


	if (!(ent->client->buttons & BUTTON_ATTACK))
	{
		ent->client->machinegun_shots = 0;
		ent->client->ps.gunframe++;
		return;
	}
	
	if (ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))] == 0)
	{
		gi.cprintf(ent, PRINT_HIGH, "Out of Ammo,Press r to reload.\n");
		return;
	}

	if (ent->client->ps.gunframe == 22)
	{
		ent->client->weapon_sound = 0;
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/chngnd1a.wav"), 1, ATTN_IDLE, 0);
	}
	else
	{
		ent->client->weapon_sound = gi.soundindex("weapons/chngnl1a.wav");
	}

	if (ent->client->pers.inventory[ent->client->ammo_index] < 1)
	{
		ent->client->ps.gunframe = 6;
		if (level.time >= ent->pain_debounce_time)
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
			ent->pain_debounce_time = level.time + 1;
		}
		NoAmmoWeaponChange(ent);
		return;
	}

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	for (i = 1; i < 3; i++)
	{
		ent->client->kick_origin[i] = crandom() * 0.175;
		ent->client->kick_angles[i] = crandom() * 0.35;
	}
	ent->client->kick_origin[0] = crandom() * 0.175;
	ent->client->kick_angles[0] = ent->client->machinegun_shots * -0.75;

	// raise the gun as it is firing
	if (!deathmatch->value)
	{
		ent->client->machinegun_shots++;
		if (ent->client->machinegun_shots > 9)
			ent->client->machinegun_shots = 9;
	}

	for (i = 0; i < 3; i++)
	{
		VectorAdd(ent->client->v_angle, ent->client->kick_angles, angles);
		AngleVectors(angles, forward, right, NULL);
		r = 7 + crandom() * 4;
		u = crandom() * 4;
		VectorSet(offset, 0, r, u+ent->viewheight - 8);
		P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
		fire_bullet(ent, start, forward, damage, kick, DEFAULT_BULLET_HSPREAD / 10, DEFAULT_BULLET_VSPREAD / 10, MOD_MACHINEGUN);
		if (!((int)dmflags->value & DF_INFINITE_AMMO))
			ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))]--;
	}

	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_MACHINEGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);



	if (ent->client->ps.gunframe == 22)
	{
		ent->client->weapon_sound = 0;
		gi.sound(ent, CHAN_AUTO, gi.soundindex("weapons/chngnd1a.wav"), 1, ATTN_IDLE, 0);
	}
	else
	{
		ent->client->weapon_sound = gi.soundindex("weapons/chngnl1a.wav");
	}

}

void Weapon_Redeemer(edict_t* ent)
{
	static int	pause_frames[] = { 38, 43, 51, 61, 0 };
	static int	fire_frames[] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 0 };

	Weapon_Generic3(ent, 4, 31, 61, 64, pause_frames, fire_frames, Weapon_Redeemer_Fire, WEP_REDEEMER);
}

void Weapon_Breaker_Fire(edict_t* ent)
{
	int	i;
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		angles;
	int			damage = 8;
	int			kick = 2;
	vec3_t		offset;



	if (!(ent->client->buttons & BUTTON_ATTACK))
	{
		ent->client->machinegun_shots = 0;
		ent->client->ps.gunframe++;
		return;
	}

	if (ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))] == 0)
	{
		gi.cprintf(ent, PRINT_HIGH, "Out of Ammo,Press r to reload.\n");
		return;
	}

	if (ent->client->ps.gunframe == 5)
		ent->client->ps.gunframe = 4;
	else
		ent->client->ps.gunframe = 5;

	if (ent->client->pers.inventory[ent->client->ammo_index] < 1)
	{
		ent->client->ps.gunframe = 6;
		if (level.time >= ent->pain_debounce_time)
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
			ent->pain_debounce_time = level.time + 1;
		}
		NoAmmoWeaponChange(ent);
		return;
	}

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	for (i = 1; i < 3; i++)
	{
		ent->client->kick_origin[i] = crandom() * 0.35;
		ent->client->kick_angles[i] = crandom() * 0.7;
	}
	ent->client->kick_origin[0] = crandom() * 0.35;
	ent->client->kick_angles[0] = ent->client->machinegun_shots * -1.5;

	// raise the gun as it is firing
	if (!deathmatch->value)
	{
		ent->client->machinegun_shots++;
		if (ent->client->machinegun_shots > 9)
			ent->client->machinegun_shots = 9;
	}

	// get start / end positions
	VectorAdd(ent->client->v_angle, ent->client->kick_angles, angles);
	AngleVectors(angles, forward, right, NULL);
	VectorSet(offset, 0, 8, ent->viewheight - 8);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
	fire_shotgun(ent, start, forward, damage, kick, 500, 500, DEFAULT_SHOTGUN_COUNT, MOD_SHOTGUN);

	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_MACHINEGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))]--;

	ent->client->anim_priority = ANIM_ATTACK;
	if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
	{
		ent->s.frame = FRAME_crattak1 - (int)(random() + 0.25);
		ent->client->anim_end = FRAME_crattak9;
	}
	else
	{
		ent->s.frame = FRAME_attack1 - (int)(random() + 0.25);
		ent->client->anim_end = FRAME_attack8;
	}
}

void Weapon_Breaker(edict_t* ent)
{
	static int	pause_frames[] = { 23, 45, 0 };
	static int	fire_frames[] = { 4, 5, 0 };

	Weapon_Generic3(ent, 3, 5, 45, 49, pause_frames, fire_frames, Weapon_Breaker_Fire, WEP_BREAKER);
}


void Weapon_Railgun2_Fire(edict_t* ent)
{
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		offset;
	int			damage;
	int			kick;

\
	damage = ent->client->pers.inventory[ITEM_INDEX(FindItem("RailPow"))];
	kick = 250;

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	if (!(ent->client->buttons & BUTTON_ATTACK))
	{
		AngleVectors(ent->client->v_angle, forward, right, NULL);

		VectorScale(forward, -3, ent->client->kick_origin);
		ent->client->kick_angles[0] = -3;

		VectorSet(offset, 0, 7, ent->viewheight - 8);
		P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
		fire_rail(ent, start, forward, damage, kick);

		// send muzzle flash
		gi.WriteByte(svc_muzzleflash);
		gi.WriteShort(ent - g_edicts);
		gi.WriteByte(MZ_RAILGUN | is_silenced);
		gi.multicast(ent->s.origin, MULTICAST_PVS);

		ent->client->ps.gunframe++;
		PlayerNoise(ent, start, PNOISE_WEAPON);

		if (!((int)dmflags->value & DF_INFINITE_AMMO))
			ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))]--;
		ent->client->pers.inventory[ITEM_INDEX(FindItem("RailPow"))] = 0;
	}

	if (ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))] == 0)
	{
		gi.cprintf(ent, PRINT_HIGH, "Out of Ammo,Press r to reload.\n");
		return;
	}

	if (p_time == 0) {
		p_time = time(NULL);
	}
	time_t current_time = time(NULL);
	if (p_time != current_time)
	{
		ent->client->pers.inventory[ITEM_INDEX(FindItem("RailPow"))] += 100;
		p_time = current_time;
	}
	
}

void Weapon_RailgunHD2(edict_t* ent)
{
	static int	pause_frames[] = { 56, 0 };
	static int	fire_frames[] = { 4, 0 };

	Weapon_Generic3(ent, 3, 18, 56, 61, pause_frames, fire_frames, Weapon_Railgun2_Fire, WEP_RAILGUNV2);
}


void Weapon_AutoCannon_Fire(edict_t* ent)
{
	vec3_t	offset, start;
	vec3_t	forward, right;
	int		damage;
	float	damage_radius;
	int		radius_damage;


	damage = 130 + (int)(random() * 20.0);
	radius_damage = 70;
	damage_radius = 120;
	if (is_quad)
	{
		damage *= 4;
		radius_damage *= 4;
	}

	if (!(ent->client->buttons & BUTTON_ATTACK))
	{
		ent->client->ps.gunframe++;
		return;
	}

	if (ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))] == 0)
	{
		gi.cprintf(ent, PRINT_HIGH, "Out of Ammo,Press r to reload.\n");
		return;
	}

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	VectorSet(offset, 8, 8, ent->viewheight - 8);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
	fire_rocket(ent, start, forward, damage, 3000, damage_radius, radius_damage);

	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_ROCKET | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))]--;
}

void Weapon_AutoCannon(edict_t* ent)
{
	static int	pause_frames[] = { 25, 33, 42, 50, 0 };
	static int	fire_frames[] = { 5, 0 };

	Weapon_Generic3(ent, 4, 12, 50, 54, pause_frames, fire_frames, Weapon_AutoCannon_Fire, WEP_AUTOCANNON);
}


void weapon_qc_fire(edict_t* ent)
{
	vec3_t	offset, start;
	vec3_t	forward, right;
	int		damage;
	float	damage_radius = 1000;
	int		radius_damage = 400;

	if (deathmatch->value)
		damage = 200;
	else
		damage = 500;

	if (ent->client->ps.gunframe == 9)
	{
		// send muzzle flash
		gi.WriteByte(svc_muzzleflash);
		gi.WriteShort(ent - g_edicts);
		gi.WriteByte(MZ_BFG | is_silenced);
		gi.multicast(ent->s.origin, MULTICAST_PVS);

		ent->client->ps.gunframe++;

		PlayerNoise(ent, ent->s.origin, PNOISE_WEAPON);
		return;
	}

	if (is_quad)
		damage *= 4;

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);

	// make a big pitch kick with an inverse fall
	ent->client->v_dmg_pitch = -40;
	ent->client->v_dmg_roll = crandom() * 8;
	ent->client->v_dmg_time = level.time + DAMAGE_TIME;

	VectorSet(offset, 8, 8, ent->viewheight - 8);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
	fire_rocket(ent, start, forward, damage, 600, damage_radius, radius_damage);

	ent->client->ps.gunframe++;

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index] -= 50;
}

void Weapon_QuasarCannon(edict_t* ent)
{
	static int	pause_frames[] = { 39, 45, 50, 55, 0 };
	static int	fire_frames[] = { 9, 17, 0 };

	Weapon_Generic3(ent, 8, 32, 55, 58, pause_frames, fire_frames, weapon_qc_fire, WEP_QUASARCANNON);
}

void weapon_grenadelauncherv2_fire(edict_t* ent)
{
	int	i;
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		angles;
	int			kick = 2;
	vec3_t		offset;
	int			damage = 120;
	float		radius;

	radius = damage + 40;

	

	if (!(ent->client->buttons & BUTTON_ATTACK))
	{
		ent->client->machinegun_shots = 0;
		ent->client->ps.gunframe++;
		return;
	}

	if (ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))] == 0)
	{
		gi.cprintf(ent, PRINT_HIGH, "Out of Ammo,Press r to reload.\n");
		return;
	}

	if (ent->client->ps.gunframe == 5)
		ent->client->ps.gunframe = 4;
	else
		ent->client->ps.gunframe = 5;

	if (ent->client->pers.inventory[ent->client->ammo_index] < 1)
	{
		ent->client->ps.gunframe = 6;
		if (level.time >= ent->pain_debounce_time)
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
			ent->pain_debounce_time = level.time + 1;
		}
		NoAmmoWeaponChange(ent);
		return;
	}

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	for (i = 1; i < 3; i++)
	{
		ent->client->kick_origin[i] = crandom() * 0.35;
		ent->client->kick_angles[i] = crandom() * 0.7;
	}
	ent->client->kick_origin[0] = crandom() * 0.35;
	ent->client->kick_angles[0] = ent->client->machinegun_shots * -1.5;

	// raise the gun as it is firing
	if (!deathmatch->value)
	{
		ent->client->machinegun_shots++;
		if (ent->client->machinegun_shots > 9)
			ent->client->machinegun_shots = 9;
	}

	// get start / end positions
	VectorAdd(ent->client->v_angle, ent->client->kick_angles, angles);
	AngleVectors(angles, forward, right, NULL);
	VectorSet(offset, 0, 8, ent->viewheight - 8);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	fire_grenade(ent, start, forward, damage, 600, 2.5, radius);

	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_GRENADE | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))]--;

	ent->client->anim_priority = ANIM_ATTACK;
	if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
	{
		ent->s.frame = FRAME_crattak1 - (int)(random() + 0.25);
		ent->client->anim_end = FRAME_crattak9;
	}
	else
	{
		ent->s.frame = FRAME_attack1 - (int)(random() + 0.25);
		ent->client->anim_end = FRAME_attack8;
	}
}

void Weapon_GrenadeLauncherv2(edict_t* ent)
{
	static int	pause_frames[] = { 34, 51, 59, 0 };
	static int	fire_frames[] = { 6, 0 };

	Weapon_Generic3(ent, 5, 16, 59, 64, pause_frames, fire_frames, weapon_grenadelauncherv2_fire, WEP_GRENADELAUNCHERV2);
}

void weapon_IncenBreaker_fire(edict_t* ent)
{
	int			i;
	int			shots;
	vec3_t		start;
	vec3_t		forward, right, up;
	float		r, u;
	vec3_t		offset;
	int			damage;
	int			kick = 2;

	damage = 4;

	if (!(ent->client->buttons & BUTTON_ATTACK))
	{
		ent->client->machinegun_shots = 0;
		ent->client->ps.gunframe++;
		return;
	}

	if (ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))] == 0)
	{
		gi.cprintf(ent, PRINT_HIGH, "Out of Ammo,Press r to reload.\n");
		return;
	}

	if (ent->client->ps.gunframe == 9)
	{
		ent->client->ps.gunframe++;
		return;
	}

	for (i = 0; i < 12; i++)
	{
		AngleVectors(ent->client->v_angle, forward, right, up);
		r = 7 + crandom() * 4;
		u = crandom() * 4;
		VectorSet(offset, 0, r, u + ent->viewheight - 8);
		P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
		fire_blaster(ent, start, forward, damage, 400, EF_BLASTER, false);
	}


	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_SHOTGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))]--;
}

void Weapon_IncenBreaker(edict_t* ent)
{
	static int	pause_frames[] = { 29, 42, 57, 0 };
	static int	fire_frames[] = { 7, 0 };

	Weapon_Generic3(ent, 6, 17, 57, 61, pause_frames, fire_frames, weapon_IncenBreaker_fire, WEP_BREAKERINCENDIARY);
}
void weapon_Slugger_fire(edict_t* ent)
{
	vec3_t		start;
	vec3_t		forward, right;
	vec3_t		offset;
	int			damage = 100;
	int			kick = 8;

	if (!(ent->client->buttons & BUTTON_ATTACK))
	{
		ent->client->machinegun_shots = 0;
		ent->client->ps.gunframe++;
		return;
	}

	if (ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))] == 0)
	{
		gi.cprintf(ent, PRINT_HIGH, "Out of Ammo,Press r to reload.\n");
		return;
	}

	if (ent->client->ps.gunframe == 9)
	{
		ent->client->ps.gunframe++;
		return;
	}

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -2;

	VectorSet(offset, 0, 8, ent->viewheight - 8);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	if (is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	fire_blaster(ent, start, forward, damage, 1800, EF_BLASTER, false);

	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_SHOTGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))]--;
}
void Weapon_Slugger(edict_t* ent)
{
	static int	pause_frames[] = { 22, 28, 34, 0 };
	static int	fire_frames[] = { 8, 9, 0 };

	Weapon_Generic3(ent, 7, 18, 36, 39, pause_frames, fire_frames, weapon_Slugger_fire, WEP_SLUGGER);
}

void Blaster_Fire_Kinetic(edict_t* ent, vec3_t g_offset, int damage, qboolean hyper, int effect)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	offset;

	if (is_quad)
		damage *= 4;
	AngleVectors(ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 24, 8, ent->viewheight - 8);
	VectorAdd(offset, g_offset, offset);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	fire_blaster(ent, start, forward, damage, 1000, effect, hyper);


	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_MACHINEGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);
}

void weapon_senator_fire(edict_t* ent)
{
	int		damage;

	if (!(ent->client->buttons & BUTTON_ATTACK))
	{
		ent->client->machinegun_shots = 0;
		ent->client->ps.gunframe++;
		return;
	}

	if (ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))] == 0)
	{
		gi.cprintf(ent, PRINT_HIGH, "Out of Ammo,Press r to reload.\n");
		return;
	}

	if (deathmatch->value)
		damage = 15;
	else
		damage = 80;
	Blaster_Fire_Kinetic(ent, vec3_origin, damage, false, EF_BLASTER);
	ent->client->pers.inventory[ITEM_INDEX(FindItem("Current Rounds"))]--;
	ent->client->ps.gunframe++;
}
void Weapon_Senator(edict_t* ent)
{
	static int	pause_frames[] = { 19, 32, 0 };
	static int	fire_frames[] = { 5, 0 };

	Weapon_Generic3(ent, 4, 8, 52, 55, pause_frames, fire_frames, weapon_senator_fire, WEP_SENATOR);
}


#define GRENADE_TIMER		3.0
#define GRENADE_MINSPEED	400
#define GRENADE_MAXSPEED	800

void weapon_grenade_fire2(edict_t* ent, qboolean held)
{
	vec3_t	offset;
	vec3_t	forward, right;
	vec3_t	start;
	int		damage = 200;
	float	timer;
	int		speed;
	float	radius;

	radius = damage + 150;
	if (is_quad)
		damage *= 4;

	VectorSet(offset, 8, 8, ent->viewheight - 8);
	AngleVectors(ent->client->v_angle, forward, right, NULL);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	timer = ent->client->grenade_time - level.time ;
	speed = GRENADE_MINSPEED + (GRENADE_TIMER - timer) * ((GRENADE_MAXSPEED - GRENADE_MINSPEED) / GRENADE_TIMER);
	fire_grenade2(ent, start, forward, damage, speed, timer+3.0, radius, held);

	if (!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]--;

	ent->client->grenade_time = level.time + 1.0;

	if (ent->deadflag || ent->s.modelindex != 255) // VWep animations screw up corpses
	{
		return;
	}

	if (ent->health <= 0)
		return;

	if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
	{
		ent->client->anim_priority = ANIM_ATTACK;
		ent->s.frame = FRAME_crattak1 - 1;
		ent->client->anim_end = FRAME_crattak3;
	}
	else
	{
		ent->client->anim_priority = ANIM_REVERSE;
		ent->s.frame = FRAME_wave08;
		ent->client->anim_end = FRAME_wave01;
	}
}


void Stratagem_OPS(edict_t* ent)
{
	if ((ent->client->newweapon) && (ent->client->weaponstate == WEAPON_READY))
	{
		ChangeWeapon(ent);
		return;
	}

	if (ent->client->weaponstate == WEAPON_ACTIVATING)
	{
		ent->client->weaponstate = WEAPON_READY;
		ent->client->ps.gunframe = 16;
		return;
	}

	if (ent->client->weaponstate == WEAPON_READY)
	{
		if (((ent->client->latched_buttons | ent->client->buttons) & BUTTON_ATTACK))
		{
			ent->client->latched_buttons &= ~BUTTON_ATTACK;
			if (ent->client->pers.inventory[ent->client->ammo_index])
			{
				ent->client->ps.gunframe = 1;
				ent->client->weaponstate = WEAPON_FIRING;
				ent->client->grenade_time = 0;
			}
			else
			{
				if (level.time >= ent->pain_debounce_time)
				{
					gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
					ent->pain_debounce_time = level.time + 1;
				}
				NoAmmoWeaponChange(ent);
			}
			return;
		}

		if ((ent->client->ps.gunframe == 29) || (ent->client->ps.gunframe == 34) || (ent->client->ps.gunframe == 39) || (ent->client->ps.gunframe == 48))
		{
			if (rand() & 15)
				return;
		}

		if (++ent->client->ps.gunframe > 48)
			ent->client->ps.gunframe = 16;
		return;
	}

	if (ent->client->weaponstate == WEAPON_FIRING)
	{
		if (ent->client->ps.gunframe == 5)
			gi.sound(ent, CHAN_WEAPON, gi.soundindex("weapons/hgrena1b.wav"), 1, ATTN_NORM, 0);

		if (ent->client->ps.gunframe == 11)
		{
			if (!ent->client->grenade_time)
			{
				ent->client->grenade_time = level.time + GRENADE_TIMER + 0.2;
				ent->client->weapon_sound = gi.soundindex("weapons/hgrenc1b.wav");
			}

			// they waited too long, detonate it in their hand
			if (!ent->client->grenade_blew_up && level.time >= ent->client->grenade_time)
			{
				ent->client->weapon_sound = 0;
				weapon_grenade_fire2(ent, true);
				ent->client->grenade_blew_up = true;
			}

			if (ent->client->buttons & BUTTON_ATTACK)
				return;

			if (ent->client->grenade_blew_up)
			{
				if (level.time >= ent->client->grenade_time)
				{
					ent->client->ps.gunframe = 15;
					ent->client->grenade_blew_up = false;
				}
				else
				{
					return;
				}
			}
		}

		if (ent->client->ps.gunframe == 12)
		{
			ent->client->weapon_sound = 0;
			weapon_grenade_fire2(ent, false);
		}

		if ((ent->client->ps.gunframe == 15) && (level.time < ent->client->grenade_time))
			return;

		ent->client->ps.gunframe++;

		if (ent->client->ps.gunframe == 16)
		{
			ent->client->grenade_time = 0;
			ent->client->weaponstate = WEAPON_READY;
		}
	}
}