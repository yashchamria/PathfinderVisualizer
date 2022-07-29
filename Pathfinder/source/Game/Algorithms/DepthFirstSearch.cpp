#include "FrameworkPCH.h"
#include "DepthFirstSearch.h"
#include "AlgorithmEnum.h"

#include "Game/Scene.h"
#include "Game/Objects/Grid/Grid.h"
#include "Game/Objects/Grid/Tile.h"
#include "Game/Objects/Grid/TileType.h"
#include "Game/Objects/Grid/Direction.h"

DepthFirstSearch::DepthFirstSearch(Grid* pGrid) : BaseAlgorithm(pGrid) {}

DepthFirstSearch::~DepthFirstSearch()
{
	Stop();
}

void DepthFirstSearch::OnInit()
{
	//Intializing all the tiles to unvisited.
	for (int x = 0; x < GetGrid()->GridSize.x; x++)
	{
		for (int y = 0; y < GetGrid()->GridSize.y; y++)
		{
			Tile* pTile = GetGrid()->GetTile(sf::Vector2u(x, y)).get();

			if (pTile->Type == TileType::WallTile)
				continue;

			m_IsTileVisited.insert(std::pair <Tile*, bool>(pTile, false));
		}
	}

	//Dealing with the Start Tile
	AddToOpenlist(GetGrid()->GetStartTile().get());
}

void DepthFirstSearch::OnExecute()
{
	while (!m_pOpenTiles.empty() && !IsPathFound() && !IsExecutionStopped())
	{
		Tile* pTile = ProcessNextTile();

		if (pTile)
		{
			if (pTile->Coord == GetGrid()->GetEndTile()->Coord)
			{
				SetPathFound(true);
				break;
			}
		}

		AddToOpenlist(pTile);
	}
}

void DepthFirstSearch::AddToOpenlist(Tile* pTile)
{
	if (pTile == nullptr) { return; }

	m_pOpenTiles.push(pTile);

	AddToTileAnimationArray(pTile, TileAnimState::Processing);
}

Tile* DepthFirstSearch::GetNeighbourTile(Tile* pCurrentTile, Direction tileDirection)
{
	sf::Vector2u CurrentTileCoord = pCurrentTile->Coord;

	Tile* neighbourTile = GetGrid()->GetNeighborTile(CurrentTileCoord, tileDirection).get();

	if (neighbourTile)
	{
		if (neighbourTile->Type != TileType::WallTile && m_IsTileVisited[neighbourTile] == false)
		{
			return neighbourTile;
		}
	}

	return nullptr;
}

Tile* DepthFirstSearch::ProcessNextTile()
{
	Tile* pTile = m_pOpenTiles.top();

	m_IsTileVisited[pTile] = true;
	IncrementTileExplored();

	AddToTileAnimationArray(pTile, TileAnimState::Processed);

	Tile* nextNeighbourTile = GetNeighbourTile(pTile, Direction::Down);
	if (nextNeighbourTile) { AddToClosestPreviousTile(nextNeighbourTile, pTile); return nextNeighbourTile; }

	nextNeighbourTile = GetNeighbourTile(pTile, Direction::Up);
	if (nextNeighbourTile) { AddToClosestPreviousTile(nextNeighbourTile, pTile); return nextNeighbourTile; }

	nextNeighbourTile = GetNeighbourTile(pTile, Direction::Right);
	if (nextNeighbourTile) { AddToClosestPreviousTile(nextNeighbourTile, pTile); return nextNeighbourTile; }

	nextNeighbourTile = GetNeighbourTile(pTile, Direction::Left);
	if (nextNeighbourTile) { AddToClosestPreviousTile(nextNeighbourTile, pTile); return nextNeighbourTile; }

	//if no unvisited neighbour left, pop the front tile from the list.
	m_pOpenTiles.pop();

	return nullptr;
}

void DepthFirstSearch::OnStop()
{
	std::stack<Tile*> emptyTileQueue;
	std::swap(m_pOpenTiles, emptyTileQueue);

	m_IsTileVisited.clear();
}