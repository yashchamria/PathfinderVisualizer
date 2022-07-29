#pragma once
#include "GameCore/Objects/GameObject.h"

enum class TileType : char;

class Tile : public GameObject
{
public:
	Tile(const sf::Vector2u coord, const float size, int weight, TileType type);
	virtual void Draw(const std::shared_ptr<sf::RenderWindow>& renderWindow) override;
	virtual void Update(float deltaTime) override;

public:
	void SetSizeAndPosition(float size);
	void Animate(const sf::Color color);

public:
	TileType Type;
	int Weight;
	const sf::Vector2u Coord {};

private:
	sf::RectangleShape m_body;

private:
	bool m_bAnimate {false};
};