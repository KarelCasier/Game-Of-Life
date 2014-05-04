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
			newStateArray[i][j] = DEAD;
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
}

void World::simulate()
{
	//std::cout << "Simulate" << std::endl;
	// logic here
	for (unsigned int i = 0; i < mGridSize.x; ++i)
	{
		for (unsigned int j = 0; j < mGridSize.y; ++j)
		{

			//////////////////////////////////////////////////////////////////////////
			//		Rules
			//1.	Any live cell with fewer than two live neighbours dies, as if caused by under - population.
			//2.	Any live cell with two or three live neighbours lives on to the next generation.
			//3.	Any live cell with more than three live neighbours dies, as if by overcrowding.
			//4.	Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction
			//////////////////////////////////////////////////////////////////////////

			bool alive = false;

			unsigned int liveNeighbours = getNumLiveNeighbours(i, j);

			//Check 1,2,3
			if (stateArray[i][j] == ALIVE)
			{
				if (liveNeighbours == 2 || liveNeighbours == 3)
				{
					alive = true;
				}
				
			}
			else // check 4
			{
				if (liveNeighbours == 3)
				{
					alive = true;
				}
				
			}
			newStateArray[i][j] = ((alive) ? ALIVE : DEAD);

			// only update colour if cellState changed
			//if (stateArray[i][j] != ((alive) ? ALIVE : DEAD))
			{

				// get pointer to current tile's quad
				sf::Vertex* quad = &mVertices[(i + j * mGridSize.x) * 4];

				changeVertexColour(quad, (stateArray[i][j] == DEAD) ? sf::Color::White : sf::Color::Black);
			}
		}
	}
	stateArray = newStateArray;
}

unsigned int World::getNumLiveNeighbours(unsigned int x, unsigned int y)
{
	unsigned int liveNeighbours = 0;

	unsigned int accessX = x;
	unsigned int accessY = y;

	/////////////
	//[1][2][3]//
	//[4][x][5]//
	//[6][7][8]//
	/////////////

	// initial setup for 1,2,4
	if (x == 0)
		accessX = mGridSize.x  - 1;
	else
		accessX = x - 1;

	if (y == 0)
		accessY = mGridSize.y  - 1;
	else
		accessY = y - 1;

	//1
	if (stateArray[accessX][accessY] == ALIVE)
		++liveNeighbours;
	//2
	if (stateArray[x][accessY] == ALIVE)
		++liveNeighbours;
	//4
	if (stateArray[accessX][y] == ALIVE)
		++liveNeighbours;

	// change to work with 6, 7
	if (y == mGridSize.y - 1)
		accessY = 0;
	else
		accessY = y + 1;

	//6
	if (stateArray[accessX][accessY] == ALIVE)
		++liveNeighbours;
	//7
	if (stateArray[x][accessY] == ALIVE)
		++liveNeighbours;

	//change to work with 8, 5
	if (x == mGridSize.x - 1)
		accessX = 0;
	else
		accessX = x + 1;

	//8
	if (stateArray[accessX][accessY] == ALIVE)
		++liveNeighbours;
	//5
	if (stateArray[accessX][y] == ALIVE)
		++liveNeighbours;

	//change to work with 3
	if (y == 0)
		accessY = mGridSize.y - 1;
	else
		accessY = y - 1;

	//3
	if (stateArray[accessX][accessY] == ALIVE)
		++liveNeighbours;


	return liveNeighbours;
}

void World::changeVertexColour(sf::Vertex* quad, sf::Color colour)
{
	// change vertices colour
	quad[0].color = colour; // top left
	quad[1].color = colour; // top right
	quad[2].color = colour; //bottom right
	quad[3].color = colour; //bottom left
}

void World::rejuvenateCell(int x, int y)
{
	int i = static_cast<int>(floor(x / tSize));
	int j = static_cast<int>(floor(y / tSize));
	if (stateArray[i][j] == ALIVE)
	{
		stateArray[i][j] = DEAD;
		sf::Vertex* quad = &mVertices[(i + j * mGridSize.x) * 4];
		changeVertexColour(quad, sf::Color::White);
	} 
	else
	{
		stateArray[i][j] = ALIVE;
		sf::Vertex* quad = &mVertices[(i + j * mGridSize.x) * 4];
		changeVertexColour(quad, sf::Color::Black);
	}
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transformation
	states.transform *= getTransform();

	// draw vertex array
	target.draw(mVertices, states);

}