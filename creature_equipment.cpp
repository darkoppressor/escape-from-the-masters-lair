/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "creature.h"

using namespace std;

int Creature::slot_equipped_with_what_item(char item_inventory_letter){
    int item_identifier=-1;

    //Look through all of the items in the inventory.
    for(int i=0;i<inventory.size();i++){
        //If this item has the searched for inventory letter.
        if(inventory[i].inventory_letter==item_inventory_letter){
            item_identifier=i;
            break;
        }
    }

    return item_identifier;
}

short Creature::item_equipped_in_which_slot(int item_identifier){
    short equip_slot=-1;

    //Look through all of the equipment slots.
    for(int i=EQUIP_HEAD;i<EQUIP_HOLD_LEFT+1;i++){
        //If this equipment slot is where the item is equipped.
        if(equipment[i]==inventory[item_identifier].inventory_letter){
            equip_slot=i;
            break;
        }
    }

    return equip_slot;
}

void Creature::unequip_item(int item_identifier){
    //Look through all of the equipment slots.
    for(int i=0;i<equipment.size();i++){
        //If this equipment slot is where the item is equipped.
        if(equipment[i]==inventory[item_identifier].inventory_letter){
            //Remove the item from this slot.
            equipment[i]=0;
        }
    }

    //This item is no longer equipped.
    inventory[item_identifier].equipped=false;
}

void Creature::equip_item(int item_identifier){
    //If we are equipping anything that isn't armor.
    if(inventory[item_identifier].category!=ITEM_ARMOR){
        //If the right hand is free.
        if(equipment[EQUIP_HOLD_RIGHT]==0){
            //Assign the item to this slot.
            equipment[EQUIP_HOLD_RIGHT]=inventory[item_identifier].inventory_letter;
        }
        //If the left hand is free.
        else if(equipment[EQUIP_HOLD_LEFT]==0){
            //Assign the item to this slot.
            equipment[EQUIP_HOLD_LEFT]=inventory[item_identifier].inventory_letter;
        }
    }

    //If we are equipping some armor.
    else{
        //If we are equipping a finger item.
        if(inventory[item_identifier].armor_category==ARMOR_FINGER){
            //If the right finger is free.
            if(equipment[EQUIP_FINGER_RIGHT]==0){
                //Assign the item to this slot.
                equipment[EQUIP_FINGER_RIGHT]=inventory[item_identifier].inventory_letter;
            }
            //If the left finger is free.
            else if(equipment[EQUIP_FINGER_LEFT]==0){
                //Assign the item to this slot.
                equipment[EQUIP_FINGER_LEFT]=inventory[item_identifier].inventory_letter;
            }
        }
        //If we are equipping a regular piece of armor.
        else{
            switch(inventory[item_identifier].armor_category){
            case ARMOR_HEAD:
                //Assign the item to this slot.
                equipment[EQUIP_HEAD]=inventory[item_identifier].inventory_letter;
                break;

            case ARMOR_SHOULDER:
                //Assign the item to this slot.
                equipment[EQUIP_SHOULDER]=inventory[item_identifier].inventory_letter;
                break;

            case ARMOR_CHEST:
                //Assign the item to this slot.
                equipment[EQUIP_CHEST]=inventory[item_identifier].inventory_letter;
                break;

            case ARMOR_BACK:
                //Assign the item to this slot.
                equipment[EQUIP_BACK]=inventory[item_identifier].inventory_letter;
                break;

            case ARMOR_WAIST:
                //Assign the item to this slot.
                equipment[EQUIP_WAIST]=inventory[item_identifier].inventory_letter;
                break;

            case ARMOR_LEG:
                //Assign the item to this slot.
                equipment[EQUIP_LEG]=inventory[item_identifier].inventory_letter;
                break;

            case ARMOR_FOOT:
                //Assign the item to this slot.
                equipment[EQUIP_FOOT]=inventory[item_identifier].inventory_letter;
                break;

            case ARMOR_HAND:
                //Assign the item to this slot.
                equipment[EQUIP_HAND]=inventory[item_identifier].inventory_letter;
                break;

            case ARMOR_SHIELD:
                //Assign the item to this slot.
                equipment[EQUIP_SHIELD]=inventory[item_identifier].inventory_letter;
                break;

            case ARMOR_NECK:
                //Assign the item to this slot.
                equipment[EQUIP_NECK]=inventory[item_identifier].inventory_letter;
                break;

            case ARMOR_WRIST:
                //Assign the item to this slot.
                equipment[EQUIP_WRIST]=inventory[item_identifier].inventory_letter;
                break;

            case ARMOR_SHIRT:
                //Assign the item to this slot.
                equipment[EQUIP_SHIRT]=inventory[item_identifier].inventory_letter;
                break;
            }
        }
    }

    //This item is now equipped.
    inventory[item_identifier].equipped=true;
}

bool Creature::equipment_slot_empty(short item_category,short equip_category){
    //If we are checking anything that isn't armor.
    if(item_category!=ITEM_ARMOR){
        ///Weapons needing 1 or 2 hands will be handled here.
        ///For now, all weapons need one slot.
        //If one of the hold slots is empty.
        if(equipment[EQUIP_HOLD_RIGHT]==0 || equipment[EQUIP_HOLD_LEFT]==0){
            return true;
        }
    }

    //If we are checking some armor.
    else{
        //If the armor needs a finger slot.
        if(equip_category==ARMOR_FINGER){
            //If one of the finger slots is empty.
            if(equipment[EQUIP_FINGER_RIGHT]==0 || equipment[EQUIP_FINGER_LEFT]==0){
                return true;
            }
        }
        //If the armor needs a regular corresponding slot.
        else{
            switch(equip_category){
            case ARMOR_HEAD:
                if(equipment[EQUIP_HEAD]==0){
                    return true;
                }
                break;

            case ARMOR_SHOULDER:
                if(equipment[EQUIP_SHOULDER]==0){
                    return true;
                }
                break;

            case ARMOR_CHEST:
                if(equipment[EQUIP_CHEST]==0){
                    return true;
                }
                break;

            case ARMOR_BACK:
                if(equipment[EQUIP_BACK]==0){
                    return true;
                }
                break;

            case ARMOR_WAIST:
                if(equipment[EQUIP_WAIST]==0){
                    return true;
                }
                break;

            case ARMOR_LEG:
                if(equipment[EQUIP_LEG]==0){
                    return true;
                }
                break;

            case ARMOR_FOOT:
                if(equipment[EQUIP_FOOT]==0){
                    return true;
                }
                break;

            case ARMOR_HAND:
                if(equipment[EQUIP_HAND]==0){
                    return true;
                }
                break;

            case ARMOR_SHIELD:
                if(equipment[EQUIP_SHIELD]==0){
                    return true;
                }
                break;

            case ARMOR_NECK:
                if(equipment[EQUIP_NECK]==0){
                    return true;
                }
                break;

            case ARMOR_WRIST:
                if(equipment[EQUIP_WRIST]==0){
                    return true;
                }
                break;

            case ARMOR_SHIRT:
                if(equipment[EQUIP_SHIRT]==0){
                    return true;
                }
                break;
            }
        }
    }

    return false;
}
