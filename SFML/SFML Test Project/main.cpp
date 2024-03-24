#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <chrono>

static std::random_device rd;
static std::mt19937 gen(rd());

int randomgen();
void TryLoadFont(sf::Font& font, std::string path);

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Test");

    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::White);

    bool ballVisible = false;
    bool countdownStarted = false;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point clickTime;
    std::chrono::steady_clock::time_point lastBallTime;
    std::chrono::steady_clock::time_point countdownStartTime;

    sf::Font font;
    TryLoadFont(font, "./8bitfont.ttf");
    sf::Text reactionTimeText;
    reactionTimeText.setFont(font);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!ballVisible && !countdownStarted && std::chrono::steady_clock::now() - lastBallTime > std::chrono::seconds(3)) {
                countdownStarted = true;
                countdownStartTime = std::chrono::steady_clock::now();
            }

            if (countdownStarted && !ballVisible) {
                auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - countdownStartTime).count();
                if (elapsedSeconds >= 3) {
                    countdownStarted = false;
                    ballVisible = true;
                    startTime = std::chrono::steady_clock::now();
                    circle.setPosition(sf::Vector2f(randomgen(), randomgen()));
                }
            }

            if (ballVisible && event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(window);
                if (circle.getGlobalBounds().contains(mousePosition)) {
                    clickTime = std::chrono::steady_clock::now();
                    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(clickTime - startTime).count();
                    std::cout << "Reaction Time: " << elapsedTime << " milliseconds" << std::endl;
                    ballVisible = false;
                    circle.setPosition(sf::Vector2f(-1000, -1000));
                    reactionTimeText.setString("Reaction Time: " + std::to_string(elapsedTime) + " ms");
                    reactionTimeText.setCharacterSize(24);
                    reactionTimeText.setFillColor(sf::Color::White);
                    reactionTimeText.setPosition(10, 10);
                }
            }
        }

        window.clear();

        if (ballVisible) {
            window.draw(circle);
        }

        if (countdownStarted) {
            auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - countdownStartTime).count();
            int remainingTime = 3 - elapsedSeconds;
            if (remainingTime <= 0) {
                remainingTime = 0;
            }
            sf::Text countdownText;
            countdownText.setFont(font);
            countdownText.setString("Next ball in: " + std::to_string(remainingTime) + " sec");
            countdownText.setCharacterSize(24);
            countdownText.setFillColor(sf::Color::White);
            countdownText.setPosition(300, 300);
            window.draw(countdownText);
        }

        window.draw(reactionTimeText);
        window.display();
    }
}

int randomgen()
{
    std::uniform_int_distribution<> dis(50, 750);
    return dis(gen);
}

void TryLoadFont(sf::Font& font, std::string path)
{
    if (!font.loadFromFile(path))
    {
        std::cout << "Error loading the font file" << std::endl;
        system("pause");
    }
}
