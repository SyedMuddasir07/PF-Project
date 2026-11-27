#include "level.h"
#include "util.h"
#include<cstring>
#include<iostream>
#include<fstream>
using namespace std;

// Modular implementation using primitive arrays for map parsing and verification
bool String_to_int(const char* text_string, int& final_answer)  // made a user-defined function so  that the string values can be coverted to int.

{
    if(text_string ==NULL || text_string[0]=='\0'){
        return false;     //string is empty or not 
    }

int position_index=0;
int sign_multiplier =1;
if (text_string[0]=='-'){
sign_multiplier=-1;
position_index=1;
}
  else if(text_string[0] =='+'){
position_index=1;
}
if(text_string[position_index]=='\0'){
    return false;   
} 
int total_value=0;

while (text_string[position_index]!='\0')   {
 char current_char= text_string[position_index];

if(current_char<'0' ||current_char>'9'){
    return false;      // the values must be valid between 0 and 9
 }
int digit_value =current_char -'0'; 
total_value=(total_value*10)+digit_value;
position_index++;
   }
final_answer= total_value*sign_multiplier;
return true;
}

const char*getEntityName(char sym){ // returns full name of item from the smbol
if(sym==ENT_MASTER_KEY){
return "Master Key";
}
else if(sym==ENT_ENERGY_PACK){
return "Energy Pack";    
}
else if(sym ==ENT_EMERGENCY_TOOLKIT){
return "Emergency Toolkit";    
}
else if(sym == ENT_CLUE_4){
    return "Clue 4";
}
else if (sym ==ENT_FUSE) {
   return "Fuse";
 }
  else if (sym ==ENT_CAT) {
 return "Cat";
 } 
 else if (sym ==ENT_CONTROL_PANEL) {
 return "Control panel";
 } 
 else if (sym ==ENT_LIFT_PASS) {
 return "Lift pass";
 }
 else if (sym ==ENT_PASSENGER) {
  return "Passenger";
}
else if (sym == ENT_MEDICAL_KIT) {
 return "Medical kit";
} else if (sym == ENT_EMERGENCY_CARD) {
        return "Emergency card";
}
 else if (sym == ENT_CLUE_2)
 {
        return "Clue 2";
 }
  else if (sym == ENT_CLUE_7) {
 return "Clue 7";
  }
   else if (sym == ENT_CABINET_TRAIN_KEY) {
        return "Cabinet / train key";
    } else if (sym == ENT_CAT_CARRIER) {
        return "Cat carrier";
    } 
    else if (sym == ENT_OBSERVATION_TOKEN) {
        return "Observation token";
    }
     else {
        
        return "Unknown";   
    }
}
bool loadBuildingMap(const char* filename,
                     char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                     char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                     int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){
                        ifstream map_file(filename);
                        if(!map_file){
                            return false;
                        }
                        char file_line[MAX_LINE_LEN];
                        bool floor_done[NUM_FLOORS];

                        for(int i=0;i<NUM_FLOORS;i++){   //floors are not loaded
                         floor_done[i]=false;
                    
                        }
                        int floors_count=0;


                       while(map_file.getline(file_line,MAX_LINE_LEN)){
                        int line_len =(int)strlen(file_line);  //lin by line read and charcter are saved in live_len
                
                       int r1_pos=-1;
                       for(int i =0; i<=line_len-3;i++){
                        if(file_line[i] =='-' && file_line[i+1]=='R' && file_line[i+2]=='1'){ // to check"-R1"
                          r1_pos=i;
                          break;
                      }
                     }
                      if(r1_pos==-1){
                      continue;
                        }
                        int start_col = r1_pos -2;                  //F#-R1 where F start 2 column befor -R1
                        if(start_col<2||file_line[start_col]!='F'){
                            continue;
                        }


                      int floor_idx=file_line[start_col+1]-'1'; //converting floor digits to the 0 based index(0-4)
                       if(floor_idx<0||floor_idx>=NUM_FLOORS){
                        return false;
                       }
                       if(floor_done[floor_idx]==true){
                        return false;       //  a duplicate floor is found here
                       }
                       if(line_len<start_col+47){
                        return false;//line too short for the four room
                       }
                       for(int room_idx=0; room_idx<ROOMS_PER_FLOOR;room_idx++){ // cheking the room labels to f#-R1 to F#-R4 and to save connection between them
                        int room_col=start_col+(12*room_idx);
                        char floor_char='1'+floor_idx;
                        char room_char ='1'+room_idx;

                        if(file_line[room_col] !='F'||file_line[room_col+1]!=floor_char||file_line[room_col+2]!='-'||file_line[room_col+3] !='R'||file_line[room_col+4] != room_char){
                            return false;                         // to validate the letter are accurate
                        }
                       
                        if(room_idx<ROOMS_PER_FLOOR-1){    // read the connection after room 1, 2 and 3.
                            char conn_char = file_line[room_col+10];
                            bool is_lift =(file_line[room_col+9]=='L'&& conn_char =='1');
                            bool valid_conn= (conn_char =='D'||conn_char=='W'||conn_char=='L'||conn_char =='#');
                            if(!is_lift && !valid_conn){
                                return false;
                            }
                            horizontalConn[floor_idx][room_idx]= conn_char;
                        }
                      }
                                                                        
                      if(!map_file.getline(file_line,MAX_LINE_LEN)){  //reading below header for room walls and items
                       return false;
                      }
                      int next_len =(int)strlen(file_line);
                      if(next_len <start_col+47){
                        return false;
                      }
                      for(int room_idx =0; room_idx <ROOMS_PER_FLOOR;room_idx++){
                        int left_wall= start_col -2+(12*room_idx);
                        int right_wall = left_wall +12;

                        if(file_line[left_wall] !='|'|| file_line[right_wall] !='|'){  // room box must start with an end
                            return false;
                        }
                        int item_count =0;

                       for (int scan_col = left_wall + 1; scan_col < right_wall; scan_col++) {
                            char item_char = file_line[scan_col];
                            if (item_char == ' ') { // skip empty space
                                continue;
                            }
                            if (stringEquals(getEntityName(item_char), "Unknown")) {
                                return false;
                            }
                            if (item_count >= MAX_ENTITIES_PER_ROOM) {
                                return false;
                            }
                            roomEntities[floor_idx][room_idx][item_count] = item_char; 
                            item_count++;
                        }

                        entityCounts[floor_idx][room_idx] = item_count; 
                    }

                    floor_done[floor_idx] = true;     // Mark Floor coplete after all 4 rooms are procesed
                    floors_count++;

                    if (floors_count == NUM_FLOORS) {
                        return true;
                    }
                } //end of while loop her

                return false; 
            }

            



