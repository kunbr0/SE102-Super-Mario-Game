#include "GameMap2.h"
#include "Game.h"
#include "RectCollision.h"
#include "RectPlatform.h"
#include "QuestionBlock.h"
#include "QuestionBlockItem.h"
#include "Venus.h"
#include "Coin.h"
#include "Goomba.h"
#include "Koopas.h"


#define marginXWindow	96
#define	marginYWindow	72

CGameMap::CGameMap()
{

}

CGameMap::CGameMap(int width, int height, int tileWidth, int tileHeight)
{
	this->width = width;
	this->height = height;
	this->tileHeight = tileHeight;
	this->tileWidth = tileWidth;
}

void CGameMap::UpdateCamPosition(Vector2 newPos) {
	camPosition = newPos;
}

Vector2 CGameMap::ConvertToPositionInCam(Vector2 oldPos) {
	return Vector2(oldPos.x - camPosition.x, oldPos.y - camPosition.y);
}

Vector2 CGameMap::GetBound()
{
	return Vector2(this->width * tileWidth, this->height * tileHeight);
}



CTileSet* CGameMap::GetTileSetByTileID(int id)
{
	
	
	if(!(id >= kRender.rangeId.x && id <= kRender.rangeId.y)) {
		for (int i = 0; i < tilesets.size(); i++) {
			Vector2 tilesetRangeId = tilesets[i]->GetRangeId();
			if (id >= tilesetRangeId.x && id <= tilesetRangeId.y) {
				kRender.rangeId.x = tilesetRangeId.x;
				kRender.rangeId.y = tilesetRangeId.y;
				kRender.tileset = tilesets[i];
			}
		}
	}
	

	return kRender.tileset;
}

void CGameMap::AddTileSet(int firstgid, CTileSet* tileSet)
{
	this->tilesets[firstgid] = tileSet;
}

void CGameMap::AddLayer(CMapLayer* layer)
{
	this->layers.push_back(layer);
}

void CGameMap::Update(int dt)
{
}

void CGameMap::Render(float bottomMargin)
{
	int col = this->camPosition.x / tileWidth;
	int row = this->camPosition.y / tileHeight;

	float screenWidth = CGame::GetInstance()->GetScreenWidth() + marginXWindow;
	float screenHeight = CGame::GetInstance()->GetScreenHeight() + marginYWindow - bottomMargin;
	Vector2 drawingSize = Vector2(screenWidth / tileWidth, screenHeight / tileHeight);
	
	for (int i = col; i < drawingSize.x + col; i++) {
		for (int j = row; j < drawingSize.y + row; j++) {

			int x = i * tileWidth + tileWidth / 2;
			int y = j * tileHeight + tileHeight / 2;


			

			for (CMapLayer* layer : layers) {
				if (layer->Hidden) continue;
				int id = layer->GetTileID(i % width, j % height);
				if (id > 0) {
					this->GetTileSetByTileID(id)->Draw(id, ConvertToPositionInCam(Vector2(x, y)));
				}
				
				
			}
		}
	}
}

CGameMap* CGameMap::FromTMX(string filePath, vector<LPGAMEOBJECT>* staticObjects, vector<LPGAMEOBJECT>* dynamicObjects, vector<LPGAMEOBJECT>* dynamicObjectsBehindMap, vector<LPGAMEOBJECT>* tempObjects)
{
	string fullPath = filePath;
	TiXmlDocument doc(fullPath.c_str());

	if (doc.LoadFile()) {
		TiXmlElement* root = doc.RootElement();
		CGameMap* gameMap = new CGameMap();

		root->QueryIntAttribute("width", &gameMap->width);
		root->QueryIntAttribute("height", &gameMap->height);
		root->QueryIntAttribute("tilewidth", &gameMap->tileWidth);
		root->QueryIntAttribute("tileheight", &gameMap->tileHeight);

		//Load tileset
		
		for (TiXmlElement* node = root->FirstChildElement("tileset"); node != nullptr; node = node->NextSiblingElement("tileset")) {
			CTileSet* tileSet = new CTileSet(node, filePath);
			
			gameMap->tilesets.push_back(tileSet);
		}

		//Load layer
		for (TiXmlElement* node = root->FirstChildElement("layer"); node != nullptr; node = node->NextSiblingElement("layer")) {
			CMapLayer* layer = new CMapLayer(node);
			gameMap->AddLayer(layer);
		}

		// Load collision group objects
		for (TiXmlElement* objGroupNode = root->FirstChildElement("objectgroup"); objGroupNode != nullptr; objGroupNode = objGroupNode->NextSiblingElement("objectgroup")) {
			if (std::string(objGroupNode->Attribute("name")) == "RectCollision") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CRectCollision(
						atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2,
						atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2,
						atoi(objNode->Attribute("width")),
						atoi(objNode->Attribute("height"))
					);
					staticObjects->push_back(obj);
				}
			}

			if (std::string(objGroupNode->Attribute("name")) == "RectPlatform") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CRectPlatform(
						atoi(objNode->Attribute("x")),
						atoi(objNode->Attribute("y")),
						atoi(objNode->Attribute("width")),
						atoi(objNode->Attribute("height"))
					);
					staticObjects->push_back(obj);
				}
			}

			if (std::string(objGroupNode->Attribute("name")) == "QuestionBox_Coin") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CQuestionBlock(
						Vector2(
							(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
							(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2))
						);
					staticObjects->push_back(obj);
				}
			}

			if (std::string(objGroupNode->Attribute("name")) == "QuestionBox_Item") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CQuestionBlockItem(
						Vector2(
							(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
							(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2))
						);
					staticObjects->push_back(obj);
				}
			}

			

			if (std::string(objGroupNode->Attribute("name")) == "Coin") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CCoin(
						Vector2(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
							(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2))
						);
					tempObjects->push_back(obj);
				}
			}

			if (std::string(objGroupNode->Attribute("name")) == "Venus") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CVenus(
						Vector2(
							(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
							(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2)
						), 
						Vector2(
							(int)atoi(objNode->Attribute("width")),
							(int)atoi(objNode->Attribute("height"))
						)
					);
					dynamicObjectsBehindMap->push_back(obj);
				}
			}


			if (std::string(objGroupNode->Attribute("name")) == "Goomba") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CGoomba(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2)
					);
					dynamicObjects->push_back(obj);
				}
			}

			if (std::string(objGroupNode->Attribute("name")) == "Koopas") {
				for (TiXmlElement* objNode = objGroupNode->FirstChildElement("object"); objNode != nullptr; objNode = objNode->NextSiblingElement("object")) {
					LPGAMEOBJECT obj = new CKoopas(
						(int)(atoi(objNode->Attribute("x")) + atoi(objNode->Attribute("width")) / 2),
						(int)(atoi(objNode->Attribute("y")) + atoi(objNode->Attribute("height")) / 2)
					);
					dynamicObjects->push_back(obj);
				}
			}
			
			
		}

		return gameMap;
	}

	throw "Load map that bai!!";
}

void CGameMap::GetMapSize(Vector2 &out) {
	out.x = this->width * tileWidth;
	out.y = this->height * tileHeight;
}

CGameMap::~CGameMap()
{
	layers.clear();
	tilesets.clear();
}
