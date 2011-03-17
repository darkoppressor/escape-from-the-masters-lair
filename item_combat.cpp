/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "item.h"

using namespace std;

void Item::assign_owner_data_thrown(Creature* creature){
    owner_data_thrown.clear();

    owner_data_thrown.push_back(Owner_Data_Thrown());

    owner_data_thrown[0].strength=creature->attributes[ATTRIBUTE_STRENGTH];
    owner_data_thrown[0].agility=creature->attributes[ATTRIBUTE_AGILITY];
    owner_data_thrown[0].fighting_skill=creature->skills[SKILL_FIGHTING];
    owner_data_thrown[0].thrown_weapons_skill=creature->skills[SKILL_THROWN_WEAPONS];
    owner_data_thrown[0].experience_level=creature->experience_level;
    owner_data_thrown[0].base_damage_min_thrown=creature->base_damage_thrown_min;
    owner_data_thrown[0].base_damage_max_thrown=creature->base_damage_thrown_max;
}

void Item::attack_thrown(Creature* target){
    int damage=0;

    string outcome="";

    //If the attacker succeeds its hit check.
    if(rc_attack_hit(this,target)){
       //If the defender fails its dodge check.
       if(!rc_attack_dodge(this,target)){
            //The attack will hit.
            //We now determine the maximum amount of damage the attacker can do (before the target's reduction(s)).

            //Damage begins with base thrown damage.
            int base_damage=random_range(base_damage_thrown_min,base_damage_thrown_max);

            damage=base_damage;

            //Add in thrown weapon damage.

            //Determine the base damage range for this item.
            int weapon_damage_min=thrown_item->damage_min_thrown;
            int weapon_damage_max=thrown_item->damage_max_thrown;
            int weapon_damage=random_range(weapon_damage_min,weapon_damage_max);

            //Apply the appropriate weapon skill, if any.

            //If the item is governed by the thrown weapons skill.
            if(thrown_item->category==ITEM_WEAPON && thrown_item->governing_skill_weapon==SKILL_THROWN_WEAPONS){
                weapon_damage+=weapon_damage*(skills[SKILL_THROWN_WEAPONS]/10);
            }

            //Add the weapon's damage to the attack's damage.
            damage+=weapon_damage;

            //Apply the agility bonus.
            damage+=damage*(attributes[ATTRIBUTE_AGILITY]/6);

            //Apply the strength bonus.
            damage+=damage*(attributes[ATTRIBUTE_STRENGTH]/8);

            //We have finished determining the maximum damage the attacker can do.
            //Now, we determine the damage reduction based on the target's stats.

            int damage_reduction=0;

            //Add in target's armor damage reduction.

            //Look through all of the target's armor slots.
            for(int i=EQUIP_HEAD;i<EQUIP_FINGER_LEFT+1;i++){
                //If this slot has an item equipped.
                if(target->equipment[i]!=0){
                    //Determine the identifier for the item equipped in this slot.
                    int item_identifier=target->slot_equipped_with_what_item(target->equipment[i]);

                    //Determine the base amount of damage absorbed by this item.
                    int armor_absorption=target->inventory[item_identifier].defense;

                    //Apply the armor skill.
                    armor_absorption+=armor_absorption*(target->skills[SKILL_ARMOR]/10);

                    damage_reduction+=armor_absorption;
                }
            }

            //Apply the hardiness bonus.
            damage_reduction+=damage_reduction*(target->attributes[ATTRIBUTE_HARDINESS]/4);

            //Subtract the total damage reduction from the maximum damage.
            damage-=damage_reduction;
            if(damage<0){
                damage=0;
            }

            //If the attacker succeeds in a critical strike.
            if(rc_attack_critical_strike(this,target)){
                damage+=random_range(damage/4,damage/2);
            }
       }
        //If the defender succeeds in its dodge check.
        else{
            if(is_player){
                outcome="The ";
                outcome+=target->return_full_name();
                outcome+=" dodges your ";
                outcome+=thrown_item->return_full_name();
                outcome+="!";
            }
            else{
                outcome="You dodge the ";
                outcome+=return_full_name();
                outcome+="'s ";
                outcome+=thrown_item->return_full_name();
                outcome+="!";
            }

            update_text_log(outcome.c_str(),true);

            return;
        }
    }
    //If the attacker fails its hit check.
    else{
        if(is_player){
            outcome="Your ";
            outcome+=thrown_item->return_full_name();
            outcome+=" misses the ";
            outcome+=target->return_full_name();
            outcome+="!";
        }
        else{
            outcome="The ";
            outcome+=return_full_name();
            outcome+="'s ";
            outcome+=thrown_item->return_full_name();
            outcome+=" misses!";
        }

        update_text_log(outcome.c_str(),true);

        return;
    }

    //If the attack succeeded and did damage.
    if(damage>0){
        //If the creature is the player.
        if(is_player){
            outcome="Your ";
            outcome+=thrown_item->return_full_name();
            outcome+=" hits the ";
            outcome+=target->return_full_name();
            outcome+="!";
        }
        //If the creature is not the player.
        else{
            outcome="The ";
            outcome+=return_full_name();
            outcome+="'s ";
            outcome+=thrown_item->return_full_name();
            outcome+=" hits you!";
        }
    }
    //If the attack succeeded but did no damage.
    else{
        //If the creature is the player.
        if(is_player){
            outcome="Your ";
            outcome+=thrown_item->return_full_name();
            outcome+=" bounces off of the ";
            outcome+=target->return_full_name();
            outcome+="!";
        }
        //If the creature is not the player.
        else{
            outcome="The ";
            outcome+=return_full_name();
            outcome+="'s ";
            outcome+=thrown_item->return_full_name();
            outcome+=" bounces off of you!";
        }
    }

    update_text_log(outcome.c_str(),true);

    //Subtract the attack damage from the target's health.
    target->health-=damage;

    //If the target's health drops to 0 or below, it has been killed.
    if(target->health<=0){
        target->die();

        //This creature gains experience.
        ///gain_experience(target->experience_level);
    }
}
