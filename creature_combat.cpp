/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"
#include "message_log.h"
#include "random_chance.h"
#include "random_number_generator.h"
#include "world.h"
#include "combat_all.h"
#include "grammar.h"

using namespace std;

void Creature::attack_melee(Creature* target){
    int damage=0;

    string outcome="";

    //If the attacker succeeds its hit check.
    /**if(rc_attack_hit(return_skill_fighting(),return_attribute_agility(),experience_level,target)){
       //If the defender fails its dodge check.
       if(!rc_attack_dodge(experience_level,target)){*/
            //The attack will hit.
            //We now determine the maximum amount of damage the attacker can do (before the target's reduction(s)).

            //Damage begins with base melee damage.
            int base_damage=random_range(base_damage_melee_min,base_damage_melee_max);

            damage=base_damage;

            //Add the racial base melee damage modifier.

            //If the race has a melee damage bonus.
            if(templates.template_races[race].base_damage_melee_max>=0){
                damage+=random_range(templates.template_races[race].base_damage_melee_min,templates.template_races[race].base_damage_melee_max);
            }
            //If the race has a melee damage penalty.
            else{
                damage-=random_range(abs(templates.template_races[race].base_damage_melee_min),abs(templates.template_races[race].base_damage_melee_max));
            }

            //Add in melee weapon damage.

            //Remember how many weapons are being wielded that the creature has insufficient strength to wield properly.
            int weapons_under_strength=0;

            //Check for items wielded in either hand.
            for(int i=EQUIP_HOLD_RIGHT;i<EQUIP_HOLD_LEFT+1;i++){
                //If there is an item wielded in this hand.
                if(equipment[i]!=0){
                    //Determine the identifier for the item equipped in this slot.
                    int item_identifier=index_of_item_in_slot(i);

                    //Determine the base damage range for this item.
                    int weapon_damage_min=inventory[item_identifier].damage_min_melee;
                    int weapon_damage_max=inventory[item_identifier].damage_max_melee;
                    int weapon_damage=random_range(weapon_damage_min,weapon_damage_max);

                    //Does the creature have enough strength to wield this item?
                    bool enough_strength=true;

                    //If the creature does not have sufficient strength to wield this item.
                    if(inventory[item_identifier].weight*1.5>attributes[ATTRIBUTE_STRENGTH]){
                        weapons_under_strength++;
                        enough_strength=false;
                    }

                    //Apply the appropriate weapon skill, if any.

                    //If the item is governed by the bladed weapons skill.
                    if(inventory[item_identifier].category==ITEM_WEAPON && inventory[item_identifier].governing_skill_weapon==SKILL_BLADED_WEAPONS){
                        weapon_damage+=weapon_damage*(return_skill_bladed_weapons()/10.0);

                        if(enough_strength){
                            //Exercise the bladed weapons skill.
                            gain_skill_experience(SKILL_BLADED_WEAPONS,1);
                        }
                    }
                    //If the item is governed by the blunt weapons skill.
                    else if(inventory[item_identifier].category==ITEM_WEAPON && inventory[item_identifier].governing_skill_weapon==SKILL_BLUNT_WEAPONS){
                        weapon_damage+=weapon_damage*(return_skill_blunt_weapons()/10.0);

                        if(enough_strength){
                            //Exercise the blunt weapons skill.
                            gain_skill_experience(SKILL_BLUNT_WEAPONS,1);
                        }
                    }
                    //If the item is governed by the stabbing weapons skill.
                    else if(inventory[item_identifier].category==ITEM_WEAPON && inventory[item_identifier].governing_skill_weapon==SKILL_STABBING_WEAPONS){
                        weapon_damage+=weapon_damage*(return_skill_stabbing_weapons()/10.0);

                        if(enough_strength){
                            //Exercise the stabbing weapons skill.
                            gain_skill_experience(SKILL_STABBING_WEAPONS,1);
                        }
                    }

                    //Add the weapon's damage to the attack's damage.
                    damage+=weapon_damage;
                }
            }

            //If items are being dual-wielded.
            if(equipment[EQUIP_HOLD_RIGHT]!=0 && equipment[EQUIP_HOLD_LEFT]!=0){
                //Apply the dual-wielding modifier.

                if(return_skill_dual_wielding()<10){
                    damage-=50.0/return_skill_dual_wielding();
                }
                else{
                    damage+=return_skill_dual_wielding()/2.0;
                }

                //As long as both weapons are being wielded with enough strength.
                if(weapons_under_strength==0){
                    //Exercise the dual wielding skill.
                    gain_skill_experience(SKILL_DUAL_WIELDING,1);
                }
            }

            //If there are no items being wielded.
            else if(equipment[EQUIP_HOLD_RIGHT]==0 && equipment[EQUIP_HOLD_LEFT]==0){
                //Apply the unarmed weapon skill.
                damage+=base_damage*(return_skill_unarmed()/10.0);

                //Exercise the unarmed skill.
                gain_skill_experience(SKILL_UNARMED,1);
            }

            //Apply the strength bonus.
            damage+=damage*(return_attribute_strength()/4.0);

            //If the creature is wielding one or more weapons under strength, reduce its damage.
            if(weapons_under_strength==1){
                damage/=2.0;
            }
            else if(weapons_under_strength==2){
                damage/=4.0;
            }

            //We have finished determining the maximum damage the attacker can do.
            //Now, we determine the damage reduction based on the target's stats.

            int damage_reduction=determine_damage_reduction(target);

            //Subtract the total damage reduction from the maximum damage.
            damage-=damage_reduction;

            if(damage<0){
                damage=0;
            }

            //If the attacker succeeds in a critical strike.
            /**if(rc_attack_critical_strike(return_attribute_agility(),experience_level,target)){
                damage+=random_range(damage/4,damage/2);
            }*/
       /**}
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
        }*/
    /**}
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
    }*/

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
                int item_identifier=index_of_item_in_slot(EQUIP_HOLD_RIGHT);

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
                outcome+=inventory[item_identifier].return_full_name(1);
                outcome+="!";
            }
            //If there is an item being wielded in the left hand only.
            else if(equipment[EQUIP_HOLD_RIGHT]==0 && equipment[EQUIP_HOLD_LEFT]!=0){
                //Determine the identifier for the item equipped in this slot.
                int item_identifier=index_of_item_in_slot(EQUIP_HOLD_LEFT);

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
                outcome+=inventory[item_identifier].return_full_name(1);
                outcome+="!";
            }
            //If there is an item being wielded in both hands.
            else if(equipment[EQUIP_HOLD_RIGHT]!=0 && equipment[EQUIP_HOLD_LEFT]!=0){
                //Determine the identifier for the item equipped in each slot.
                int item_identifier_right=index_of_item_in_slot(EQUIP_HOLD_RIGHT);
                int item_identifier_left=index_of_item_in_slot(EQUIP_HOLD_LEFT);

                outcome="You attack the ";
                outcome+=target->return_full_name();
                outcome+=" with your ";
                outcome+=inventory[item_identifier_right].return_full_name(1);
                outcome+=" and your ";
                outcome+=inventory[item_identifier_left].return_full_name(1);
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
                int item_identifier=index_of_item_in_slot(EQUIP_HOLD_RIGHT);

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
                        outcome+=" whacks you ";
                    }
                }
                //If the item being wielded is not a weapon.
                else{
                    outcome+=" whacks you ";
                }

                outcome+="with its ";
                outcome+=inventory[item_identifier].return_full_name(1);
                outcome+="!";
            }
            //If there is an item being wielded in the left hand only.
            else if(equipment[EQUIP_HOLD_RIGHT]==0 && equipment[EQUIP_HOLD_LEFT]!=0){
                //Determine the identifier for the item equipped in this slot.
                int item_identifier=index_of_item_in_slot(EQUIP_HOLD_LEFT);

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
                        outcome+=" whacks you ";
                    }
                }
                //If the item being wielded is not a weapon.
                else{
                    outcome+=" whacks you ";
                }

                outcome+="with its ";
                outcome+=inventory[item_identifier].return_full_name(1);
                outcome+="!";
            }
            //If there is an item being wielded in both hands.
            else if(equipment[EQUIP_HOLD_RIGHT]!=0 && equipment[EQUIP_HOLD_LEFT]!=0){
                //Determine the identifier for the item equipped in each slot.
                int item_identifier_right=index_of_item_in_slot(EQUIP_HOLD_RIGHT);
                int item_identifier_left=index_of_item_in_slot(EQUIP_HOLD_LEFT);

                outcome="The ";
                outcome+=return_full_name();
                outcome+=" attacks you with its ";
                outcome+=inventory[item_identifier_right].return_full_name(1);
                outcome+=" and its ";
                outcome+=inventory[item_identifier_left].return_full_name(1);
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
    if(target->return_health()<=0){
        target->die(CAUSE_OF_DEATH_MELEE,return_full_name(),"",a_vs_an(this),"");
    }
}

void Creature::die(short cause_of_death,string killer,string killer_item,string killer_article,string killer_item_article){
    //The creature is now dead.
    alive=false;

    string message="";

    if(is_player){
        if(cause_of_death==CAUSE_OF_DEATH_THIRST){
            message="You succumb to thirst... Press [Spacebar] to continue.";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_LAVA){
            message="You melt into nothing... Press [Spacebar] to continue.";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_DROWN){
            message="You sink beneath the waves... Press [Spacebar] to continue.";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_BURNED_BY_LIGHT){
            message="You burn away into nothing in the blinding light... Press [Spacebar] to continue.";
        }
        else{
            message="You die... Press [Spacebar] to continue.";
        }

        player.save_game_log_entry(cause_of_death,killer,killer_item,killer_article,killer_item_article);

        player.current_window=WINDOW_DEATH_1;
    }
    else{
        if(cause_of_death==CAUSE_OF_DEATH_MELEE){
            message="You have slain the ";
            message+=return_full_name();
            message+="!";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_THROWN){
            message="Your ";
            message+=killer_item;
            message+=" kills the ";
            message+=return_full_name();
            message+="!";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_RANGED){
            message="Your ";
            message+=killer_item;
            message+=" kills the ";
            message+=return_full_name();
            message+="!";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_THIRST){
            message="The ";
            message+=return_full_name();
            message+=" dies of thirst!";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_LAVA){
            message="The ";
            message+=return_full_name();
            message+=" is incinerated by the lava!";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_DROWN){
            message="The ";
            message+=return_full_name();
            message+=" drowns!";
        }
        else if(cause_of_death==CAUSE_OF_DEATH_BURNED_BY_LIGHT){
            message="The ";
            message+=return_full_name();
            message+=" burns up in the light!";
        }
    }

    update_text_log(message.c_str(),true);
}
