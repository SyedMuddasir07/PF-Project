#include "logger.h"
#include "util.h"
#include"level.h"
#include<iostream>
#include<fstream>
using namespace std;

// File persistence for savegame.txt and game_summary.txt using primitive variables
bool saveGameStateToFile(const char* filename,              //saving the game to a file
                         int playerFloor, int playerRoom,
                         int energy, int time, int score,
                         const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                         const bool objectives[NUM_OBJECTIVES], int emergencyTripsLeft,
                         const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                         const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
 ofstream file(filename);
  if(!file) return false;

 file<<"SAVE"<<endl; //Tag for file
  file<<playerFloor<<' '<<playerRoom<<' '<<energy<<' '<<time<<' '<<score<<' '<<emergencyTripsLeft<<endl;

  file<<inventoryCount;
  for(int i=0;i<inventoryCount;i++){
        file<<' '<<inventory[i];    }
    file<<endl;

for(int i=0;i<NUM_OBJECTIVES;i++){
        file<<objectives[i]<<' ';    //  0 and 1 for obhjectives
    }
    file<<endl;


 for(int f=0;f<NUM_FLOORS;f++){    //check the room entities
        for(int r=0;r<ROOMS_PER_FLOOR;r++){
        file<<entityCounts[f][r];
            for(int i=0;i<entityCounts[f][r];i++){
            file<<' '<<roomEntities[f][r][i];
            }
            file<<endl;
        }
    }
  return file.good();  //everthing is written correctly
}

bool loadGameStateFromFile(const char* filename,           //loading saved game from a file
                           int& playerFloor, int& playerRoom,
                           int& energy, int& time, int& score,
                           char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                           bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                           char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                           int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
 ifstream in(filename);
        if(!in) return false;
    char head[10]={};   //reading the file
      in.width(10);
      in>>head;
        if(!stringEquals(head,"SAVE")) return false;

         int fl,rm,en,tm;
    int sc,tr,cnt;
              if(!(in>>fl>>rm>>en>>tm   //storig the read number in temp variable
         >>sc>>tr>>cnt)) return false;

          if(fl<1||fl>NUM_FLOORS) return false;
      if(rm<1||rm>ROOMS_PER_FLOOR) return false;
    if(en<0||en>MAX_ENERGY) return false;     //Validatation against the range
      if(tm<0||tm>MAX_TIME) return false;
    if(sc<0) return false;
       if(tr<0||tr>1) return false;
    if(cnt<0||cnt>MAX_INVENTORY_CAPACITY) return false;

     char t_inv[MAX_INVENTORY_CAPACITY]={};    //temp array
       bool t_obj[NUM_OBJECTIVES]={};
       char t_ent[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM]={};
    int t_cnt[NUM_FLOORS][ROOMS_PER_FLOOR]={};

 for(int i=0;i<cnt;i++){
      if(!(in>>t_inv[i])) return false;
        if(stringEquals(getEntityName(t_inv[i]),"Unknown")) return false;
    }

  for(int i=0;i<NUM_OBJECTIVES;i++){
         int flag;
          if(!(in>>flag)) return false;
        if(flag!=0&&flag!=1) return false;
           t_obj[i]=(flag==1);
    }

    for(int f=0;f<NUM_FLOORS;f++){
        for(int r=0;r<ROOMS_PER_FLOOR;r++){
            int n;
            if(!(in>>n)) return false;
            if(n<0||n>MAX_ENTITIES_PER_ROOM) return false;
            t_cnt[f][r]=n;

            for(int i=0;i<n;i++){
                if(!(in>>t_ent[f][r][i])) return false;
                if(stringEquals(getEntityName(t_ent[f][r][i]),"Unknown")) return false;
            }
        }
    }

playerFloor=fl;  //validation complete so copying into real game
   playerRoom=rm;
    energy=en;
  time=tm;
 score=sc;
  emergencyTripsLeft=tr;
 inventoryCount=cnt;
  for(int i=0;i<MAX_INVENTORY_CAPACITY;i++){
        if(i<cnt) inventory[i]=t_inv[i];
        else inventory[i]=ENT_NONE;
    }

 for(int i=0;i<NUM_OBJECTIVES;i++){
        objectives[i]=t_obj[i];
    }

 for(int f=0;f<NUM_FLOORS;f++){
        for(int r=0;r<ROOMS_PER_FLOOR;r++){
            entityCounts[f][r]=t_cnt[f][r];
            for(int i=0;i<t_cnt[f][r];i++){
                roomEntities[f][r][i]=t_ent[f][r][i];
            }
        }
    }
  return true;
}

void writeGameSummaryReport(const char* filename,          //writing the final report
                            int score, int remainingEnergy, int remainingTime,
                            const bool objectives[NUM_OBJECTIVES], bool victory)
{
      ofstream file(filename);
 if(!file) return;

 const char* status;
   if(victory) status="Victory";
 else if(objectives[OBJ_MISSION_FAILED]) status="Defeat";
    else status="Stopped";
    file<<"Game Report | "<<status<<endl;
    file<<"Score "<<score<<endl;
    file<<"Energy "<<remainingEnergy<<endl;
    file<<"Time "<<remainingTime<<endl;
    file<<"Passenger rescued "<<objectives[OBJ_PASSENGER_RESCUED]<<endl;
    file<<"Cat secured "<<objectives[OBJ_CAT_SECURED]<<endl;
}