/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef creature_h
#define creature_h

#include <vector>

#include "fov.h"
#include "light_data.h"
#include "object.h"
#include "item.h"
#include "enumerations.h"

//Holds data used in determing inventory matches.
struct inventory_match{
    bool match_found;
    int inventory_slot;
};

class Creature: public Object{
    private:
    public:

    //Current movement state of the creature.
    short move_state;

    //When non-zero, the game is waiting for directional input for some command.
    short input_directional;

    //When non-zero, the game is waiting for inventory input for some command.
    short input_inventory;

    //Kind of like a directional key for input_direction, but for inventory letters instead.
    char inventory_input_state;

    //Used to store the inventory item being affected by a directional command.
    char directional_inventory_input_state;

    //A standard command to be executed on the next call to move().
    short command_standard;

    //If true, the creature has given some input that should initiate a call to movement().
    //Only used by the Player class.
    bool initiate_move;

    //If true, this creature is the player.
    //If false, this creature is not the player.
    bool is_player;

    bool alive;

    short gender;

    //*****************//
    // Game variables: //
    //*****************//

    short race;
    std::string race_name;

    int health;
    int health_max;

    int mana;
    int mana_max;

    int experience_level;

    int experience;
    //Experience needed for next level.
    int experience_max;

    //The current thirst level of the creature.
    //Lower numbers are better.
    short thirst;

    //The amount of damage the creature does with its bare hands with no bonuses from anything.
    //Determined by race.
    short base_damage_melee_min;
    short base_damage_melee_max;

    short base_damage_ranged_min;
    short base_damage_ranged_max;

    short base_damage_thrown_min;
    short base_damage_thrown_max;

    //The creature's base attribute values.
    short attributes[ATTRIBUTE_LUCK+1];

    //The creature's base skill values.
    short skills[SKILL_MAGIC_SUMMONING+1];

    //The base speed of this creature, used for the turns system.
    //Lower numbers are faster.
    ///I am thinking about race determining the base value of this.
    unsigned short movement_speed;

    //The base number of "turn ticks" until the next move for this creature.
    short next_move;

    //The creature's inventory.
    std::vector<Item> inventory;

    //The creature's equipped items.
    std::vector<char> equipment;

    //Field of view variables:

    fov_direction_type facing;
    ///This can probably be replaced by something global.
    fov_settings_type fov_settings;
    ///This can probably be replaced by something global.
    light_data source_data;

    //A list of available inventory letters.
    std::vector<char> inventory_letters;

    Creature();

    void create_money_item();

    void create_light_item();

    void create_water_bottle();

    //Assign an identifier to the creature.
    void assign_identifier();

    //Return the creature's identifier.
    void return_identifier();

    //Determines which item is equipped in an equipment slot.
    int index_of_item_in_slot(short equip_slot);

    //Determines which equipment slot an item is equipped in.
    short item_equipped_in_which_slot(int item_identifier);

    //Unequips the item with index item_identifier.
    void unequip_item(int item_identifier);

    //Equips the item with index item_identifier.
    //If equip_slot is not -1, that slot will be filled.
    //If equip_slot is -1, the armor slot corresponding with the item indexed by item_identifier will be filled.
    void equip_item(int item_identifier,short equip_slot);

    //Determines if the passed equipment slot is currently empty.
    //Returns true if the slot is empty and available.
    //Returns false if the slot is already occupied.
    //If equip_slot is not -1, that slot will be checked.
    //If equip_slot is -1, the armor slot corresponding with the item indexed by item_identifier will be checked.
    bool equipment_slot_empty(int item_identifier,short equip_slot);

    //Returns the number of items in the passed item category currently in the creature's inventory.
    int item_category_in_inventory(short category);

    //Checks for a match with the passed item.
    //Returns true if a match was found.
    //Returns false is a match was not found.
    inventory_match check_for_inventory_match(Item* item_to_check);

    //Checks the current availability of the passed inventory letter.
    //Returns true if the letter is available.
    //Returns false if the letter is unavailable.
    bool check_inventory_letter_availability(char letter_to_check);

    //Assigns an inventory letter to an item.
    //If letter_remove is non-zero, the letter stored in letter_remove is used.
    //Otherwise, the next letter in the list is used.
    char assign_inventory_letter(char letter_remove=0);

    //Returns an inventory letter to the list.
    void return_inventory_letter(char returning_letter);

    //Check to see if a command can be done.
    //If so, queue it for the next call to move().
    //If not, handle it appropriately.
    void check_command(short command);

    //Execute a standard (non-directional) command.
    void execute_command(short command);

    //Holds the coordinates of a tile to be checked.
    struct directional_check_coordinates{
        short check_x;
        short check_y;
    };

    //Determine what tile to move to, given a direction.
    directional_check_coordinates determine_direction(short direction);

    //Determine the momentum of an item.
    int determine_momentum(double item_weight,bool fired);

    //Check to see if a directional command can be done.
    //If so, queue it for the next call to move().
    //If not, handle it appropriately.
    void check_command_directional(short direction);

    //Execute a direction command.
    void execute_command_directional(short direction);

    //Check to see if an inventory command can be done.
    //If so, queue it for the next call to move().
    //If not, handle it appropriately.
    void check_command_inventory(char inventory_letter);

    //Execute an inventory command.
    void execute_command_inventory(char inventory_letter);

    void attack_melee(Creature* target);

    void die(short cause_of_death,std::string killer,std::string killer_item);

    //Level up the creature.
    void level_up();

    //Gain experience points from killing a creature.
    void gain_experience(int killed_experience_level);

    //Evaluate the tile the creature is trying to move to.
    //Returns true if the creature can move, false if the creature can not move.
    bool check_movement(short check_x,short check_y);

    void execute_movement(short check_x,short check_y);

    void search();

    //Handle anything that needs handling on each turn.
    void process_turn();

    //Increase or decrease thirst, and then handle thirst states.
    void change_thirst(bool increase,short amount);

    std::string return_thirst_state();

    //Apply a race's benefits and negatives to the creature.
    void apply_race(short race_to_apply);

    //Remove a race's benefits and negatives from the creature.
    void remove_race(short race_to_remove);

    //Returns the full name of the creature, which is:
    //race_name + " named " + name
    std::string return_full_name();

    //
    unsigned short return_movement_speed();

    //
    short return_next_move();

    //Each of these functions returns the current TRUE attribute value (with all relevant bonuses, penalties, etc.) for the corresponding attribute.
    short return_attribute_strength();
    short return_attribute_agility();
    short return_attribute_hardiness();
    short return_attribute_comprehension();
    short return_attribute_acumen();
    short return_attribute_luck();

    //Each of these functions returns the current TRUE skill value (with all relevant bonuses, penalties, etc.) for the corresponding skill.
    short return_skill_bladed_weapons();
    short return_skill_blunt_weapons();
    short return_skill_stabbing_weapons();
    short return_skill_unarmed();
    short return_skill_security();
    short return_skill_stealth();
    short return_skill_launcher_weapons();
    short return_skill_thrown_weapons();
    short return_skill_dual_wielding();
    short return_skill_speed();
    short return_skill_fighting();
    short return_skill_dodging();
    short return_skill_armor();
    short return_skill_magic_air();
    short return_skill_magic_fire();
    short return_skill_magic_water();
    short return_skill_magic_cold();
    short return_skill_magic_earth();
    short return_skill_magic_conjuration();
    short return_skill_magic_enchantment();
    short return_skill_magic_summoning();
};

#endif
