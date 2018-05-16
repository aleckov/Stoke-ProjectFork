#include "include/play_state.hpp"

const float PlayState::mPlayerSpeed = 100.f;

PlayState::PlayState(StateMachine& machine, sf::RenderWindow& window, 
			ResourceManager& assets) : 
			State(machine, window, assets), 
			mEntityManager(mWindow, mAssets), mBackground(),
			mPlayer(), mIsMovingLeft(false), mIsMovingRight(false),
			mPlayerScore(0), mEntityIndex(0)
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	
	mBackground.setTexture(mAssets.getTexture("background"));
	mBackground.setPosition(0.0f,0.0f);
	
	mPlayer.setTexture(mAssets.getTexture("player"));
	mPlayer.setScale(0.5f, 0.5f);
	mPlayer.setPosition(10.f, 156.f);

	// set vertical bounds
	mLeftBound.setSize(sf::Vector2f(1.0, 180.0));
	mLeftBound.setPosition(0, 0);
	mRightBound.setSize(sf::Vector2f(1.0, 180.0));
	mRightBound.setPosition(119, 0);

	// score display setup
	std::string mScoreText = "Score " + std::to_string(mPlayerScore);
	mScore.setFont(mAssets.getFont("scoreFont"));
	mScore.setCharacterSize(40);
	mScore.setString(mScoreText);
	mScore.setColor(sf::Color::White);
	mScore.scale(0.2f, 0.2f);
	mScore.setPosition(2, 2);

	mTime.setFont(mAssets.getFont("scoreFont"));
	mTime.setCharacterSize(40);
	mTime.setString("0:00");
	mTime.setColor(sf::Color::White);
	mTime.scale(0.2f, 0.2f);
	mTime.setPosition(90, 2);
}

PlayState::~PlayState() { }

void PlayState::handleEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		// check for specific events
		switch (event.type)
		{
			case sf::Event::KeyPressed:
				handleInput(event.key.code, true);
				break;

			case sf::Event::KeyReleased:
				handleInput(event.key.code, false);
				break;

			case sf::Event::Closed:
				mStateMachine.close();
				break;

			//case sf::Event::LostFocus:
			//case sf::Event::GainedFocus:
		}
	}
}

// translate key press into game action
void PlayState::handleInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::A)
                mIsMovingLeft = isPressed;
        else if (key == sf::Keyboard::D)
                mIsMovingRight = isPressed;
        else if (key == sf::Keyboard::Escape)
                mStateMachine.close();
        else if (key == sf::Keyboard::G)
		mStateMachine.changeStateFlag();
}

// updates the game logic
void PlayState::update(const sf::Time& dt)
{
	if (mSpawnClock.getElapsedTime().asSeconds() > 0.5f)
	{
		generateEntity();
		mSpawnClock.restart();
	}

	// gain score if a 'fire' object is caught
	// game over if a 'water' object hits the player
	int tempScore;
	mEntityManager.update(dt);
	if ((tempScore = mEntityManager.checkCollision(mPlayer)) < 0)
		mStateMachine.changeStateFlag();
	else
		mPlayerScore += tempScore;

	// move the player
	sf::Vector2f velocity(0.f, 0.f);
	if (mIsMovingLeft)
		velocity.x -= mPlayerSpeed;
	if (mIsMovingRight)
		velocity.x += mPlayerSpeed;
	mPlayer.move(velocity * dt.asSeconds());

	// if player enters bounds, move him back
	if (mPlayer.getGlobalBounds().intersects(mLeftBound.getGlobalBounds()) 
	|| mPlayer.getGlobalBounds().intersects(mRightBound.getGlobalBounds()))
		mPlayer.move(-velocity * dt.asSeconds());
}

// renders the game to the screen
void PlayState::render()
{
	mWindow.clear();
	mWindow.draw(mBackground);
	mWindow.draw(mPlayer);
	mEntityManager.render();
	renderScore();
	renderTime();
	mWindow.display();
}

void PlayState::updateStateMachine()
{
	if (mStateMachine.checkFlag())
	{
		GameState newGameOverState(new GameOverState(mStateMachine, mWindow, mAssets));
		mStateMachine.changeState(std::move(newGameOverState));
	}
}

void PlayState::renderScore()
{
	std::string mScoreText = "Score " + std::to_string(mPlayerScore);
	mScore.setString(mScoreText);
	mWindow.draw(mScore);
}

void PlayState::renderTime()
{
	std::string mTimeText;
	int seconds = mGameClock.getElapsedTime().asSeconds();
	int minutes = seconds / 60;
	seconds = seconds % 60;
	if (seconds >= 10)
		mTimeText = std::to_string(minutes) + ":" + std::to_string(seconds);
	else if (seconds < 10)
		mTimeText = std::to_string(minutes) + ":0" + std::to_string(seconds);
	mTime.setString(mTimeText);
	mWindow.draw(mTime);
}

void PlayState::generateEntity()
{
	std::string entityType;
	if (rand() % 2 == 0)
		entityType = "water";
	else
		entityType = "fire";
	float entityPosition = rand() % 101;
	float entitySpeed = (rand() % 5 + 10) * 10.0f;
	std::string stringIndex = std::to_string(mEntityIndex);

	mEntityManager.addEntity(entityType, stringIndex, entityPosition, entitySpeed);
	++mEntityIndex;
}
