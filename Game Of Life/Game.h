#pragma once

#include <SFML/System/Time.hpp>
#include "World.h"
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>
#include <sstream>

class Game : private sf::NonCopyable
{
public:
							Game();
	void					run();

private:
	void					processEvents();
	void					update(sf::Time dtTime);
	void					render();

	void					updateStatistics(sf::Time elapsedTime);

private:
	//For FPS calculations and consistent movement
	static const sf::Time	TimePerFrame;

	sf::RenderWindow		mWindow;
	World					mWorld;
	sf::Time				timeStep;
	sf::Time				timeToNextSimulate;

	sf::Font				mFont;
	sf::Text				mStatisticsText;
	sf::Time				mStatisticsUpdateTime;
	std::size_t				mStatisticsNumFrames;
	sf::RectangleShape		mTextBackground;

	enum GameState
	{
		Play,
		Pause,
	};
	GameState				gameState;

	//Buttons
	sf::RectangleShape playpauseButton;

};
