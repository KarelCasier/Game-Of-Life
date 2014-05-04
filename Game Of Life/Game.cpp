#include "Game.h"

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f); // = 0.6 seconds per frame, therefore 60fps

Game::Game()
: mWindow(sf::VideoMode(1000, 1000), "Game of Life")
, mWorld(100, 100)
, timeStep(sf::seconds(.1))
, timeToNextSimulate(timeStep)
, mFont()
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
, gameState(Pause)
{
	mWindow.setKeyRepeatEnabled(false);

	mFont.loadFromFile("MYRIADPRO-BOLD.OTF");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);

	mGenerationText.setFont(mFont);
	mGenerationText.setPosition(5.f, 30.f);
	mGenerationText.setCharacterSize(10);

	mTextBackground.setSize(sf::Vector2f(115.f, 55.f));
	mTextBackground.setFillColor(sf::Color(0, 0, 0, 80));

	playpauseButton.setSize(sf::Vector2f(20.f, 20.f));
	playpauseButton.setPosition(5.f, mWindow.getSize().y - 25.f);
	playpauseButton.setFillColor(sf::Color(0, 0, 0, 100));
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart(); //Note restart returns the time on the clock then restarts the clock
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame) //Ensures that the logic is caught up before rendering again
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);
		}

		updateStatistics(elapsedTime);
		render();
	}
	mWindow.close();
}

void Game::processEvents()
{

	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
			if (playpauseButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
			{
				if (gameState == Pause)
					gameState = Play;
				else
					gameState = Pause;
			}

			mWorld.switchCell(mousePos.x, mousePos.y);
		}
		

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}

	// realtime events
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2i mousePos(sf::Mouse::getPosition(mWindow));
		mWorld.rejuvenateCell(mousePos.x, mousePos.y);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		sf::Vector2i mousePos(sf::Mouse::getPosition(mWindow));
		mWorld.killCell(mousePos.x, mousePos.y);
	}
	

}

void Game::update(sf::Time dtTime)
{
	if (gameState == Play)
	{
		timeToNextSimulate -= dtTime;
		if (timeToNextSimulate.asSeconds() <= 0)
		{
			timeToNextSimulate = timeStep;
			mWorld.simulate();
		}
	}
	std::stringstream strStream;

	strStream << "Generation: " << mWorld.getGeneration() << std::endl <<
		"Timestep: " << timeStep.asSeconds() << "seconds";

	mGenerationText.setString(strStream.str());
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	std::stringstream strStream;

	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		strStream << "Frames / Second = " << mStatisticsNumFrames << "\n" <<
			"Time / Update = " << mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames << "us" << std::endl;

		mStatisticsText.setString(strStream.str());

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Game::render()
{
	mWindow.clear();
	mWindow.draw(mWorld);
	mWindow.draw(mTextBackground);
	mWindow.draw(mStatisticsText);
	mWindow.draw(mGenerationText);
	mWindow.draw(playpauseButton);
	mWindow.display();
}