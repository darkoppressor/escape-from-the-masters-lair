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

    Creature();

    //Determines which item is equipped in an equipment slot.
    int slot_equipped_with_what_item(char item_inventory_letter);

    //Determines which equipment slot an item is equipped in.
    short item_equipped_in_which_slot(int item_identifier);

    //Unequips the item with index item_identifier.
    void unequip_item(int item_identifier);

    //Equips the item with index item_identifier.
    void equip_item(int item_identifier);

    //Determines if the equipment slot that the passed item could fill is currently empty.
    //Returns true if the slot is empty and available.
    //Returns false if the slot is already occupied.
    bool equipment_slot_empty(short item_category,short equip_category);

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
    char assign_inventory_letter();

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

    //
    directional_check_coordinates determine_direction(short direction);

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

    void attack_thrown(Creature* target);

    void attack_melee(Creature* target);

    void die();

    //Evaluate the tile the creature is trying to move to.
    //Returns true if the creature can move, false if the creature can not move.
    bool check_movement(short check_x,short check_y);

    void execute_movement(short check_x,short check_y);

    void search();

    //Handle anything that needs handling on each turn.
    void process_turn();

    //Apply a race's benefits and negatives to the creature.
    void apply_race(short race_to_apply);

    //Remove a race's benefits and negatives from the creature.
    void remove_race(short race_to_remove);

    //Returns the full name of the creature, which is:
    //race_name + " named " + name
    std::string return_full_name();

    //If true, this creature is the player.
    //If false, this creature is not the player.
    bool is_player;

    bool alive;

    short gender;

    //The weight of the creature.
    short weight;

    //*****************//
    // Game variables: //
    //*****************//

    short race;
    std::string race_name;

    short health;
    short health_max;

    short mana;
    short mana_max;

    short experience_level;

    short experience;
    //Experience needed for next level.
    short experience_max;

    //The amount of damage the creature does with its bare hands with no bonuses from anything.
    //Determined by race.
    short base_damage_melee_min;
    short base_damage_melee_max;

    short base_damage_ranged_min;
    short base_damage_ranged_max;

    short base_damage_thrown_min;
    short base_damage_thrown_max;

    short attributes[ATTRIBUTE_LUCK+1];

    short skills[SKILL_MAGIC_SUMMONING+1];

    //The speed of this creature, used for the turns system.
    //Lower numbers are faster.
    ///I am thinking about race determining the base value of this.
    unsigned short movement_speed;

    //The number of "turn ticks" until the next move for this creature.
    short next_move;

    //The creature's inventory.
    std::vector<Item> inventory;

    //The creature's equipped items.
    std::vector<char> equipment;

    //Field of view variables:

    fov_direction_type facing;
    fov_settings_type fov_settings;
    unsigned int fov_radius;
    //The width of the beam.
    float fov_angle;
    bool beam;
    bool light_on;
    light_data source_data;

    //A list of available inventory letters.
    std::vector<char> inventory_letters;
};

#endif
