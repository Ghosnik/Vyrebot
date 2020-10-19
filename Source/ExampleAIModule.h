#pragma once
#include <BWAPI.h>

using namespace BWAPI;

// Remember not to use "Broodwar" in any global class constructor!

class ExampleAIModule : public BWAPI::AIModule
{
public:
class Refinery
{
public:
	Unit Extractor;
	Unit gasDrones[3] = {};
};



  // Virtual functions for callbacks, leave these as they are.
  virtual void onStart(); 
  virtual void onEnd(bool isWinner);
  virtual void onFrame();
  virtual void onSendText(std::string text);
  virtual void onReceiveText(BWAPI::Player player, std::string text);
  virtual void onPlayerLeft(BWAPI::Player player);
  virtual void onNukeDetect(BWAPI::Position target);
  virtual void onUnitDiscover(BWAPI::Unit unit);
  virtual void onUnitEvade(BWAPI::Unit unit);
  virtual void onUnitShow(BWAPI::Unit unit);
  virtual void onUnitHide(BWAPI::Unit unit);
  virtual void onUnitCreate(BWAPI::Unit unit);
  virtual void onUnitDestroy(BWAPI::Unit unit);
  virtual void onUnitMorph(BWAPI::Unit unit);
  virtual void onUnitRenegade(BWAPI::Unit unit);
  virtual void onSaveGame(std::string gameName);
  virtual void onUnitComplete(BWAPI::Unit unit);
  // Everything below this line is safe to modify.
  virtual int FakeResources(int reReq);
  virtual void BuildingBuildings(BWAPI::Unit unit, BWAPI::UnitType bigbee);
  Unit ExampleAIModule::PullDrones();




  int pool = 0;
  int Lair = 0;
  int Hive = 0;
  int queensNest = 0;
  bool extractor = false;
  bool den = false;
  bool hatch = false;
  bool attack = false;

  
 
  int droneCount = 0;
  int droneNeed = 18;//idk
  int hydraCount = 0;
  int preSpent[3] = {0,0,-8};



  
  
  Unit droneArry[201] = {};
  
  
  int refineryCount = 0;
  Refinery refineryArry[30];
};
