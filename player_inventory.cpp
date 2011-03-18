/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "player.h"
#include "world.h"
#include "render.h"

using namespace std;

void Player::render_inventory(){
    if(display_inventory){
        msg="";
        short render_color=COLOR_WHITE;

        //Keeps track of the number of lines of inventory text rendered.
        int lines_rendered=0;

        render_rectangle(0,0,800,600,1.0,COLOR_BLACK);

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

                render_rectangle(5,7+22*lines_rendered,11*title.length()+3,22,1.0,COLOR_GRAY);
                font.show(5,10+22*lines_rendered++,title,COLOR_BLACK);

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

                        string str_item_equipped="";

                        //If the item is equipped.
                        if(inventory[i].equipped){
                            //Determine what slot the item is equipped in.
                            short equip_slot=item_equipped_in_which_slot(i);

                            str_item_equipped=" (";

                            //If the item is a stack larger than 1 and the equipment slot is a hold slot or the light source slot.
                            if(inventory[i].stack>1 && (equip_slot==EQUIP_HOLD_RIGHT || equip_slot==EQUIP_HOLD_LEFT || equip_slot==EQUIP_LIGHT_SOURCE)){
                                str_item_equipped+="one ";
                            }

                            switch(equip_slot){
                            case EQUIP_HOLD_RIGHT:
                                str_item_equipped+="being wielded in right hand)";
                                break;

                            case EQUIP_HOLD_LEFT:
                                str_item_equipped+="being wielded in left hand)";
                                break;

                            case EQUIP_QUIVER:
                                str_item_equipped+="in quiver)";
                                break;

                            case EQUIP_LIGHT_SOURCE:
                                str_item_equipped+="fastened to belt)";
                                break;

                            case EQUIP_HEAD:
                                str_item_equipped+="being worn on head)";
                                break;

                            case EQUIP_SHOULDER:
                                str_item_equipped+="being worn on shoulders)";
                                break;

                            case EQUIP_CHEST:
                                str_item_equipped+="being worn on chest)";
                                break;

                            case EQUIP_BACK:
                                str_item_equipped+="being worn on back)";
                                break;

                            case EQUIP_WAIST:
                                str_item_equipped+="being worn on waist)";
                                break;

                            case EQUIP_LEG:
                                str_item_equipped+="being worn on legs)";
                                break;

                            case EQUIP_FOOT:
                                str_item_equipped+="being worn on feet)";
                                break;

                            case EQUIP_HAND:
                                str_item_equipped+="being worn on hands)";
                                break;

                            case EQUIP_SHIELD:
                                str_item_equipped+="being worn on arm)";
                                break;

                            case EQUIP_NECK:
                                str_item_equipped+="being worn around neck)";
                                break;

                            case EQUIP_WRIST:
                                str_item_equipped+="being worn on wrists)";
                                break;

                            case EQUIP_SHIRT:
                                str_item_equipped+="being worn on chest as a shirt)";
                                break;

                            case EQUIP_FINGER_RIGHT:
                                str_item_equipped+="being worn on right ring finger)";
                                break;

                            case EQUIP_FINGER_LEFT:
                                str_item_equipped+="being worn on left ring finger)";
                                break;
                            }
                        }

                        ss.clear();ss.str("");ss<<" ";ss<<inventory[i].inventory_letter;ss<<" - ";ss<<item_amount_prefix;ss<<inventory[i].return_full_name();ss<<str_item_equipped;ss<<"\xA";msg=ss.str();

                        font.show(5,10+22*lines_rendered++,msg,render_color);
                    }
                }
            }
        }
    }
}
