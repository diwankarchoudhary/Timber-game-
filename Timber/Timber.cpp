

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <SFML/Audio.hpp>

using namespace sf;



//Growing Branches-----------------------

void updateBranches(int seed);// Set the texture for each branch sprite
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];


int main()
{

    VideoMode vm(960, 540);
    
    
    
    // Create and open a window for the game
	RenderWindow window(vm, "Timber!!!");
        View view(sf::FloatRect(0, 0, 1920, 1080));
	window.setView(view);

    
   // Create a texture to hold a graphic on the GPU
   
    Texture textureBackground;
    
   // Load a graphic into the texture 
    textureBackground.loadFromFile("graphics/background.png");
    
    
    Sprite spriteBackground;// Set the texture for each branch sprite
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);
    
    //Make a tree Sprite
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);
    
    //Make a bee Sprite
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);
    bool beeActive = false;
    float beeSpeed= 0.0f;
    
    
    //Make a cloud Sprite
    Texture textureCloud;// Set the texture for each branch sprite
    textureCloud.loadFromFile("graphics/cloud.png");
    Sprite spriteCloud1, spriteCloud2, spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 250);
    spriteCloud3.setPosition(0, 500);
    spriteCloud1.setScale(0.5f, 0.5f);
    spriteCloud2.setScale(0.5f, 0.5f);
    spriteCloud3.setScale(0.5f, 0.5f);
    
    // Are the clouds currently on screen?

    bool cloudActive1= false;
    bool cloudActive2= false;
    bool cloudActive3= false;
    // Set the texture for each branch sprite

   // How fast is each cloud?
	float cloudSpeed1 = 0.0f;
	float cloudSpeed2 =0.0f;
	float cloudSpeed3 =0.0f;
	
Clock clock;
   // Time dt;
bool paused = true;  



/*-----------------------------------------------------
        Prompting a Message to start the Game
-------------------------------------------------------*/        
Text messageText;
Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	
	messageText.setFont(font);
	messageText.setString("PRESS ENTER TO START!!!");
	messageText.setCharacterSize(75);
	  
	messageText.setFillColor(Color::White);
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + 	textRect.height / 2.0f);  
	messageText.setPosition(1920 / 2.0f, 1080 / 4.0f); 
//-----------------------------------------------------------



/*-------------------------------------------------// Set the texture for each branch sprite----
        Displaying the score
-------------------------------------------------------*/  
int score = 0;  
Text scoreText;
scoreText.setFont(font);
scoreText.setString("Score = 0");
scoreText.setFillColor(Color::White);
scoreText.setCharacterSize(100);
scoreText.setPosition(20, 20);
//---------------------------------------------------------------------------


// Variables to control time itself
RectangleShape timeBar;
float timeBarStartWidth = 400;
float timeBarHeight = 80;
timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
timeBar.setFillColor(Color::Red);
timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);
/*--------------------------------------------------------------------*/

Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;




// Prepare 6 branches-----------------------------------------------------

Texture textureBranch;
textureBranch.loadFromFile("graphics/branch.png");
// Set the texture for each branch sprite
for(int i = 0; i<NUM_BRANCHES; i++){
branches[i].setTexture(textureBranch);
branches[i].setPosition(-2000, -2000);

// Set the sprite's origin to dead centre
branches[i].setOrigin(220, 20);
}



/*
updateBranches(1);
updateBranches(2);
updateBranches(3);
updateBranches(4);
updateBranches(5);*/
//-----------------------------------------------------------------------------------------------------


/*------------------------------------------------
----------------------Prepare the player----------
------------------------------------------------*/
Texture texturePlayer;
texturePlayer.loadFromFile("graphics/player.png");
Sprite spritePlayer;
spritePlayer.setTexture(texturePlayer);
spritePlayer.setPosition(580, 720);

// The player starts on the left
side playerSide = side::LEFT;


// Prepare the gravestone
Texture textureRIP;
textureRIP.loadFromFile("graphics/rip.png");
Sprite spriteRIP;
spriteRIP.setTexture(textureRIP);
spriteRIP.setPosition(600, 860);

// Prepare the axe
Texture textureAxe;
textureAxe.loadFromFile("graphics/axe.png");
Sprite spriteAxe;
spriteAxe.setTexture(textureAxe);
spriteAxe.setPosition(700, 830);

// Line the axe up with the tree
const float AXE_POSITION_LEFT = 700;
const float AXE_POSITION_RIGHT = 1075;



// Prepare the flying log
Texture textureLog;
textureLog.loadFromFile("graphics/log.png");
Sprite spriteLog;
spriteLog.setTexture(textureLog);
spriteLog.setPosition(810, 720);

// Some other useful log related variables
bool logActive = false;
float logSpeedX = 1000;
float logSpeedY = -1500;


// Control the player input--------------
bool acceptInput = false;



// Prepare the sounds
// The player chopping sound
SoundBuffer chopBuffer;
chopBuffer.loadFromFile("sound/chop.wav");
Sound chop;
chop.setBuffer(chopBuffer);
// The player has met his end under a branch
SoundBuffer deathBuffer;
deathBuffer.loadFromFile("sound/death.wav");
Sound death;
death.setBuffer(deathBuffer);
// Out of time
SoundBuffer ootBuffer;
ootBuffer.loadFromFile("sound/out_of_time.wav");
Sound outOfTime;
outOfTime.setBuffer(ootBuffer);

while(window.isOpen()){


    /*Event event1;
        while (window.pollEvent(event1))
        {
            if (event1.type == event1.Closed)
            {
                window.close();
            }
        }*/
   
    //dt = clock.restart();
    
    
    /*
****************************************
Handle the players input
****************************************
*/

    //Detecting when a key is Released 
     Event event;
    while (window.pollEvent(event))
     {
         if (event.type == Event::KeyReleased && !paused)
            {
                  // Listen for key presses again
              acceptInput = true;
               // hide the axe
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
                }
           }
    
         if(Keyboard::isKeyPressed(Keyboard::Escape)){
            window.close();
              }
              
              
       //Displaying the message

              
              
        if(Keyboard::isKeyPressed(Keyboard::Return)){
            paused = false;
            
            // Reset the time and the score
            score = 0;
            timeRemaining = 6;
            
            // Make all the branches disappear -
            // starting in the second position
            
            for (int i = 1; i < NUM_BRANCHES; i++)
            {
               branchPositions[i] = side::NONE;
               }
             
             // Make sure the gravestone is hidden
               spriteRIP.setPosition(675, 2000);
                
                // Move the player into position
            spritePlayer.setPosition(580, 720);
            acceptInput = true;
            
            }
            
            
            // Wrap the player controls to
            // Make sure we are accepting input  
            
            if (acceptInput){
            
               if (Keyboard::isKeyPressed(Keyboard::Right))
                {
                    // Make sure the player is on the right
                       playerSide = side::RIGHT;
                       score ++;
                       // Add to the amount of time remaining
                         timeRemaining += (2 / score) + .15;
                         
                         spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                          spritePlayer.setPosition(1200, 720);
                          
                          
                          // Update the branches
                         updateBranches(score);
                         
                        // Set the log flying to the left
                         spriteLog.setPosition(810, 720);
                         logSpeedX = -5000;
                          logActive = true;
                          acceptInput = false;
                          
                          chop.play();
            
                              }
                              
                              
                   // Handle the left cursor key           
                   if (Keyboard::isKeyPressed(Keyboard::Left))
                      {
                    // Make sure the player is on the left
                      playerSide = side::LEFT;
                      score++;
		    // Add to the amount of time remaining
			timeRemaining += (2 / score) + .15;
			spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
			spritePlayer.setPosition(580, 720);
			
		   // update the branches
			updateBranches(score);
		
		   // set the log flying
		       spriteLog.setPosition(810, 720);
			logSpeedX = 5000;
			logActive = true;
			acceptInput = false;
                          
                          
                          // Play a chop sound
                        chop.play();
                        }
            
            }   
            
      /*****************************************
		Update the scene
	****************************************
		*/             
     
 if(!paused){     //Condition : starting the game by pressing enter from paused state.  
 
 Time dt = clock.restart();
        //Incrementing the score 
 
          /* score = 0;
           score++;
           std::stringstream ss;
            ss << "Score=" << score;
            scoreText.setString(ss.str());
            */
            
       timeRemaining -= dt.asSeconds();
// size up the time bar
timeBar.setSize(Vector2f(timeBarWidthPerSecond *timeRemaining, timeBarHeight));  
if (timeRemaining<= 0.0f) {
// Pause the game
paused = true;

// Change the message shown to the player

messageText.setString("Out of time!!");

//Reposition the text based on its new size

FloatRect textRect = messageText.getLocalBounds();
messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);



// Play the out of time sound
outOfTime.play();
}   

              
/*---------------------------------------------------
      Making the bee to fly-----------------------
-----------------------------------------------------*/         
   
    if (!beeActive)
        {
            srand((int)time(0));
            beeSpeed = (rand() % 200) + 200; //generate random number between 200 and 400
            srand((int)time(0) * 10);
            float height = (rand() % 500) + 500;
            spriteBee.setPosition(2000, height);
            beeActive = true;
        }
        else
        {
            spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);//dt.asSeconds  to get the time in seconds
            if (spriteBee.getPosition().x < -100)
            {
                beeActive = false;
            }
        }
        
        
 /*---------------------------------------------------
      Moving the cloud-----------------------
-----------------------------------------------------*/  

if(!cloudActive1) {
     srand((int)time(0) *10);
     cloudSpeed1 = (rand()%200) ;
     srand((int)time(0) * 10);
     float height = (rand() % 150);
     spriteCloud1.setPosition(-200, height);
            cloudActive1 = true;
        }
        else
        {
            spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloudSpeed1 * dt.asSeconds()), spriteCloud1.getPosition().y);
            if (spriteCloud1.getPosition().x > 1920)
            {
                cloudActive1 = false;
            }
        }     
   
   
 if(!cloudActive2) {
     srand((int)time(0) *20);
     cloudSpeed2 = (rand()%200) ;
     srand((int)time(0) * 20);
     float height = (rand() % 300) -150;
     spriteCloud1.setPosition(-200, height);
            cloudActive2 = true;
        }
        else
        {// Play the death sound
death.play();
            spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloudSpeed2 * dt.asSeconds()), spriteCloud2.getPosition().y);
            if (spriteCloud2.getPosition().x > 1920)
            {
                cloudActive2 = false;
            }
        }    
   
   
   
   if (!cloudActive3)
        {
            srand((int)time(0) * 30);
            cloudSpeed3 = (rand() % 200);
            srand((int)time(0) * 30);
            float height = (rand() % 450) - 150;
            spriteCloud3.setPosition(-200, height);
            cloudActive3 = true;
        }
        else
        {
            spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloudSpeed3 * dt.asSeconds()), spriteCloud3.getPosition().y);
            if (spriteCloud3.getPosition().x > 1920)
            {
                cloudActive3 = false;
            }
         
         }
         
         
         
         // Update the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());
         
         
                 
         //update the branch sprites
         for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;
                if (branchPositions[i] == side ::LEFT)
                {
                    branches[i].setPosition(610, height);
                    //Flip the sprite round the other way
                    branches[i].setOrigin(220, 40);
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side ::RIGHT)
                {
                    branches[i].setPosition(1330, height);
                    // Set the sprite rotation to normal
                    branches[i].setRotation(0);
                    branches[i].setOrigin(220, 40);
                }
                else
                //Hide the branches
                {
                    branches[i].setPosition(3000, height);
                }
            }
         
    /*--------------------------------------------------   
    *****************Animating the chopped logs and the axe   
     -----------------------------------------------------*/
     
    	 // Handle a flying log
		if (logActive)
		{
			spriteLog.setPosition(
			spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
			spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
		// Has the log reached the right hand edge?
			if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
			 {
				// Set it up ready to be a whole new log next frame
				logActive = false;
				spriteLog.setPosition(810, 720);
			          }
		            }
		            
		            
/****************************************************************************
------------------Handling Death---------------------------------------------
*****************************************************************************/
if (branchPositions[5] == playerSide)
{
// death
paused = true;
acceptInput = false;
// Draw the gravestone
spriteRIP.setPosition(525, 760);
// hide the player
spritePlayer.setPosition(2000, 660);
// Change the text of the message
messageText.setString("SQUISHED!!");
// Center it on the screen
FloatRect textRect = messageText.getLocalBounds();
messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

// Play the death sound
death.play();
}		            
		            
		            
         
   } // End if(!paused)  
   
   
         window.clear();
         window.draw(spriteBackground);
         window.draw(spriteTree);
         window.draw(spriteBee);
         window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        
        
        //Drawing the branches
        for (int i = 0; i < NUM_BRANCHES; i++) {
           window.draw(branches[i]);
            }
        
        //Draw our score
        window.draw(scoreText);
        
        // Draw the timebar
          window.draw(timeBar);
        
        if (paused)
	{
	// Draw our message
	window.draw(messageText);
	}
	
	
	// Draw the player
          window.draw(spritePlayer);
          
           // Draw the axe
           window.draw(spriteAxe);
	 
	 // Draw the flying log
	   window.draw(spriteLog);
	   
	// Draw the gravestone
	window.draw(spriteRIP);
        window.display();
        
   }
        return 0;
}


void updateBranches(int seed){
for(int j = NUM_BRANCHES-1; j>0; j--){
  branchPositions[j] = branchPositions[j-1];
  }
  
  // Spawn a new branch at position 0
// LEFT, RIGHT or NONE

srand((int)time(0)+seed);
int r = (rand() % 5);
switch (r) {
case 0:
branchPositions[0] = side::LEFT;
break;
case 1:
branchPositions[0] = side::RIGHT;
break;
default:
branchPositions[0] = side::NONE;
break;
}
  
  }


