/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "templates.h"

using namespace std;

short Templates::string_to_item_effect(string line){
    short effect=0;

    if(line=="fuel"){
        effect=ITEM_EFFECT_FUEL;
    }
    else if(line=="use:light"){
        effect=ITEM_EFFECT_USE_LIGHT;
    }
    else if(line=="slight healing"){
        effect=ITEM_EFFECT_HEALING_SLIGHT;
    }
    else if(line=="healing"){
        effect=ITEM_EFFECT_HEALING;
    }
    else if(line=="considerable healing"){
        effect=ITEM_EFFECT_HEALING_CONSIDERABLE;
    }

    return effect;
}

short Templates::string_to_material(string line){
    short material=0;

    if(line=="cloth"){
        material=MATERIAL_CLOTH;
    }
    else if(line=="leather"){
        material=MATERIAL_LEATHER;
    }
    else if(line=="copper"){
        material=MATERIAL_COPPER;
    }
    else if(line=="brass"){
        material=MATERIAL_BRASS;
    }
    else if(line=="bronze"){
        material=MATERIAL_BRONZE;
    }
    else if(line=="iron"){
        material=MATERIAL_IRON;
    }
    else if(line=="steel"){
        material=MATERIAL_STEEL;
    }
    else if(line=="silver"){
        material=MATERIAL_SILVER;
    }
    else if(line=="gold"){
        material=MATERIAL_GOLD;
    }
    else if(line=="wood"){
        material=MATERIAL_WOOD;
    }
    else if(line=="bone"){
        material=MATERIAL_BONE;
    }
    else if(line=="corpse"){
        material=MATERIAL_CORPSE;
    }
    else if(line=="food"){
        material=MATERIAL_FOOD;
    }
    else if(line=="stone"){
        material=MATERIAL_STONE;
    }
    else if(line=="dirt"){
        material=MATERIAL_DIRT;
    }
    else if(line=="glass"){
        material=MATERIAL_GLASS;
    }
    else if(line=="paper"){
        material=MATERIAL_PAPER;
    }
    else if(line=="water"){
        material=MATERIAL_WATER;
    }
    else if(line=="lava"){
        material=MATERIAL_LAVA;
    }
    /**else if(line=="ice"){
        material=MATERIAL_ICE;
    }
    else if(line=="frost"){
        material=MATERIAL_FROST;
    }*/

    return material;
}

short Templates::string_to_skill(string line){
    short governing_skill=0;

    if(line=="bladed weapons"){
        governing_skill=SKILL_BLADED_WEAPONS;
    }
    else if(line=="blunt weapons"){
        governing_skill=SKILL_BLUNT_WEAPONS;
    }
    else if(line=="stabbing weapons"){
        governing_skill=SKILL_STABBING_WEAPONS;
    }
    else if(line=="unarmed"){
        governing_skill=SKILL_UNARMED;
    }

    else if(line=="security"){
        governing_skill=SKILL_SECURITY;
    }
    else if(line=="stealth"){
        governing_skill=SKILL_STEALTH;
    }
    else if(line=="launcher weapons"){
        governing_skill=SKILL_LAUNCHER_WEAPONS;
    }
    else if(line=="thrown weapons"){
        governing_skill=SKILL_THROWN_WEAPONS;
    }
    else if(line=="dual wielding"){
        governing_skill=SKILL_DUAL_WIELDING;
    }
    else if(line=="speed"){
        governing_skill=SKILL_SPEED;
    }
    else if(line=="fighting"){
        governing_skill=SKILL_FIGHTING;
    }
    else if(line=="dodging"){
        governing_skill=SKILL_DODGING;
    }

    else if(line=="armor"){
        governing_skill=SKILL_ARMOR;
    }

    else if(line=="air magic"){
        governing_skill=SKILL_MAGIC_AIR;
    }
    else if(line=="fire magic"){
        governing_skill=SKILL_MAGIC_FIRE;
    }
    else if(line=="water magic"){
        governing_skill=SKILL_MAGIC_WATER;
    }
    else if(line=="cold magic"){
        governing_skill=SKILL_MAGIC_COLD;
    }

    else if(line=="earth magic"){
        governing_skill=SKILL_MAGIC_EARTH;
    }
    else if(line=="conjuration magic"){
        governing_skill=SKILL_MAGIC_CONJURATION;
    }
    else if(line=="enchantment magic"){
        governing_skill=SKILL_MAGIC_ENCHANTMENT;
    }
    else if(line=="summoning magic"){
        governing_skill=SKILL_MAGIC_SUMMONING;
    }

    return governing_skill;
}

short Templates::string_to_weapon_category(string line){
    short weapon_category=0;

    if(line=="short blades"){
        weapon_category=WEAPON_SHORT_BLADES;
    }
    else if(line=="long blades"){
        weapon_category=WEAPON_LONG_BLADES;
    }
    else if(line=="axes"){
        weapon_category=WEAPON_AXES;
    }
    else if(line=="maces"){
        weapon_category=WEAPON_MACES;
    }
    else if(line=="polearms"){
        weapon_category=WEAPON_POLEARMS;
    }
    else if(line=="staves"){
        weapon_category=WEAPON_STAVES;
    }

    else if(line=="bows"){
        weapon_category=WEAPON_BOWS;
    }
    else if(line=="crossbows"){
        weapon_category=WEAPON_CROSSBOWS;
    }
    else if(line=="slings"){
        weapon_category=WEAPON_SLINGS;
    }
    else if(line=="thrown"){
        weapon_category=WEAPON_THROWN;
    }

    return weapon_category;
}

short Templates::string_to_armor_category(string line){
    short armor_category=0;

    if(line=="head"){
        armor_category=ARMOR_HEAD;
    }
    else if(line=="shoulders"){
        armor_category=ARMOR_SHOULDER;
    }
    else if(line=="chest"){
        armor_category=ARMOR_CHEST;
    }
    else if(line=="back"){
        armor_category=ARMOR_BACK;
    }
    else if(line=="waist"){
        armor_category=ARMOR_WAIST;
    }
    else if(line=="legs"){
        armor_category=ARMOR_LEG;
    }
    else if(line=="feet"){
        armor_category=ARMOR_FOOT;
    }
    else if(line=="hands"){
        armor_category=ARMOR_HAND;
    }
    else if(line=="shield"){
        armor_category=ARMOR_SHIELD;
    }
    else if(line=="neck"){
        armor_category=ARMOR_NECK;
    }
    else if(line=="wrists"){
        armor_category=ARMOR_WRIST;
    }
    else if(line=="fingers"){
        armor_category=ARMOR_FINGER;
    }
    else if(line=="shirt"){
        armor_category=ARMOR_SHIRT;
    }

    return armor_category;
}

short Templates::string_to_color(string line){
    short color=0;

    if(line=="rainbow"){
        color=COLOR__RAINBOW;
    }

    else if(line=="black"){
        color=COLOR__BLACK;
    }

    else if(line=="gray"){
        color=COLOR__GRAY;
    }
    else if(line=="light gray"){
        color=COLOR__LIGHT_GRAY;
    }
    else if(line=="dark gray"){
        color=COLOR__DARK_GRAY;
    }
    else if(line=="payne's gray"){
        color=COLOR__PAYNES_GRAY;
    }

    else if(line=="white"){
        color=COLOR__WHITE;
    }

    else if(line=="brown"){
        color=COLOR__BROWN;
    }
    else if(line=="dark brown"){
        color=COLOR__DARK_BROWN;
    }
    else if(line=="sepia"){
        color=COLOR__SEPIA;
    }
    else if(line=="sandy brown"){
        color=COLOR__SANDY_BROWN;
    }
    else if(line=="wheat"){
        color=COLOR__WHEAT;
    }
    else if(line=="tan"){
        color=COLOR__TAN;
    }
    else if(line=="pale brown"){
        color=COLOR__PALE_BROWN;
    }
    else if(line=="burnt sienna"){
        color=COLOR__BURNT_SIENNA;
    }

    else if(line=="yellow"){
        color=COLOR__YELLOW;
    }
    else if(line=="system"){
        color=COLOR__SYSTEM;
    }

    else if(line=="orange"){
        color=COLOR__ORANGE;
    }
    else if(line=="orange peel"){
        color=COLOR__ORANGE_PEEL;
    }
    else if(line=="carrot orange"){
        color=COLOR__ORANGE_CARROT;
    }
    else if(line=="burnt orange"){
        color=COLOR__ORANGE_BURNT;
    }
    else if(line=="pumpkin"){
        color=COLOR__PUMPKIN;
    }

    else if(line=="red"){
        color=COLOR__RED;
    }
    else if(line=="dark red"){
        color=COLOR__DARK_RED;
    }
    else if(line=="flame"){
        color=COLOR__FLAME;
    }

    else if(line=="pink"){
        color=COLOR__PINK;
    }
    else if(line=="rose"){
        color=COLOR__ROSE;
    }
    else if(line=="shocking pink"){
        color=COLOR__SHOCKING_PINK;
    }

    else if(line=="purple"){
        color=COLOR__PURPLE;
    }
    else if(line=="violet"){
        color=COLOR__VIOLET;
    }
    else if(line=="indigo"){
        color=COLOR__INDIGO;
    }

    else if(line=="blue"){
        color=COLOR__BLUE;
    }
    else if(line=="dark blue"){
        color=COLOR__DARK_BLUE;
    }
    else if(line=="sky blue"){
        color=COLOR__SKY_BLUE;
    }
    else if(line=="un blue"){
        color=COLOR__UN_BLUE;
    }

    else if(line=="green"){
        color=COLOR__GREEN;
    }
    else if(line=="jungle"){
        color=COLOR__JUNGLE;
    }
    else if(line=="spring"){
        color=COLOR__SPRING;
    }

    else if(line=="cloth"){
        color=COLOR__CLOTH;
    }
    else if(line=="leather"){
        color=COLOR__LEATHER;
    }
    else if(line=="copper"){
        color=COLOR__COPPER;
    }
    else if(line=="brass"){
        color=COLOR__BRASS;
    }
    else if(line=="bronze"){
        color=COLOR__BRONZE;
    }
    else if(line=="iron"){
        color=COLOR__IRON;
    }
    else if(line=="steel"){
        color=COLOR__STEEL;
    }
    else if(line=="silver"){
        color=COLOR__SILVER;
    }
    else if(line=="gold"){
        color=COLOR__GOLD;
    }
    else if(line=="wood"){
        color=COLOR__WOOD;
    }
    else if(line=="bone"){
        color=COLOR__BONE;
    }
    else if(line=="corpse"){
        color=COLOR__CORPSE;
    }
    else if(line=="food"){
        color=COLOR__FOOD;
    }
    else if(line=="stone"){
        color=COLOR__STONE;
    }
    else if(line=="dirt"){
        color=COLOR__DIRT;
    }
    else if(line=="glass"){
        color=COLOR__GLASS;
    }
    else if(line=="paper"){
        color=COLOR__PAPER;
    }
    else if(line=="water"){
        color=COLOR__WATER;
    }
    else if(line=="lava"){
        color=COLOR__LAVA;
    }

    else if(line=="ice"){
        color=COLOR__ICE;
    }
    else if(line=="frost"){
        color=COLOR__FROST;
    }

    return color;
}
