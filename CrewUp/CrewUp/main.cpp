#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // 1. Creating the Window
    sf::RenderWindow window(sf::VideoMode(1360, 800), "CrewUp");

    // 2. Loading Font
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/Roboto-Bold.ttf")) {
        std::cout << "ERROR: Could not find font! Check assets folder." << std::endl;
        return -1;
    }

    window.clear(sf::Color(40, 44, 52)); // 1. Dark Background

	sf::CircleShape circle(50); // Radius 50
	circle.setFillColor(sf::Color(50, 40, 50));
	circle.setPosition(150, 100);
	circle.scale(0.5f, 0.5f); // Scale by 1.5x


    // --- PANEL 1: LEFT (Team Members Profiles) ---
    sf::RectangleShape leftPanel(sf::Vector2f(280, 780)); // Width 280, Height 780
    leftPanel.setPosition(10, 10);                        // 10px padding
    leftPanel.setFillColor(sf::Color(55, 60, 70));        // Lighter Grey
    leftPanel.setOutlineThickness(2);
    leftPanel.setOutlineColor(sf::Color::Black);
    window.draw(leftPanel);

    // --- PANEL 2: CENTER (Task Board) ---
    sf::RectangleShape centerPanel(sf::Vector2f(740, 780));
    centerPanel.setPosition(300, 10);                     // Starts at x=300
    centerPanel.setFillColor(sf::Color(55, 60, 70));
    centerPanel.setOutlineThickness(2);
    centerPanel.setOutlineColor(sf::Color::Black);
    window.draw(centerPanel);

    // --- PANEL 3: RIGHT (Leader privilages and Shame Board) ---
    sf::RectangleShape rightPanel(sf::Vector2f(300, 780));
    rightPanel.setPosition(1050, 10);                     // Starts at x=1050
    rightPanel.setFillColor(sf::Color(55, 60, 70));
    rightPanel.setOutlineThickness(2);
    rightPanel.setOutlineColor(sf::Color::Black);
    window.draw(rightPanel);

    
    // 3. Create the Text Object "We can do it! Hive Trio"
    sf::Text message;
    message.setFont(font);             // 1. Choose the font
    message.setString("We can do it!\nHive Trio");  // 2. What to say
    message.setCharacterSize(50);      // 3. How big (pixels)
    message.setFillColor(sf::Color::White); // 4. Color
    message.setPosition(420, 350);     // 5. Where? (x, y)

    // 4. Main Loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		// DRAWING
        window.clear(sf::Color(40, 44, 52)); // 1. Wipe screen with Grey

        
		window.draw(leftPanel);            // Draw left panel
		window.draw(centerPanel);          // Draw center panel
		window.draw(rightPanel);           // Draw right panel
        window.draw(message);               // Draw the text on top
        window.draw(circle);


        window.display();                    // 3. Show the frame


    }

    return 0;
}