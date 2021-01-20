#include "Grid.h"
#include "Const.h"
#include "Camera.h"
#include "Game.h"

using namespace std;

CGrid::CGrid(int w, int h) {
	numOfColumns = (int)ceil((float)w / (SCREEN_WIDTH >> 1));
	numOfRows = (int)ceil((float)h / (SCREEN_HEIGHT >> 1));
	

	for (int y = 0; y < numOfRows; ++y)
	{
		vector<LPCell> row = vector<LPCell>();
		for (int x = 0; x < numOfColumns; ++x)
		{
			row.push_back(new CCell(Vector2(x,y)));
		}
		gridCells.push_back(row);
	}
}

CGrid::~CGrid() {
	//if (activeCells)	delete activeCells;
}

void CGrid::AddObjectToGrid(CGameObject* obj) {
	LPCell cell = GetCellByPos(obj->GetPosition().x, obj->GetPosition().y);
	cell->AddObject(obj);
	obj->SetCell(cell);
}

void CGrid::AddObjectToGrid(CGameObject* obj, int x, int y) {
	LPCell cell = GetCellByPos(x, y);
	cell->AddObject(obj);
	obj->SetCell(cell);
}

vector<LPCell> CGrid::GetCellsInGrid() {
	activeCells.clear();
	CCamera* camera = CGame::GetInstance()->GetCurrentScene()->GetCamera();
	Vector2 camPos = camera->GetCamPosition();

	int startX = (int)((camPos.x - SCREEN_WIDTH / 2) / CCell::cellWidth);
	int endX = (int)((camPos.x + SCREEN_WIDTH * 3 / 2) / CCell::cellWidth);
	int startY = (int)((camPos.y - SCREEN_HEIGHT / 2) / CCell::cellHeight);
	int endY = (int)(camPos.y + SCREEN_HEIGHT * 3 / 2) / CCell::cellHeight;

	for (int i = startX; i <= endX; ++i)
	{
		if (i<0 || i>numOfColumns) continue;
		for (int j = startY; j <= endY; ++j)
		{
			if (j<0 || j > numOfRows) continue;
			activeCells.push_back(gridCells[j][i]);
		}
	}

	return activeCells;
}

vector<CGameObject*> CGrid::GetObjectsInGrid() {
	vector<CGameObject*> coObjects;
	auto cellsInGrid = GetCellsInGrid();

	for (size_t i = 0; i < cellsInGrid.size(); i++)
	{
		auto objInCell = cellsInGrid[i]->GetObjects();

		auto o = objInCell.begin();
		while (o != objInCell.end())
		{
			coObjects.push_back(*o);
			++o;
		}
	}
	return coObjects;
}

LPCell CGrid::GetCellByPos(float x, float y) {

	int cellX = (int)(x / CCell::cellWidth);
	int cellY = (int)(y / CCell::cellHeight);

	//Bound Check
	if (cellX < 0) cellX = 0;
	if (cellX >= numOfColumns) cellX = numOfColumns;

	if (cellY < 0) cellY = 0;
	if (cellY >= numOfRows) cellY = numOfRows;

	return gridCells[cellY][cellX]; //Treat 1D vector as 2D vector
}

void CGrid::Update() {
	UpdateGridObjects();
}

void CGrid::UpdateGridObjects() {
	vector<CGameObject*> gridObjects = GetObjectsInGrid();

	auto o = gridObjects.begin();
	while (o != gridObjects.end())
	{
		UpdateObjectCellPosInGrid(*o);
		++o;
	}
}

void CGrid::UpdateObjectCellPosInGrid(CGameObject* obj) {
	LPCell newCell = GetCellByPos(obj->GetPosition().x, obj->GetPosition().y);
	if (newCell != obj->GetCell())
	{
		obj->GetCell()->RemoveObject(obj);
		newCell->AddObject(obj);
		obj->SetCell(newCell);
	}
}