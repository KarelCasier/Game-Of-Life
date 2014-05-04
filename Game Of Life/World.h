#pragma once
#include <SFML\Graphics.hpp>

#include <array>
#include <cmath>

class World : public sf::Drawable, public sf::Transformable
{
public:
					World(unsigned int width, unsigned int height);
	void			simulate();
	void			switchCell(int x, int y);
	unsigned int	getGeneration() const;

	virtual void	draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	unsigned int	getNumLiveNeighbours(unsigned int x, unsigned int y);
	void			changeVertexColour(sf::Vertex* quad, sf::Color colour);

private:
	enum cellState
	{
		DEAD,
		ALIVE
	};

	sf::VertexArray						mVertices;

	sf::Vector2u						mGridSize;
	unsigned int						tSize;
	unsigned int						mGeneration;

	std::array<std::array<cellState, 100>, 100> stateArray;
	std::array<std::array<cellState, 100>, 100> newStateArray;
};
