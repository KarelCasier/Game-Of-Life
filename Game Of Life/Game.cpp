#include "Game.h"

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f); // = 0.6 seconds per frame, therefore 60fps

Game::Game()
: mWindow(sf::VideoMode(1000, 1000), "Game of Life")
, mWorld(100, 100)
, mFont()
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(false);

	mFont.loadFromFile("MYRIADPRO-BOLD.OTF");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);

	mTextBackground.setSize(sf::Vector2f(115.f, 35.f));
	mTextBackground.setFillColor(sf::Color(0, 0, 0, 80));
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
		//mPlayer.handleEvent(event, commands);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}

	//mPlayer.handleRealtimeInput(commands);
}

void Game::update(sf::Time dtTime)
{
	mWorld.simulate(dtTime);
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	std::stringstream strStream;

	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		strStream << "Frames / Second = " << mStatisticsNumFrames << "\n" <<
			"Time / Update = " << mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames << "us";

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
	mWindow.display();
}