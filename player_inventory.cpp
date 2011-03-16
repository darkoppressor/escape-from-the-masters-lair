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
                            //If the item is anything other than armor.
                            if(inventory[i].category!=ITEM_ARMOR){
                                //The item is being wielded as a weapon.
                                str_item_equipped=" (being wielded ";

                                //If wielded in right hand.
                                if(item_equipped_in_which_slot(i)==EQUIP_HOLD_RIGHT){
                                    str_item_equipped+="in right hand)";
                                }
                                //If wielded in left hand.
                                else if(item_equipped_in_which_slot(i)==EQUIP_HOLD_LEFT){
                                    str_item_equipped+="in left hand)";
                                }
                            }
                            //If the item is armor.
                            else{
                                //The item is being worn as armor.
                                str_item_equipped=" (being worn ";

                                switch(inventory[i].armor_category){
                                case ARMOR_HEAD:
                                    str_item_equipped+="on head)";
                                    break;

                                case ARMOR_SHOULDER:
                                    str_item_equipped+="on shoulders)";
                                    break;

                                case ARMOR_CHEST:
                                    str_item_equipped+="on chest)";
                                    break;

                                case ARMOR_BACK:
                                    str_item_equipped+="on back)";
                                    break;

                                case ARMOR_WAIST:
                                    str_item_equipped+="on waist)";
                                    break;

                                case ARMOR_LEG:
                                    str_item_equipped+="on legs)";
                                    break;

                                case ARMOR_FOOT:
                                    str_item_equipped+="on feet)";
                                    break;

                                case ARMOR_HAND:
                                    str_item_equipped+="on hands)";
                                    break;

                                case ARMOR_SHIELD:
                                    str_item_equipped+="on arm)";
                                    break;

                                case ARMOR_NECK:
                                    str_item_equipped+="around neck)";
                                    break;

                                case ARMOR_WRIST:
                                    str_item_equipped+="on wrists)";
                                    break;

                                case ARMOR_SHIRT:
                                    str_item_equipped+="on chest as a shirt)";
                                    break;

                                case ARMOR_FINGER:
                                    //If worn on right finger.
                                    if(item_equipped_in_which_slot(i)==EQUIP_FINGER_RIGHT){
                                        str_item_equipped+="on right ring finger)";
                                    }
                                    //If worn on left finger.
                                    else if(item_equipped_in_which_slot(i)==EQUIP_FINGER_LEFT){
                                        str_item_equipped+="on left ring finger)";
                                    }
                                    break;
                                }
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
