#include "ExampleAIModule.h"
#include "BWEM 1.4.1\src\bwem.h"  
#include <iostream>
#include <sstream> 
#include <time.h>

using namespace BWAPI;
using namespace Filter;
using namespace BWEM;
//using namespace BWEM::BWAPI_ext;
using namespace BWEM::utils;

namespace { auto& theMap = BWEM::Map::Instance(); }



const Base *space;
/*PLANS
* 
* Lurker Contain at every choke and only attacks on supply counts
* Getting upgrades and making them come out by turning other shit into elses or by some other means
* Getting scorge out to actually be able to comebat air
* Maybe replacing lings with hydras due to how they stack up vs all 3 races
* turning some of the arrays into vectors without breaking everything
* making more youtube vids on it
* figuring out the drone transfer
* making use of "pulldrones" to get some stuff out of "onframe"
* making a building fail safe for when the don't make
* 
*/

Unit ExampleAIModule::PullDrones()
{
    for (int i = 0; i < droneCount; i++)
    {

        if (droneArry[i] != NULL && droneArry[i]->getType() == UnitTypes::Zerg_Drone && !droneArry[i]->isGatheringGas() && !droneArry[i]->isConstructing())
        {
           
            //Broodwar->sendText("%s fffff", droneArry[i]->getType().c_str());
            return droneArry[i];
        }
       

    }
    system("pause");
    return NULL;
}

int ExampleAIModule::FakeResources(int reReq)
{
    int Resource;

    if (reReq == 0)
    {
        Resource = Broodwar->self()->minerals();
    }else if (reReq == 1)
    {
        Resource = Broodwar->self()->gas();
    }else if (reReq == 2)
    {
        Resource = Broodwar->self()->supplyTotal() /2;
        
    }
    
     return Resource - preSpent[reReq];
}



void ExampleAIModule::BuildingBuildings(BWAPI::Unit unit, BWAPI::UnitType bigbee, TilePosition buildPosition)
{
   
    if (buildPosition.x == NULL)
    {
        buildPosition = Broodwar->getBuildLocation(bigbee, unit->getTilePosition());

    }

    
        
        unit->build(bigbee, buildPosition);
        //Broodwar->sendText("%s", bigbee.c_str());
        preSpent[0] += bigbee.mineralPrice();
        preSpent[1] += bigbee.gasPrice();
        Broodwar->sendText("Holy Fuck %s", unit->isConstructing() ? "True" : "False");
       
        
    
    /*else if (unit->getType().isBuilding())
    {
        unit->train(bigbee);
        Broodwar->sendText("%s", bigbee.c_str());
        preSpent[0] += bigbee.mineralPrice();
        preSpent[1] += bigbee.gasPrice();
        Broodwar->sendText("overtime");
    }*/
    
}

void ExampleAIModule::onStart()
{
    //system("pause");
    try {

        // Hello World!
        Broodwar->sendText("Hey lol");

        // Print the map name.
        // BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
        Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;

        // Enable the UserInput flag, which allows us to control the bot and type messages.
        Broodwar->enableFlag(Flag::UserInput);




        Broodwar->setLocalSpeed(10);

        // Uncomment the following line and the bot will know about everything through the fog of war (cheat).
        Broodwar->enableFlag(Flag::CompleteMapInformation);

        // Set the command optimization level so that common commands can be grouped
        // and reduce the bot's APM (Actions Per Minute).
        Broodwar->setCommandOptimizationLevel(2);

        // Check if this is a replay
        if (Broodwar->isReplay())
        {

            // Announce the players in the replay
            Broodwar << "The following are fucking:" << std::endl;

            // Iterate all the players in the game using a std:: iterator
            Playerset players = Broodwar->getPlayers();
            for (auto p : players)
            {
                // Only print the player if they are not an observer
                if (!p->isObserver())
                    Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;
            }

        }
        else // if this is not a replay
        {
            // Retrieve you and your enemy's races. enemy() will just return the first enemy.
            // If you wish to deal with multiple enemies then you must use enemies().
            if (Broodwar->enemy()) // First make sure there is an enemy
                Broodwar << "The matchup is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;

            Broodwar << "Map initialization..." << std::endl;

            theMap.Initialize();
            theMap.EnableAutomaticPathAnalysis();
            bool startingLocationsOK = theMap.FindBasesForStartingLocations();
            assert(startingLocationsOK);

            BWEM::utils::MapPrinter::Initialize(&theMap);
            BWEM::utils::printMap(theMap);      // will print the map into the file <StarCraftFolder>bwapi-data/map.bmp
            BWEM::utils::pathExample(theMap);   // add to the printed map a path between two starting locations

            BWEM::utils::MapDrawer::ProcessCommand("all");
            Broodwar << "Done" << std::endl;


            int closebase = 10000;

            for (const Area& area : theMap.Areas())
            {
                for (const Base& base : area.Bases())
                {
                    if (base.Starting())
                    {
                        continue;
                    }

                    //WalkPosition origin(base.Location());
                    //WalkPosition size(UnitType(UnitTypes::Terran_Command_Center).tileSize());	// same size for other races
                    //auto dashMode = base.BlockingMinerals().empty() ? MapPrinter::not_dashed : MapPrinter::dashed;
                    //MapPrinter::Get().Rectangle(origin, origin + size - 1, MapPrinter::Color::bases, MapPrinter::do_not_fill, dashMode);

                    
                    int dist = Broodwar->self()->getStartLocation().getApproxDistance(base.Location());

                    if (closebase > dist)
                    {
                        closebase = dist;
                        space = &base;
                    }
                }

                
            }

        }
        
    }
    catch(const std::exception & e)
    {
        Broodwar << "EXCEPTION: " << e.what() << std::endl;
    }
}

void ExampleAIModule::onEnd(bool isWinner)
{
  // Called when the game ends
  if ( isWinner )
  {
    // Log your win here!
  }
}

void ExampleAIModule::onFrame()
{
  // Called once every game frame
    try
    {
        BWEM::utils::gridMapExample(theMap);
        BWEM::utils::drawMap(theMap);
    }
    catch (const std::exception& e)
    {
        Broodwar << "EXCEPTION: " << e.what() << std::endl;
    }
  // Display the game frame rate as text in the upper left area of the screen
  Broodwar->drawTextScreen(0, 20,  "FPS: %d", Broodwar->getFPS() );
  Broodwar->drawTextScreen(0, 40, "Average FPS: %f", Broodwar->getAverageFPS() );
  int seconds = Broodwar->getFrameCount() / 24;
  int minutes = seconds / 60;
  seconds = seconds % 60;
  Broodwar->drawTextScreen(0, 60, "Ingame Time: %.2d:%.2d", minutes, seconds);

  Broodwar->drawTextScreen(500, 20, "Internal Minerals: %d ", FakeResources(0));
  Broodwar->drawTextScreen(500, 30, "Internal Gas: %d ", FakeResources(1));
  Broodwar->drawTextScreen(500, 40, "Internal Supply: %d ", FakeResources(2));
  Broodwar->drawTextScreen(500, 50, "Drone Count: %d", droneCount);

  
  
  for (int i = 0; i < droneCount; i++)
  {
      if (droneArry[i]->isConstructing() || droneArry[i]->isGatheringGas())
      {
          Broodwar->drawTextMap(droneArry[i]->getPosition(), "%c Holy shit", Text::Cyan);

      }else if (i == randDrone)
      {
          Broodwar->drawTextMap(droneArry[i]->getPosition(), "%c %s", Text::Orange, insults[i%6].c_str());
      } 

      if (timer <= Broodwar->getFrameCount() - 50)
      {
          timer = Broodwar->getFrameCount();
          randDrone = rand() % droneCount;
      }

     
  }
  
  //Broodwar->drawTextScreen(500, 70, ": %d",

  // Return if the game is a replay or is paused
  if ( Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self() )
    return;

  // Prevent spamming by only running our onFrame once every number of latency frames.
  // Latency frames are the number of frames before commands are processed.
  if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 )
    return;

  
  
  //Broodwar->sendText("%s", hatch == 0 ? "true":"false");

    
  // Iterate through all the units that we own
  for (auto &u : Broodwar->self()->getUnits())
  {
      if ((u->getType() == UnitTypes::Zerg_Lurker) && u->isBurrowed())
      {
          Broodwar->drawTextMap(u->getPosition(), "%c Invis", Text::Yellow);
      }
      
    // Ignore the unit if it no longer exists
    // Make sure to include this block when handling any Unit pointer!
    if ( !u->exists() )
      continue;
    




    // Ignore the unit if it has one of the following status ailments
    if ( u->isLockedDown() || u->isMaelstrommed() || u->isStasised() )
      continue;

    // Ignore the unit if it is in one of the following states
    if ( u->isLoaded() || !u->isPowered() || u->isStuck() )
      continue;

    // Ignore the unit if it is incomplete or busy constructing
    if (!u->isCompleted() || (u->getType() == UnitTypes::Zerg_Egg)) 
        continue;
      
     

    if (!u->isCompleted() || (u->getType() == UnitTypes::Zerg_Egg))
    {
        Broodwar->sendText("%s", u->getType().c_str());
    }




    // Finally make the unit do some stuff!
    


    if (u->getType() == UnitTypes::Zerg_Zergling)
    {
        if (u->getClosestUnit(Filter::IsEnemy && !IsFlying) != NULL)
        {
            u->attack(u->getClosestUnit(Filter::IsEnemy && !IsFlying)->getPosition());
        }
    }

    if (u->getType() == UnitTypes::Zerg_Hydralisk)
    {
        if (FakeResources(0) >= 50 && FakeResources(1) >= 100)
        {
            u->morph(UnitTypes::Zerg_Lurker);

        }
    }

    if (u->getType() == UnitTypes::Zerg_Lurker)
    {
        // Position closeEnemy = (u->getClosestUnit(Filter::IsEnemy)->getPosition()); 
        if (u->getClosestUnit(Filter::IsEnemy && !IsFlying) != NULL)
        {
            if (u->getDistance(u->getClosestUnit(Filter::IsEnemy && !IsFlying)->getPosition()) <= 180)
            {

                u->burrow();//idk

            }else if (u->getClosestUnit(Filter::IsEnemy && !IsFlying)->getPosition())
            {
                u->unburrow();
                u->move(u->getClosestUnit(Filter::IsEnemy && !IsFlying)->getPosition());
            }
        }
    }

    if ((u->getType() == UnitTypes::Zerg_Creep_Colony))
    {
        if (FakeResources(0) >= 50)
        {
            u->morph(UnitTypes::Zerg_Sunken_Colony);
           // sunken++;
        }

    }
    
    




    // If the unit is a worker unit
    if ( u->getType().isWorker() )
    {
        Position pos = u->getPosition();
        
       
      
        // build order

        if (u->isConstructing() || u->isGatheringGas())
        {
            continue;
        }
        if (u->getClosestUnit(Filter::IsEnemy && !IsFlying) != NULL)
        {
            if (u->getDistance(u->getClosestUnit(Filter::IsEnemy && !IsFlying)->getPosition()) <= 125)
            {
                Position enemypos = u->getClosestUnit(Filter::IsEnemy && !IsFlying)->getPosition();

                pos = { pos.x + (pos.x - enemypos.x),pos.y + (pos.y - enemypos.y) };

                u->move(pos);
            }
        }
        

        if (extractor == 0 && FakeResources(0) >= 50 && pool == 1 || extractor == 0 && FakeResources(0) >= 50 && pool == 2)//idk)
        {
            BuildingBuildings(u, UnitTypes::Zerg_Extractor);
            extractor = 1;
            Broodwar->sendText("Extractor %s", u->isConstructing() ? "True" : "False");

        }else if (pool == 0 && FakeResources(0) >= UnitTypes::Zerg_Spawning_Pool.mineralPrice())
        {
            BuildingBuildings(u, UnitTypes::Zerg_Spawning_Pool);
            pool = 1;
            Broodwar->sendText("Spawning Pool  %s", u->isConstructing() ? "True" : "False");

        }else if (pool == 2 && !den && FakeResources(0) >= UnitTypes::Zerg_Hydralisk_Den.mineralPrice() && FakeResources(1) >= UnitTypes::Zerg_Hydralisk_Den.gasPrice())
        {
            BuildingBuildings(u, UnitTypes::Zerg_Hydralisk_Den);
            den = 1;
            Broodwar->sendText("Hydra Den %s", u->isConstructing() ? "True" : "False");

        }else if (hatch == 0 && FakeResources(0) >= UnitTypes::Zerg_Hatchery.mineralPrice() || hatch == 1 && Hive == 1 && FakeResources(0) >= UnitTypes::Zerg_Hatchery.mineralPrice())
        {
            if (hatch == 1 && Hive == 1 )
            {
                BuildingBuildings(u, UnitTypes::Zerg_Hatchery);
                hatch = 2;
                Broodwar->sendText("Hatchery %s", u->isConstructing() ? "True" : "False");
            }
            else
            {
                Position pos(space->Location());
                u->move(pos);
                walkingWorker = u;
                hatch = 1;
                Broodwar->sendText("Exspanding %s", u->isMoving() ? "True" : "False");
            }

        }else if (queensNest == 0 && Lair == 2 && FakeResources(0) >= UnitTypes::Zerg_Queens_Nest.mineralPrice() && FakeResources(1) >= UnitTypes::Zerg_Queens_Nest.gasPrice())
        {
            BuildingBuildings(u, UnitTypes::Zerg_Queens_Nest);
            queensNest = 1;
            Broodwar->sendText("Queen's Nest %s", u->isConstructing() ? "True" : "False");

        }else if (cColony == 0 && extractor == 1 && FakeResources(0) >= UnitTypes::Zerg_Creep_Colony.mineralPrice())
        {
            BuildingBuildings(u, UnitTypes::Zerg_Creep_Colony);
            cColony++;
            Broodwar->sendText("Creep Colony %s", u->isConstructing() ? "True" : "False");
        }
      
       
        
       if (walkingWorker == u)
       {
           if (u->isIdle())
           {
               BuildingBuildings(u, UnitTypes::Zerg_Hatchery, space->Location());
               Broodwar->sendText("Hatchery %s", u->isConstructing() ? "True" : "False");
           }
       }
      

      // if our worker is idle
      if ( u->isIdle() && !u->isConstructing() && !u->isMorphing())
      {
            // Order workers carrying a resource to return them to the center,
            // otherwise find a mineral patch to harvest.


           
            if ( u->isCarryingGas() || u->isCarryingMinerals() )
            {
              u->returnCargo();
            }
            

           
           

          
          
          
             u->gather(u->getClosestUnit(IsMineralField));

              // If the call fails, then print the last error message
              //Broodwar << Broodwar->getLastError() << std::endl;
          

       } // closure: if idle

    }else if (u->getType() == (UnitTypes::Zerg_Larva))
    {
        if (FakeResources(2) <= 1.4 * (Broodwar->self()->supplyUsed() / 2) && u->isIdle() && FakeResources(0) >= 100)
        {
            u->train(UnitTypes::Zerg_Overlord);

            preSpent[2] = preSpent[2] - 8;

            




        }else if (FakeResources(2) > 1.4 * (Broodwar->self()->supplyUsed() / 2))
        {
            if ((FakeResources(0) >= 50) && droneCount < droneNeed)
            {
                    u->train(UnitTypes::Zerg_Drone);
                    
                
            }else if (FakeResources(0) >= 75 && FakeResources(1) >= 25 && den == 1 && (hydraCount < 6 || FakeResources(0) >= 125 && FakeResources(1) >= 125 && lurkerAspect == true))
            {
                
               u->train(UnitTypes::Zerg_Hydralisk);
                
               
            }else if (FakeResources(0) >= 50 && pool == 2 && Hive == 2 && FakeResources(1) < 299)
            {
                u->train(UnitTypes::Zerg_Zergling);
            }
         
        }


    } else if (u->getType() == UnitTypes::Zerg_Hatchery && u->isIdle()) // A resource depot is a Command Center, Nexus, or Hatchery
    {
         
        if (FakeResources(0) >= UnitTypes::Zerg_Lair.mineralPrice() && FakeResources(1) >= UnitTypes::Zerg_Lair.gasPrice() && Lair == 0 && pool == 2)
        {
            u->morph(UnitTypes::Zerg_Lair);
            
        }
           
         
    }else if (u->getType() == UnitTypes::Zerg_Lair && u->isIdle())
    {

        if (FakeResources(0) >= UnitTypes::Zerg_Hive.mineralPrice() && FakeResources(1) >= UnitTypes::Zerg_Hive.gasPrice() && Hive == 0 && queensNest == 2)
        {
            u->morph(UnitTypes::Zerg_Hive);
        }

    }else if (u->getType() == UnitTypes::Zerg_Hydralisk_Den)
    {
        if (u->isIdle() && Lair == 2 && FakeResources(0) >= TechTypes::Lurker_Aspect.mineralPrice() && FakeResources(1) >= TechTypes::Lurker_Aspect.gasPrice())
        {
            u->research(TechTypes::Lurker_Aspect);
            lurkerAspect = true;
        }

    }else if (u->getType() == UnitTypes::Zerg_Spawning_Pool && u->isIdle())
    {

        if (den == 1 && FakeResources(0) >= UpgradeTypes::Metabolic_Boost.mineralPrice() && FakeResources(1) >= UpgradeTypes::Metabolic_Boost.gasPrice())
        {
            u->upgrade(UpgradeTypes::Metabolic_Boost);

        }
        if (Hive == 2 && FakeResources(0) >= UpgradeTypes::Adrenal_Glands.mineralPrice() && FakeResources(1) >= UpgradeTypes::Adrenal_Glands.gasPrice())
        {
            u->upgrade(UpgradeTypes::Adrenal_Glands);
            
        }

    }
    
  }
 
  } // closure: unit iterator

    //find a location for spawning pool and construct it

  


void ExampleAIModule::onSendText(std::string text)
{
    BWEM::utils::MapDrawer::ProcessCommand(text);

  // Send the text to the game if it is not being processed.
  Broodwar->sendText("%s", text.c_str());

  
  // Make sure to use %s and pass the text as a parameter,
  // otherwise you may run into problems when you use the %(percent) character!

  if (text[0]>= '0' && text[0] <= '9')
  {
      std::stringstream wantedSpeed(text);
      int speedBoi;
      wantedSpeed >> speedBoi;
      Broodwar->setLocalSpeed(speedBoi);
  }
}

void ExampleAIModule::onReceiveText(BWAPI::Player player, std::string text)
{
  // Parse the received text
  Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void ExampleAIModule::onPlayerLeft(BWAPI::Player player)
{
  // Interact verbally with the other players in the game by
  // announcing that the other player has left.
  Broodwar->sendText("Later Nerd");
}

void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{

  // Check if the target is a valid position
  if ( target )
  {
    // if so, print the location of the nuclear strike target
    Broodwar << "Nuclear Launch Detected at " << target << std::endl;
  }
  else 
  {
    // Otherwise, ask other players where the nuke is!
    Broodwar->sendText("Where's the nuke?");
  }

  // You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void ExampleAIModule::onUnitDiscover(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitEvade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitShow(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitHide(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitCreate(BWAPI::Unit unit)
{
   

  if ( Broodwar->isReplay() )
  {
    // if we are in a replay, then we will print out the build order of the structures
    if ( unit->getType().isBuilding() && !unit->getPlayer()->isNeutral() )
    {
      int seconds = Broodwar->getFrameCount()/24;
      int minutes = seconds/60;
      seconds %= 60;
      Broodwar->sendText("%.2d:%.2d: %s creates a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
    }
  }
}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit)
{

    try
    {
        if (unit->getType().isMineralField())    theMap.OnMineralDestroyed(unit);
        else if (unit->getType().isSpecialBuilding()) theMap.OnStaticBuildingDestroyed(unit);
    }
    catch (const std::exception& e)
    {
        Broodwar << "EXCEPTION: " << e.what() << std::endl;
    }

    if (unit->getPlayer() == Broodwar->self())
    {
        if (unit->getType() == UnitTypes::Zerg_Drone)
        {
            //droneCount = droneCount - 1;
            droneNeed++;
            //Broodwar->sendText("one of the boiz died");

        }else if (unit->getType() == UnitTypes::Zerg_Sunken_Colony)
        {
            //cColony = 0;
            //sunken = 0;
            Broodwar->sendText("Sunken died");

        }else if (unit->getType() == UnitTypes::Zerg_Lair)
        {
            hatch = 0;
            Lair = 0;
            Broodwar->sendText("Lairs died");

        }else if (unit->getType() == UnitTypes::Zerg_Hive)
        {
            hatch = 0;
            Lair = 0;
            Hive = 0;
            Broodwar->sendText("Hives died");

        }else if (unit->getType() == UnitTypes::Zerg_Spawning_Pool)
        {
            pool = 0;
            Broodwar->sendText("Pools died");

        }else if (unit->getType() == UnitTypes::Zerg_Queens_Nest)
        {
            //queensNest = 0;
            Broodwar->sendText("Queen's Nest died");

        }else if (unit->getType() == UnitTypes::Zerg_Creep_Colony)
        {
            //cColony = cColony - 1;
            Broodwar->sendText("Creep Colony died");

        }else if (unit->getType() == UnitTypes::Zerg_Hatchery)
        {
            hatch = 0;
            Broodwar->sendText("Hatchery died");

        }else if (unit->getType() == UnitTypes::Zerg_Hydralisk_Den)
        {
            den = 0;
            Broodwar->sendText("Hydra Den died");

        }else if (unit->getType() == UnitTypes::Zerg_Extractor)
        {
            extractor = 0;
            Broodwar->sendText("Extracor died");

        }
    }
}
   


void ExampleAIModule::onUnitMorph(BWAPI::Unit unit)
{
   
    if (unit->getPlayer() == Broodwar->self())
    {
        
        //Broodwar->sendText("morphed: %s", unit->getType().c_str());
        if (unit->getType() == UnitTypes::Zerg_Lair || unit->getType() == UnitTypes::Zerg_Hive)
        {
            if (unit->getType() == UnitTypes::Zerg_Lair)
            {
                Lair = 1;
            }
            if (unit->getType() == UnitTypes::Zerg_Hive)
            {
                Hive = 1;
            }
            
            

        }else if (unit->getType().isBuilding() || unit->getType() == UnitTypes::Zerg_Extractor || unit->getType() == UnitTypes::Zerg_Sunken_Colony)
        {
            preSpent[0] = preSpent[0] - unit->getType().mineralPrice();
            preSpent[1] = preSpent[1] - unit->getType().gasPrice();
            //droneNeed++;

            if (unit->getType() == UnitTypes::Zerg_Sunken_Colony)
            {
                preSpent[0] = preSpent[0] - -50;
            }
            //droneCount = droneCount -1;
            
            
         

        }
        
    }


  if ( Broodwar->isReplay() )
  {
    // if we are in a replay, then we will print out the build order of the structures
    if ( unit->getType().isBuilding() && !unit->getPlayer()->isNeutral() )
    {
      int seconds = Broodwar->getFrameCount()/24;
      int minutes = seconds/60;
      seconds %= 60;
      Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
    }
  }
}

void ExampleAIModule::onUnitRenegade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onSaveGame(std::string gameName)
{
  Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void ExampleAIModule::onUnitComplete(BWAPI::Unit unit)
{
    if (unit->getPlayer() == Broodwar->self())
    {
        if (unit->getType() == UnitTypes::Zerg_Overlord)
        {
            preSpent[2] = preSpent[2] + 8;
        }
        else if (unit->getType() == UnitTypes::Zerg_Hatchery)
        {
            Broodwar->sendText("Hatchery done");

            if (!NULL && !transfer && hatch == 1)
            {
                
                for (int i = 0; i < 9; i++)
                {
                    PullDrones()->move(unit->getPosition());

                    Broodwar->sendText("Transfer done %d", i);
                    if (i == 9)
                    {
                        transfer = true;
                    }
                }
            }

        }else if (unit->getType() == UnitTypes::Zerg_Lair)
        {
            Lair = 2;
            Broodwar->sendText("Lairs done");

        }else if (unit->getType() == UnitTypes::Zerg_Hive)
        {
            Hive = 2;
            Broodwar->sendText("Hives done");

        }else if (unit->getType() == UnitTypes::Zerg_Spawning_Pool)
        {
            pool = 2;
            Broodwar->sendText("Pools done");

        }
        else if (unit->getType() == UnitTypes::Zerg_Queens_Nest)
        {
            queensNest = 2;
            Broodwar->sendText("Queen's Nest done");

        }else if (unit->getType() == UnitTypes::Zerg_Creep_Colony)
        {
            //cColony = 2;
            Broodwar->sendText("Creep Colony done");
           
        }
        else if (unit->getType() == UnitTypes::Zerg_Creep_Colony)
        {
            //sunken = 2;
            Broodwar->sendText("Sunken Colony done");
           
        }
        else if (unit->getType() == UnitTypes::Zerg_Extractor)
        {
            //currentRefinery = unit;
            
            refineryArry[refineryCount].Extractor = unit;
            refineryCount++;

            for(int i = 0; i < 3; i++)
            {
                PullDrones()->gather(unit);
            }
           
           
           
               
            

        }else if (unit->getType() == UnitTypes::Zerg_Hydralisk)
        {
            hydraCount++;

        }else if (unit->getType() == UnitTypes::Zerg_Drone)
        {

            droneArry[droneCount] = unit;
            droneCount++;
        }
    }
    
} 

