/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"
#include "message_log.h"
#include "random_chance.h"
#include "random_number_generator.h"
#include "world.h"

using namespace std;

void Creature::attack_thrown(Creature* target,Item* thrown_item){
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
        gain_experience(target->experience_level);
    }
}

void Creature::attack_melee(Creature* target){
    int damage=0;

    string outcome="";

    //If the attacker succeeds its hit check.
    if(rc_attack_hit(this,target)){
       //If the defender fails its dodge check.
       if(!rc_attack_dodge(this,target)){
            //The attack will hit.
            //We now determine the maximum amount of damage the attacker can do (before the target's reduction(s)).

            //Damage begins with base melee damage.
            int base_damage=random_range(base_damage_melee_min,base_damage_melee_max);

            damage=base_damage;

            //Add in melee weapon damage.

            //Check for items wielded in either hand.
            for(int i=EQUIP_HOLD_RIGHT;i<EQUIP_HOLD_LEFT+1;i++){
                //If there is an item wielded in this hand.
                if(equipment[i]!=0){
                    //Determine the identifier for the item equipped in this slot.
                    int item_identifier=slot_equipped_with_what_item(equipment[i]);

                    //Determine the base damage range for this item.
                    int weapon_damage_min=inventory[item_identifier].damage_min_melee;
                    int weapon_damage_max=inventory[item_identifier].damage_max_melee;
                    int weapon_damage=random_range(weapon_damage_min,weapon_damage_max);

                    //Apply the appropriate weapon skill, if any.

                    //If the item is governed by the bladed weapons skill.
                    if(inventory[item_identifier].category==ITEM_WEAPON && inventory[item_identifier].governing_skill_weapon==SKILL_BLADED_WEAPONS){
                        weapon_damage+=weapon_damage*(skills[SKILL_BLADED_WEAPONS]/10);
                    }
                    //If the item is governed by the blunt weapons skill.
                    else if(inventory[item_identifier].category==ITEM_WEAPON && inventory[item_identifier].governing_skill_weapon==SKILL_BLUNT_WEAPONS){
                        weapon_damage+=weapon_damage*(skills[SKILL_BLUNT_WEAPONS]/10);
                    }
                    //If the item is governed by the stabbing weapons skill.
                    else if(inventory[item_identifier].category==ITEM_WEAPON && inventory[item_identifier].governing_skill_weapon==SKILL_STABBING_WEAPONS){
                        weapon_damage+=weapon_damage*(skills[SKILL_STABBING_WEAPONS]/10);
                    }

                    //Add the weapon's damage to the attack's damage.
                    damage+=weapon_damage;
                }
            }

            //If items are being dual-wielded.
            if(equipment[EQUIP_HOLD_RIGHT]!=0 && equipment[EQUIP_HOLD_LEFT]!=0){
                //Add the dual-wielding bonus.
                damage+=skills[SKILL_DUAL_WIELDING]/2;
            }

            //If there are no items being wielded.
            else if(equipment[EQUIP_HOLD_RIGHT]==0 && equipment[EQUIP_HOLD_LEFT]==0){
                //Apply the unarmed weapon skill.
                damage+=base_damage*(skills[SKILL_UNARMED]/10);
            }

            //Apply the strength bonus.
            damage+=damage*(attributes[ATTRIBUTE_STRENGTH]/4);

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
                outcome+=" dodges out of the way!";
            }
            else{
                outcome="You dodge the ";
                outcome+=return_full_name();
                outcome+="'s attack!";
            }

            update_text_log(outcome.c_str(),true);

            return;
        }
    }
    //If the attacker fails its hit check.
    else{
        if(is_player){
            outcome="You miss the ";
            outcome+=target->return_full_name();
            outcome+="!";
        }
        else{
            outcome="The ";
            outcome+=return_full_name();
            outcome+=" misses!";
        }

        update_text_log(outcome.c_str(),true);

        return;
    }

    //If the attack succeeded and did damage.
    if(damage>0){
        //If the creature is the player.
        if(is_player){
            //If there are no items being wielded.
            if(equipment[EQUIP_HOLD_RIGHT]==0 && equipment[EQUIP_HOLD_LEFT]==0){
                outcome="You attack the ";
                outcome+=target->return_full_name();
                outcome+=" with your bare hands!";
            }
            //If there is an item being wielded in the right hand only.
            else if(equipment[EQUIP_HOLD_RIGHT]!=0 && equipment[EQUIP_HOLD_LEFT]==0){
                //Determine the identifier for the item equipped in this slot.
                int item_identifier=slot_equipped_with_what_item(equipment[EQUIP_HOLD_RIGHT]);

                //If the item being wielded is a weapon.
                if(inventory[item_identifier].category==ITEM_WEAPON){
                    //If the item is a bladed weapon.
                    if(inventory[item_identifier].governing_skill_weapon==SKILL_BLADED_WEAPONS){
                        outcome="You slash the ";
                    }
                    //If the item is a blunt weapon.
                    else if(inventory[item_identifier].governing_skill_weapon==SKILL_BLUNT_WEAPONS){
                        outcome="You smash the ";
                    }
                    //If the item is a stabbing weapon.
                    else if(inventory[item_identifier].governing_skill_weapon==SKILL_STABBING_WEAPONS){
                        outcome="You stab the ";
                    }
                    //If the item is not a melee weapon.
                    else{
                        outcome="You whack the ";
                    }
                }
                //If the item being wielded is not a weapon.
                else{
                    outcome="You whack the ";
                }

                outcome+=target->return_full_name();
                outcome+=" with your ";
                outcome+=inventory[item_identifier].return_full_name();
                outcome+="!";
            }
            //If there is an item being wielded in the left hand only.
            else if(equipment[EQUIP_HOLD_RIGHT]==0 && equipment[EQUIP_HOLD_LEFT]!=0){
                //Determine the identifier for the item equipped in this slot.
                int item_identifier=slot_equipped_with_what_item(equipment[EQUIP_HOLD_LEFT]);

                //If the item being wielded is a weapon.
                if(inventory[item_identifier].category==ITEM_WEAPON){
                    //If the item is a bladed weapon.
                    if(inventory[item_identifier].governing_skill_weapon==SKILL_BLADED_WEAPONS){
                        outcome="You slash the ";
                    }
                    //If the item is a blunt weapon.
                    else if(inventory[item_identifier].governing_skill_weapon==SKILL_BLUNT_WEAPONS){
                        outcome="You smash the ";
                    }
                    //If the item is a stabbing weapon.
                    else if(inventory[item_identifier].governing_skill_weapon==SKILL_STABBING_WEAPONS){
                        outcome="You stab the ";
                    }
                    //If the item is not a melee weapon.
                    else{
                        outcome="You whack the ";
                    }
                }
                //If the item being wielded is not a weapon.
                else{
                    outcome="You whack the ";
                }

                outcome+=target->return_full_name();
                outcome+=" with your ";
                outcome+=inventory[item_identifier].return_full_name();
                outcome+="!";
            }
            //If there is an item being wielded in both hands.
            else if(equipment[EQUIP_HOLD_RIGHT]!=0 && equipment[EQUIP_HOLD_LEFT]!=0){
                //Determine the identifier for the item equipped in each slot.
                int item_identifier_right=slot_equipped_with_what_item(equipment[EQUIP_HOLD_RIGHT]);
                int item_identifier_left=slot_equipped_with_what_item(equipment[EQUIP_HOLD_LEFT]);

                outcome="You attack the ";
                outcome+=target->return_full_name();
                outcome+=" with your ";
                outcome+=inventory[item_identifier_right].return_full_name();
                outcome+=" and your ";
                outcome+=inventory[item_identifier_left].return_full_name();
                outcome+="!";
            }
        }
        //If the creature is not the player.
        else{
            //If there are no items being wielded.
            if(equipment[EQUIP_HOLD_RIGHT]==0 && equipment[EQUIP_HOLD_LEFT]==0){
                outcome="The ";
                outcome+=return_full_name();
                outcome+=" attacks you with its bare hands!";
            }
            //If there is an item being wielded in the right hand only.
            else if(equipment[EQUIP_HOLD_RIGHT]!=0 && equipment[EQUIP_HOLD_LEFT]==0){
                //Determine the identifier for the item equipped in this slot.
                int item_identifier=slot_equipped_with_what_item(equipment[EQUIP_HOLD_RIGHT]);

                outcome="The ";
                outcome+=return_full_name();

                //If the item being wielded is a weapon.
                if(inventory[item_identifier].category==ITEM_WEAPON){
                    //If the item is a bladed weapon.
                    if(inventory[item_identifier].governing_skill_weapon==SKILL_BLADED_WEAPONS){
                        outcome+=" slashes you ";
                    }
                    //If the item is a blunt weapon.
                    else if(inventory[item_identifier].governing_skill_weapon==SKILL_BLUNT_WEAPONS){
                        outcome+=" smashes you ";
                    }
                    //If the item is a stabbing weapon.
                    else if(inventory[item_identifier].governing_skill_weapon==SKILL_STABBING_WEAPONS){
                        outcome+=" stabs you ";
                    }
                    //If the item is not a melee weapon.
                    else{
                        outcome=" whacks you ";
                    }
                }
                //If the item being wielded is not a weapon.
                else{
                    outcome+=" whacks you ";
                }

                outcome+=" with its ";
                outcome+=inventory[item_identifier].return_full_name();
                outcome+="!";
            }
            //If there is an item being wielded in the left hand only.
            else if(equipment[EQUIP_HOLD_RIGHT]==0 && equipment[EQUIP_HOLD_LEFT]!=0){
                //Determine the identifier for the item equipped in this slot.
                int item_identifier=slot_equipped_with_what_item(equipment[EQUIP_HOLD_LEFT]);

                outcome="The ";
                outcome+=return_full_name();

                //If the item being wielded is a weapon.
                if(inventory[item_identifier].category==ITEM_WEAPON){
                    //If the item is a bladed weapon.
                    if(inventory[item_identifier].governing_skill_weapon==SKILL_BLADED_WEAPONS){
                        outcome+=" slashes you ";
                    }
                    //If the item is a blunt weapon.
                    else if(inventory[item_identifier].governing_skill_weapon==SKILL_BLUNT_WEAPONS){
                        outcome+=" smashes you ";
                    }
                    //If the item is a stabbing weapon.
                    else if(inventory[item_identifier].governing_skill_weapon==SKILL_STABBING_WEAPONS){
                        outcome+=" stabs you ";
                    }
                    //If the item is not a melee weapon.
                    else{
                        outcome=" whacks you ";
                    }
                }
                //If the item being wielded is not a weapon.
                else{
                    outcome+=" whacks you ";
                }

                outcome+=" with its ";
                outcome+=inventory[item_identifier].return_full_name();
                outcome+="!";
            }
            //If there is an item being wielded in both hands.
            else if(equipment[EQUIP_HOLD_RIGHT]!=0 && equipment[EQUIP_HOLD_LEFT]!=0){
                //Determine the identifier for the item equipped in each slot.
                int item_identifier_right=slot_equipped_with_what_item(equipment[EQUIP_HOLD_RIGHT]);
                int item_identifier_left=slot_equipped_with_what_item(equipment[EQUIP_HOLD_LEFT]);

                outcome="The ";
                outcome+=return_full_name();
                outcome=" attacks you with its ";
                outcome+=inventory[item_identifier_right].return_full_name();
                outcome+=" and its ";
                outcome+=inventory[item_identifier_left].return_full_name();
                outcome+="!";
            }
        }
    }
    //If the attack succeeded but did no damage.
    else{
        //If the creature is the player.
        if(is_player){
            outcome="Your attack glances off the ";
            outcome+=target->return_full_name();
            outcome+="!";
        }
        //If the creature is not the player.
        else{
            outcome="The ";
            outcome+=return_full_name();
            outcome+="'s attack glances off!";
        }
    }

    update_text_log(outcome.c_str(),true);

    //Subtract the attack damage from the target's health.
    target->health-=damage;

    //If the target's health drops to 0 or below, it has been killed.
    if(target->health<=0){
        target->die();

        //This creature gains experience.
        gain_experience(target->experience_level);
    }
}

void Creature::die(){
    //The creature is now dead.
    alive=false;

    string message="";

    if(is_player){
        message="You have been slain";
        message+="!";
    }
    else{
        message="The ";
        message+=return_full_name();
        message+=" dies!";
    }

    update_text_log(message.c_str(),true);

    //Drop the creature's inventory.

    for(int i=0;i<inventory.size();i++){
        //If the item is either not money, or is money but with at least 1 in the stack.
        if(inventory[i].inventory_letter!='$' || (inventory[i].inventory_letter=='$' && inventory[i].stack>0)){
            //Set the item's position to the creature's current position.
            inventory[i].x=x;
            inventory[i].y=y;

            //Add the item to the dungeon items vector.
            vector_levels[current_level].items.push_back(inventory[i]);

            //Assign an identifier to the new item.
            vector_levels[current_level].items[vector_levels[current_level].items.size()-1].assign_identifier();

            //Return the inventory item's identifier.
            inventory[i].return_identifier();
        }
    }

    //Drop the creature's corpse.

    //Create a temporary corpse item.
    Item temp_item=templates.template_items[ITEM_FOOD][0];

    //Set the item's info to the corpse's race.
    string temp_name=templates.template_races[race].name;
    temp_name+=" ";
    temp_item.name=temp_name+temp_item.name;

    temp_item.color=templates.template_races[race].color;
    temp_item.weight=templates.template_races[race].weight;

    //Set the item's position to the creature's current position.
    temp_item.x=x;
    temp_item.y=y;

    //Add the item to the dungeon items vector.
    vector_levels[current_level].items.push_back(temp_item);

    //Assign an identifier to the new item.
    vector_levels[current_level].items[vector_levels[current_level].items.size()-1].assign_identifier();
}
