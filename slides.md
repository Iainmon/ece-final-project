```cpp
void game::Player::physics_update(const float &delta_time)
{
    Vector<2, float> gravity_force = Vector<2, float>(0.0, -25.0);

    if (game::user_input::jump) {
        // Markov solution for edge 'e'
        if (game::user_input::squat) {
            game::user_input::squat = false;
            run();
        }

        jump(); // Enter jump state
        game::user_input::jump = false; // Recieved jump signal
    } else if (action_markov != airborn && game::user_input::squat) {
        squat(); // Enter ducking state
    } else if (action_markov != airborn && action_markov != jogging) {
        run(); // Enter running state
    }
  	
  	// Step player physics
  	// ds/dt ~= sum( dv/dt * dx ) where x is each point in
  	// time, and dx = x1 - x0.
    pos += vel * delta_time;

    if (pos.y > 0.2) {
        // Apply the gravity
      	// Same sort of idea as changing the pos, but at the
      	// second degree of differentiation for s(t).
        vel += gravity_force * delta_time;
    } else if (pos.y < 0.0) {
      	// Ground colliders are too expensive. Change state if
      	// at ground level.
        run();
    }
}
```
```cpp
void game::Obstacle::physics_update(const float &delta_time)
{
  	// Respawn if way off the screen.
    if (pos.x > 150.) {
        respawn();
    }
  	
  	// Adds some nonlinearity to the obstacle's movements 
    vel.y += sin(pos.x * 8.) * 2.;
  
    pos += vel * delta_time; // Step physics
}

void game::Obstacle::render() 
{
  	// Selects the icon from a default font package
    constexpr uint16_t box_glyph_addy = 0x50;
    game::graphics::draw_glyph(pos.x - 6.0f, pos.y + 6.0f, box_glyph_addy);
}

// This is the same implementation for `Reward`.
void game::Reward::respawn()
{
    pos.y = (float)random(6, 70);
    pos.x = (float)random(-64, 0);
    vel.y = 0.;
}

void game::Reward::physics_update(const float &delta_time)
{    
  	// Respawn if way off the screen
    if (pos.x > 150.0) {
        respawn();
    }

    pos += vel * delta_time; // Step physics
}

void game::Reward::render()
{
  	// Draws the custom bitmap I made (stole from mario).
    game::graphics::draw_bitmap(pos.x - 8.0f,
                                pos.y - 8.0f,
                                2, 16,
                                game::graphics::sprites::reward_frames);
}
```