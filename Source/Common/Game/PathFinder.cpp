//
//  PathFinder.cpp
//  GAM-1532 OSX Game
//
//  Created by Bradley Flood on 2013-03-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "PathFinder.h"
#include "PathNode.h"
#include "Level.h"
#include "Tiles/Tile.h"
#include "../Constants/Constants.h"
#include <stdlib.h>
#include <algorithm>
#include <math.h>



PathFinder::PathFinder(Level* level, PathFinderListener* listener) :
    m_Level(level),
    m_Listener(listener),
    m_State(StateIdle),
    m_DestinationTileIndex(-1),
    m_SearchDelay(0.0),
    m_EnableSearchDelay(false)
{

}

PathFinder::~PathFinder()
{
    clearPathNodes();
}

void PathFinder::findPath(Tile* aCurrentTile, Tile* aDestinationTile)
{
    //Safty Check that the sate is Idle, if it isnt tgen a oath s aalready found
    if(m_State != StateIdle)
    {
        return;
    }
    
    for(int i = 0; i < m_Level->getNumberOfTiles(); i++)
    {
        Tile* tile = m_Level->getTileForIndex(i);
        if(tile != NULL && tile->isWalkableTile() == true)
        {
            tile->setIsPath(false);
        }
    }
    
    clearPathNodes();
    
    int currentTileIndex = m_Level->getTileIndexForTile(aCurrentTile);
    m_DestinationTileIndex = m_Level->getTileIndexForTile(aDestinationTile);
    
    //safety check that the destination tile and the current tile indexes are
    if(currentTileIndex == m_DestinationTileIndex)
    {
        return;
    }
    
    //Make sure the desitnation tle is walkable
    if(aDestinationTile->isWalkableTile() == false)
    {
        return;
    }
    
    //allocate the current tile's path node and add it to the open list
    PathNode* pathNode = new PathNode(aCurrentTile);
    addPathNodeToOpenList(pathNode);
    
    m_State = StateSearchingPath;
    
    m_SearchDelay = 0.0f;
}

void PathFinder::update(double aDelta)
{
    if(m_SearchDelay > 0.0)
    {
        m_SearchDelay -= aDelta;
        if(m_SearchDelay <= 0.0)
        {
            m_SearchDelay = 0.0;
        }
        return;
    }
    
    
    while(isSearchingPath() == true && m_DestinationTileIndex != -1)
    {
        //Safety check that there is actually something in the openlist
        if(m_PathNodeOpen.size() == 0)
        {
            //set the state to error
            m_State = StateError;
            
            //notify the listener
            if(m_Listener != NULL)
            {
                m_Listener->pathFinderFinishedSearching(this, false);
            }
            return;
        }
        
        //Get the first node  with the lowest f score
        //it should be the first element in the open list
        PathNode* currentNode = m_PathNodeOpen.front();
        
        //add the node tot he closed list and remove it foem the openlist
        m_PathNodeClosed.push_back(currentNode);
        m_PathNodeOpen.erase(m_PathNodeOpen.begin());
        
        //check to see if the node is at the destination tile
        int currentNodeTileIndex = m_Level->getTileIndexForTile(currentNode->getTile());
        if(currentNodeTileIndex == m_DestinationTileIndex)
        {
            // build the final node path, this will use the current nodes parent node
            //to track back through the path nodes all the way back to the start
            buildFinalNodePath(currentNode);
            
            //set the state ti path found
            m_State = StateFoundPath;
            
            //notify the listener
            if(m_Listener != NULL)
            {
                m_Listener->pathFinderFinishedSearching(this,true);
            }
            return;
        }
        
            //if it is we stop searching and build our final path
        
        //if we get here that eamn we haven't reached out destination tile we need
        //to get the adjecent tiles from the node and add them to the open list
        std::vector<Tile*> adjacentTiles;
        
            //check the tile above, is it walkable
        addAdjacentTile(adjacentTiles, currentNode->getTile(), 0, -1);
        
            // check the tile below, is it walkable
        addAdjacentTile(adjacentTiles, currentNode->getTile(),0,1);
        
            //check the tile tot he left, is it walkable
        addAdjacentTile(adjacentTiles, currentNode->getTile(),-1,0);
        
            //check the tile to the right, is it walkable
        addAdjacentTile(adjacentTiles, currentNode->getTile(),1,0);
        
        //cycle throug the adjacent tiles that are walkable
        for(int i = 0; i < adjacentTiles.size(); i++)
        {
            Tile* adjacentTile = adjacentTiles.at(i);
        
        
            //does the tile exist in the closed list
            if(doesTileExistInClosedList(adjacentTile) == true)
            {
        
                //if it does disregard
                continue;
            }
        
            //does the tile exist in the open list
            if(doesTileExistInOpenList(adjacentTile) == false)
            {
        
                //if it doesn't creat a new path node for the tile
                PathNode* adjacentNode = new PathNode(adjacentTile);
        
                //set the parent node
                adjacentNode->setParentNode(currentNode);
        
                // calculate the g and h scores
                adjacentNode->setScoreG(currentNode->getScoreG() + adjacentTile->getMovemnetCost());
                Tile* destinationTile = m_Level->getTileForIndex(m_DestinationTileIndex);
                int scoreH = getManhattanDistanceCost(adjacentTile, destinationTile);
                adjacentNode->setScoreH(scoreH);
        
                // add the tile to the open list and sort it
                addPathNodeToOpenList(adjacentNode);
            }
            else
            {
        
                //if it does exist in the open list compare the score and
                //keep the one with the lower f score, whoch really means
                //keep the one with the lower g score, since the H score will be identical
                PathNode* existingNode = getOpenPathNodeForTile(adjacentTile);
        
                    //if the tile has a lower f score, update the g score
                    //and update the parent node
                if(currentNode->getScoreG() + adjacentTile->getMovemnetCost() < existingNode->getScoreG())
                {
                    existingNode->setScoreG(currentNode->getScoreG() + adjacentTile->getMovemnetCost());
                    existingNode->setParentNode(currentNode);
                                    
                    //re-sort the open list
                    sortOpenList();
                }
        
                    
            }
        }
    }
    
    
    //If the search delay is enabled, set the delay timer
    if(m_EnableSearchDelay == true)
    {
        m_SearchDelay = PATH_FINDING_DELAY;
    }
}

void PathFinder::paint()
{
    //Paint the open list path scoring
    for(int i = 0; i < m_PathNodeOpen.size(); i++)
    {
        m_PathNodeOpen.at(i)->paintScore(OpenGLColorRed());
    }
    
    //Paint the closed list path scoring
    for(int i = 0; i < m_PathNodeClosed.size(); i++)
    {
        m_PathNodeClosed.at(i)->paintScore(OpenGLColorBlue());
    }
    
    //Paint the final path scoring
    for(int i = 0; i < m_PathNodeFinal.size(); i++)
    {
        m_PathNodeFinal.at(i)->paintScore(OpenGLColorYellow());
    }
}

void PathFinder::reset()
{
    m_State = StateIdle;
}

void PathFinder::addAdjacentTile(std::vector<Tile*>& aAdjacentTiles, Tile* aCurrentTile, int aDeltaX, int aDeltaY)
{
    int adjacentCoordinateX = m_Level->getTileCoordinateForPosition(aCurrentTile->getX()) + aDeltaX;
    int adjacentCoordinateY = m_Level->getTileCoordinateForPosition(aCurrentTile->getY()) + aDeltaY;
    
    Tile* adjacentTile = m_Level->getTileForCoordinates(adjacentCoordinateX, adjacentCoordinateY);
    if(adjacentTile != NULL)
    {
        //is the tile walkable and valid
        bool isWalkable = adjacentTile->isWalkableTile();
        bool isValid = m_Level->validateTileCoordinates(adjacentCoordinateX, adjacentCoordinateY);
        
        if(isWalkable == true && isValid == true)
        {
            aAdjacentTiles.push_back(adjacentTile);
        }
    }
}

bool PathFinder::doesTileExistInClosedList(Tile* aTile)
{
    //Get the tile index from the level for the tile pointer
    int tileIndex = m_Level->getTileIndexForTile(aTile);
    
    //Cycle through the closed list and compare the tile indexes
    for(int i = 0; i < m_PathNodeClosed.size(); i++)
    {
        PathNode* pathNode = m_PathNodeClosed.at(i);
        if(m_Level->getTileIndexForTile(pathNode->getTile()) == tileIndex)
        {
            return true;
        }
    }
    
    //The tile doesn't exist in the closed list
    return false;
}

bool PathFinder::doesTileExistInOpenList(Tile* aTile)
{
    return getOpenPathNodeForTile(aTile) != NULL;
}

PathNode* PathFinder::getOpenPathNodeForTile(Tile* aTile)
{
    //Get the tile index from the level for the tile pointer
    int tileIndex = m_Level->getTileIndexForTile(aTile);
    
    //Cycle through the open list and compare the tile indexes
    for(int i = 0; i < m_PathNodeOpen.size(); i++)
    {
        PathNode* pathNode = m_PathNodeOpen.at(i);
        if(m_Level->getTileIndexForTile(pathNode->getTile()) == tileIndex)
        {
            return pathNode;
        }
    }
    
    //The tile doesn't exist in the open list, return NULL
    return NULL;
}

bool PathFinder::isSearchingPath()
{
    return m_State == StateSearchingPath;
}

int PathFinder::getPathSize()
{
    return m_PathNodeFinal.size();
}

void PathFinder::sortOpenList()
{
    std::sort(m_PathNodeOpen.begin(),m_PathNodeOpen.end(), PathNode::compareNodes);
}

PathNode* PathFinder::getPathNodeAtIndex(int aIndex)
{
    if(aIndex >= 0 && aIndex < getPathSize())
    {
        return m_PathNodeFinal.at(aIndex);
    }
    return NULL;
}

void PathFinder::addPathNodeToOpenList(PathNode* aPathNode)
{
	//Insert the Path node into the Open path node vector
	m_PathNodeOpen.push_back(aPathNode);
    
    //Sort the open list
    sortOpenList();
}

void PathFinder::buildFinalNodePath(PathNode* aPathNode)
{
	do
	{
        //Safety check the parentNode
		if(aPathNode->getParentNode() != NULL)
		{
			m_PathNodeFinal.insert(m_PathNodeFinal.begin(), aPathNode);
		}
        
		//Set the tile path flag to true
		aPathNode->getTile()->setIsPath(true);
        
		//Set the path node's pointer to it's parent
		aPathNode = aPathNode->getParentNode();
	}
	while (aPathNode != NULL);
}

void PathFinder::clearPathNodes()
{
	//Now cycle through the Open node path vector, and delete all the path node
	while(m_PathNodeOpen.size() > 0)
	{
		//Get the last element in the vector
		PathNode* node = m_PathNodeOpen.back();
        
		//Delete the path node
		delete node;
        
		//Remove the last element in the vector
		m_PathNodeOpen.pop_back();
	}
    
	//Lastly cycle through the Closed node path vector, and delete all the path node
	while(m_PathNodeClosed.size() > 0)
	{
		//Get the last element in the vector
		PathNode* node = m_PathNodeClosed.back();
        
		//Delete the path node
		delete node;
        
		//Remove the last element in the vector
		m_PathNodeClosed.pop_back();
	}
    
    //Clear the final path node list
    m_PathNodeFinal.clear();
    
    //Reset the destination tile index
    m_DestinationTileIndex = -1;
}

void PathFinder::togglePathFindingDelay()
{
    m_EnableSearchDelay = !m_EnableSearchDelay;
}


int PathFinder::getManhattanDistanceCost(Tile* aStartTile, Tile* aDestinationTile)
{
	//Here we use the Manhattan method, which calculates the total number of step moved horizontally and vertically to reach the
	//final desired step from the current step, ignoring any obstacles that may be in the way
	int distance = abs(m_Level->getTileCoordinateForPosition(aDestinationTile->getX()) - m_Level->getTileCoordinateForPosition(aStartTile->getX())) 
    + abs(m_Level->getTileCoordinateForPosition(aDestinationTile->getY()) - m_Level->getTileCoordinateForPosition(aStartTile->getY()));
    
    //Return the distance between the two tiles
	return distance;
}