/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "item.h"
#include "creature.h"
#include "enumerations.h"
#include "random_chance.h"
#include "random_number_generator.h"
#include "message_log.h"

using namespace std;

void Item::clear_owner_data_all(){
    owner_data_thrown.clear();
    owner_data_fired.clear();
    owner_data_kicked.clear();
}

void Item::assign_owner_data_thrown(Creature* creature){
    clear_owner_data_all();

    owner_data_thrown.push_back(Owner_Data_Thrown());

    owner_data_thrown[0].strength=creature->attributes[ATTRIBUTE_STRENGTH];
    owner_data_thrown[0].agility=creature->attributes[ATTRIBUTE_AGILITY];
    owner_data_thrown[0].fighting_skill=creature->skills[SKILL_FIGHTING];
    owner_data_thrown[0].thrown_weapons_skill=creature->skills[SKILL_THROWN_WEAPONS];
    owner_data_thrown[0].experience_level=creature->experience_level;
    owner_data_thrown[0].base_damage_min_thrown=creature->base_damage_thrown_min;
    owner_data_thrown[0].base_damage_max_thrown=creature->base_damage_thrown_max;
    owner_data_thrown[0].full_name=creature->return_full_name();
    owner_data_thrown[0].is_player=creature->is_player;
}

void Item::assign_owner_data_fired(Creature* creature){
    clear_owner_data_all();

    owner_data_fired.push_back(Owner_Data_Fired());

    ///owner_data_fired[0].strength=creature->attributes[ATTRIBUTE_STRENGTH];
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

    //If the item was kicked.
    /**else if(owner_data_kicked.size()>0){
        attack_kicked(target);
    }*/
}

void Item::attack_thrown(Creature* target){
    int damage=0;

    string outcome="";

    //If the attacker succeeds its hit check.
    if(rc_attack_hit(owner_data_thrown[0].fighting_skill,owner_data_thrown[0].agility,owner_data_thrown[0].experience_level,target)){
       //If the defender fails its dodge check.
       if(!rc_attack_dodge(owner_data_thrown[0].experience_level,target)){
            //The attack will hit.
            //We now determine the maximum amount of damage the attacker can do (before the target's reduction(s)).

            //Damage begins with base thrown damage.
            int base_damage=random_range(owner_data_thrown[0].base_damage_min_thrown,owner_data_thrown[0].base_damage_max_thrown);

            damage=base_damage;

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
            if(rc_attack_critical_strike(owner_data_thrown[0].agility,owner_data_thrown[0].experience_level,target)){
                damage+=random_range(damage/4,damage/2);
            }
       }
        //If the defender succeeds in its dodge check.
        else{
            if(owner_data_thrown[0].is_player){
                outcome="The ";
                outcome+=target->return_full_name();
                outcome+=" dodges your ";
                outcome+=return_full_name(1);
                outcome+="!";
            }
            else{
                outcome="You dodge the ";
                outcome+=owner_data_thrown[0].full_name;
                outcome+="'s ";
                outcome+=return_full_name(1);
                outcome+="!";
            }

            update_text_log(outcome.c_str(),true);

            return;
        }
    }
    //If the attacker fails its hit check.
    else{
        if(owner_data_thrown[0].is_player){
            outcome="Your ";
            outcome+=return_full_name(1);
            outcome+=" misses the ";
            outcome+=target->return_full_name();
            outcome+="!";
        }
        else{
            outcome="The ";
            outcome+=owner_data_thrown[0].full_name;
            outcome+="'s ";
            outcome+=return_full_name(1);
            outcome+=" misses!";
        }

        update_text_log(outcome.c_str(),true);

        return;
    }

    //If the attack succeeded and did damage.
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
    if(target->health<=0){
        target->die();

        //This creature gains experience.
        ///gain_experience(target->experience_level);
    }
}

void Item::attack_fired(Creature* target){
    int damage=0;

    string outcome="";

    //If the attacker succeeds its hit check.
    if(rc_attack_hit(owner_data_thrown[0].fighting_skill,owner_data_thrown[0].agility,owner_data_thrown[0].experience_level,target)){
       //If the defender fails its dodge check.
       if(!rc_attack_dodge(owner_data_thrown[0].experience_level,target)){
            //The attack will hit.
            //We now determine the maximum amount of damage the attacker can do (before the target's reduction(s)).

            //Damage begins with base thrown damage.
            int base_damage=random_range(owner_data_thrown[0].base_damage_min_thrown,owner_data_thrown[0].base_damage_max_thrown);

            damage=base_damage;

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
            if(rc_attack_critical_strike(owner_data_thrown[0].agility,owner_data_thrown[0].experience_level,target)){
                damage+=random_range(damage/4,damage/2);
            }
       }
        //If the defender succeeds in its dodge check.
        else{
            if(owner_data_thrown[0].is_player){
                outcome="The ";
                outcome+=target->return_full_name();
                outcome+=" dodges your ";
                outcome+=return_full_name(1);
                outcome+="!";
            }
            else{
                outcome="You dodge the ";
                outcome+=owner_data_thrown[0].full_name;
                outcome+="'s ";
                outcome+=return_full_name(1);
                outcome+="!";
            }

            update_text_log(outcome.c_str(),true);

            return;
        }
    }
    //If the attacker fails its hit check.
    else{
        if(owner_data_thrown[0].is_player){
            outcome="Your ";
            outcome+=return_full_name(1);
            outcome+=" misses the ";
            outcome+=target->return_full_name();
            outcome+="!";
        }
        else{
            outcome="The ";
            outcome+=owner_data_thrown[0].full_name;
            outcome+="'s ";
            outcome+=return_full_name(1);
            outcome+=" misses!";
        }

        update_text_log(outcome.c_str(),true);

        return;
    }

    //If the attack succeeded and did damage.
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
    if(target->health<=0){
        target->die();

        //This creature gains experience.
        ///gain_experience(target->experience_level);
    }
}
