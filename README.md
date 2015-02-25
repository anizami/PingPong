==========================================
PingPong homework for Interactive Graphics
==========================================

Asra Nizami and Guillermo Vera
==============================

This is an implementation of an interactive Ping Pong game using G3D. The player is provided with a paddle, whose movement is controlled by the mouse. The game starts off with a ball being served from the opponent's end, which the player has to hit back with the paddle. 

Implementations
===============

The code first draws the ping pong table, the paddle and the ball according to actual physical dimensions. The ping pong ball has a radius of 2cm, the paddle has a radius of 8cm and the table has length, width and height being 274cm, 152.5cm and 76cm respectively. It also draws a net in the middle of the table of width 1cm and height 15.25cm. 

The code uses the equations of projectile motion, taking gravity into account, to determine the path the ball will follow as it moves. It collides and interacts with the ping pong table, the net, and the paddle in a physically meaningful way. When it bounces off the table, its velocity in the y-direction reverses and decreases by a factor, defined by constant TABLE_RESTITUTION. It only bounces off the table when it is above the table, and falls down infinitely when it is off the table. When the ball collides with the net, its velocity in the y-direction is not affected, but its z-direction velocity is reduced by a NET_RESTITUTION factor. This is in line with the physical situation of objects colliding at an angle to the normal of a surface.

When the ball collides with the paddle, it reverses its velocity in the z-direction. It also takes into account the velocity of the paddle at the point of impact and imparts some of the paddle's velocity to the ball. Since the paddle is always at a fixed height of 20cm, its y-velocity is always 0, so the ball's y-velocity is not affected. The code takes into account the x and z velocities of the paddle and adds them to the ball's post-collision velocities respectively. It uses the law of conservation of momentum equation
m<sub>1<sub>v<sub>1<sub> - m<sub>2<sub>m<sub>2<sub> = m<sub>2<sub>v<sub>3<sub>
(assuming that the paddle's velocity is 0 after the collision) to impart velocity, and scales it by different factors PADDLEX_RESTITUTION and PADDLEZ_RESTITUTION, to make it easier to hit angled shots. 

Commands
========

Pressing the spacebar key resets the ball to its initial position and velocity. The camera also pans using regular G3D panning command keys. 
