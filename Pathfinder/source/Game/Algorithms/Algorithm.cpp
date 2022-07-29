#include "FrameworkPCH.h"
#include "Algorithm.h"

#include "Game/Objects/Grid/Grid.h"
#include "Game/Scene.h"
#include "Game/Objects/Grid/Tile.h"
#include "Game/Objects/UI/Display.h"
#include "AlgorithmEnum.h"
#include "BreadthFirstSearch.h"
#include "DepthFirstSearch.h"
#include "DijkstrasAlgorithm.h"
#include "Astar.h"

Algorithms::Algorithms(Grid* pGrid, Scene* pScene)
{
	m_pGrid = pGrid;
	m_pScene = pScene;

	m_pBreathFirstSearch = new BreadthFirstSearch(pGrid);
	m_pDepthFirstSearch = new DepthFirstSearch(pGrid);
	m_pDijkstrasAlgorithm = new DijkstrasAlgorithm(pGrid);
	m_pAstar = new Astar(pGrid);
}

Algorithms::~Algorithms()
{
	m_pCurrentAlgorithm = nullptr;
	m_pGrid = nullptr;
	m_pScene = nullptr;

	delete m_pBreathFirstSearch;
	delete m_pDepthFirstSearch;
	delete m_pDijkstrasAlgorithm;
	delete m_pAstar;
}

bool Algorithms::Execute(AlgorithmType algorithmType)
{
	//Check for start and end tile
	if (m_pGrid->GetStartTile() == nullptr)
	{
		m_pScene->GetDisplay()->Log("Select Start Tile (Press 'S')");
		return false;
	} 
	if (m_pGrid->GetEndTile() == nullptr)
	{
		m_pScene->GetDisplay()->Log("Select End Tile (Press 'E')");
		return false;
	}
	if (m_pGrid->GetStartTile()->Coord == m_pGrid->GetEndTile()->Coord) { return false; }

	m_pScene->GetDisplay()->Log("Executing Algorithm");

	switch (algorithmType)
	{
	case AlgorithmType::Dijkstra:
		m_pCurrentAlgorithm = m_pDijkstrasAlgorithm;
		break;

	case AlgorithmType::DepthFirstSearch:
		m_pCurrentAlgorithm = m_pDepthFirstSearch;
		break;

	case AlgorithmType::BreadthFirstSearch:
		m_pCurrentAlgorithm = m_pBreathFirstSearch;
		break;

	case AlgorithmType::AStar:
		m_pCurrentAlgorithm = m_pAstar;
		break;
	}

	return m_pCurrentAlgorithm->Execute(algorithmType);
}

bool Algorithms::PlayVisualization(int speed, float deltaTime)
{
	if (IsPathFound())
	{
		m_pScene->GetDisplay()->Log("Path Found! Visualizing Path!");
	}
	else
	{
		m_pScene->GetDisplay()->Log("Path Not Found! Visualizing Path!");
	}

	if (m_pCurrentAlgorithm)
	{
		return m_pCurrentAlgorithm->PlayVisualization(speed, deltaTime);
	}

	return true;
}

void Algorithms::Stop()
{
	if (m_pCurrentAlgorithm)
	{
		m_pCurrentAlgorithm->Stop();
	}
}

AlgorithmState Algorithms::GetAlgorithmState()
{
	if (m_pCurrentAlgorithm)
	{
		return m_pCurrentAlgorithm->GetAlgorithmState();
	}

	return AlgorithmState::Executed;
}

bool Algorithms::IsPathFound()
{
	if (m_pCurrentAlgorithm)
	{
		return m_pCurrentAlgorithm->IsPathFound();
	}

	return false;
}

std::string Algorithms::GetAlgorithmName()
{
	if (m_pCurrentAlgorithm)
	{
		return m_pCurrentAlgorithm->GetAlgorithmName();
	}

	return "Not Selected!";
}

std::string Algorithms::GetTimeTaken()
{
	if (m_pCurrentAlgorithm)
	{
		return m_pCurrentAlgorithm->GetTimeTaken();
	}

	return "Not Selected!";
}

std::string Algorithms::GetTotalCost()
{
	if (m_pCurrentAlgorithm)
	{
		return m_pCurrentAlgorithm->GetTotalCost();
	}

	return "Not Selected!";
}

std::string Algorithms::GetTilesExplored()
{
	if (m_pCurrentAlgorithm)
	{
		return m_pCurrentAlgorithm->GetTilesExplored();
	}

	return "Not Selected!";
}