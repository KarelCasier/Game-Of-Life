#include "World.h"

#include <algorithm>
#include <cmath>

#include "iostream"

World::World(unsigned int width, unsigned int height)
: mGridSize(width, height)
, tSize(10)
{
	// set up stateArray
	for (unsigned int i = 0; i < width; ++i)
	{
		for (unsigned int j = 0; j < height; ++j)
		{
			stateArray[i][j] = DEAD;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	
	mVertices.setPrimitiveType(sf::Quads);
	mVertices.resize(width * height * 4); // set vertex count to be 4 (corners) * width * height

	// setup the vertex array
	for (unsigned int i = 0; i < width; ++i)
	{
		for (unsigned int j = 0; j < height; ++j)
		{
			// get pointer to current tile's quad
			sf::Vertex* quad = &mVertices[(i + j * width) * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(static_cast<float>(i * tSize), static_cast<float>(j * tSize)); // top left
			quad[1].position = sf::Vector2f(static_cast<float>((i + 1) * tSize), static_cast<float>(j * tSize)); // top right
			quad[2].position = sf::Vector2f(static_cast<float>((i + 1) * tSize), static_cast<float>((j + 1) * tSize)); //bottom right
			quad[3].position = sf::Vector2f(static_cast<float>(i * tSize), static_cast<float>((j + 1) * tSize)); //bottom left
		}
	}

	stateArray[10][10] = ALIVE;
	stateArray[10][11] = ALIVE;
	stateArray[10][12] = ALIVE;
}

void World::simulate(sf::Time timeStep)
{
	// logic here
	for (unsigned int i = 0; i < mGridSize.x; ++i)
	{
		for (unsigned int j = 0; j < mGridSize.y; ++j)
		{
			// get pointer to current tile's quad
			sf::Vertex* quad = &mVertices[(i + j * mGridSize.x) * 4];

		}
	}
}

void World::changeVertexColour(sf::Vertex* quad, sf::Color colour)
{
	// change vertices colour
	quad[0].color = colour; // top left
	quad[1].color = colour; // top right
	quad[2].color = colour; //bottom right
	quad[3].color = colour; //bottom left
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transformation
	states.transform *= getTransform();

	// draw vertex array
	target.draw(mVertices, states);

}