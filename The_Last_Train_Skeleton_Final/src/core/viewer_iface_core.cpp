#include "viewer_iface.h"
#include "util.h"

#include<iostream>
#include "level.h"
using namespace std;
// Terminal UI and dashboard rendering using primitive parameters
void renderDashboard(int playerFloor, int playerRoom, int energy, int time, int score,
                     const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                     int emergencyTripsLeft)
                     {
                         printHeader("Status");    // calling from util.h so that i get the header
                        
                         cout<<"F"<<playerFloor<<"-R"<<playerRoom<<" | Energy "<<energy
                         <<" | Time "<<time<<" | Score "<<score<<" | Emergency trips "<<emergencyTripsLeft<<'\n';
                         
                        
                        cout<<"Bag:";
                         if(inventoryCount ==0){
                           cout<<" empty";
                        }
                         else{
                            for(int i =0; i<inventoryCount;++i){
                              cout<<" ["<<inventory[i]<<"] "<<getEntityName(inventory[i]);  //here used function from level.h so that we  return the anme instead of symbol
                            }
                        }
                         cout<<"\n";
                     }
                   
void renderRoomDetails(int floor, int room,                 // used when the player uses the inspect command
                       const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],   // a 3D characte array is used to store the symbols of the entities in each room, const is used so that we can only read it
                       int entityCount)
                       {

                          cout<<"F"<<floor<<"-R"<<room<<" contains:";
                           if(entityCount ==0){
                             cout<<" nothing";
                           }
                           else{
                            for(int i=0; i<entityCount; ++i){
                                char symbol = roomEntities[floor-1][room-1][i];
                                cout<<" ["<<symbol<<"] "<<getEntityName(symbol);    // seding to level.h funstion to get full name and symbol

                           }
                        }
                        cout<<"\n";
                       }                     