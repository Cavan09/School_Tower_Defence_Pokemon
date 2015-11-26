//Student: Cavan MacPhail
// Creation Date: Oct 30,2013
// Course Number: GAM1514
// Professor: Brad Flood
// Purpose: Level Creation

#include "Level.h"
#include "Player.h"
#include "Hero.h"
#include "TowerTypes/MainBase.h"
#include "Enemy.h"
#include "PathFinder.h"
#include "Tiles/Tile.h"
#include "Tiles/GroundTile.h"
#include "Tiles/RockTile.h"
#include "Tiles/TreeTile.h"
#include "Tiles/WaterTile.h"
#include "Tiles/GrassTile.h"
#include "Tiles/PavedTile.h"
#include "TowerTypes/Charmander.h"
#include "TowerTypes/Squirtle.h"
#include "TowerTypes/Bulbasaur.h"
#include "PickUps/PickUp.h"
#include "PickUps/AmmoPickup.h"
#include "Game.h"
#include "../Math/GDRandom.h"
#include "../Constants/Constants.h"
#include "../Input/Input.h"
#include "../Screen Manager/ScreenManager.h"
#include <stdlib.h>
#include <fstream>
#include "../Constants/Game/GameConstants.h"
#include "../Utils/Logger/Logger.h"


Level::Level(bool isEditingLevel) :
    m_HorizontalTiles(0),
    m_VerticalTiles(0),
    m_TileSize(EMPTY_LEVEL_TILE_SIZE),
    m_PlayerStartingTileIndex(EMPTY_LEVEL_STARTING_PLAYER_TILE_INDEX),
    m_Hero(NULL),
	m_MainBase(NULL),
    m_Tiles(NULL),
    m_SelectedTileIndex(-1),
    m_PaintTileScoring(false),
    m_PaintTileIndexes(false)
{
    m_EnemyCount = ENEMY_COUNT_EASY;
    //Create the player object
    if(isEditingLevel == false)
    {
        m_MainBase = new MainBase(this,0.0f,50,"PokemonCenter");
        m_Timer = 1.0;
        m_SpawnDelay = 0.0;
        
        GDRandom random;
        random.randomizeSeed();
        
        //create the enemies
        for(int i = 0; i < m_EnemyCount; i++)
        {
            float speed = 50.0f;
			Enemy* enemy = new Enemy(this,speed,"Enemy");
            m_Enemies.push_back(enemy);
        }
        m_SpawnPoints.push_back(320);
        m_SpawnPoints.push_back(223);
        m_SpawnPoints.push_back(319);
        m_SpawnPoints.push_back(639);
        m_SpawnPoints.push_back(703);
        
        
    }
    m_Currency = 100;
    
    //Calculate the number of horizontal and vertical tiles
    m_HorizontalTiles = ScreenManager::getInstance()->getScreenWidth() / m_TileSize;
    m_VerticalTiles = ScreenManager::getInstance()->getScreenHeight() / m_TileSize;
    
    //Allocate the tiles array, the inheriting class will populate this array with Tile objects
	m_Tiles = new Tile*[m_HorizontalTiles * m_VerticalTiles];
    
    //Initialize all the tiles to NULL
    for(int i = 0; i < m_HorizontalTiles * m_VerticalTiles; i++)
    {
        m_Tiles[i] = NULL;
    }
    
    //Load an empty level
    load(NULL);
}

Level::~Level()
{
    //Delete the player object
	if(m_MainBase != NULL)  
	{
		delete m_MainBase;
		m_MainBase = NULL;		
	}
    for( int i = 0; i < m_Enemies.size(); i++)
    {
        delete m_Enemies.at(i);
        m_Enemies.at(i) = NULL;
    }
    for( int i = 0; i < m_Charmander.size(); i++)
    {
        delete m_Charmander.at(i);
        m_Charmander.at(i) = NULL;
    }
    for( int i = 0; i < m_Squirtle.size(); i++)
    {
        delete m_Squirtle.at(i);
        m_Squirtle.at(i) = NULL;
    }
    for( int i = 0; i < m_Bulbasaur.size(); i++)
    {
        delete m_Bulbasaur.at(i);
        m_Bulbasaur.at(i) = NULL;
    }
    
    //Delete the tiles array, the inheriting class
    //must delete the object in this array itself
	if(m_Tiles != NULL)
	{
        //Cycle through and delete all the tile objects in the array
        for(int i = 0; i < getNumberOfTiles(); i++)
        {
            if(m_Tiles[i] != NULL)
            {
                delete m_Tiles[i];
                m_Tiles[i] = NULL;
            }
        }
        
		delete[] m_Tiles;
		m_Tiles = NULL;
	}
}

void Level::update(double aDelta)
{
    if(m_Timer > 0.0)
    {
        m_Timer -= aDelta;
        if(m_Timer <= 0.0)
        {
            m_Timer = 1.0;
            m_Currency += (int)m_Timer;
        }
    }
    
    m_SpawnDelay += aDelta;
        
	//Update all the game tiles
	for(int i = 0; i < getNumberOfTiles(); i++)
	{
		if(m_Tiles[i] != NULL)
		{
			m_Tiles[i]->update(aDelta);
		}
	}
    
	//Update the Player
	if(m_MainBase != NULL)
	{
		m_MainBase->update(aDelta);
	}
    
    //Update the enemy object
    for(int i = 0; i < m_Enemies.size(); i++)
    {
        if(m_Enemies.at(i)->getIsActive() == true)
        {
            if (m_SpawnDelay >= i)
            {
                m_Enemies.at(i)->update(aDelta);
            }
        }
        else
        {
            
        }
    }
    for(int i = 0; i < m_Charmander.size(); i++)
    {
        if(m_Charmander.at(i) != NULL && m_Charmander.at(i)->getIsActive() == true)
        {
            m_Charmander.at(i)->update(aDelta);
        }
        else
        {
            
        }
    }
    for(int i = 0; i < m_Squirtle.size(); i++)
    {
        if(m_Squirtle.at(i)->getIsActive() == true)
        {
            m_Squirtle.at(i)->update(aDelta);
        }
        else
        {
            
        }
    }
    for(int i = 0; i < m_Bulbasaur.size(); i++)
    {
        if(m_Bulbasaur.at(i) != NULL && m_Bulbasaur.at(i)->getIsActive() == true)
        {
            m_Bulbasaur.at(i)->update(aDelta);
        }
        else
        {
            
        }
    }
}

void Level::CheckForLevelComplete()
{
	int activeCounter = 0;

	for(int i = 0; i < m_Enemies.size(); i++)
	{
		if(m_Enemies[i]->getIsActive() == true)
		{
			activeCounter++;
		}
	}


}

void Level::paint()
{

	//Cycle through and paint all the tiles
	for(int i = 0; i < getNumberOfTiles(); i++)
	{
        //Safety check the tile
		if(m_Tiles[i] != NULL)
		{
            //Paint the tile
            m_Tiles[i]->paint();
            
            //If the paint tile indexes flag is set to true,
            //draw the index number on the tiles
            if(m_PaintTileIndexes == true)
            {
                m_Tiles[i]->paintIndex(i);
            }
		}
	}
    
	//Paint the Player
	if(m_MainBase != NULL && m_MainBase->getIsActive() == true)
	{
        //If paint tile scoring flag is set to true,
        //draw the path scoring        
        //Paint the player
		m_MainBase->paint();
	}
    for( int i = 0; i < m_Enemies.size(); i++)
    {
        if(m_Enemies.at(i)->getIsActive() == true)
        {
            m_Enemies.at(i)->paint();
        }
        else
        {
            
        }
    }
    for( int i = 0; i < m_Charmander.size(); i++)
    {
        if(m_Charmander.at(i) != NULL && m_Charmander.at(i)->getIsActive() == true)
        {
            m_Charmander.at(i)->paint();
        }
        else
        {
            
        }
    }
    for( int i = 0; i < m_Squirtle.size(); i++)
    {
        if(m_Squirtle.at(i)->getIsActive() == true)
        {
            m_Squirtle.at(i)->paint();
        }
        else
        {
            
        }
    }
    for( int i = 0; i < m_Bulbasaur.size(); i++)
    {
        if(m_Bulbasaur.at(i) != NULL && m_Bulbasaur.at(i)->getIsActive() == true)
        {
            m_Bulbasaur.at(i)->paint();
        }
        else
        {
            
        }
    }
    
}

void Level::reset()
{
	//Cycle through and reset all the tiles
	for(int i = 0; i < getNumberOfTiles(); i++)
	{
		if(m_Tiles[i] != NULL)
		{
			m_Tiles[i]->reset();
		}
	}
    
	//Reset the Player
    std::vector<Player*> players;
    if(m_MainBase != NULL)
    {
        players.push_back(m_MainBase);
    }
    
    for( int i = 0 ; i < m_Enemies.size(); i++)
    {
        players.push_back(m_Enemies.at(i));
    }
    
    //random number generator for the spawn indexx
    GDRandom random;
    random.randomizeSeed();
    int tileIndex = -1;
    std::vector<int> usedTileIndexes;
    int counter = 0;
    for(int i = 0; i < players.size(); i++)
    {
        
        
        //set tile index to -1
        tileIndex = -1;
        
        while (tileIndex == -1)
        {
            tileIndex = random.random(getNumberOfTiles());
            
            //safety check the it is walkable
            if(getTileForIndex(tileIndex)->isWalkableTile() == false)
            {
                tileIndex = -1;
            }
            else
            {
                for( int j = 0; j < usedTileIndexes.size(); j++)
                {
                    if(usedTileIndexes.at(j) == tileIndex)
                    {
                        tileIndex = -1;
                        break;
                    }
                }
                
                if(tileIndex != -1)
                {
                    if(counter >= m_SpawnPoints.size())
                    {
                        counter = 1;
                    }
                    
                        players.at(i)->setCurrentTile(getTileForIndex(m_SpawnPoints.at(counter)));
                        players.at(i)->reset();
                        usedTileIndexes.push_back(tileIndex);
                        counter++;
                    
                }
            }
            
        }
    }
    
}

void Level::mouseMovementEvent(float deltaX, float deltaY, float positionX, float positionY)
{
    if(m_Hero != NULL)
    {
        m_Hero->mouseMovementEvent(deltaX, deltaY, positionX, positionY);
    }
}

void Level::mouseLeftClickUpEvent(float aPositionX, float aPositionY)
{
            if(m_Hero != NULL)
            {
                m_Hero->mouseLeftClickUpEvent(aPositionX, aPositionY);
            }
}
void Level::mouseRightClickUpEvent(float positionX, float positionY)
{
    Tile* tile = getTileForPosition(positionX, positionY);
    if(tile != NULL && tile->isWalkableTile())
    {
        if(((Game*)ScreenManager::getInstance()->getScreenForName(GAME_SCREEN_NAME))->getSelectedToggle() == 2)
        {
            if(m_Currency >= 200)
            {
                Charmander* charmander = new Charmander(this,tile,0.0,100,"Charmander");
                m_Charmander.push_back(charmander);
                m_Currency -= 200;
            }
        }
        else if(((Game*)ScreenManager::getInstance()->getScreenForName(GAME_SCREEN_NAME))->getSelectedToggle() == 1)
        {
            if(m_Currency >= 100)
            {
                Squirtle* squirtle = new Squirtle(this,tile,0.0,50,"Squirtle");
                m_Squirtle.push_back(squirtle);
                m_Currency -= 100;
            }
        }
        else if(((Game*)ScreenManager::getInstance()->getScreenForName(GAME_SCREEN_NAME))->getSelectedToggle() == 0)
        {
            if(m_Currency >= 50)
            {
                Bulbasaur* bulbasaur = new Bulbasaur(this,tile,0.0,25,"Bulbasaur");
                m_Bulbasaur.push_back(bulbasaur);
                m_Currency -= 50;
            }
        }
        
    }
}

void Level::keyUpEvent(int keyCode)
{
    if(keyCode == KEYCODE_R)
    {
        reset();
    }
    else if(keyCode == KEYCODE_S)
    {
        togglePaintTileScoring();
    }
    else if(keyCode == KEYCODE_I)
    {
        togglePaintTileIndexes();
    }
    else if(keyCode == KEYCODE_D)
    {
        if(m_Hero != NULL)
        {
            m_Hero->getPathFinder()->togglePathFindingDelay();
        }
    }
    else
    {
        if(m_Hero != NULL)
        {
            m_Hero->keyUpEvent(keyCode);
        }
    }
}

void Level::load(const char* levelName)
{
    //If the level name isn't NULL load the level from the filesystem,
    //if it is NULL load an empty level with just ground tiles
    if(levelName != NULL)
    {
        std::ifstream inputStream;
        
        inputStream.open(levelName,std::ifstream::in | std::ifstream::binary);
        
        if(inputStream.is_open() == true)
        {
            inputStream.seekg(0, inputStream.end);
            long long bufferSize = inputStream.tellg();
            
            inputStream.seekg(0,inputStream.beg);
            
            char* buffer = new char[bufferSize];
            
            inputStream.read(buffer, (int)bufferSize);
            
            inputStream.close();
            
            for (int i = 0; i < bufferSize ; i++)
            {
                PickupType pickupType = PickupTypeUnknown;
                
                //check to see fi the buffer[i] constains the AmmoPickup bit
                if((buffer[i] & PickupTypeAmmo) > 0)
                {
                    //it deos
                    pickupType = PickupTypeAmmo;
                    
                    //clear the ammo pickup bit
                    buffer[i] &= ~PickupTypeAmmo;
                }
                
                //set the tile type
                setTileTypeAtIndex((TileType)buffer[i],i);
                
                //set the pickup type
                setPickupTypeAtIndex(pickupType, i);
            }
            
            if(buffer != NULL)
            {
                delete[] buffer;
                buffer = NULL;
            }
            
        }
    }
    else
    {
        for( int i = 0; i < getNumberOfTiles(); i++)
        {
            setTileTypeAtIndex((TileType)(TileTypeGround),i);
        }
    }
    
    //The level is loaded, reset everything
    reset();
}

void Level::save(const char* levelName)
{
    if(levelName != NULL)
    {
        std::ofstream outputStream;
        
        outputStream.open(levelName, std::ofstream::out);
        if(outputStream.is_open() == true)
        {
            size_t levelSize = getNumberOfHorizontalTiles() * getNumberOfVerticalTiles();
            char* levelData = new char[levelSize];
            for(int i = 0; i < levelSize; i++)
            {
                levelData[i] = m_Tiles[i]->getTileType();
                
                // save the tiles pickup if there is one
                if(m_Tiles[i]->getPickup() != NULL && m_Tiles[i]->getPickup()->getPickupType() != PickupTypeUnknown)
                {
                    levelData[i] |= m_Tiles[i]->getPickup()->getPickupType();
                }
            }
            
            outputStream.write((char*)levelData, levelSize * sizeof(char));
            outputStream.close();
            
            delete[] levelData;
        }
    }
    
}

TileType Level::getTileTypeForIndex(int index)
{
    if(index >= 0 && index < getNumberOfTiles())
    {
        return m_Tiles[index]->getTileType();
    }
    return TileTypeUnknown;
}

PickupType Level::getPickupTypeForIndex(int index)
{
    if(index >= 0 && index < getNumberOfTiles())
    {
        if(m_Tiles[index]->getPickup() != NULL)
        {
            return m_Tiles[index]->getPickup()->getPickupType();
        }
    }
    return PickupTypeUnknown;
}

unsigned int Level::getNumberOfTiles()
{
    return getNumberOfHorizontalTiles() * getNumberOfVerticalTiles();
}

unsigned int Level::getNumberOfHorizontalTiles()
{
	return m_HorizontalTiles;
}

unsigned int Level::getNumberOfVerticalTiles()
{
	return m_VerticalTiles;
}

bool Level::validateTileCoordinates(int aCoordinatesX, int aCoordinatesY)
{
    if(aCoordinatesX < 0 || aCoordinatesY < 0 || aCoordinatesX >= getNumberOfHorizontalTiles() || aCoordinatesY >= getNumberOfVerticalTiles())
	{
        return false;
    }
    return true;
}

int Level::getTileCoordinateForPosition(int aPosition)
{
	return (aPosition / m_TileSize);
}

int Level::getTileIndexForPosition(int aPositionX, int aPositionY)
{
	int coordinatesX = getTileCoordinateForPosition(aPositionX);
	int coordinatesY = getTileCoordinateForPosition(aPositionY);
	return getTileIndexForCoordinates(coordinatesX, coordinatesY);
}

int Level::getTileIndexForCoordinates(int aCoordinatesX, int aCoordinatesY)
{
	//Validate the coordinates, then calculate the array index
	if(validateTileCoordinates(aCoordinatesX, aCoordinatesY) == true)
	{
		return aCoordinatesX + (aCoordinatesY * getNumberOfHorizontalTiles());
	}
    
	return -1;
}

int Level::getTileIndexForTile(Tile* aTile)
{
	return getTileIndexForPosition(aTile->getX(), aTile->getY());
}
int Level::getTileIndexForPlayer(Player* player)
{
    return getTileIndexForPosition(player->getX(), player->getY());
}

Tile* Level::getTileForPosition(int aPositionX, int aPositionY)
{
	return getTileForIndex(getTileIndexForPosition(aPositionX, aPositionY));
}

Tile* Level::getTileForCoordinates(int aCoordinatesX, int aCoordinatesY)
{
	return getTileForIndex(getTileIndexForCoordinates(aCoordinatesX, aCoordinatesY));
}

Tile* Level::getTileForIndex(int aIndex)
{
    //Safety check the index bounds
	if(aIndex >= 0 && aIndex < getNumberOfTiles())
	{
		return m_Tiles[aIndex];
	}
    
    //If we got here, it means the index passed in was out of bounds
	return NULL;
}

Tile* Level::getTileForPlayer(Player* player)
{
    return getTileForPosition(player->getX(), player->getY());
}

void Level::setTileTypeAtPosition(TileType tileType, int positionX, int positionY)
{
    setTileTypeAtIndex(tileType, getTileIndexForPosition(positionX, positionY));
}

void Level::setTileTypeAtCoordinates(TileType tileType, int coordinatesX, int coordinatesY)
{
    setTileTypeAtIndex(tileType, getTileIndexForCoordinates(coordinatesX, coordinatesY));
}

void Level::setEnemyCount(int count)
{
    m_EnemyCount = count;
}

void Level::addCurency()
{
    m_Currency += 20;
}
void Level::setTileTypeAtIndex(TileType tileType, int index)
{
    //Safety check the index
    if(index >= 0 && index < getNumberOfTiles())
	{
        //Don't replace the tile if its the same type of tile that already exists
        if(m_Tiles[index] != NULL && m_Tiles[index]->getTileType() == tileType)
        {
            return;
        }
    
        //Delete the tile at the index, if one exists
        if(m_Tiles[index] != NULL)
        {
            delete m_Tiles[index];
            m_Tiles[index] = NULL;
        }
        
        //Create the new tile based on the TileType
        switch (tileType)
        {
            case TileTypeGround:
                m_Tiles[index] = new GroundTile();
                break;
			case TileTypeTree:
				m_Tiles[index] = new TreeTile();
				break;
			case TileTypeRock:
				m_Tiles[index] = new RockTile();
				break;
			case TileTypeWater:
				m_Tiles[index] = new WaterTile();
				break;
			case TileTypeGrass:
				m_Tiles[index] = new GrassTile();
				break;
			case TileTypePaved:
				m_Tiles[index] = new PavedTile();
				break;
            case TileTypeUnknown:
            default:
                m_Tiles[index] = NULL;
                break;
        }
        
        //Calculate the coordinates and set the tile position and size
        int coordinateX = (index % getNumberOfHorizontalTiles());
        int coordinateY = ((index - coordinateX) / getNumberOfHorizontalTiles());
        m_Tiles[index]->setPosition(coordinateX  * m_TileSize, coordinateY * m_TileSize);
        m_Tiles[index]->setSize(m_TileSize, m_TileSize);
	}
}

void Level::setPickupTypeAtPosition(PickupType pickupType, int positionX, int positionY)
{
    setPickupTypeAtIndex(pickupType, getTileIndexForPosition(positionX, positionY));
}

void Level::setPickupTypeAtCoordinates(PickupType pickupType, int coordinateX, int coordinateY)
{
    setPickupTypeAtIndex(pickupType, getTileIndexForCoordinates(coordinateX, coordinateY));
}

void Level::setPickupTypeAtIndex(PickupType pickupType, int index)
{
    //safety check the index
    if(index >= 0 && index < getNumberOfTiles())
    {
        //dont replace the tile if a pickup iof the same type already exisit there
        if(m_Tiles[index] != NULL && m_Tiles[index]->getPickup() != NULL)
        {
            if(m_Tiles[index]->getPickup()->getPickupType() == pickupType)
            {
                return;
            }
        }
        //delete the pickip at the index if on already exisits
        if(m_Tiles[index] != NULL)
        {
            if(m_Tiles[index]->getPickup() != NULL)
            {
                m_Tiles[index]->setPickup(NULL);
            }
            
            //create a new poickup onject based on the PickupType
            switch (pickupType)
            {
                case PickupTypeAmmo:
                    m_Tiles[index]->setPickup(new AmmoPickup(50));
                    break;
                //TODO: Make sure to ass new pickups
                    
                case PickupTypeUnknown:
                default:
                    break;
            }
            
            
            //set the pickups position and size
            if(m_Tiles[index]->getPickup() != NULL)
            {
                int coordinateX =  (index % getNumberOfHorizontalTiles());
                int coordinateY = ((index - coordinateX) / getNumberOfHorizontalTiles());
                
                Pickup* pickup = m_Tiles[index]->getPickup();
                float x = (coordinateX * m_TileSize) + (m_TileSize - pickup->getWidth()) / 2.0f;
                float y = (coordinateY * m_TileSize) + (m_TileSize -pickup->getHeight()) / 2.0f;
                pickup->setPosition(x, y);
            }
        }
    }
}



void Level::togglePaintTileScoring()
{
    m_PaintTileScoring = !m_PaintTileScoring;
}

void Level::togglePaintTileIndexes()
{
    m_PaintTileIndexes = !m_PaintTileIndexes;
}

void Level::setSelectedTileIndex(int aSelectedIndex)
{
	//Deselect the previously selected tile
	if(m_SelectedTileIndex >= 0 && m_SelectedTileIndex < getNumberOfTiles())
	{
		m_Tiles[m_SelectedTileIndex]->setIsSelected(false);
	}
    
	//Set the new tile index
	m_SelectedTileIndex = aSelectedIndex;
    
	//Selected the newly selected tile
	if(m_SelectedTileIndex >= 0 && m_SelectedTileIndex < getNumberOfTiles())
	{
		m_Tiles[m_SelectedTileIndex]->setIsSelected(true);
	}
}

int Level::getSelectedTileIndex()
{
    return m_SelectedTileIndex;
}

MainBase* Level::getMainBase()
{
    return m_MainBase;
}

std::vector<Enemy*> Level::getEnemies()
{
    return m_Enemies;
}
std::vector<Charmander*> Level::getCharmander()
{
    return m_Charmander;
}
std::vector<Squirtle*> Level::getSquirtle()
{
    return m_Squirtle;
}
std::vector<Bulbasaur*> Level::getBulbasaur()
{
    return m_Bulbasaur;
}

void Level::Randomize()
{
    GDRandom random;
    random.randomizeSeed();
    
    for( int i = 0; i < getNumberOfTiles(); i++)
    {
        setTileTypeAtIndex((TileType)random.random(TileTypeCount),i);
    }
}
int Level::getCurrency()
{
    return m_Currency;
}
void Level::gameOver()
{
    
}
