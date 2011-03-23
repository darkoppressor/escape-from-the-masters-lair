/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef item_h
#define item_h

#include "object.h"
#include "light_data.h"
#include "effect.h"
#include "fov.h"

#include <vector>
#include <sstream>

//Incomplete declaration of Creature.
//We have to do this here, because a Creature object is used by a function in Item, but Creature relies on Item.
class Creature;

//All of the special data needed by an Item after being thrown by a Creature.
struct Owner_Data_Thrown{
    short strength;
    short agility;
    short fighting_skill;
    short thrown_weapons_skill;
    short experience_level;
    short base_damage_min_thrown;
    short base_damage_max_thrown;
    std::string full_name;
    bool is_player;
};

//All of the special data needed by an Item after being fired by a Creature.
struct Owner_Data_Fired{
    short strength;
    short agility;
    short fighting_skill;
    short launcher_weapons_skill;
    short experience_level;
    short base_damage_min_ranged;
    short base_damage_max_ranged;
    std::string full_name;
    bool is_player;
};

class Item: public Object{
    private:
    public:

    //*********************************************//
    // Variables that do NOT come from a template: //
    //*********************************************//

    //The identifier of the item's owner.
    uint32_t owner;

    //Used to store special data for when the item has been thrown by a creature.
    std::vector<Owner_Data_Thrown> owner_data_thrown;

    //Used to store special data for when the item has been fired by a creature.
    std::vector<Owner_Data_Fired> owner_data_fired;

    //Is the light currently on.
    bool light_on;
    light_data source_data;
    fov_direction_type facing;

    //The letter for inventory interaction.
    char inventory_letter;

    //If true, item is currently equipped by its owner.
    //If false, item is not equipped.
    bool equipped;

    //The race the item belonged to.
    //Currently, this just applies to corpses.
    short race;

    //The color of the dye applied to the item.
    //A value of 0 means no dye.
    short dye;

    ///Not sure if this will be used or not, yet.
    //Special effects (both positive and negative) conferred by the item to its owner.
    ///std::vector<Effect> effects;

    //The direction the item is moving.
    short move_direction;

    //The current momentum of the item.
    //This is the maximum number of tiles the item can move.
    short momentum;

    //The stacked number of this item.
    int stack;

    //*****************************************//
    // Variables that DO come from a template: //
    //*****************************************//

    //If true, this item can stack.
    //If false, this item cannot stack.
    bool stackable;

    //If true, the item can be spawned in levels, inventories, etc.
    //If false, the item will never be spawned in the game in the normal ways.
    bool spawnable;

    //The writing on the item, if any.
    std::string writing;

    //The name of the item if stack>1.
    std::string plural_name;

    //The item's value in terms of money.
    short monetary_value;

    //The item category to which the item belongs.
    short category;

    //The material the item is composed of.
    short material;

    //Base damage range when wielded as a melee weapon.
    short damage_min_melee;
    short damage_max_melee;

    //Base damage range as a projectile.
    short damage_min_thrown;
    short damage_max_thrown;

    //The base damage range added to a missile when fired from this item.
    short damage_min_ranged;
    short damage_max_ranged;

    //Field of view variables:

    //The radius of the light.
    //If 0, the item does not give off light.
    unsigned int fov_radius;

    //If true, the light is a beam.
    //If false, the light is not a beam.
    bool beam;

    //The width of the beam.
    float fov_angle;

    //The item's special effect(s).
    std::vector<short> effects;

    //Weapon-specific//

    //The skill that this weapon exercises and uses for damage bonuses, etc.
    short governing_skill_weapon;

    //The weapon category to which the weapon belongs.
    short weapon_category;

    //Armor-specific//

    //The armor category to which the armor belongs.
    short armor_category;

    //The base amount of damage absorbed by the armor.
    short defense;

    //Food-specific//

    //If true, this food is a corpse.
    //If false, this food is not a corpse.
    bool is_corpse;

    //Drink-specific//

    //The amount of thirst quenched by quaffing this drink.
    short thirst_quenched;

    //Scroll-specific//

    //Book-specific//

    //Container-specific//

    //Other-specific//

    //If this item is a light source, this is its fuel supply.
    short fuel;

    //The maximum amount of fuel this item can hold.
    short fuel_max;

    Item();

    //Setup any starting stuff for the item.
    void setup();

    //Returns a pointer to the item's owner.
    //If a NULL pointer is returned, no owner was found.
    ///Creature* determine_owner_address();

    //Assign an identifier to the item.
    void assign_identifier();

    //Return the item's identifier.
    void return_identifier();

    //Evaluate the tile the item is trying to move to.
    //Returns true if the item can move, false if the item can not move.
    bool check_movement(short check_x,short check_y);

    void execute_movement(short check_x,short check_y);

    //Clear all of the special data vectors.
    void clear_owner_data_all();

    //Assign special thrown data to the item.
    void assign_owner_data_thrown(Creature* creature);

    //Assign special fired data to the item.
    void assign_owner_data_fired(Creature* creature);

    //Initiate an attack.
    void attack(Creature* target);

    void attack_thrown(Creature* target);

    void attack_fired(Creature* target);

    void move();

    //Handle anything that needs handling on each game turn.
    void process_turn();

    std::string return_full_name(int override_stack=-1);

    //Returns true if the passed effect is possessed by the item.
    //Returns false if the passed effect is not possessed by the item.
    bool possesses_effect(short effect);

    void render(std::vector< std::vector<bool> >* tile_rendered);

    //Returns a stringstream containing all of this item's save data.
    std::string return_save_data();

    //Load data from a save file.
    void load_data(std::stringstream* load);
};

#endif
