#pragma once

#include <SFML/System/Time.hpp>
#include "World.h"
#include <memory>
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

	sf::Text				mGenerationText;

	std::array<sf::Time, 5>	possibleTimeSteps;
	unsigned int			timeStepChoice;

	enum GameState
	{
		Play,
		Pause,
	};
	GameState				gameState;

	//Buttons
	sf::Texture controlsTexture;

	sf::Sprite playpauseButton;
	sf::Sprite stopButton;
	sf::Sprite fasterStepButton;
	sf::Sprite slowerStepButton;

};
