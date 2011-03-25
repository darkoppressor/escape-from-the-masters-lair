/* Copyright (c) 2011 Kevin Wells */
/* Escape from the Master's Lair may be freely redistributed.  See license for details. */

#include "player.h"
#include "world.h"
#include "render.h"
#include "quit.h"

using namespace std;
using namespace boost::algorithm;

void Player::handle_input_no_game(){
    int mouse_x,mouse_y;

    //Get the SDL keystates and store them in the keystates variable for evaluation.
    keystates=SDL_GetKeyState(NULL);

    modstate=SDL_GetModState();

    SDL_GetMouseState(&mouse_x,&mouse_y);

    while(SDL_PollEvent(&event)){
        SDL_EnableUNICODE(SDL_ENABLE);

        switch(event.type){
            case SDL_QUIT:
                quit_game();
                break;

            case SDL_KEYDOWN:
                //If no player name has yet been entered.
                if(name=="\x1F"){
                    handle_input_get_name();
                }
                //If no race has been selected.
                else if(race==-1){
                    handle_input_get_race();
                }
                else{
                    handle_input_get_focused_skills();
                }
                break;
        }
    }

    //Toggle fullscreen.
    //This is located here in the code because it needs to come before the chat toggle,
    //which also uses SDLK_RETURN.
    if((keystates[SDLK_LALT] || keystates[SDLK_RALT]) && keystates[SDLK_RETURN]){
        main_window.toggle_fullscreen();

        if(keystates[SDLK_LALT]){
            keystates[SDLK_LALT]=NULL;
        }
        if(keystates[SDLK_RALT]){
            keystates[SDLK_RALT]=NULL;
        }
        keystates[SDLK_RETURN]=NULL;
    }

    //If the player hits the screenshot key, take a screenshot.
    if(keystates[SDLK_F5]){
        main_window.screenshot();

        //Once the screenshot key has been hit, the player must release it for it to function again.
        keystates[SDLK_F5]=NULL;
    }

    //Quit the game.
    if((keystates[SDLK_LALT] || keystates[SDLK_RALT]) && keystates[SDLK_F4]){
        quit_game();

        if(keystates[SDLK_LALT]){
            keystates[SDLK_LALT]=NULL;
        }
        if(keystates[SDLK_RALT]){
            keystates[SDLK_RALT]=NULL;
        }
        keystates[SDLK_F4]=NULL;
    }
}

void Player::handle_input_get_name(){
    if(get_name.length()<12){
        if(event.key.keysym.unicode>=(Uint16)' ' && event.key.keysym.unicode<=(Uint16)'~'){
            get_name+=(char)event.key.keysym.unicode;
        }
    }

    if(event.key.keysym.sym==SDLK_ESCAPE){
        quit_game();
    }

    else if(event.key.keysym.sym==SDLK_BACKSPACE && get_name.length()>0){
        //Remove one character from the end of the string.
        get_name.erase(get_name.length()-1);
    }

    else if((event.key.keysym.sym==SDLK_RETURN || event.key.keysym.sym==SDLK_KP_ENTER) && get_name.length()>0){
        trim(get_name);

        name=get_name;

        string check_name="saves/";
        check_name+=name;

        get_name.clear();

        //If the name is associated with a character that already exists.
        if(boost::filesystem3::is_regular_file(to_lower_copy(check_name))){
            game.old_game();
        }

        //Make sure that enter is cleared before moving to the next screen.
        Uint8 *keystates=SDL_GetKeyState(NULL);
        keystates[SDLK_RETURN]=NULL;
        keystates[SDLK_KP_ENTER]=NULL;
    }
}

void Player::handle_input_get_race(){
    if(get_race.length()<3){
        if(event.key.keysym.unicode>=(Uint16)'0' && event.key.keysym.unicode<=(Uint16)'9'){
            get_race+=(char)event.key.keysym.unicode;
        }
    }

    if(event.key.keysym.sym==SDLK_ESCAPE){
        //Go back one screen.
        name="\x1F";
        get_race.clear();
    }

    else if(event.key.keysym.sym==SDLK_BACKSPACE && get_race.length()>0){
        //Remove one character from the end of the string.
        get_race.erase(get_race.length()-1);
    }

    else if((event.key.keysym.sym==SDLK_RETURN || event.key.keysym.sym==SDLK_KP_ENTER) && get_race.length()>0 && atoi(get_race.c_str())<templates.template_races.size()){
        race=atoi(get_race.c_str());

        get_race.clear();

        //Make sure that enter is cleared before moving to the next screen.
        Uint8 *keystates=SDL_GetKeyState(NULL);
        keystates[SDLK_RETURN]=NULL;
        keystates[SDLK_KP_ENTER]=NULL;
    }
}

void Player::handle_input_get_focused_skills(){
    if(event.key.keysym.sym==SDLK_ESCAPE){
        //Go back one screen.
        race=-1;
        focused_skills[0]=-1;
        focused_skills[1]=-1;
        focused_skills[2]=-1;
    }

    else if(event.key.keysym.unicode>=(Uint16)'a' && event.key.keysym.unicode<=(Uint16)'u'){
        short skill=-1;

        switch(event.key.keysym.unicode){
        case (Uint16)'a':
            skill=SKILL_BLADED_WEAPONS;
            break;
        case (Uint16)'b':
            skill=SKILL_BLUNT_WEAPONS;
            break;
        case (Uint16)'c':
            skill=SKILL_STABBING_WEAPONS;
            break;
        case (Uint16)'d':
            skill=SKILL_UNARMED;
            break;
        case (Uint16)'e':
            skill=SKILL_SECURITY;
            break;
        case (Uint16)'f':
            skill=SKILL_STEALTH;
            break;
        case (Uint16)'g':
            skill=SKILL_LAUNCHER_WEAPONS;
            break;
        case (Uint16)'h':
            skill=SKILL_THROWN_WEAPONS;
            break;
        case (Uint16)'i':
            skill=SKILL_DUAL_WIELDING;
            break;
        case (Uint16)'j':
            skill=SKILL_SPEED;
            break;
        case (Uint16)'k':
            skill=SKILL_FIGHTING;
            break;
        case (Uint16)'l':
            skill=SKILL_DODGING;
            break;
        case (Uint16)'m':
            skill=SKILL_ARMOR;
            break;
        case (Uint16)'n':
            skill=SKILL_MAGIC_AIR;
            break;
        case (Uint16)'o':
            skill=SKILL_MAGIC_FIRE;
            break;
        case (Uint16)'p':
            skill=SKILL_MAGIC_WATER;
            break;
        case (Uint16)'q':
            skill=SKILL_MAGIC_COLD;
            break;
        case (Uint16)'r':
            skill=SKILL_MAGIC_EARTH;
            break;
        case (Uint16)'s':
            skill=SKILL_MAGIC_CONJURATION;
            break;
        case (Uint16)'t':
            skill=SKILL_MAGIC_ENCHANTMENT;
            break;
        case (Uint16)'u':
            skill=SKILL_MAGIC_SUMMONING;
            break;
        }

        //If the skill is already a focused skill.
        if(is_focused_skill(skill)){
            //Look through the focused skills.
            for(int i=0;i<3;i++){
                //If this skill is associated with this focused skill.
                if(skill==focused_skills[i]){
                    //Clear this focused skill.
                    focused_skills[i]=-1;
                }
            }
        }
        //If the skill is not already a focused skill.
        else{
            //Look through the focused skills.
            for(int i=0;i<3;i++){
                //If this focused skill slot is empty.
                if(focused_skills[i]==-1){
                    //Set this focused skill slot to the skill.
                    focused_skills[i]=skill;

                    break;
                }
            }
        }
    }

    else if((event.key.keysym.sym==SDLK_RETURN || event.key.keysym.sym==SDLK_KP_ENTER) && focused_skills[0]!=-1 && focused_skills[1]!=-1 && focused_skills[2]!=-1){
        //Once the focused skills are set, apply their initial bonuses to their corresponding skills.
        for(int i=0;i<3;i++){
            gain_skill_experience(focused_skills[i],skills[focused_skills[i]][SKILL_EXPERIENCE_MAX]-skills[focused_skills[i]][SKILL_EXPERIENCE],0,false);
        }

        game.new_game();

        //Make sure that enter is cleared before moving to the next screen.
        Uint8 *keystates=SDL_GetKeyState(NULL);
        keystates[SDLK_RETURN]=NULL;
        keystates[SDLK_KP_ENTER]=NULL;
    }
}

void Player::handle_input_inventory(){
    //Close the window.
    if(event.key.keysym.sym==SDLK_i || event.key.keysym.sym==SDLK_ESCAPE || event.key.keysym.sym==SDLK_SPACE){
        current_window=WINDOW_NONE;
    }
}

void Player::handle_input_stats(){
    //Close the window.
    if(event.key.keysym.sym==SDLK_F1 || event.key.keysym.sym==SDLK_ESCAPE || event.key.keysym.sym==SDLK_SPACE){
        current_window=WINDOW_NONE;
    }
}

void Player::handle_input_levelup(){
    if(event.key.keysym.unicode>=(Uint16)'a' && event.key.keysym.unicode<=(Uint16)'f'){
        short attribute=-1;

        switch(event.key.keysym.unicode){
        case (Uint16)'a':
            attribute=ATTRIBUTE_STRENGTH;
            break;
        case (Uint16)'b':
            attribute=ATTRIBUTE_AGILITY;
            break;
        case (Uint16)'c':
            attribute=ATTRIBUTE_HARDINESS;
            break;
        case (Uint16)'d':
            attribute=ATTRIBUTE_COMPREHENSION;
            break;
        case (Uint16)'e':
            attribute=ATTRIBUTE_ACUMEN;
            break;
        case (Uint16)'f':
            attribute=ATTRIBUTE_LUCK;
            break;
        }

        //If this attribute has already been selected.
        if(levelup_is_selected_attribute(attribute)){
            //Look through the selected attributes.
            for(int i=0;i<levelup_attributes.size();i++){
                //If this attribute is associated with this selected attribute.
                if(attribute==levelup_attributes[i]){
                    //Clear this selected attribute.
                    levelup_attributes[i]=-1;
                }
            }
        }
        //If the attribute has not been selected.
        else{
            //Look through the selected attributes.
            for(int i=0;i<levelup_attributes.size();i++){
                //If this attribute slot is empty.
                if(levelup_attributes[i]==-1){
                    //Set this attribute slot to the attribute.
                    levelup_attributes[i]=attribute;

                    break;
                }
            }
        }
    }

    else if((event.key.keysym.sym==SDLK_RETURN || event.key.keysym.sym==SDLK_KP_ENTER) && levelup_all_attributes_set()){
        //Apply the selected attribute bonuses.
        for(int i=0;i<levelup_attributes.size();i++){
            attributes[levelup_attributes[i]]+=1+attribute_level_bonuses[levelup_attributes[i]];
        }

        current_window=WINDOW_NONE;

        //Clear the attribute bonuses.
        for(int i=0;i<ATTRIBUTE_LUCK+1;i++){
            attribute_level_bonuses[i]=0;
        }

        //If experience reaches the current maximum experience.
        if(experience>=experience_max){
            //The creature levels up again.
            level_up();
        }

        //Make sure that enter is cleared before moving to the next screen.
        Uint8 *keystates=SDL_GetKeyState(NULL);
        keystates[SDLK_RETURN]=NULL;
        keystates[SDLK_KP_ENTER]=NULL;
    }
}

void Player::render_no_game(){
    //If no player name has yet been entered.
    if(name=="\x1F"){
        render_get_name();
    }
    //If no race has been selected.
    else if(race==-1){
        render_get_race();
    }
    else{
        render_get_focused_skills();
    }
}

void Player::render_get_name(){
    ss.clear();ss.str("");ss<<"Welcome to Escape from the Master's Lair!";ss<<"\xA";ss<<"\xA";msg=ss.str();
    ss.clear();ss.str("");ss<<"Who are you? ";ss<<get_name;ss<<"\xA";msg+=ss.str();

    font_small.show(0,0,msg,COLOR_WHITE);

    font_small.show(13*font_small.spacing_x+font_small.spacing_x*get_name.length(),font_small.spacing_y*2,"\x7F",COLOR_WHITE,cursor_opacity*0.1);
}

void Player::render_get_race(){
    ss.clear();ss.str("");ss<<"What race do you want to be? ";ss<<get_race;ss<<"\xA";msg=ss.str();

    font_small.show(0,0,msg,COLOR_WHITE);

    font_small.show(29*font_small.spacing_x+font_small.spacing_x*get_race.length(),0,"\x7F",COLOR_WHITE,cursor_opacity*0.1);

    msg="";

    //Keeps track of the number of lines of inventory text rendered.
    int lines_rendered=0;

    //The maximum number of lines before a new column is started.
    int max_lines=35;

    //The current column.
    int column=0;

    //The width of each column.
    int column_width=200;

    for(int i=0;i<templates.template_races.size();i++){
        //If the maximum number of lines for this column have been rendered.
        if(lines_rendered>=max_lines){
            lines_rendered=0;
            column++;
            i--;
            continue;
        }

        ss.clear();ss.str("");ss<<i;ss<<" - ";ss<<templates.template_races[i].name;ss<<"\xA";msg=ss.str();

        font_small.show(5+column*column_width,font_small.spacing_y+font_small.spacing_y*lines_rendered++,msg,COLOR_WHITE);
    }
}

void Player::render_get_focused_skills(){
    ss.clear();ss.str("");ss<<"Select 3 skills to focus in.";ss<<"\xA";msg=ss.str();

    font_small.show((main_window.SCREEN_WIDTH-msg.length()*font_small.spacing_x)/2,0,msg,COLOR_WHITE);

    msg="";

    ss.clear();ss.str("");ss<<"(a) ";msg+=ss.str();
    if(is_focused_skill(SKILL_BLADED_WEAPONS)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Bladed Weapons";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(b) ";msg+=ss.str();
    if(is_focused_skill(SKILL_BLUNT_WEAPONS)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Blunt Weapons";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(c) ";msg+=ss.str();
    if(is_focused_skill(SKILL_STABBING_WEAPONS)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Stabbing Weapons";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(d) ";msg+=ss.str();
    if(is_focused_skill(SKILL_UNARMED)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Unarmed";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(e) ";msg+=ss.str();
    if(is_focused_skill(SKILL_SECURITY)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Security";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(f) ";msg+=ss.str();
    if(is_focused_skill(SKILL_STEALTH)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Stealth";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(g) ";msg+=ss.str();
    if(is_focused_skill(SKILL_LAUNCHER_WEAPONS)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Launcher Weapons";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(h) ";msg+=ss.str();
    if(is_focused_skill(SKILL_THROWN_WEAPONS)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Thrown Weapons";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(i) ";msg+=ss.str();
    if(is_focused_skill(SKILL_DUAL_WIELDING)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Dual Wielding";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(j) ";msg+=ss.str();
    if(is_focused_skill(SKILL_SPEED)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Speed";ss<<"\xA";msg+=ss.str();

    font_small.show(150,60,msg,COLOR_WHITE);

    msg="";

    ss.clear();ss.str("");ss<<"(k) ";msg+=ss.str();
    if(is_focused_skill(SKILL_FIGHTING)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Fighting";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(l) ";msg+=ss.str();
    if(is_focused_skill(SKILL_DODGING)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Dodging";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(m) ";msg+=ss.str();
    if(is_focused_skill(SKILL_ARMOR)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Armor";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(n) ";msg+=ss.str();
    if(is_focused_skill(SKILL_MAGIC_AIR)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Air Magic";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(o) ";msg+=ss.str();
    if(is_focused_skill(SKILL_MAGIC_FIRE)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Fire Magic";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(p) ";msg+=ss.str();
    if(is_focused_skill(SKILL_MAGIC_WATER)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Water Magic";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(q) ";msg+=ss.str();
    if(is_focused_skill(SKILL_MAGIC_COLD)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Cold Magic";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(r) ";msg+=ss.str();
    if(is_focused_skill(SKILL_MAGIC_EARTH)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Earth Magic";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(s) ";msg+=ss.str();
    if(is_focused_skill(SKILL_MAGIC_CONJURATION)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Conjuration Magic";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(t) ";msg+=ss.str();
    if(is_focused_skill(SKILL_MAGIC_ENCHANTMENT)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Enchantment Magic";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(u) ";msg+=ss.str();
    if(is_focused_skill(SKILL_MAGIC_SUMMONING)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Summoning Magic";ss<<"\xA";msg+=ss.str();

    font_small.show(450,60,msg,COLOR_WHITE);
}

void Player::render_levelup(){
    ss.clear();ss.str("");ss<<"You have gained a level!";msg=ss.str();

    font_small.show((main_window.SCREEN_WIDTH-msg.length()*font_small.spacing_x)/2,0,msg,COLOR_WHITE);

    ss.clear();ss.str("");ss<<"Select ";msg=ss.str();
    ss.clear();ss.str("");ss<<levelup_attributes.size();msg+=ss.str();
    if(levelup_attributes.size()==1){
        ss.clear();ss.str("");ss<<" attribute ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<" attributes ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"to improve.";ss<<"\xA";msg+=ss.str();

    font_small.show((main_window.SCREEN_WIDTH-msg.length()*font_small.spacing_x)/2,font.spacing_y,msg,COLOR_WHITE);

    msg="";

    ss.clear();ss.str("");ss<<"(a) ";msg+=ss.str();
    if(levelup_is_selected_attribute(ATTRIBUTE_STRENGTH)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Strength";msg+=ss.str();
    ss.clear();ss.str("");ss<<" (+";ss<<1+attribute_level_bonuses[ATTRIBUTE_STRENGTH];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(b) ";msg+=ss.str();
    if(levelup_is_selected_attribute(ATTRIBUTE_AGILITY)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Agility";msg+=ss.str();
    ss.clear();ss.str("");ss<<" (+";ss<<1+attribute_level_bonuses[ATTRIBUTE_AGILITY];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(c) ";msg+=ss.str();
    if(levelup_is_selected_attribute(ATTRIBUTE_HARDINESS)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Hardiness";msg+=ss.str();
    ss.clear();ss.str("");ss<<" (+";ss<<1+attribute_level_bonuses[ATTRIBUTE_HARDINESS];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(d) ";msg+=ss.str();
    if(levelup_is_selected_attribute(ATTRIBUTE_COMPREHENSION)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Comprehension";msg+=ss.str();
    ss.clear();ss.str("");ss<<" (+";ss<<1+attribute_level_bonuses[ATTRIBUTE_COMPREHENSION];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(e) ";msg+=ss.str();
    if(levelup_is_selected_attribute(ATTRIBUTE_ACUMEN)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Acumen";msg+=ss.str();
    ss.clear();ss.str("");ss<<" (+";ss<<1+attribute_level_bonuses[ATTRIBUTE_ACUMEN];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"(f) ";msg+=ss.str();
    if(levelup_is_selected_attribute(ATTRIBUTE_LUCK)){
        ss.clear();ss.str("");ss<<"+ ";msg+=ss.str();
    }
    else{
        ss.clear();ss.str("");ss<<"- ";msg+=ss.str();
    }
    ss.clear();ss.str("");ss<<"Luck";msg+=ss.str();
    ss.clear();ss.str("");ss<<" (+";ss<<1+attribute_level_bonuses[ATTRIBUTE_LUCK];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    font_small.show(330,60,msg,COLOR_WHITE);
}

void Player::render_stats(){
    string title="";

    title=name;
    title+=" the ";
    title+="<class>";
    render_rectangle(5,5,font_small.spacing_x*title.length()+3,font_small.spacing_y,1.0,COLOR_GRAY);
    font_small.show(6,8,title,COLOR_BLACK);

    ss.clear();ss.str("");ss<<"Experience Level: ";ss<<experience_level;ss<<"\xA";msg=ss.str();
    ss.clear();ss.str("");ss<<"Experience: ";ss<<experience;ss<<"/";ss<<experience_max;ss<<"\xA";msg+=ss.str();
    ss.clear();ss.str("");ss<<"Health: ";ss<<return_health();ss<<"/";ss<<return_health_max();ss<<"\xA";msg+=ss.str();
    ss.clear();ss.str("");ss<<"Mana: ";ss<<return_mana();ss<<"/";ss<<return_mana_max();ss<<"\xA";msg+=ss.str();
    ss.clear();ss.str("");ss<<"Armor: ";ss<<return_armor();ss<<"\xA";msg+=ss.str();
    ss.clear();ss.str("");ss<<"Money: ";ss<<inventory[0].stack;ss<<"\xA";msg+=ss.str();

    font_small.show(5,30,msg,COLOR_WHITE);

    title="Attributes";
    render_rectangle(5,200,font_small.spacing_x*title.length()+3,font_small.spacing_y,1.0,COLOR_GRAY);
    font_small.show(6,203,title,COLOR_BLACK);

    ss.clear();ss.str("");ss<<"Strength - ";ss<<return_attribute_strength();ss<<"\xA";msg=ss.str();
    ss.clear();ss.str("");ss<<"Agility - ";ss<<return_attribute_agility();ss<<"\xA";msg+=ss.str();
    ss.clear();ss.str("");ss<<"Hardiness - ";ss<<return_attribute_hardiness();ss<<"\xA";msg+=ss.str();
    ss.clear();ss.str("");ss<<"Comprehension - ";ss<<return_attribute_comprehension();ss<<"\xA";msg+=ss.str();
    ss.clear();ss.str("");ss<<"Acumen - ";ss<<return_attribute_acumen();ss<<"\xA";msg+=ss.str();
    ss.clear();ss.str("");ss<<"Luck - ";ss<<return_attribute_luck();ss<<"\xA";msg+=ss.str();

    font_small.show(5,225,msg,COLOR_WHITE);

    title="Skills";
    render_rectangle(400,5,font_small.spacing_x*title.length()+3,font_small.spacing_y,1.0,COLOR_GRAY);
    font_small.show(401,8,title,COLOR_BLACK);

    ss.clear();ss.str("");ss<<"Bladed Weapons - ";ss<<return_skill_bladed_weapons();ss<<" (";ss<<skills[SKILL_BLADED_WEAPONS][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_BLADED_WEAPONS][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg=ss.str();
    msg+="\xA";
    ss.clear();ss.str("");ss<<"Blunt Weapons - ";ss<<return_skill_blunt_weapons();ss<<" (";ss<<skills[SKILL_BLUNT_WEAPONS][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_BLUNT_WEAPONS][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";
    ss.clear();ss.str("");ss<<"Stabbing Weapons - ";ss<<return_skill_stabbing_weapons();ss<<" (";ss<<skills[SKILL_STABBING_WEAPONS][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_STABBING_WEAPONS][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";
    ss.clear();ss.str("");ss<<"Unarmed - ";ss<<return_skill_unarmed();ss<<" (";ss<<skills[SKILL_UNARMED][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_UNARMED][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"Security - ";ss<<return_skill_security();ss<<" (";ss<<skills[SKILL_SECURITY][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_SECURITY][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";
    ss.clear();ss.str("");ss<<"Stealth - ";ss<<return_skill_stealth();ss<<" (";ss<<skills[SKILL_STEALTH][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_STEALTH][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";
    ss.clear();ss.str("");ss<<"Launcher Weapons - ";ss<<return_skill_launcher_weapons();ss<<" (";ss<<skills[SKILL_LAUNCHER_WEAPONS][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_LAUNCHER_WEAPONS][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";
    ss.clear();ss.str("");ss<<"Thrown Weapons - ";ss<<return_skill_thrown_weapons();ss<<" (";ss<<skills[SKILL_THROWN_WEAPONS][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_THROWN_WEAPONS][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";
    ss.clear();ss.str("");ss<<"Dual Wielding - ";ss<<return_skill_dual_wielding();ss<<" (";ss<<skills[SKILL_DUAL_WIELDING][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_DUAL_WIELDING][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";
    ss.clear();ss.str("");ss<<"Speed - ";ss<<return_skill_speed();ss<<" (";ss<<skills[SKILL_SPEED][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_SPEED][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();

    font_small.show(200,30,msg,COLOR_WHITE);

    ss.clear();ss.str("");ss<<"Fighting - ";ss<<return_skill_fighting();ss<<" (";ss<<skills[SKILL_FIGHTING][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_FIGHTING][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg=ss.str();
    msg+="\xA";
    ss.clear();ss.str("");ss<<"Dodging - ";ss<<return_skill_dodging();ss<<" (";ss<<skills[SKILL_DODGING][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_DODGING][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"Armor - ";ss<<return_skill_armor();ss<<" (";ss<<skills[SKILL_ARMOR][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_ARMOR][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"Air Magic - ";ss<<return_skill_magic_air();ss<<" (";ss<<skills[SKILL_MAGIC_AIR][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_MAGIC_AIR][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";
    ss.clear();ss.str("");ss<<"Fire Magic - ";ss<<return_skill_magic_fire();ss<<" (";ss<<skills[SKILL_MAGIC_FIRE][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_MAGIC_FIRE][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";
    ss.clear();ss.str("");ss<<"Water Magic - ";ss<<return_skill_magic_water();ss<<" (";ss<<skills[SKILL_MAGIC_WATER][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_MAGIC_WATER][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";
    ss.clear();ss.str("");ss<<"Cold Magic - ";ss<<return_skill_magic_cold();ss<<" (";ss<<skills[SKILL_MAGIC_COLD][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_MAGIC_COLD][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";

    ss.clear();ss.str("");ss<<"Earth Magic - ";ss<<return_skill_magic_earth();ss<<" (";ss<<skills[SKILL_MAGIC_EARTH][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_MAGIC_EARTH][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";
    ss.clear();ss.str("");ss<<"Conjuration Magic - ";ss<<return_skill_magic_conjuration();ss<<" (";ss<<skills[SKILL_MAGIC_CONJURATION][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_MAGIC_CONJURATION][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";
    ss.clear();ss.str("");ss<<"Enchantment Magic - ";ss<<return_skill_magic_enchantment();ss<<" (";ss<<skills[SKILL_MAGIC_ENCHANTMENT][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_MAGIC_ENCHANTMENT][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();
    msg+="\xA";
    ss.clear();ss.str("");ss<<"Summoning Magic - ";ss<<return_skill_magic_summoning();ss<<" (";ss<<skills[SKILL_MAGIC_SUMMONING][SKILL_EXPERIENCE];ss<<"/";ss<<skills[SKILL_MAGIC_SUMMONING][SKILL_EXPERIENCE_MAX];ss<<")";ss<<"\xA";msg+=ss.str();

    font_small.show(500,30,msg,COLOR_WHITE);
}

void Player::render_inventory(){
    msg="";
    short render_color=COLOR_WHITE;

    //Keeps track of the number of lines of inventory text rendered.
    int lines_rendered=0;

    //The maximum number of lines before a new column is started.
    int max_lines=35;

    //The current column.
    int column=0;

    //The width of each column.
    int column_width=400;

    ss.clear();ss.str("");ss<<"Weight/Capacity: ";ss<<return_inventory_weight();ss<<"/";ss<<return_carry_capacity();msg=ss.str();
    font_small.show((main_window.SCREEN_WIDTH-msg.length()*font_small.spacing_x)/2,0,msg,render_color);

    ss.clear();ss.str("");ss<<"You are currently ";ss<<return_encumbrance_state();ss<<".";msg=ss.str();
    font_small.show((main_window.SCREEN_WIDTH-msg.length()*font_small.spacing_x)/2,font_small.spacing_y,msg,render_color);

    for(short n=ITEM_WEAPON;n<ITEM_OTHER+1;n++){
        //If the maximum number of lines for this column have been rendered.
        if(lines_rendered>=max_lines){
            lines_rendered=0;
            column++;
            n--;
            continue;
        }

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

            render_rectangle(5+column*column_width,font_small.spacing_y*2+font_small.spacing_y*lines_rendered,font_small.spacing_x*title.length()+3,font_small.spacing_y,1.0,COLOR_GRAY);
            font_small.show(6+column*column_width,font_small.spacing_y*2+3+font_small.spacing_y*lines_rendered++,title,COLOR_BLACK);

            for(int i=0;i<inventory.size();i++){
                //If the maximum number of lines for this column have been rendered.
                if(lines_rendered>=max_lines){
                    lines_rendered=0;
                    column++;
                    i--;
                    continue;
                }

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
                            str_item+=" oil)";
                        }
                        else{
                            str_item+="out of oil)";
                        }
                    }

                    ss.clear();ss.str("");ss<<" ";ss<<inventory[i].inventory_letter;ss<<" - ";ss<<item_amount_prefix;ss<<inventory[i].return_full_name();ss<<str_item;ss<<"\xA";msg=ss.str();

                    font_small.show(5+column*column_width,font_small.spacing_y*2+2+font_small.spacing_y*lines_rendered++,msg,render_color);
                }
            }
        }
    }
}
