#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <deque>
#include <utility>
#include <iostream>

struct Config {
    float height_level;  // px
    float player_start_x;  // px
    float player_w;  // px
    float player_h;  // px
    float player_v0;  // px/s
    float obstacle_speed;  // px/s
    float obstacle_w;  // px
    float obstacle_h;  // px
    float gravity_a;  // px/s/s
    int window_w;  // px
    int window_h;  // px
    int framerate_limit; // Hz
};

struct Player {
    float x, y, w, h, v;
    bool jumping;

    Player(float x, float y, float w, float h) : x(x), y(y), w(w), h(h), v(0), jumping(false) {}

    sf::FloatRect rect() const {
        return {x, y, w, h};
    }

    void draw(sf::RenderTarget& target) const {
        sf::RectangleShape rect(sf::Vector2f(w, h));
        rect.setPosition(x, target.getSize().y - y - h);
        rect.setFillColor(sf::Color::Green);
        target.draw(rect);
    }
};

struct Obstacle {
    float x, y, w, h;

    Obstacle(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

    sf::FloatRect rect() const {
        return {x, y, w, h};
    }

    void draw(sf::RenderTarget& target) const {
        sf::RectangleShape rect(sf::Vector2f(w, h));
        rect.setPosition(x, target.getSize().y - y - h);
        rect.setFillColor(sf::Color::Red);
        target.draw(rect);
    }
};

class Game {
    Config config;
    Player player;
    std::deque<Obstacle> obstacles;

  public:
    Game(const Config& config, const Player& player, std::deque<Obstacle> obstacles = {})
            : config(config), player(player), obstacles(std::move(obstacles)) {}

    bool update(float dt /* seconds */) {
        if (player.jumping) {
            player.y += player.v*dt;
            player.v += config.gravity_a*dt;
            if (player.y < config.height_level) {
                player.y = config.height_level;
                player.jumping = false;
            }
        }

        for (Obstacle& o : obstacles) {
            o.x += dt*config.obstacle_speed;

            if (o.rect().intersects(player.rect())) {
                return false;
            }
        }

        // clean front
        while (!obstacles.empty() && obstacles.front().x < 0) obstacles.pop_front();

        // generate back
        if (obstacles.empty() || obstacles.back().x < config.window_w - 300) {
            obstacles.emplace_back(config.window_w, config.height_level,
                    config.obstacle_w, config.obstacle_h);
        }

        return true;
    }
    void draw(sf::RenderTarget& target) const {
        player.draw(target);
        for (const Obstacle& o : obstacles) {
            o.draw(target);
        }
    }
    void startJump() {
        if (!player.jumping) {
            player.jumping = true;
            player.v = config.player_v0;
        }
    }
};

int main() {
    // initial config
    Config config{
        200, // height level [px]
        400, // player start x [px]
        20, // player width [px]
        30, // player height [px]
        450, // player jump velocity [px/s]
        -250, // obstacle speed [px/s]
        10, // obstacle width [px]
        10, // obstacle height [px]
        -1500, // obstacle height [px/s]
        800, // window width [px]
        600, // window height [px]
        60,  // frames per second
    };

    sf::RenderWindow window(sf::VideoMode(config.window_w, config.window_h), "Jump!");
    window.setFramerateLimit(config.framerate_limit);

    Player player{config.player_start_x, config.height_level, config.player_w, config.player_h};
    Game game(config, player);

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Space:
                        game.startJump();
                        break;
                    default: break;
                }
            }
        }

        bool alive = game.update(elapsed.asSeconds());
        if (!alive) {
            std::cout << "You died!" << std::endl;
            return 0;
        }

        window.clear();
        game.draw(window);
        window.display();
    }

    return 0;
}
