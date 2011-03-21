/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "player.h"
#include "world.h"
#include "render.h"

using namespace std;

void Player::render_stats(){
    if(display_stats){
        string title="";

        render_rectangle(0,0,800,600,1.0,COLOR_BLACK);

        title=name;
        render_rectangle(5,5,11*title.length()+3,22,1.0,COLOR_GRAY);
        font.show(5,8,title,COLOR_BLACK);

        ss.clear();ss.str("");ss<<"Experience Level: ";ss<<experience_level;ss<<"\xA";msg=ss.str();
        ss.clear();ss.str("");ss<<"Experience: ";ss<<experience;ss<<"/";ss<<experience_max;ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Health: ";ss<<return_health();ss<<"/";ss<<return_health_max();ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Mana: ";ss<<return_mana();ss<<"/";ss<<return_mana_max();ss<<"\xA";msg+=ss.str();

        font.show(5,30,msg,COLOR_WHITE);

        title="Attributes";
        render_rectangle(5,200,11*title.length()+3,22,1.0,COLOR_GRAY);
        font.show(5,203,title,COLOR_BLACK);

        ss.clear();ss.str("");ss<<"Strength - ";ss<<return_attribute_strength();ss<<"\xA";msg=ss.str();
        ss.clear();ss.str("");ss<<"Agility - ";ss<<return_attribute_agility();ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Hardiness - ";ss<<return_attribute_hardiness();ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Comprehension - ";ss<<return_attribute_comprehension();ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Acumen - ";ss<<return_attribute_acumen();ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Luck - ";ss<<return_attribute_luck();ss<<"\xA";msg+=ss.str();

        font.show(5,225,msg,COLOR_WHITE);

        title="Skills";
        render_rectangle(275,5,11*title.length()+3,22,1.0,COLOR_GRAY);
        font.show(275,8,title,COLOR_BLACK);

        ss.clear();ss.str("");ss<<"Bladed Weapons - ";ss<<return_skill_bladed_weapons();ss<<" (";ss<<skills[SKILL_BLADED_WEAPONS][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_BLADED_WEAPONS][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg=ss.str();
        ss.clear();ss.str("");ss<<"Blunt Weapons - ";ss<<return_skill_blunt_weapons();ss<<" (";ss<<skills[SKILL_BLUNT_WEAPONS][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_BLUNT_WEAPONS][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Stabbing Weapons - ";ss<<return_skill_stabbing_weapons();ss<<" (";ss<<skills[SKILL_STABBING_WEAPONS][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_STABBING_WEAPONS][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Unarmed - ";ss<<return_skill_unarmed();ss<<" (";ss<<skills[SKILL_UNARMED][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_UNARMED][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();

        ss.clear();ss.str("");ss<<"Security - ";ss<<return_skill_security();ss<<" (";ss<<skills[SKILL_SECURITY][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_SECURITY][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Stealth - ";ss<<return_skill_stealth();ss<<" (";ss<<skills[SKILL_STEALTH][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_STEALTH][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Launcher Weapons - ";ss<<return_skill_launcher_weapons();ss<<" (";ss<<skills[SKILL_LAUNCHER_WEAPONS][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_LAUNCHER_WEAPONS][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Thrown Weapons - ";ss<<return_skill_thrown_weapons();ss<<" (";ss<<skills[SKILL_THROWN_WEAPONS][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_THROWN_WEAPONS][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Dual Wielding - ";ss<<return_skill_dual_wielding();ss<<" (";ss<<skills[SKILL_DUAL_WIELDING][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_DUAL_WIELDING][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Speed - ";ss<<return_skill_speed();ss<<" (";ss<<skills[SKILL_SPEED][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_SPEED][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Fighting - ";ss<<return_skill_fighting();ss<<" (";ss<<skills[SKILL_FIGHTING][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_FIGHTING][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Dodging - ";ss<<return_skill_dodging();ss<<" (";ss<<skills[SKILL_DODGING][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_DODGING][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();

        ss.clear();ss.str("");ss<<"Armor - ";ss<<return_skill_armor();ss<<" (";ss<<skills[SKILL_ARMOR][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_ARMOR][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();

        ss.clear();ss.str("");ss<<"Air Magic - ";ss<<return_skill_magic_air();ss<<" (";ss<<skills[SKILL_MAGIC_AIR][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_MAGIC_AIR][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Fire Magic - ";ss<<return_skill_magic_fire();ss<<" (";ss<<skills[SKILL_MAGIC_FIRE][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_MAGIC_FIRE][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Water Magic - ";ss<<return_skill_magic_water();ss<<" (";ss<<skills[SKILL_MAGIC_WATER][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_MAGIC_WATER][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Cold Magic - ";ss<<return_skill_magic_cold();ss<<" (";ss<<skills[SKILL_MAGIC_COLD][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_MAGIC_COLD][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();

        ss.clear();ss.str("");ss<<"Earth Magic - ";ss<<return_skill_magic_earth();ss<<" (";ss<<skills[SKILL_MAGIC_EARTH][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_MAGIC_EARTH][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Conjuration Magic - ";ss<<return_skill_magic_conjuration();ss<<" (";ss<<skills[SKILL_MAGIC_CONJURATION][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_MAGIC_CONJURATION][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Enchantment Magic - ";ss<<return_skill_magic_enchantment();ss<<" (";ss<<skills[SKILL_MAGIC_ENCHANTMENT][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_MAGIC_ENCHANTMENT][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
        ss.clear();ss.str("");ss<<"Summoning Magic - ";ss<<return_skill_magic_summoning();ss<<" (";ss<<skills[SKILL_MAGIC_SUMMONING][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_MAGIC_SUMMONING][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();

        font.show(275,30,msg,COLOR_WHITE);
    }
}

void Player::render_inventory(){
    if(display_inventory){
        msg="";
        short render_color=COLOR_WHITE;

        //Keeps track of the number of lines of inventory text rendered.
        int lines_rendered=0;

        render_rectangle(0,0,800,600,1.0,COLOR_BLACK);

        ss.clear();ss.str("");ss<<"Weight/Capacity: ";ss<<return_inventory_weight();ss<<"/";ss<<return_carry_capacity();msg=ss.str();
        font.show((main_window.SCREEN_WIDTH-msg.length()*11)/2,0,msg,render_color);

        ss.clear();ss.str("");ss<<"You are currently ";ss<<return_encumbrance_state();ss<<".";msg=ss.str();
        font.show((main_window.SCREEN_WIDTH-msg.length()*11)/2,22,msg,render_color);

        for(short n=ITEM_WEAPON;n<ITEM_OTHER+1;n++){
            if(item_category_in_inventory(n)>0){
                string title="";

                if(n==ITEM_WEAPON){
                    title="Weapons";
                }
                else if(n==ITEM_ARMOR){
                    title="Armor";
                }
                else if(n==ITEM_FOOD){
                    title="Food";
                }
                else if(n==ITEM_DRINK){
                    title="Drinks";
                }
                else if(n==ITEM_SCROLL){
                    title="Scrolls";
                }
                else if(n==ITEM_BOOK){
                    title="Books";
                }
                else if(n==ITEM_CONTAINER){
                    title="Containers";
                }
                else if(n==ITEM_OTHER){
                    title="Others";
                }

                render_rectangle(5,44+22*lines_rendered,11*title.length()+3,22,1.0,COLOR_GRAY);
                font.show(5,47+22*lines_rendered++,title,COLOR_BLACK);

                for(int i=0;i<inventory.size();i++){
                    if(inventory[i].category==n && inventory[i].inventory_letter!='$'){
                        render_color=inventory[i].color;

                        //If the item is dyed.
                        if(inventory[i].dye!=0){
                            render_color=inventory[i].dye;
                        }

                        string item_amount_prefix="";

                        if(inventory[i].stack==1){
                            item_amount_prefix="a ";
                        }

                        string str_item="";

                        //If the item is equipped.
                        if(inventory[i].equipped){
                            //Determine what slot the item is equipped in.
                            short equip_slot=item_equipped_in_which_slot(i);

                            str_item=" (";

                            //If the item is a stack larger than 1 and the equipment slot is a hold slot or the light source slot.
                            if(inventory[i].stack>1 && (equip_slot==EQUIP_HOLD_RIGHT || equip_slot==EQUIP_HOLD_LEFT)){
                                str_item+="one ";
                            }

                            switch(equip_slot){
                            case EQUIP_HOLD_RIGHT:
                                str_item+="being wielded in right hand)";
                                break;

                            case EQUIP_HOLD_LEFT:
                                str_item+="being wielded in left hand)";
                                break;

                            case EQUIP_QUIVER:
                                str_item+="in quiver)";
                                break;

                            case EQUIP_HEAD:
                                str_item+="being worn on head)";
                                break;

                            case EQUIP_SHOULDER:
                                str_item+="being worn on shoulders)";
                                break;

                            case EQUIP_CHEST:
                                str_item+="being worn on chest)";
                                break;

                            case EQUIP_BACK:
                                str_item+="being worn on back)";
                                break;

                            case EQUIP_WAIST:
                                str_item+="being worn on waist)";
                                break;

                            case EQUIP_LEG:
                                str_item+="being worn on legs)";
                                break;

                            case EQUIP_FOOT:
                                str_item+="being worn on feet)";
                                break;

                            case EQUIP_HAND:
                                str_item+="being worn on hands)";
                                break;

                            case EQUIP_SHIELD:
                                str_item+="being worn on arm)";
                                break;

                            case EQUIP_NECK:
                                str_item+="being worn around neck)";
                                break;

                            case EQUIP_WRIST:
                                str_item+="being worn on wrists)";
                                break;

                            case EQUIP_SHIRT:
                                str_item+="being worn on chest as a shirt)";
                                break;

                            case EQUIP_FINGER_RIGHT:
                                str_item+="being worn on right ring finger)";
                                break;

                            case EQUIP_FINGER_LEFT:
                                str_item+="being worn on left ring finger)";
                                break;
                            }
                        }

                        //If the item has fuel.
                        if(inventory[i].fuel_max>0){
                            if(inventory[i].light_on){
                                str_item=" (lit) (";
                            }
                            else{
                                str_item=" (";
                            }

                            if(inventory[i].fuel>0){
                                ss.clear();ss.str("");ss<<inventory[i].fuel;ss<<"/";ss<<inventory[i].fuel_max;str_item+=ss.str();
                                str_item+=" fuel)";
                            }
                            else{
                                str_item+="out of fuel)";
                            }
                        }

                        ss.clear();ss.str("");ss<<" ";ss<<inventory[i].inventory_letter;ss<<" - ";ss<<item_amount_prefix;ss<<inventory[i].return_full_name();ss<<str_item;ss<<"\xA";msg=ss.str();

                        font.show(5,47+22*lines_rendered++,msg,render_color);
                    }
                }
            }
        }
    }
}
