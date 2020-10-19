#include "TileSet.h"
#include "Game.h"
#include "TextureManager.h"

CTileSet::CTileSet(int firstgid, Vector2 tileSize, int tileCount, int columns, string imgPath)
{
	this->firstgid = firstgid;
	this->tileSize = Vector2(tileSize.x, tileSize.y);
	this->tileCount = tileCount;
	this->columns = columns;
	//this->texture = TextureManager::Load(ToLPCWSTR(imgPath), D3DCOLOR_ARGB(0, 0, 0, 0));
}

CTileSet::CTileSet(TiXmlElement* data, string xmlPath)
{
	data->QueryIntAttribute("firstgid", &this->firstgid);
	data->QueryFloatAttribute("tilewidth", &this->tileSize.x);
	data->QueryFloatAttribute("tileheight", &this->tileSize.y);
	data->QueryIntAttribute("tilecount", &this->tileCount);
	data->QueryIntAttribute("columns", &this->columns);
	
	this->name = data->Attribute("name");
	
	TiXmlElement* imgDom = data->FirstChildElement("image");
	string imgPath = imgDom->Attribute("source");
	//this->texture = TextureManager::Load(ToLPCWSTR(imgPath), D3DCOLOR_ARGB(0, 0, 0, 0));
	CTextures::GetInstance()->Add("tileset_" + this->name, ToLPCWSTR("Resources/"+imgPath), D3DCOLOR_ARGB(0, 0, 0, 0));
	this->texture = CTextures::GetInstance()->Get("tileset_" + this->name);


	for (TiXmlElement* node = data->FirstChildElement("tile"); node != nullptr ; node = node->NextSiblingElement("tile"))
	{
		int id = 0; node->QueryIntAttribute("id", &id);
		TiXmlElement* objects = node->FirstChildElement("objectgroup");
		for (TiXmlElement* object = data->FirstChildElement("tile"); object != nullptr; object = object->NextSiblingElement("tile")) {
			if (object->Attribute("height") != NULL && object->NoChildren()) {
				RectF* rect = new RectF();
				object->QueryFloatAttribute("x", &rect->left);
				object->QueryFloatAttribute("y", &rect->top);
				object->QueryFloatAttribute("width", &rect->right);
				object->QueryFloatAttribute("height", &rect->bottom);

				rect->right = rect->right + rect->left;
				rect->bottom = rect->top + rect->bottom;

				this->blocks[id] = shared_ptr<RectF>(rect);
			}
		}
	}
}

int CTileSet::GetFirstGID()
{
	return firstgid;
}

shared_ptr<RectF> CTileSet::GetBlockBoundingBox(int id)
{
	if (id < firstgid) return nullptr;
	if (blocks.find(id) == blocks.end()) return nullptr;
	return blocks[id];
}

void CTileSet::Draw(int gid, Vector2 finalPos)
{
	if (gid < firstgid) return;
	RECT r;
	r.top = ((gid - firstgid) / columns) * tileSize.y;
	r.left = ((gid - firstgid) % columns) * tileSize.x;
	r.bottom = r.top + tileSize.y;
	r.right = r.left + tileSize.x;

	CGame::GetInstance()->Draw(finalPos, texture, r, 255);
}

CTileSet::~CTileSet()
{
	if (texture) {
		delete texture;
	}
	texture = NULL;
}
