/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#ifndef enumerations_h
#define enumerations_h

#include "light_data.h"

//*********//
// Colors: //
//*********//

enum{
    //Standard colors//

    COLOR__RAINBOW,

    COLOR__BLACK,

    COLOR__GRAY,
    COLOR__LIGHT_GRAY,
    COLOR__DARK_GRAY,
    COLOR__PAYNES_GRAY,

    COLOR__WHITE,

    COLOR__BROWN,
    COLOR__DARK_BROWN,
    COLOR__SEPIA,
    COLOR__SANDY_BROWN,
    COLOR__WHEAT,
    COLOR__TAN,
    COLOR__PALE_BROWN,
    COLOR__BURNT_SIENNA,

    COLOR__YELLOW,
    COLOR__SYSTEM,

    COLOR__ORANGE,
    COLOR__ORANGE_PEEL,
    COLOR__ORANGE_CARROT,
    COLOR__ORANGE_BURNT,
    COLOR__PUMPKIN,

    COLOR__RED,
    COLOR__DARK_RED,
    COLOR__FLAME,

    COLOR__PINK,
    COLOR__ROSE,
    COLOR__SHOCKING_PINK,

    COLOR__PURPLE,
    COLOR__VIOLET,
    COLOR__INDIGO,

    COLOR__BLUE,
    COLOR__DARK_BLUE,
    COLOR__SKY_BLUE,
    COLOR__UN_BLUE,

    COLOR__GREEN,
    COLOR__JUNGLE,
    COLOR__SPRING,

    //Material colors//

    COLOR__CLOTH,
    COLOR__LEATHER,
    COLOR__COPPER,
    COLOR__BRASS,
    COLOR__BRONZE,
    COLOR__IRON,
    COLOR__STEEL,
    COLOR__SILVER,
    COLOR__GOLD,
    COLOR__WOOD,
    COLOR__BONE,
    COLOR__CORPSE,
    COLOR__FOOD,
    COLOR__STONE,
    COLOR__DIRT,
    COLOR__GLASS,
    COLOR__PAPER,
    COLOR__WATER,
    COLOR__LAVA,

    //Covering colors//

    COLOR__ICE,
    COLOR__FROST
};

//******//
// Fog: //
//******//

enum{
    FOG_BLACK,
    FOG_FOG,
    FOG_REVEALED_PERMANENT,
    FOG_REVEALED_TEMPORARY
};

//*******************//
// Light Source Off: //
//*******************//
//These variables are used when a creature's light source is off.

const unsigned int LIGHT_SOURCE_OFF_RADIUS=2;
const light_data LIGHT_SOURCE_OFF_DATA={false};

//*************//
// Tile Types: //
//*************//

enum{
    TILE_TYPE_WALL,
    TILE_TYPE_FLOOR,
    TILE_TYPE_SOLID,
    TILE_TYPE_LIQUID,
    TILE_TYPE_DOWN_STAIRS,
    TILE_TYPE_UP_STAIRS,
    TILE_TYPE_DOOR_CLOSED,
    TILE_TYPE_DOOR_OPEN,
    TILE_TYPE_FOUNTAIN,
    TILE_TYPE_SECRET_DOOR,
    TILE_TYPE_TRAP
};

//************//
// Materials: //
//************//

enum{
    MATERIAL_CLOTH,
    MATERIAL_LEATHER,
    MATERIAL_COPPER,
    MATERIAL_BRASS,
    MATERIAL_BRONZE,
    MATERIAL_IRON,
    MATERIAL_STEEL,
    MATERIAL_SILVER,
    MATERIAL_GOLD,
    MATERIAL_WOOD,
    MATERIAL_BONE,
    MATERIAL_CORPSE,
    MATERIAL_FOOD,
    MATERIAL_STONE,
    MATERIAL_DIRT,
    MATERIAL_GLASS,
    MATERIAL_PAPER,
    MATERIAL_WATER,
    MATERIAL_LAVA
};

//************//
// Coverings: //
//************//

enum{
    COVERING_ICE,
    COVERING_FROST
};

//***************//
// Item Effects: //
//***************//
///May not be needed.
/**enum{
    EFFECT_
};*/

//***************//
// Temperatures: //
//***************//

enum{
    TEMP_ABSOLUTE_ZERO=-273,
    TEMP_FREEZING=0,
    TEMP_ROOM_TEMPERATURE=20,
    TEMP_BOILING=100
};

//******************//
// Item Categories: //
//******************//

enum{
    //Weapons

    ITEM_WEAPON,

    //Armor

    ITEM_ARMOR,

    //Consumables

    ITEM_FOOD,
    ITEM_DRINK,
    ITEM_SCROLL,
    ITEM_BOOK,

    //Containers

    ITEM_CONTAINER,

    //Miscellaneous

    ITEM_OTHER
};

//***************//
// Item Effects: //
//***************//

enum{
    ITEM_EFFECT_USE_LIGHT,
    ITEM_EFFECT_FUEL,
    ITEM_EFFECT_HEALING_SLIGHT,
    ITEM_EFFECT_HEALING,
    ITEM_EFFECT_HEALING_CONSIDERABLE,
};

//******************//
// Healing Amounts: //
//******************//

enum{
    HEALING_SLIGHT_MIN=5,
    HEALING_SLIGHT_MAX=20,
    HEALING_MIN=50,
    HEALING_MAX=200,
    HEALING_CONSIDERABLE_MIN=500,
    HEALING_CONSIDERABLE_MAX=2000
};

//*************//
// Attributes: //
//*************//

enum{
    ATTRIBUTE_STRENGTH,
    ATTRIBUTE_AGILITY,
    ATTRIBUTE_HARDINESS,
    ATTRIBUTE_COMPREHENSION,
    ATTRIBUTE_ACUMEN,
    ATTRIBUTE_LUCK
};

//*********//
// Skills: //
//*********//

enum{
    SKILL_EXPERIENCE_LEVEL,
    SKILL_EXPERIENCE,
    SKILL_EXPERIENCE_MAX,

    SKILL_BLADED_WEAPONS=0,
    SKILL_BLUNT_WEAPONS,
    SKILL_STABBING_WEAPONS,
    SKILL_UNARMED,

    SKILL_SECURITY,
    SKILL_STEALTH,
    SKILL_LAUNCHER_WEAPONS,
    SKILL_THROWN_WEAPONS,
    SKILL_DUAL_WIELDING,
    SKILL_SPEED,
    SKILL_FIGHTING,
    SKILL_DODGING,

    SKILL_ARMOR,

    SKILL_MAGIC_AIR,
    SKILL_MAGIC_FIRE,
    SKILL_MAGIC_WATER,
    SKILL_MAGIC_COLD,

    SKILL_MAGIC_EARTH,
    SKILL_MAGIC_CONJURATION,
    SKILL_MAGIC_ENCHANTMENT,
    SKILL_MAGIC_SUMMONING
};

//********************//
// Weapon Categories: //
//********************//

enum{
    //Melee

    WEAPON_SHORT_BLADES,
    WEAPON_LONG_BLADES,
    WEAPON_AXES,
    WEAPON_MACES,
    WEAPON_POLEARMS,
    WEAPON_STAVES,

    //Ranged

    WEAPON_BOWS,
    WEAPON_CROSSBOWS,
    WEAPON_SLINGS,
    WEAPON_THROWN
};

//*******************//
// Armor Categories: //
//*******************//

enum{
    ARMOR_HEAD,
    ARMOR_SHOULDER,
    ARMOR_CHEST,
    ARMOR_BACK,
    ARMOR_WAIST,
    ARMOR_LEG,
    ARMOR_FOOT,
    ARMOR_HAND,
    ARMOR_SHIELD,
    ARMOR_NECK,
    ARMOR_WRIST,
    ARMOR_SHIRT,
    ARMOR_FINGER
};

//******************//
// Equipment Slots: //
//******************//

enum{
    EQUIP_HEAD,
    EQUIP_SHOULDER,
    EQUIP_CHEST,
    EQUIP_BACK,
    EQUIP_WAIST,
    EQUIP_LEG,
    EQUIP_FOOT,
    EQUIP_HAND,
    EQUIP_SHIELD,
    EQUIP_NECK,
    EQUIP_WRIST,
    EQUIP_SHIRT,
    EQUIP_FINGER_RIGHT,
    EQUIP_FINGER_LEFT,
    EQUIP_QUIVER,
    EQUIP_HOLD_RIGHT,
    EQUIP_HOLD_LEFT
};

//**********//
// Genders: //
//**********//

enum{
    GENDER_MALE,
    GENDER_FEMALE,
    GENDER_NEUTRAL
};

//******************//
// Causes of death: //
//******************//

enum{
    CAUSE_OF_DEATH_MELEE,
    CAUSE_OF_DEATH_THROWN,
    CAUSE_OF_DEATH_RANGED,
    CAUSE_OF_DEATH_THIRST
};

//****************//
// Thirst States: //
//****************//

enum{
    THIRST_DEATH=1850,
    THIRST_FAINTING=1650,
    THIRST_WEAK=1600,
    THIRST_THIRSTY=1500,
    THIRST_NOT_THIRSTY=500,
    THIRST_SATIATED=0,
    THIRST_BLOATED=-1,
    THIRST_FLOOR=-20000
};

//****************//
// Light Sources: //
//****************//

enum{
    LIGHT_NONE
};

//***********//
// Commands: //
//***********//

enum{
    COMMAND_NONE,
    COMMAND_GO_DOWN_STAIRS,
    COMMAND_GO_UP_STAIRS,
    COMMAND_PICK_UP,
    COMMAND_WAIT,
    COMMAND_SEARCH
};

//***********************//
// Directional Commands: //
//***********************//

enum{
    DIRECTIONAL_COMMAND_NONE,
    DIRECTIONAL_COMMAND_OPEN_DOOR,
    DIRECTIONAL_COMMAND_CLOSE_DOOR,
    DIRECTIONAL_COMMAND_MOVE_LEFT,
    DIRECTIONAL_COMMAND_MOVE_UP,
    DIRECTIONAL_COMMAND_MOVE_RIGHT,
    DIRECTIONAL_COMMAND_MOVE_DOWN,
    DIRECTIONAL_COMMAND_MOVE_LEFT_UP,
    DIRECTIONAL_COMMAND_MOVE_RIGHT_UP,
    DIRECTIONAL_COMMAND_MOVE_RIGHT_DOWN,
    DIRECTIONAL_COMMAND_MOVE_LEFT_DOWN,
    DIRECTIONAL_COMMAND_THROW_ITEM,
    DIRECTIONAL_COMMAND_FIRE_ITEM
};

//*********************//
// Inventory Commands: //
//*********************//

enum{
    INVENTORY_COMMAND_NONE,
    INVENTORY_COMMAND_DROP_ITEM,
    INVENTORY_COMMAND_EQUIP_RIGHT_HAND,
    INVENTORY_COMMAND_EQUIP_LEFT_HAND,
    INVENTORY_COMMAND_QUIVER_ITEM,
    INVENTORY_COMMAND_EQUIP_ARMOR,
    INVENTORY_COMMAND_UNEQUIP_ITEM,
    INVENTORY_COMMAND_THROW_ITEM,
    INVENTORY_COMMAND_QUAFF_ITEM,
    INVENTORY_COMMAND_USE_ITEM,
    INVENTORY_COMMAND_MIX_ITEMS_1,
    INVENTORY_COMMAND_MIX_ITEMS_2,
    INVENTORY_COMMAND_READ_ITEM,
    INVENTORY_COMMAND_ITEM_INFO
};

//***************//
// Object Types: //
//***************//

enum{
    OBJECT_CREATURE,
    OBJECT_ITEM
};

//***********//
// Messages: //
//***********//

enum{
    MESSAGE_SYSTEM,
    MESSAGE_STANDARD
};

//**********//
// Windows: //
//**********//

enum{
    WINDOW_NONE,
    WINDOW_INVENTORY,
    WINDOW_STATS,
    WINDOW_LEVELUP,
    WINDOW_DEATH,
    WINDOW_INTERACTIVE_INVENTORY_RELEVANT,
    WINDOW_INTERACTIVE_INVENTORY_ALL,
    WINDOW_ITEM_INFO
};

//*************//
// Directions: //
//*************//

enum{
    NONE,
    LEFT,
    UP,
    RIGHT,
    DOWN,
    LEFT_UP,
    RIGHT_UP,
    RIGHT_DOWN,
    LEFT_DOWN,
    //This constant is used as a cam_state value, to tell the camera whether it is free or attached to a player.
    CAM_STICKY
};

#endif
