<h1> Pong</h1>
This is a version of Pong that is playable on a physical LCD screen and joystick. It was built


<p>Pong made from scratch on an LCD screen with a joystick.</p>

<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
  * [Built With](#built-with)
* [Getting Started](#getting-started)
* [Usage](#usage)



<!-- ABOUT THE PROJECT -->
## About The Project
A portable update of a classic! For those of us who always wanted to play pong on the go, but love the retro feel of a joystick and screen. We all know we can pull out our phone and play almost any game released over 20 years ago, but mobile games lack the tactile feel of a joystick. Why compromise? So I built this version of pong on a mini LCD with a joystick in baremetal arduino code.

### Built With
Baremetal C
Arduino
Atmel Studio


<!-- GETTING STARTED -->
## Getting Started

We essentially just use the normal logic of a pong game which has been hashed out many times, we just need to create the assets. We start by defining some functions that allow us to draw on the LCD screen in a library, such as drawCircle, drawLine, setScreen, etc. We then use those functions to draw the assets we need for our game, such as a paddle, a ball, and a scoreboard. We then keep track of the positions of each of those things on the screen,  dynamically update the position of the paddles when they move, and then make those boundaries that cause the ball to reverse it's velocity. When the ball hits the wall behind a player, we add a point and reset the its position.

I added a speaker so that when the ball collides with something it plays a sound, and when someone scores a corresponding LED lights up, and that's essentilly the game!

I also added wireless capabilities using an ESP32 module and the Blynk app, so the game can be played remotely as well if you want to play while on the couch!




<!-- USAGE EXAMPLES -->
## Usage
A Youtube demonstration of the program: https://youtu.be/jsX_0IXj7Bo




<!-- CONTACT -->
## Contact

Made by Mostafa Afr 
