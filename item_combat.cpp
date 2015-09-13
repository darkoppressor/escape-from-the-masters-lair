/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "item.h"
#include "creature.h"
#include "enumerations.h"
#include "random_chance.h"
#include "random_number_generator.h"
#include "message_log.h"
#include "combat_all.h"
#include "world.h"
#include "grammar.h"

using namespace std;

void Item::clear_owner_data_all(){
    owner_data_thrown.clear();
    owner_data_fired.clear();
}

void Item::assign_owner_data_thrown(Creature* creature){
    clear_owner_data_all();

    owner_data_thrown.push_back(Owner_Data_Thrown());

    owner_data_thrown[0].strength=creature->return_attribute_strength();
    owner_data_thrown[0].agility=creature->return_attribute_agility();
    owner_data_thrown[0].thrown_weapons_skill=creature->return_skill_thrown_weapons();
    owner_data_thrown[0].experience_level=creature->experience_level;
    owner_data_thrown[0].base_damage_min_thrown=creature->base_damage_thrown_min;
    owner_data_thrown[0].base_damage_max_thrown=creature->base_damage_thrown_max;
    owner_data_thrown[0].race=creature->race;
    owner_data_thrown[0].full_name=creature->return_full_name();
    owner_data_thrown[0].is_player=creature->is_player;
}

void Item::assign_owner_data_fired(Creature* creature,int launcher_item){
    clear_owner_data_all();

    owner_data_fired.push_back(Owner_Data_Fired());

    owner_data_fired[0].strength=creature->return_attribute_strength();
    owner_data_fired[0].agility=creature->return_attribute_agility();
    owner_data_fired[0].launcher_weapons_skill=creature->return_skill_launcher_weapons();
    owner_data_fired[0].experience_level=creature->experience_level;
    owner_data_fired[0].base_damage_min_ranged=creature->base_damage_ranged_min;
    owner_data_fired[0].base_damage_max_ranged=creature->base_damage_ranged_max;
    owner_data_fired[0].launcher_damage_min=creature->inventory[launcher_item].damage_min_ranged;
    owner_data_fired[0].launcher_damage_max=creature->inventory[launcher_item].damage_max_ranged;
    owner_data_fired[0].race=creature->race;
    owner_data_fired[0].full_name=creature->return_full_name();
    owner_data_fired[0].is_player=creature->is_player;

    //If the launcher is not this item's assigned launcher.
    if(creature->inventory[launcher_item].weapon_category!=launcher){
        //The launcher's damage does not apply.
        owner_data_fired[0].launcher_damage_min=0;
        owner_data_fired[0].launcher_damage_max=0;
    }
}

void Item::attack(Creature* target){
    //If the item was thrown.
    if(owner_data_thrown.size()>0){
        attack_thrown(target);
    }

    //If the item was fired.
    else if(owner_data_fired.size()>0){
        attack_fired(target);
    }
}

void Item::attack_thrown(Creature* target){
    int damage=0;

    string outcome="";

    //Determine the maximum amount of damage the attacker can do (before the target's reduction(s)).

    //Damage begins with base thrown damage.
    int base_damage=random_range(owner_data_thrown[0].base_damage_min_thrown,owner_data_thrown[0].base_damage_max_thrown);

    damage=base_damage;

    //Add the momentum bonus.
    damage+=momentum*2.0;

    //Add the racial base thrown damage modifier.

    //If the race has a thrown damage bonus.
    if(templates.template_races[owner_data_thrown[0].race].base_damage_thrown_max>=0){
        damage+=random_range(templates.template_races[owner_data_thrown[0].race].base_damage_thrown_min,templates.template_races[owner_data_thrown[0].race].base_damage_thrown_max);
    }
    //If the race has a thrown damage penalty.
    else{
        damage-=random_range(abs(templates.template_races[owner_data_thrown[0].race].base_damage_thrown_min),abs(templates.template_races[owner_data_thrown[0].race].base_damage_thrown_max));
    }

    //Add in thrown weapon damage.

    //Determine the base damage range for this item.
    int weapon_damage_min=damage_min_thrown;
    int weapon_damage_max=damage_max_thrown;
    int weapon_damage=random_range(weapon_damage_min,weapon_damage_max);

    //Apply the appropriate weapon skill, if any.

    //If the item is governed by the thrown weapons skill.
    if(category==ITEM_WEAPON && governing_skill_weapon==SKILL_THROWN_WEAPONS){
        weapon_damage+=weapon_damage*(owner_data_thrown[0].thrown_weapons_skill/10);
    }

    //Add the weapon's damage to the attack's damage.
    damage+=weapon_damage;

    //Apply the agility bonus.
    damage+=damage*(owner_data_thrown[0].agility/6);

    //Apply the strength bonus.
    damage+=damage*(owner_data_thrown[0].strength/8);

    //We have finished determining the maximum damage the attacker can do.
    //Now, we determine the damage reduction based on the target's stats.

    int damage_reduction=determine_damage_reduction(target);

    //Subtract the total damage reduction from the maximum damage.
    damage-=damage_reduction;

    if(damage<0){
        damage=0;
    }

    //If the attack did damage.
    if(damage>0){
        //If the creature is the player.
        if(owner_data_thrown[0].is_player){
            outcome="Your ";
            outcome+=return_full_name(1);
            outcome+=" hits the ";
            outcome+=target->return_full_name();
            outcome+="!";
        }
        //If the creature is not the player.
        else{
            outcome="The ";
            outcome+=owner_data_thrown[0].full_name;
            outcome+="'s ";
            outcome+=return_full_name(1);
            outcome+=" hits you!";
        }
    }
    //If the attack succeeded but did no damage.
    else{
        //If the creature is the player.
        if(owner_data_thrown[0].is_player){
            outcome="Your ";
            outcome+=return_full_name(1);
            outcome+=" bounces off of the ";
            outcome+=target->return_full_name();
            outcome+="!";
        }
        //If the creature is not the player.
        else{
            outcome="The ";
            outcome+=owner_data_thrown[0].full_name;
            outcome+="'s ";
            outcome+=return_full_name(1);
            outcome+=" bounces off of you!";
        }
    }

    update_text_log(outcome.c_str(),true);

    //Subtract the attack damage from the target's health.
    target->health-=damage;

    //If the target's health drops to 0 or below, it has been killed.
    if(target->return_health()<=0){
        target->die(CAUSE_OF_DEATH_THROWN,owner_data_thrown[0].full_name,return_full_name(1),a_vs_an(owner_data_thrown[0].race),a_vs_an(this));
    }
}

void Item::attack_fired(Creature* target){
    int damage=0;

    string outcome="";

    //Determine the maximum amount of damage the attacker can do (before the target's reduction(s)).

    //Damage begins with base ranged damage.
    int base_damage=random_range(owner_data_fired[0].base_damage_min_ranged,owner_data_fired[0].base_damage_max_ranged);

    damage=base_damage;

    //Add the momentum bonus.
    damage+=momentum*2.0;

    //Add the racial base ranged damage modifier.

    //If the race has a ranged damage bonus.
    if(templates.template_races[owner_data_fired[0].race].base_damage_ranged_max>=0){
        damage+=random_range(templates.template_races[owner_data_fired[0].race].base_damage_ranged_min,templates.template_races[owner_data_fired[0].race].base_damage_ranged_max);
    }
    //If the race has a ranged damage penalty.
    else{
        damage-=random_range(abs(templates.template_races[owner_data_fired[0].race].base_damage_ranged_min),abs(templates.template_races[owner_data_fired[0].race].base_damage_ranged_max));
    }

    //Add the actual projectile's thrown weapon damage.

    //Determine the base damage range for this item.
    int weapon_damage_min=damage_min_thrown;
    int weapon_damage_max=damage_max_thrown;
    int weapon_damage=random_range(weapon_damage_min,weapon_damage_max);

    //Add the weapon's damage to the attack's damage.
    damage+=weapon_damage;

    //Add in the launcher's damage.

    weapon_damage_min=owner_data_fired[0].launcher_damage_min;
    weapon_damage_max=owner_data_fired[0].launcher_damage_max;
    weapon_damage=random_range(weapon_damage_min,weapon_damage_max);

    //Apply the ranged weapon skill.
    weapon_damage+=weapon_damage*(owner_data_fired[0].launcher_weapons_skill/10);

    //Add the weapon's damage to the attack's damage.
    damage+=weapon_damage;

    //Apply the agility bonus.
    damage+=damage*(owner_data_fired[0].agility/6);

    //Apply the strength bonus.
    damage+=damage*(owner_data_fired[0].strength/8);

    //We have finished determining the maximum damage the attacker can do.
    //Now, we determine the damage reduction based on the target's stats.

    int damage_reduction=determine_damage_reduction(target);

    //Subtract the total damage reduction from the maximum damage.
    damage-=damage_reduction;

    if(damage<0){
        damage=0;
    }

    //If the attack did damage.
    if(damage>0){
        //If the creature is the player.
        if(owner_data_fired[0].is_player){
            outcome="Your ";
            outcome+=return_full_name(1);
            outcome+=" hits the ";
            outcome+=target->return_full_name();
            outcome+="!";
        }
        //If the creature is not the player.
        else{
            outcome="The ";
            outcome+=owner_data_fired[0].full_name;
            outcome+="'s ";
            outcome+=return_full_name(1);
            outcome+=" hits you!";
        }
    }
    //If the attack succeeded but did no damage.
    else{
        //If the creature is the player.
        if(owner_data_fired[0].is_player){
            outcome="Your ";
            outcome+=return_full_name(1);
            outcome+=" bounces off of the ";
            outcome+=target->return_full_name();
            outcome+="!";
        }
        //If the creature is not the player.
        else{
            outcome="The ";
            outcome+=owner_data_fired[0].full_name;
            outcome+="'s ";
            outcome+=return_full_name(1);
            outcome+=" bounces off of you!";
        }
    }

    update_text_log(outcome.c_str(),true);

    //Subtract the attack damage from the target's health.
    target->health-=damage;

    //If the target's health drops to 0 or below, it has been killed.
    if(target->return_health()<=0){
        target->die(CAUSE_OF_DEATH_RANGED,owner_data_fired[0].full_name,return_full_name(1),a_vs_an(owner_data_fired[0].race),a_vs_an(this));
    }
}
