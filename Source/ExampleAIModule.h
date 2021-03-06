#pragma once
#include <BWAPI.h>
#include "BWEM 1.4.1\src\bwem.h"  
#include <iostream>
#include <sstream> 
#include <time.h>
#include <cstdlib>
#include <algorithm> //sort

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
  virtual void BuildingBuildings(BWAPI::Unit unit, BWAPI::UnitType bigbee, int *stageint = 0, BWAPI::TilePosition buildPosition = {});
 // bool isOfType(BWAPI::Unit unit, BWAPI::UnitType type);
  //virtual bool disCalc(BWAPI::TilePosition a,BWAPI::TilePosition b);
  Unit ExampleAIModule::PullDrones();
  

  struct postTown
  {
	  UnitType building;
	  Unit walkingWorker = NULL;
  };

  std::vector<postTown> towns;

  int darkSwarmFrame = 0;
 

  int cColony = 0;
  int sunken = 0;

  int pool = 0;
  int queensNest = 0;
  int extractor = 0;
  int den = 0;
  int spire = 0;
  int mound = 0;

  int hatch = 0;
  int Lair = 0;
  int Hive = 0;

  int timer = 0;
  int randDrone = 0;

  std::vector<TilePosition> basesLoc;
  //Base bases;
  //const BWEM::Base* space;
 // std::vector<UnitType> untsque;
 
  int droneCount = 0;
  int droneNeed = 22;//idk
  int hydraCount = 0;
  int defilerCount = 0;

  int hatchCount = 0;


  int preSpent[3] = {0,0,-8};

  bool lurkerAspect = false;
  bool transfer = false;

 // std::string insults[6] = { "1", "2", "3", "idi nahoi","4","ironman btw"};
  
  std::vector<Unit> droneVec;
  
  
  Unit walkingWorker;
  
  int refineryCount = 0;
  Refinery refineryArry[30];
};
