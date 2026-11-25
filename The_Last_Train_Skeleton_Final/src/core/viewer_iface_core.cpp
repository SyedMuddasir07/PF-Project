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
                         printHeader("Status");
                        
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