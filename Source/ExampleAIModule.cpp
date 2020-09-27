#include "ExampleAIModule.h"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

void ExampleAIModule::onStart()
{
  // Hello World!
  Broodwar->sendText("Hey lol");

  // Print the map name.
  // BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
  Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;

  // Enable the UserInput flag, which allows us to control the bot and type messages.
  Broodwar->enableFlag(Flag::UserInput);

  Broodwar->setLocalSpeed(20);

  // Uncomment the following line and the bot will know about everything through the fog of war (cheat).
  Broodwar->enableFlag(Flag::CompleteMapInformation);

  // Set the command optimization level so that common commands can be grouped
  // and reduce the bot's APM (Actions Per Minute).
  Broodwar->setCommandOptimizationLevel(2);

  // Check if this is a replay
  if ( Broodwar->isReplay() )
  {

    // Announce the players in the replay
    Broodwar << "The following are fucking:" << std::endl;
    
    // Iterate all the players in the game using a std:: iterator
    Playerset players = Broodwar->getPlayers();
    for(auto p : players)
    {
      // Only print the player if they are not an observer
      if ( !p->isObserver() )
        Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;
    }

  }
  else // if this is not a replay
  {
    // Retrieve you and your enemy's races. enemy() will just return the first enemy.
    // If you wish to deal with multiple enemies then you must use enemies().
    if ( Broodwar->enemy() ) // First make sure there is an enemy
      Broodwar << "The matchup is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;
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

  // Display the game frame rate as text in the upper left area of the screen
  Broodwar->drawTextScreen(200, 0,  "FPS: %d", Broodwar->getFPS() );
  Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS() );

  // Return if the game is a replay or is paused
  if ( Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self() )
    return;

  // Prevent spamming by only running our onFrame once every number of latency frames.
  // Latency frames are the number of frames before commands are processed.
  if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 )
    return;




  /*if (isBuilding2 == false)
  {
      time = Broodwar->getFrameCount();
          
  }
  else if(time + 500 < Broodwar->getFrameCount()) {
      isBuilding2 = false;
      Broodwar->sendText("Done");
  }*/


  Unit currnetRefinery = NULL;
 
  // Iterate through all the units that we own
  for (auto &u : Broodwar->self()->getUnits())
  {
      
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

    if ((u->getType() == UnitTypes::Zerg_Zergling) && u->isIdle())
    {
        u->attack(u->getClosestUnit(Filter::IsEnemy)->getPosition());
    }
 



    if (u->getType().isRefinery())
    {
        currnetRefinery = u;
       
        if (Refinerycount < 3 )//&& u->isBeingGathered() == false)might not need the isBeingGathered part of it, downside is that stuck drones don't fix as fast
       {
           needsGasWorkers = true;
           
           
       }else if (u->isBeingGathered()==true)
       {
           needsGasWorkers = false;
          
       }





    }




    // If the unit is a worker unit
    if ( u->getType().isWorker() )
    {

        if (u->isConstructing())
        {
            Broodwar->sendText("Holy shit");
            isBuilding2 = true;
        }

      if (!pool && (Broodwar->self()->minerals() >= UnitTypes::Zerg_Spawning_Pool.mineralPrice()))
        {
            TilePosition buildPosition = Broodwar->getBuildLocation(BWAPI::UnitTypes::Zerg_Spawning_Pool, u->getTilePosition());
            u->build(UnitTypes::Zerg_Spawning_Pool, buildPosition);
            pool = true;
            isBuilding2 = true;
            Broodwar->sendText("pool");
        }



      if (!extractor && (Broodwar->self()->minerals() >= UnitTypes::Zerg_Extractor.mineralPrice()))
      {
          TilePosition buildPosition = Broodwar->getBuildLocation(BWAPI::UnitTypes::Zerg_Extractor, u->getTilePosition());
          u->build(UnitTypes::Zerg_Extractor, buildPosition);
          extractor = true;
          isBuilding2 = true;
          Broodwar->sendText("Extractor");
      }
      

      if (pool == true && !den && (Broodwar->self()->minerals() >= UnitTypes::Zerg_Hydralisk_Den.mineralPrice()) && (Broodwar->self()->gas() >= UnitTypes::Zerg_Hydralisk_Den.gasPrice()))
      {
          TilePosition buildPosition = Broodwar->getBuildLocation(BWAPI::UnitTypes::Zerg_Hydralisk_Den, u->getTilePosition());
          u->build(UnitTypes::Zerg_Hydralisk_Den, buildPosition);
          den = true;
          isBuilding2 = true;
          Broodwar->sendText("Hydra Den");
      }
      

      // if our worker is idle
      if ( u->isIdle() )
      {
            // Order workers carrying a resource to return them to the center,
            // otherwise find a mineral patch to harvest.



            if ( u->isCarryingGas() || u->isCarryingMinerals() )
            {
              u->returnCargo();
            }
            
            
           

          if (needsGasWorkers == true)
          {
              u->gather(currnetRefinery);

              
              Broodwar->sendText("Tried+1");
              if (u->isGatheringGas())
              {
                  Refinerycount++;
                  Broodwar->sendText("Worked");
              }

          }else
          {
              u->gather(u->getClosestUnit(IsMineralField));

              // If the call fails, then print the last error message
              //Broodwar << Broodwar->getLastError() << std::endl;
          }

       } // closure: if idle

    }
    else if ( u->getType().isResourceDepot() ) // A resource depot is a Command Center, Nexus, or Hatchery
    {

      // Order the depot to construct more workers! But only when it is idle.

       //if (pool == true && u->isIdle() && !u->train(UnitTypes::Zerg_Zergling))

      if (isBuilding2 == false && u->isIdle())
      {
         
          if (droneCount < 15)
          {
              u->train(UnitTypes::Zerg_Drone);
              
          }

          if (Broodwar->self()->supplyTotal() == Broodwar->self()->supplyUsed() && !u->isConstructing())
          {
          
              


              u->train(UnitTypes::Zerg_Overlord);
          }
      } // closure: insufficient supply
    } // closure: failed to train idle unit

  }

  } // closure: unit iterator

    //find a location for spawning pool and construct it

  


void ExampleAIModule::onSendText(std::string text)
{

  // Send the text to the game if it is not being processed.
  Broodwar->sendText("%s", text.c_str());


  // Make sure to use %s and pass the text as a parameter,
  // otherwise you may run into problems when you use the %(percent) character!

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
  Broodwar->sendText("Goodbye %s!", player->getName().c_str());
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
}

void ExampleAIModule::onUnitMorph(BWAPI::Unit unit)
{
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

        Broodwar->sendText("%s", unit->getType().c_str());

        if (unit->getType().isBuilding() || unit->getType() == UnitTypes::Zerg_Extractor)
        {
            isBuilding2 = false;

            //requiresCreep() == true)
        }
    }

    if (unit->getType() == UnitTypes::Zerg_Extractor)
    {
        Refinerycount = 0;
    }


    if (unit->getType().requiresCreep() == true)
    {
        Broodwar->sendText("Cool");
    }


    if (unit->getType() == UnitTypes::Zerg_Drone)
    {
        droneCount++;
        Broodwar->sendText("%d", droneCount);
    }
} 
