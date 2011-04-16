/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef creature_h
#define creature_h

#include <vector>
#include <sstream>

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

//Holds data referring to a specific item template.
struct item_template_data{
    int category;
    int index;
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
    char two_part_inventory_input_state;

    //A standard command to be executed on the next call to move().
    short command_standard;

    //Used to store the index of the inventory item being examined in the item info window.
    //Only used by the player.
    int item_info;

    //If true, the creature has given some input that should initiate a call to turn().
    //Only used by the Player class.
    ///This should really be renamed initiate_turn.
    bool initiate_move;

    //If true, this creature is the player.
    //If false, this creature is not the player.
    bool is_player;

    //Stores the attribute(s) being selected for improvement when leveling up.
    std::vector<short> levelup_attributes;

    //*****************//
    // Game variables: //
    //*****************//

    short gender;

    short race;
    std::string race_name;

    std::string class_name;

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

    //The base amount of damage the creature does with its bare hands.
    short base_damage_melee_min;
    short base_damage_melee_max;

    //The base amount of damage the creature does with ranged weapons.
    short base_damage_ranged_min;
    short base_damage_ranged_max;

    //The base amount of damage the creature does when throwing weapons.
    short base_damage_thrown_min;
    short base_damage_thrown_max;

    //The creature's base attribute values.
    int attributes[ATTRIBUTE_LUCK+1];

    //The creature's base skill values, and their experience and experience max values.
    int skills[SKILL_MAGIC_SUMMONING+1][3];

    //The creature's 3 focused skills.
    short focused_skills[3];

    //Keeps track of the bonus for each attribute per level.
    //Each number represents the number of skill-ups for skills governed by that attribute over the last level.
    //These are all reset to 0 after each level up.
    short attribute_level_bonuses[ATTRIBUTE_LUCK+1];

    //The creature's base carrying capacity.
    int carry_capacity;

    //The base speed of this creature, used for the turns system.
    //Lower numbers are faster.
    int movement_speed;

    //The base number of "turn ticks" until the next move for this creature.
    int next_move;

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

    //Adds stack_size passed items to the creature's inventory.
    //Returns true if the item was added to inventory.
    //Returns false if the item was not added to inventory.
    bool give_item(std::string item_name,int stack_size=1,char force_inventory_letter=-1);

    //Returns the template category and index of the item with passed name.
    item_template_data return_item_template(std::string item_name);

    //Assign an identifier to the creature.
    void assign_identifier();

    //Return the creature's identifier.
    void return_identifier();

    //Drops the item at item_index in inventory.
    void drop_item(int item_index);

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

    void die(short cause_of_death,std::string killer="",std::string killer_item="");

    //Level up the creature.
    void level_up();

    //Gain experience points.
    void gain_experience(int points_gained);

    //Level up a skill.
    void level_up_skill(short skill,int experience_gained);

    //Gain skill experience points.
    void gain_skill_experience(short skill,int points_gained,int experience_gained=1,bool allow_focused_bonus=true);

    //Evaluate the tile the creature is trying to move to.
    //Returns true if the creature can move, false if the creature can not move.
    bool check_movement(short check_x,short check_y);

    void execute_movement(short check_x,short check_y);

    void search();

    //Check the tile this creature is currently on.
    void check_this_tile();

    //Handle anything that needs handling on each game turn.
    void process_turn();

    //Handle anything that needs handling on each move.
    void process_move();

    //Increase or decrease thirst, and then handle thirst states.
    void change_thirst(bool increase,short amount);

    std::string return_thirst_state();

    std::string return_encumbrance_state();

    //Mix two items together.
    //The first item is applied to the second item.
    void mix_items(int item_index_1,int item_index_2);

    //Returns true if the items can be mixed.
    //Returns false if the items cannot be mixed.
    bool items_mixable(int item_index_1,int item_index_2);

    //Apply a race's benefits and negatives to the creature.
    void apply_race(short race_to_apply);

    //Remove a race's benefits and negatives from the creature.
    void remove_race(short race_to_remove);

    //Returns the total weight of all items in the creature's inventory.
    double return_inventory_weight(short item_category=-1);

    //Returns the full name of the creature, which is:
    //race_name + " named " + name
    std::string return_full_name();

    //Returns the current TRUE value (with all relevant bonuses, penalties, etc.) for the corresponding stats.
    int return_health();
    int return_health_max();
    ///int return_mana();
    ///int return_mana_max();
    int return_armor();
    int return_movement_speed();
    int return_next_move();
    double return_carry_capacity();

    //Each of these functions returns the current TRUE attribute value (with all relevant bonuses, penalties, etc.) for the corresponding attribute.
    int return_attribute_strength();
    int return_attribute_agility();
    int return_attribute_hardiness();
    ///int return_attribute_comprehension();
    ///int return_attribute_acumen();
    ///int return_attribute_luck();

    //Each of these functions returns the current TRUE skill value (with all relevant bonuses, penalties, etc.) for the corresponding skill.
    int return_skill_bladed_weapons();
    int return_skill_blunt_weapons();
    int return_skill_stabbing_weapons();
    int return_skill_unarmed();
    ///int return_skill_security();
    ///int return_skill_stealth();
    int return_skill_launcher_weapons();
    int return_skill_thrown_weapons();
    int return_skill_dual_wielding();
    int return_skill_speed();
    ///int return_skill_fighting();
    ///int return_skill_dodging();
    int return_skill_armor();
    /**int return_skill_magic_air();
    int return_skill_magic_fire();
    int return_skill_magic_water();
    int return_skill_magic_cold();
    int return_skill_magic_earth();
    int return_skill_magic_conjuration();
    int return_skill_magic_enchantment();
    int return_skill_magic_summoning();*/

    //Returns true if the passed skill is a focused skill.
    //Returns false if the passed skill is not a focused skill.
    bool is_focused_skill(short skill);

    //Returns true if the passed attribute is currently selected for improvement.
    //Returns false if the passed attribute is not currently selected for improvement.
    bool levelup_is_selected_attribute(short attribute);

    //Returns true if all attributes have been set.
    //Returns false if all attributes have not been set.
    bool levelup_all_attributes_set();

    //Returns a stringstream containing all of this creature's save data.
    std::string return_save_data();

    //Load data from a save file.
    void load_data(std::stringstream* load);
};

#endif
