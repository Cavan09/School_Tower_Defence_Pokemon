#include "GameConstants.h"


//Game Constants
const char* GAME_SCREEN_NAME = "Game";


//Tiles Constants
const char* TILE_GROUND_TYPE = "GroundTile";
const char* TILE_TREE_TYPE = "TreeTile";
const char* TILE_ROCK_TYPE = "RockTile";
const char* TILE_WATER_TYPE = "WaterTile";
const char* TILE_GRASS_TYPE = "GrassTile";
const char* TILE_PAVED_TYPE = "PavedTile";
const int TILE_PADDING = 2;

//Player Constants

const int PLAYER_SIZE = 24;
const float PLAYER_SPEED = 150.0f;
const OpenGLColor PLAYER_INSIDE_COLOR = OpenGLColor(0.0f, 0.0f, 0.0f, 1.0f);
const OpenGLColor PLAYER_OUTLINE_COLOR = OpenGLColor(1.0f, 1.0f, 1.0f, 1.0f);

//Hero Constants
const char* HERO_TYPE = "Hero";
const char* MAIN_BASE_TYPE = "MainBase";
const char* BULBASAUR_TYPE = "Bulbasaur";
const char* SQUIRTLE_TYPE = "Squirtle";
const char* CHARMANDER_TYPE = "Charmander";

//Enemy Constants
const char* ENEMY_TYPE = "Enemy";
const int ENEMY_COUNT_EASY = 30;
const int ENEMY_COUNT_NORMAL = 60;
const int ENEMY_COUNT_HARD = 100;


//ammo pickup constants
const char* AMMO_PICKUP_TYPE = "AmmoPickup";