#include "TileSet.h"
#include "Game.h"
#include "TextureManager.h"

CTileSet::CTileSet(int firstgid, Vec2 tileSize, int tileCount, int columns, string imgPath)
{
	this->firstgid = firstgid;
	this->tileSize = Vec2(tileSize.x, tileSize.y);
	this->tileCount = tileCount;
	this->columns = columns;
	this->texture = TextureManager::Load(ToLPCWSTR(imgPath), D3DCOLOR_ARGB(0, 0, 0, 0));
}

CTileSet::CTileSet(TiXmlElement* data, string xmlPath)
{
	data->QueryIntAttribute("firstgid", &this->firstgid);
	data->QueryFloatAttribute("tilewidth", &this->tileSize.x);
	data->QueryFloatAttribute("tileheight", &this->tileSize.y);
	data->QueryIntAttribute("tilecount", &this->tileCount);
	data->QueryIntAttribute("columns", &this->columns);

	TiXmlElement* imgDom = data->FirstChildElement("image");
	string imgPath = xmlPath + "/" + imgDom->Attribute("source");
	this->texture = TextureManager::Load(ToLPCWSTR(imgPath), D3DCOLOR_ARGB(0, 0, 0, 0));

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

void CTileSet::Draw(int gid, float x, float y, Transform& transform, D3DCOLOR overlay)
{
	if (gid < firstgid) return;
	RECT r;
	r.top = ((gid - firstgid) / columns) * tileSize.y;
	r.left = ((gid - firstgid) % columns) * tileSize.x;
	r.bottom = r.top + tileSize.y;
	r.right = r.left + tileSize.x;

	CGame::GetInstance()->GetGraphic().Draw(x, y, texture, r, transform, overlay);
}

CTileSet::~CTileSet()
{
	if (texture) {
		delete texture;
	}
	texture = NULL;
}
