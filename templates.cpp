/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "templates.h"
#include "enumerations.h"
#include "render.h"
#include "material_properties.h"

using namespace std;
using namespace boost::algorithm;

Templates::Templates(){
    for(int i=ITEM_WEAPON;i<ITEM_OTHER+1;i++){
        template_items.push_back(vector<Item>());
    }
}

bool Templates::load_templates(){
    //Look through all of the files in the directory.
    for(boost::filesystem3::directory_iterator it("data/templates");it!=boost::filesystem3::directory_iterator();it++){
        //Determine the file's full path name.
        string file_path=it->path().string();

        //Determine the file's file name.
        string file_name=it->path().filename().string();

        //If the file is not a directory.
        if(boost::filesystem3::is_regular_file(it->path())){
            load.open(file_path.c_str(),ifstream::in);

            if(load!=NULL){
                fprintf(stdout,"Template file loaded: %s\n",file_path.c_str());

                //As long as we haven't reached the end of the file.
                while(!load.eof()){
                    string line="";

                    //Grab the next line of the file.
                    getline(load,line);

                    //Clear initial whitespace from the line.
                    trim(line);

                    //While the line still has text.
                    /**while(line.length()>0){
                        //If the character is a space or tab, delete it.
                        if(line[0]==' ' || line[0]=='\x09'){
                            line.erase(0,1);
                        }
                        //If the character is anything else, stop checking the line.
                        else{
                            break;
                        }
                    }*/

                    //If the line is a comment.
                    if(istarts_with(line,"//")){
                        //Ignore this line.
                    }

                    //If the line begins an item.
                    else if(icontains(line,"<item")){
                        //Determine the item's category.

                        //If the line begins a weapon item.
                        if(icontains(line,"weapon")){
                            load_template_item(ITEM_WEAPON);
                        }
                        //If the line begins an armor item.
                        else if(icontains(line,"armor")){
                            load_template_item(ITEM_ARMOR);
                        }
                        //If the line begins a food item.
                        else if(icontains(line,"food")){
                            load_template_item(ITEM_FOOD);
                        }
                        //If the line begins a drink item.
                        else if(icontains(line,"drink")){
                            load_template_item(ITEM_DRINK);
                        }
                        //If the line begins a scroll item.
                        else if(icontains(line,"scroll")){
                            load_template_item(ITEM_SCROLL);
                        }
                        //If the line begins a book item.
                        else if(icontains(line,"book")){
                            load_template_item(ITEM_BOOK);
                        }
                        //If the line begins a container item.
                        else if(icontains(line,"container")){
                            load_template_item(ITEM_CONTAINER);
                        }
                        //If the line begins an other item.
                        else if(icontains(line,"other")){
                            load_template_item(ITEM_OTHER);
                        }
                        else{
                            load.close();
                            load.clear();
                            fprintf(stderr,"Error while loading templates: Attempted to load an item with an unknown category.\n");
                            return false;
                        }
                    }

                    //If the line begins a race.
                    else if(icontains(line,"<race>")){
                        load_template_race();
                    }

                    //If the line begins a base stats block.
                    else if(icontains(line,"<base stats>")){
                        load_base_stats();
                    }
                }

                load.close();
                load.clear();
            }
            else{
                load.close();
                load.clear();
                fprintf(stderr,"Error loading templates file: %s\n",file_path.c_str());
                return false;
            }
        }
    }

    return true;
}

void Templates::load_base_stats(){
    //Create a temporary creature.

    Race temp_race;

    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        string health="health:";
        string mana="mana:";
        string base_damage_melee="damage melee:";
        string base_damage_ranged="damage ranged:";
        string base_damage_thrown="damage thrown:";
        string movement_speed="movement speed:";
        string carry_capacity="carrying capacity:";
        string hp_range="levelup hp range:";
        string mana_range="levelup mana range:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.
        trim(line);

        //If the line is a comment.
        if(istarts_with(line,"//")){
            //Ignore this line.
        }

        //Load race data based on the line.

        //Health.
        else if(icontains(line,health)){
            //Clear the data name.
            line.erase(0,health.length());

            temp_race.health_max=atoi(line.c_str());
        }
        //Mana.
        else if(icontains(line,mana)){
            //Clear the data name.
            line.erase(0,mana.length());

            temp_race.mana_max=atoi(line.c_str());
        }
        //Base melee damage.
        else if(icontains(line,base_damage_melee)){
            //Clear the data name.
            line.erase(0,base_damage_melee.length());

            string min_damage="";
            string max_damage="";

            //Look through the characters remaining in line.
            for(int i=0;i<line.length();i++){
                //If the dash is encountered.
                if(line[i]=='-'){
                    //Erase the dash.
                    line.erase(i,1);

                    for(int n=i;n<line.length();n++){
                        max_damage+=line[n];
                    }

                    line.erase(i);

                    min_damage=line;
                    break;
                }
            }

            temp_race.base_damage_melee_min=atoi(min_damage.c_str());
            temp_race.base_damage_melee_max=atoi(max_damage.c_str());
        }
        //Base ranged damage.
        else if(icontains(line,base_damage_ranged)){
            //Clear the data name.
            line.erase(0,base_damage_ranged.length());

            string min_damage="";
            string max_damage="";

            //Look through the characters remaining in line.
            for(int i=0;i<line.length();i++){
                //If the dash is encountered.
                if(line[i]=='-'){
                    //Erase the dash.
                    line.erase(i,1);

                    for(int n=i;n<line.length();n++){
                        max_damage+=line[n];
                    }

                    line.erase(i);

                    min_damage=line;
                    break;
                }
            }

            temp_race.base_damage_ranged_min=atoi(min_damage.c_str());
            temp_race.base_damage_ranged_max=atoi(max_damage.c_str());
        }
        //Base thrown damage.
        else if(icontains(line,base_damage_thrown)){
            //Clear the data name.
            line.erase(0,base_damage_thrown.length());

            string min_damage="";
            string max_damage="";

            //Look through the characters remaining in line.
            for(int i=0;i<line.length();i++){
                //If the dash is encountered.
                if(line[i]=='-'){
                    //Erase the dash.
                    line.erase(i,1);

                    for(int n=i;n<line.length();n++){
                        max_damage+=line[n];
                    }

                    line.erase(i);

                    min_damage=line;
                    break;
                }
            }

            temp_race.base_damage_thrown_min=atoi(min_damage.c_str());
            temp_race.base_damage_thrown_max=atoi(max_damage.c_str());
        }
        //Movement speed.
        else if(icontains(line,movement_speed)){
            //Clear the data name.
            line.erase(0,movement_speed.length());

            temp_race.movement_speed=atoi(line.c_str());
        }
        //Carrying capacity.
        else if(icontains(line,carry_capacity)){
            //Clear the data name.
            line.erase(0,carry_capacity.length());

            temp_race.carry_capacity=atoi(line.c_str());
        }
        //Level up HP Range.
        else if(icontains(line,hp_range)){
            //Clear the data name.
            line.erase(0,hp_range.length());

            string min_number="";
            string max_number="";

            //Look through the characters remaining in line.
            for(int i=0;i<line.length();i++){
                //If the dash is encountered.
                if(line[i]=='-'){
                    //Erase the dash.
                    line.erase(i,1);

                    for(int n=i;n<line.length();n++){
                        max_number+=line[n];
                    }

                    line.erase(i);

                    min_number=line;
                    break;
                }
            }

            temp_race.levelup_hp_min=atoi(min_number.c_str());
            temp_race.levelup_hp_max=atoi(max_number.c_str());
        }
        //Level up mana Range.
        else if(icontains(line,mana_range)){
            //Clear the data name.
            line.erase(0,mana_range.length());

            string min_number="";
            string max_number="";

            //Look through the characters remaining in line.
            for(int i=0;i<line.length();i++){
                //If the dash is encountered.
                if(line[i]=='-'){
                    //Erase the dash.
                    line.erase(i,1);

                    for(int n=i;n<line.length();n++){
                        max_number+=line[n];
                    }

                    line.erase(i);

                    min_number=line;
                    break;
                }
            }

            temp_race.levelup_mana_min=atoi(min_number.c_str());
            temp_race.levelup_mana_max=atoi(max_number.c_str());
        }

        //If the line ends the base stats.
        else if(icontains(line,"</base stats>")){
            ///Ensure the base stats are legitimate.

            //Apply these base stats to the base_stats object.
            base_stats=temp_race;

            return;
        }
    }
}

void Templates::load_template_race(){
    //Create a temporary race.

    Race temp_race;

    //Set all of the bonuses to 0.

    temp_race.health_max=0;
    temp_race.mana_max=0;
    temp_race.movement_speed=0;

    for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
        temp_race.attributes[i]=0;
    }

    for(int i=0;i<SKILL_MAGIC_SUMMONING+1;i++){
        temp_race.skills[i][SKILL_EXPERIENCE_LEVEL]=0;
    }

    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        string name="name:";
        string appearance="appearance:";
        string color="color:";
        string weight="weight:";
        string hp_range="levelup hp range:";
        string mana_range="levelup mana range:";
        string health="health:";
        string mana="mana:";
        string movement_speed="movement speed:";
        string strength="strength:";
        string agility="agility:";
        string hardiness="hardiness:";
        string comprehension="comprehension:";
        string acumen="acumen:";
        string luck="luck:";
        string bladed_weapons="bladed weapons:";
        string blunt_weapons="blunt weapons:";
        string stabbing_weapons="stabbing weapons:";
        string unarmed="unarmed:";
        string security="security:";
        string stealth="stealth:";
        string launcher_weapons="launcher weapons:";
        string thrown_weapons="thrown weapons:";
        string dual_wielding="dual wielding:";
        string speed="speed:";
        string fighting="fighting:";
        string dodging="dodging:";
        string armor="armor:";
        string air_magic="air magic:";
        string fire_magic="fire magic:";
        string water_magic="water magic:";
        string cold_magic="cold magic:";
        string earth_magic="earth magic:";
        string conjuration_magic="conjuration magic:";
        string enchantment_magic="enchantment magic:";
        string summoning_magic="summoning magic:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.
        trim(line);

        //If the line is a comment.
        if(istarts_with(line,"//")){
            //Ignore this line.
        }

        //Load race data based on the line.

        //Name.
        else if(icontains(line,name)){
            //Clear the data name.
            line.erase(0,name.length());

            temp_race.name=line;
            if(temp_race.name.length()==0){
                temp_race.name=" ";
            }
        }
        //Appearance.
        else if(icontains(line,appearance)){
            //Clear the data name.
            line.erase(0,appearance.length());

            temp_race.appearance=line;
        }
        //Color.
        else if(icontains(line,color)){
            //Clear the data name.
            line.erase(0,color.length());

            temp_race.color=string_to_color(line);
        }
        //Weight.
        else if(icontains(line,weight)){
            //Clear the data name.
            line.erase(0,weight.length());

            temp_race.weight=atof(line.c_str());
        }
        //Level up HP Range.
        else if(icontains(line,hp_range)){
            //Clear the data name.
            line.erase(0,hp_range.length());

            string min_number="";
            string max_number="";

            //Look through the characters remaining in line.
            for(int i=0;i<line.length();i++){
                //If the dash is encountered.
                if(line[i]=='-'){
                    //Erase the dash.
                    line.erase(i,1);

                    for(int n=i;n<line.length();n++){
                        max_number+=line[n];
                    }

                    line.erase(i);

                    min_number=line;
                    break;
                }
            }

            temp_race.levelup_hp_min=atoi(min_number.c_str());
            temp_race.levelup_hp_max=atoi(max_number.c_str());
        }
        //Level up mana Range.
        else if(icontains(line,mana_range)){
            //Clear the data name.
            line.erase(0,mana_range.length());

            string min_number="";
            string max_number="";

            //Look through the characters remaining in line.
            for(int i=0;i<line.length();i++){
                //If the dash is encountered.
                if(line[i]=='-'){
                    //Erase the dash.
                    line.erase(i,1);

                    for(int n=i;n<line.length();n++){
                        max_number+=line[n];
                    }

                    line.erase(i);

                    min_number=line;
                    break;
                }
            }

            temp_race.levelup_mana_min=atoi(min_number.c_str());
            temp_race.levelup_mana_max=atoi(max_number.c_str());
        }
        //Health.
        else if(icontains(line,health)){
            //Clear the data name.
            line.erase(0,health.length());

            temp_race.health_max=atoi(line.c_str());
        }
        //Mana.
        else if(icontains(line,mana)){
            //Clear the data name.
            line.erase(0,mana.length());

            temp_race.mana_max=atoi(line.c_str());
        }
        //Movement speed.
        else if(icontains(line,movement_speed)){
            //Clear the data name.
            line.erase(0,movement_speed.length());

            temp_race.movement_speed=atoi(line.c_str());
        }
        //Strength.
        else if(icontains(line,strength)){
            //Clear the data name.
            line.erase(0,strength.length());

            temp_race.attributes[ATTRIBUTE_STRENGTH]=atoi(line.c_str());
        }
        //Agility.
        else if(icontains(line,agility)){
            //Clear the data name.
            line.erase(0,agility.length());

            temp_race.attributes[ATTRIBUTE_AGILITY]=atoi(line.c_str());
        }
        //Hardiness.
        else if(icontains(line,hardiness)){
            //Clear the data name.
            line.erase(0,hardiness.length());

            temp_race.attributes[ATTRIBUTE_HARDINESS]=atoi(line.c_str());
        }
        //Comprehension.
        else if(icontains(line,comprehension)){
            //Clear the data name.
            line.erase(0,comprehension.length());

            temp_race.attributes[ATTRIBUTE_COMPREHENSION]=atoi(line.c_str());
        }
        //Acumen.
        else if(icontains(line,acumen)){
            //Clear the data name.
            line.erase(0,acumen.length());

            temp_race.attributes[ATTRIBUTE_ACUMEN]=atoi(line.c_str());
        }
        //Luck.
        else if(icontains(line,luck)){
            //Clear the data name.
            line.erase(0,luck.length());

            temp_race.attributes[ATTRIBUTE_LUCK]=atoi(line.c_str());
        }
        //Bladed weapons
        else if(icontains(line,bladed_weapons)){
            //Clear the data name.
            line.erase(0,bladed_weapons.length());

            temp_race.skills[SKILL_BLADED_WEAPONS][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Blunt weapons
        else if(icontains(line,blunt_weapons)){
            //Clear the data name.
            line.erase(0,blunt_weapons.length());

            temp_race.skills[SKILL_BLUNT_WEAPONS][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Stabbing weapons
        else if(icontains(line,stabbing_weapons)){
            //Clear the data name.
            line.erase(0,stabbing_weapons.length());

            temp_race.skills[SKILL_STABBING_WEAPONS][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Unarmed
        else if(icontains(line,unarmed)){
            //Clear the data name.
            line.erase(0,unarmed.length());

            temp_race.skills[SKILL_UNARMED][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Security
        else if(icontains(line,security)){
            //Clear the data name.
            line.erase(0,security.length());

            temp_race.skills[SKILL_SECURITY][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Stealth
        else if(icontains(line,stealth)){
            //Clear the data name.
            line.erase(0,stealth.length());

            temp_race.skills[SKILL_STEALTH][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Launcher weapons
        else if(icontains(line,launcher_weapons)){
            //Clear the data name.
            line.erase(0,launcher_weapons.length());

            temp_race.skills[SKILL_LAUNCHER_WEAPONS][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Thrown weapons
        else if(icontains(line,thrown_weapons)){
            //Clear the data name.
            line.erase(0,thrown_weapons.length());

            temp_race.skills[SKILL_THROWN_WEAPONS][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Dual wielding
        else if(icontains(line,dual_wielding)){
            //Clear the data name.
            line.erase(0,dual_wielding.length());

            temp_race.skills[SKILL_DUAL_WIELDING][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Speed
        else if(icontains(line,speed)){
            //Clear the data name.
            line.erase(0,speed.length());

            temp_race.skills[SKILL_SPEED][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Fighting
        else if(icontains(line,fighting)){
            //Clear the data name.
            line.erase(0,fighting.length());

            temp_race.skills[SKILL_FIGHTING][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Dodging
        else if(icontains(line,dodging)){
            //Clear the data name.
            line.erase(0,dodging.length());

            temp_race.skills[SKILL_DODGING][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Armor
        else if(icontains(line,armor)){
            //Clear the data name.
            line.erase(0,armor.length());

            temp_race.skills[SKILL_ARMOR][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Air magic
        else if(icontains(line,air_magic)){
            //Clear the data name.
            line.erase(0,air_magic.length());

            temp_race.skills[SKILL_MAGIC_AIR][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Fire magic
        else if(icontains(line,fire_magic)){
            //Clear the data name.
            line.erase(0,fire_magic.length());

            temp_race.skills[SKILL_MAGIC_FIRE][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Water magic
        else if(icontains(line,water_magic)){
            //Clear the data name.
            line.erase(0,water_magic.length());

            temp_race.skills[SKILL_MAGIC_WATER][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Cold magic
        else if(icontains(line,cold_magic)){
            //Clear the data name.
            line.erase(0,cold_magic.length());

            temp_race.skills[SKILL_MAGIC_COLD][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Earth magic
        else if(icontains(line,earth_magic)){
            //Clear the data name.
            line.erase(0,earth_magic.length());

            temp_race.skills[SKILL_MAGIC_EARTH][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Conjuration magic
        else if(icontains(line,conjuration_magic)){
            //Clear the data name.
            line.erase(0,conjuration_magic.length());

            temp_race.skills[SKILL_MAGIC_CONJURATION][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Enchantment magic
        else if(icontains(line,enchantment_magic)){
            //Clear the data name.
            line.erase(0,enchantment_magic.length());

            temp_race.skills[SKILL_MAGIC_ENCHANTMENT][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }
        //Summoning magic
        else if(icontains(line,summoning_magic)){
            //Clear the data name.
            line.erase(0,summoning_magic.length());

            temp_race.skills[SKILL_MAGIC_SUMMONING][SKILL_EXPERIENCE_LEVEL]=atoi(line.c_str());
        }

        //If the line ends the race.
        else if(icontains(line,"</race>")){
            ///Ensure the race is legitimate.

            //Add this race to its templates vector.
            template_races.push_back(Race());
            template_races[template_races.size()-1]=temp_race;

            return;
        }
    }
}

void Templates::load_template_item(short category){
    //Create a temporary item based on category.

    Item temp_item;

    temp_item.category=category;

    temp_item.spawnable=true;

    temp_item.weight=-1.0;
    temp_item.defense=-1;
    temp_item.damage_max_melee=-1;
    temp_item.damage_max_thrown=-1;
    temp_item.damage_max_ranged=-1;

    double temp_item_size=0.0;

    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The item data name strings used in the file.

        string spawnable="<DON'T SPAWN>";
        string name="name:";
        string plural_name="plural name:";
        string writing="writing:";
        string appearance="appearance:";
        string color="color:";
        string stackable="stackable:";
        string weight="weight:";
        string size="size:";
        string monetary_value="value:";
        string material="material:";
        string damage_melee="melee damage:";
        string damage_thrown="thrown damage:";
        string damage_ranged="ranged damage:";
        string fov_radius="light radius:";
        string beam="light beam:";
        string fov_angle="beam angle:";
        string effect="effect:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.
        trim(line);

        //If the line is a comment.
        if(istarts_with(line,"//")){
            //Ignore this line.
        }

        //Load item data based on the line.

        //Spawnable.
        else if(icontains(line,spawnable)){
            //Clear the data name.
            line.erase(0,spawnable.length());

            temp_item.spawnable=false;
        }
        //Plural name.
        else if(icontains(line,plural_name)){
            //Clear the data name.
            line.erase(0,plural_name.length());

            temp_item.plural_name=line;
            if(temp_item.plural_name.length()==0){
                temp_item.plural_name=" ";
            }
        }
        //Writing.
        else if(icontains(line,writing)){
            //Clear the data name.
            line.erase(0,writing.length());

            temp_item.writing=line;
            if(temp_item.writing.length()==0){
                temp_item.writing=" ";
            }
        }
        //Name.
        else if(icontains(line,name)){
            //Clear the data name.
            line.erase(0,name.length());

            temp_item.name=line;
            if(temp_item.name.length()==0){
                temp_item.name=" ";
            }
        }
        //Appearance.
        else if(icontains(line,appearance)){
            //Clear the data name.
            line.erase(0,appearance.length());

            temp_item.appearance=line;
        }
        //Color.
        else if(icontains(line,color)){
            //Clear the data name.
            line.erase(0,color.length());

            temp_item.color=string_to_color(line);
        }
        //Stackable.
        else if(icontains(line,stackable)){
            //Clear the data name.
            line.erase(0,stackable.length());

            temp_item.stackable=(bool)atoi(line.c_str());
        }
        //Weight.
        else if(icontains(line,weight)){
            //Clear the data name.
            line.erase(0,weight.length());

            temp_item.weight=atof(line.c_str());
        }
        //Size.
        else if(icontains(line,size)){
            //Clear the data name.
            line.erase(0,size.length());

            temp_item_size=atof(line.c_str());
        }
        //Monetary value.
        else if(icontains(line,monetary_value)){
            //Clear the data name.
            line.erase(0,monetary_value.length());

            temp_item.monetary_value=atoi(line.c_str());
        }
        //Material.
        else if(icontains(line,material)){
            //Clear the data name.
            line.erase(0,material.length());

            temp_item.material=string_to_material(line);
        }
        //Melee damage.
        else if(icontains(line,damage_melee)){
            //Clear the data name.
            line.erase(0,damage_melee.length());

            string min_damage="";
            string max_damage="";

            //Look through the characters remaining in line.
            for(int i=0;i<line.length();i++){
                //If the dash is encountered.
                if(line[i]=='-'){
                    //Erase the dash.
                    line.erase(i,1);

                    for(int n=i;n<line.length();n++){
                        max_damage+=line[n];
                    }

                    line.erase(i);

                    min_damage=line;
                    break;
                }
            }

            temp_item.damage_min_melee=atoi(min_damage.c_str());
            temp_item.damage_max_melee=atoi(max_damage.c_str());
        }
        //Thrown damage.
        else if(icontains(line,damage_thrown)){
            //Clear the data name.
            line.erase(0,damage_thrown.length());

            string min_damage="";
            string max_damage="";

            //Look through the characters remaining in line.
            for(int i=0;i<line.length();i++){
                //If the dash is encountered.
                if(line[i]=='-'){
                    //Erase the dash.
                    line.erase(i,1);

                    for(int n=i;n<line.length();n++){
                        max_damage+=line[n];
                    }

                    line.erase(i);

                    min_damage=line;
                    break;
                }
            }

            temp_item.damage_min_thrown=atoi(min_damage.c_str());
            temp_item.damage_max_thrown=atoi(max_damage.c_str());
        }
        //Ranged damage.
        else if(icontains(line,damage_ranged)){
            //Clear the data name.
            line.erase(0,damage_ranged.length());

            string min_damage="";
            string max_damage="";

            //Look through the characters remaining in line.
            for(int i=0;i<line.length();i++){
                //If the dash is encountered.
                if(line[i]=='-'){
                    //Erase the dash.
                    line.erase(i,1);

                    for(int n=i;n<line.length();n++){
                        max_damage+=line[n];
                    }

                    line.erase(i);

                    min_damage=line;
                    break;
                }
            }

            temp_item.damage_min_ranged=atoi(min_damage.c_str());
            temp_item.damage_max_ranged=atoi(max_damage.c_str());
        }
        //Light radius.
        else if(icontains(line,fov_radius)){
            //Clear the data name.
            line.erase(0,fov_radius.length());

            temp_item.fov_radius=atoi(line.c_str());
        }
        //Light beam.
        else if(icontains(line,beam)){
            //Clear the data name.
            line.erase(0,beam.length());

            temp_item.beam=(bool)atoi(line.c_str());
        }
        //Beam angle.
        else if(icontains(line,fov_angle)){
            //Clear the data name.
            line.erase(0,fov_angle.length());

            temp_item.fov_angle=atoi(line.c_str());
        }
        //Effect.
        else if(icontains(line,effect)){
            //Clear the data name.
            line.erase(0,effect.length());

            temp_item.effects.push_back(string_to_item_effect(line));
        }

        //If the line begins weapon-specific data.
        else if(icontains(line,"<weapon")){
            load_template_item_weapon(&temp_item);
        }

        //If the line begins armor-specific data.
        else if(icontains(line,"<armor")){
            load_template_item_armor(&temp_item);
        }

        //If the line begins food-specific data.
        else if(icontains(line,"<food")){
            load_template_item_food(&temp_item);
        }

        //If the line begins drink-specific data.
        else if(icontains(line,"<drink")){
            load_template_item_drink(&temp_item);
        }

        //If the line begins scroll-specific data.
        else if(icontains(line,"<scroll")){
            load_template_item_scroll(&temp_item);
        }

        //If the line begins book-specific data.
        else if(icontains(line,"<book")){
            load_template_item_book(&temp_item);
        }

        //If the line begins container-specific data.
        else if(icontains(line,"<container")){
            load_template_item_container(&temp_item);
        }

        //If the line begins other-specific data.
        else if(icontains(line,"<other")){
            load_template_item_other(&temp_item);
        }

        //If the line ends the item.
        else if(icontains(line,"</item>")){
            //As long as no weight was set.
            if(temp_item.weight==-1.0){
                temp_item.weight=(temp_item_size*specific_gravities[temp_item.material])/2.0;
            }

            //If the item is armor and no defense was set.
            if(temp_item.category==ITEM_ARMOR && temp_item.defense==-1){
                temp_item.defense=(temp_item_size*densities[temp_item.material])/2.0;
            }

            //If no melee damage max was set.
            if(temp_item.damage_max_melee==-1){
                //If the item is a melee weapon.
                if(temp_item.category==ITEM_WEAPON && temp_item.weapon_category>=WEAPON_SHORT_BLADES && temp_item.weapon_category<=WEAPON_STAVES){
                    temp_item.damage_min_melee=temp_item_size*densities[temp_item.material]*3.0;
                    temp_item.damage_max_melee=temp_item_size*densities[temp_item.material]*4.0;
                }
                //If the item is not a melee weapon.
                else{
                    temp_item.damage_min_melee=1;
                    temp_item.damage_max_melee=(temp_item_size*temp_item.weight)/6.0;
                }

                if(temp_item.damage_min_melee<1){
                    temp_item.damage_min_melee=1;
                }
                if(temp_item.damage_max_melee<1){
                    temp_item.damage_max_melee=1;
                }
            }

            //If no thrown damage max was set.
            if(temp_item.damage_max_thrown==-1){
                //If the item is a thrown weapon.
                if(temp_item.category==ITEM_WEAPON && temp_item.weapon_category==WEAPON_THROWN){
                    temp_item.damage_min_thrown=temp_item_size*densities[temp_item.material]*3.0;
                    temp_item.damage_max_thrown=temp_item_size*densities[temp_item.material]*4.0;
                }
                //If the item is not a thrown weapon.
                else{
                    temp_item.damage_min_thrown=1;
                    temp_item.damage_max_thrown=temp_item.damage_max_melee/2.0;
                }

                if(temp_item.damage_min_thrown<1){
                    temp_item.damage_min_thrown=1;
                }
                if(temp_item.damage_max_thrown<1){
                    temp_item.damage_max_thrown=1;
                }
            }

            //If no ranged damage max was set.
            if(temp_item.damage_max_ranged==-1){
                //If the item is a ranged weapon.
                if(temp_item.category==ITEM_WEAPON && temp_item.weapon_category>=WEAPON_BOWS && temp_item.weapon_category<=WEAPON_SLINGS){
                    temp_item.damage_min_ranged=temp_item_size*densities[temp_item.material]*3.0;
                    temp_item.damage_max_ranged=temp_item_size*densities[temp_item.material]*4.0;

                    if(temp_item.damage_min_ranged<1){
                        temp_item.damage_min_ranged=1;
                    }
                    if(temp_item.damage_max_ranged<1){
                        temp_item.damage_max_ranged=1;
                    }
                }
                //If the item is not a ranged weapon.
                else{
                    temp_item.damage_min_ranged=0;
                    temp_item.damage_max_ranged=0;
                }
            }

            ///Ensure the item is legitimate.

            //Add this item to its templates vector.
            template_items[category].push_back(Item());
            template_items[category][template_items[category].size()-1]=temp_item;

            return;
        }
    }
}

void Templates::load_template_item_weapon(Item* temp_item){
    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        string governing_skill_weapon="governing weapon skill:";
        string weapon_category="weapon category:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.
        trim(line);

        //If the line is a comment.
        if(istarts_with(line,"//")){
            //Ignore this line.
        }

        //Load data based on the line.

        //Governing weapon skill.
        else if(icontains(line,governing_skill_weapon)){
            //Clear the data name.
            line.erase(0,governing_skill_weapon.length());

            temp_item->governing_skill_weapon=string_to_skill(line);
        }
        //Weapon category.
        else if(icontains(line,weapon_category)){
            //Clear the data name.
            line.erase(0,weapon_category.length());

            temp_item->weapon_category=string_to_weapon_category(line);
        }

        //If the line ends the weapon.
        else if(icontains(line,"</weapon>")){
            //Done reading weapon data.
            return;
        }
    }
}

void Templates::load_template_item_armor(Item* temp_item){
    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        string armor_category="armor category:";
        string defense="defense:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.
        trim(line);

        //If the line is a comment.
        if(istarts_with(line,"//")){
            //Ignore this line.
        }

        //Load data based on the line.

        //Armor category.
        else if(icontains(line,armor_category)){
            //Clear the data name.
            line.erase(0,armor_category.length());

            temp_item->armor_category=string_to_armor_category(line);
        }
        //Defense.
        else if(icontains(line,defense)){
            //Clear the data name.
            line.erase(0,defense.length());

            temp_item->defense=atoi(line.c_str());
        }

        //If the line ends the armor.
        else if(icontains(line,"</armor>")){
            //Done reading armor data.
            return;
        }
    }
}

void Templates::load_template_item_food(Item* temp_item){
    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        string is_corpse="is corpse:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.
        trim(line);

        //If the line is a comment.
        if(istarts_with(line,"//")){
            //Ignore this line.
        }

        //Load data based on the line.

        //Is corpse.
        else if(icontains(line,is_corpse)){
            //Clear the data name.
            line.erase(0,is_corpse.length());

            temp_item->is_corpse=(bool)atoi(line.c_str());
        }

        //If the line ends the food.
        else if(icontains(line,"</food>")){
            //Done reading food data.
            return;
        }
    }
}

void Templates::load_template_item_drink(Item* temp_item){
    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        string thirst_quenched="thirst quenched:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.
        trim(line);

        //If the line is a comment.
        if(istarts_with(line,"//")){
            //Ignore this line.
        }

        //Load data based on the line.

        //Thirst quenched.
        else if(icontains(line,thirst_quenched)){
            //Clear the data name.
            line.erase(0,thirst_quenched.length());

            temp_item->thirst_quenched=atoi(line.c_str());
        }

        //If the line ends the drink.
        else if(icontains(line,"</drink>")){
            //Done reading drink data.
            return;
        }
    }
}

void Templates::load_template_item_scroll(Item* temp_item){
    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        ///string is_corpse="is corpse:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.
        trim(line);

        //If the line is a comment.
        if(istarts_with(line,"//")){
            //Ignore this line.
        }

        //Load data based on the line.

        //Is corpse.
        /**else if(icontains(line,is_corpse)){
            //Clear the data name.
            line.erase(0,is_corpse.length());

            temp_item->is_corpse=(bool)atoi(line.c_str());
        }*/

        //If the line ends the scroll.
        else if(icontains(line,"</scroll>")){
            //Done reading scroll data.
            return;
        }
    }
}

void Templates::load_template_item_book(Item* temp_item){
    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        ///string is_corpse="is corpse:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.
        trim(line);

        //If the line is a comment.
        if(istarts_with(line,"//")){
            //Ignore this line.
        }

        //Load data based on the line.

        //Is corpse.
        /**else if(icontains(line,is_corpse)){
            //Clear the data name.
            line.erase(0,is_corpse.length());

            temp_item->is_corpse=(bool)atoi(line.c_str());
        }*/

        //If the line ends the book.
        else if(icontains(line,"</book>")){
            //Done reading book data.
            return;
        }
    }
}

void Templates::load_template_item_container(Item* temp_item){
    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        ///string is_corpse="is corpse:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.
        trim(line);

        //If the line is a comment.
        if(istarts_with(line,"//")){
            //Ignore this line.
        }

        //Load data based on the line.

        //Is corpse.
        /**else if(icontains(line,is_corpse)){
            //Clear the data name.
            line.erase(0,is_corpse.length());

            temp_item->is_corpse=(bool)atoi(line.c_str());
        }*/

        //If the line ends the container.
        else if(icontains(line,"</container>")){
            //Done reading container data.
            return;
        }
    }
}

void Templates::load_template_item_other(Item* temp_item){
    //As long as we haven't reached the end of the file.
    while(!load.eof()){
        string line="";

        //The data name strings used in the file.

        string fuel_max="max fuel:";

        //Grab the next line of the file.
        getline(load,line);

        //Clear initial whitespace from the line.
        trim(line);

        //If the line is a comment.
        if(istarts_with(line,"//")){
            //Ignore this line.
        }

        //Load data based on the line.

        //Max fuel amount.
        else if(icontains(line,fuel_max)){
            //Clear the data name.
            line.erase(0,fuel_max.length());

            temp_item->fuel_max=atoi(line.c_str());
        }

        //If the line ends the other.
        else if(icontains(line,"</other>")){
            //Done reading other data.
            return;
        }
    }
}
