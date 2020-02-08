#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unistd.h>
#define up 0
#define left 1
#define down 2
#define right 3

using namespace std;


sf::Clock gameClock;
int gameSpeed = 1;
unsigned int level;
const int game_x = 1024, game_y = 700;
int gameDuration = 10000;
int gameMode = 1; //1 for Oneplayer, 2 for 2Players
int theme = 1;

class Spaceship {
	
	public:
		Spaceship (int, int);
		Spaceship ();
		bool can_up ();
		void move_up ();
		bool can_down ();
		void move_down ();
		bool can_left ();
		void move_left ();
		bool can_right ();
		void move_right ();
		int line, x;
		vector <int> moves;
		void overtake (const Spaceship &other_spaceship);
		void move();
		sf::Time lastMove, lastShoot;
		static const int moveDelay = 200, shootDelay = 500;
		int getX () const;
		int getY () const;
		static const int length = 95 , height = 95;;
		void restart ();
		

};

Spaceship::Spaceship (int l, int xx) {
	line = l;
	x = xx;
	lastMove = gameClock.getElapsedTime();
}

Spaceship::Spaceship () {
	lastMove = gameClock.getElapsedTime();
}

void Spaceship::restart () {
	lastMove = lastShoot = gameClock.getElapsedTime();
}


int Spaceship::getX () const {
	return x * 100 + 95;
}

int Spaceship::getY () const {
	return line * 100 + 5;
}

bool Spaceship::can_up (){
	if (!moves.size() && line > 0 &&  (gameClock.getElapsedTime()-lastMove) >= sf::milliseconds(moveDelay)) {
		lastMove = gameClock.getElapsedTime();
		return true;
	}
	return false;
}

bool Spaceship::can_down (){
	if (!moves.size() && line < 5 && (gameClock.getElapsedTime()-lastMove) >= sf::milliseconds(moveDelay)) {
		lastMove = gameClock.getElapsedTime();
		return true;
	}
	return false;
}

void Spaceship::move_up () {
	if (can_up())
		line--;
	else move();
}

void Spaceship::move_down () {
	if (can_down())
		line++;
	else move();
}

bool Spaceship::can_left () {
	if (x == 1 && (gameClock.getElapsedTime()-lastMove) >= sf::milliseconds(moveDelay)) {
		lastMove = gameClock.getElapsedTime();	
		return true;
	}
	return false;
}

void Spaceship::move_left () {
	if (can_left()) x--;
}

bool Spaceship::can_right () {
	if (x == 0 && (gameClock.getElapsedTime()-lastMove) >= sf::milliseconds(moveDelay)) {	
		lastMove = gameClock.getElapsedTime();
		return true;
	}
	return false;
}

void Spaceship::move_right () {
	if (can_right()) x++;
}

void Spaceship::overtake (const Spaceship &other_spaceship) {	
	if (!moves.size() && can_left () && other_spaceship.x == x) {
		if (other_spaceship.line == line - 1 && line - 2 >= 0) {
			moves.push_back (right);
			moves.push_back (up);
			moves.push_back (up);
			moves.push_back (left);
		}
		else if (other_spaceship.line == line + 1 && line + 2 < 6) {
			moves.push_back (right);
			moves.push_back (down);
			moves.push_back (down);
			moves.push_back (left);			
		}
	}
	else if (!moves.size() && can_left () && other_spaceship.x == x - 1) {
		if (other_spaceship.line == line)
			move_left ();
			moves.push_back (right);
	}
	
}

void Spaceship::move () {
	if (moves.size() && (gameClock.getElapsedTime()-lastMove) >= sf::milliseconds(moveDelay)) {
		lastMove = gameClock.getElapsedTime();
		if (moves.back () == right)
			x++;
		else if (moves.back () == up)
			line--;
		else if (moves.back () == down)
			line++;
		else if (moves.back () == left)
			x--;
		moves.pop_back();
	}
}





class Alien {
	public:
		int x, y;
		int dx, dy , dl;
		bool type; //0 for Small, 1 for Large
		int length, height;
		Alien (int, bool);
		void moveAlien ();
		int getY () const;
		sf::Time lastShoot;
		static const int shootDelay = 500;
};

Alien::Alien (int ly, bool t) {
	lastShoot = gameClock.getElapsedTime();
	type = t;
	x = game_x + 100;
//	if (level == 2) line = ly;
//	else if (level == 3) y = ly;
	y = ly;
	if (type) {
		length = 150;
		height = 170;
	}
	else {
		length = 80;
		height = 80;
	}
	dx = -gameSpeed;
	dy = rand() % 2;
	if (!dy) dy = -1;
}

int Alien::getY () const {
//	if (level == 2)
//		return line * 100 + 10;
//	else if (level == 3)
		return y;
}

void Alien::moveAlien () {
	x += dx;
//	if (level == 2 && ! (x % 300)) {
//		if (!line || line == 5) dl *= -1;
//		line += dl;	
//	}
	y += dy;
	if (type && y >= 400 && dy > 0) dy *= -1;
	if (type && y <= 0 && dy < 0) dy *= -1;
	if (!type && y >= 500 && dy > 0) dy *= -1;
	if (!type && y <= 0 && dy < 0)
		dy *= -1;
}




class Monster {
	public:
		int y, dy, UP;
		static const int height = 300, x = 500, length = 311;
		Monster ();
		void moveMonster ();
		sf::Time lastShoot;
		static const int shootDelay = 700;

};


Monster::Monster () {
	UP = 10;
	lastShoot = gameClock.getElapsedTime();
	y = 150;
	dy = rand() % 2;
	if (!dy) dy = -1;
}


void Monster::moveMonster () {
	y += dy;
	if (y <= 0 && dy == -1) dy *= -1;
	if (y + height >= 600 && dy == 1) dy *= -1;
}



class Meteor {
	public:
		int x, line;
		int dx;
		static const int length = 45;
		Meteor (int);
		void moveMeteor ();
};


Meteor::Meteor (int l) {
	x = game_x + length;
	line = l;
	dx = -gameSpeed;
}

void Meteor:: moveMeteor () {
	x += dx;
}






class Star {
	public:
		int x, line;
		int dx;
		static const int length = 45;
		Star (int);
		void moveStar ();
};

Star::Star (int l) {
	x = game_x + length;
	line = l;
	dx = -gameSpeed;
}

void Star::moveStar () {
	x += dx;
}








class Heart {
	public:
		int x, line;
		int dx;
		static const int length = 45;
		Heart (int);
		void moveHeart ();
		bool created;
};

Heart::Heart (int l) {
	x = game_x + length;
	line = l;
	dx = -gameSpeed;
	created = false;
}

void Heart::moveHeart () {
	if (x > -length) x += dx;
}







class Bullet {
	public:
		int x, line, y;
		int dx;
		int type; //0 for Alien, 1 for Spaceship1 & 2 for Spaceship2
		Bullet(int, int, int);
		void moveBullet ();	
		int getY () const;		
};

Bullet::Bullet (int xx, int ly, int ty){
	x = xx;
	type = ty;
	if (!type) y = ly;
	else line = ly;
	if (!type) dx = -2 * gameSpeed;
	else dx = 2 * gameSpeed;
}

void Bullet::moveBullet () {
	x += dx;
}


int Bullet::getY() const{
	if (type) return line * 100 + 50;
	return y;
}


class Map
{	
	public:
		Spaceship spaceship1, spaceship2;
		string player1Name;
		string player2Name;
		unsigned int player1Score;
		unsigned int player2Score;
		unsigned int player1UP, player2UP;
		Map();
		vector <Meteor> meteors;
		vector <Star> stars;
		vector <Heart> hearts;
		vector <Bullet> bullets;
		vector <Alien> aliens;
		Monster monster;
		//Heart hearts[3]; //0 Between, 1 Spaceship1, 2 Spaceship2;
		~Map();
		void resetSpaceships ();
	
};




Map::Map() {
//	sf::Time time = gameClock.getElapsedTime();
//	cerr << time.asMilliseconds() << endl;
//	cerr << gameClock.getElapsedTime() << endl;
	resetSpaceships ();		
	player1UP = player2UP = 5;
	player1Score = player2Score = 0;
	player1Name = "Player 1";
	player2Name = "Player 2";
	
}



Map::~Map () {

}


void Map::resetSpaceships () {

	spaceship1.line = 1;
	spaceship1.x = 1;
	spaceship2.line = 4;
	spaceship2.x = 1;
}

class Graphic
{
	public:

		Graphic();
		~Graphic();
		void showMenu();
		virtual void repaint();
		void gameOver (int);
		void gameOver ();
		virtual bool is_open();
		void start_game ();
		void startLevel ();
		void moveBackground ();
		sf::RenderWindow game;
		sf::SoundBuffer mm , stsd , eog, htsd, mtbulsd, winsd;
		sf::Sound monster_move , star_sound , end_music, heart_sound, metbul_sound, win_sound;
		Map map;

	private:
		const static int levelTextDelay = 600 * 1000;
		const static int menuButtonLength = 350;
		const static int menuButtonHeight = 112;
		//static const int game_x = 1024, game_y = 600; 
		//sf::RenderWindow game;
		bool start;
		int background_x;
	
		sf::Music game_music, menu_music;		

		sf::Font font;
		sf::Text player1Text, player2Text, player1ScoreText, player2ScoreText, endText, levelText, demolText;
		bool isClicked (sf::Sprite&, int, int, sf::RenderWindow &);
		//void show_about ();
		void play_game ();
		sf::Texture al1, al2, backgroundtex, spacetex1, spacetex2, met, ht, st, lh, bul1, endt, mon;
		sf::Sprite alien1_pic, alien2_pic, background_pic, spaceship1_pic, spaceship2_pic, meteor_pic, heart_pic, star_pic, littleheart_pic, bullet1_pic, end_pic, monster_pic;
		string convertInt (int);
		//void start_game ();
};



Graphic::Graphic () {

	ht.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/heart.png");
	heart_pic.setTexture (ht);	
	
	lh.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Little-Heart.png");
	littleheart_pic.setTexture (lh);	
	
	st.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/star.png");
	star_pic.setTexture (st);

	bul1.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Bullet1.png");
	bullet1_pic.setTexture (bul1);

	
	switch (theme) {

		case 1:
			
			mon.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Monster.png");
			monster_pic.setTexture (mon);

			al1.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Alien1.png");
			alien1_pic.setTexture (al1);

			al2.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Alien2.png");
			alien2_pic.setTexture (al2);

			spacetex1.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/spaceship1.png");
			spaceship1_pic.setTexture (spacetex1);

			spacetex2.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/spaceship2.png");
			spaceship2_pic.setTexture (spacetex2);

			met.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/meteor.png");
			meteor_pic.setTexture (met);

			endt.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/End.jpg");
			end_pic.setTexture (endt);

		break;

		case 2:
			spacetex1.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Submarine1.PNG");
			spaceship1_pic.setTexture (spacetex1);
		
			al2.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Jellyfish.png");
			alien2_pic.setTexture (al2);

			spacetex2.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Submarine2.PNG");
			spaceship2_pic.setTexture (spacetex2);

			endt.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/2_End.jpg");
			end_pic.setTexture (endt);
		break;
	}

	game_music.openFromFile ("/home/eizhen/Documents/AP Project/Code/Sounds/Game_Music.wav");
	menu_music.openFromFile ("/home/eizhen/Documents/AP Project/Code/Sounds/Menu.wav");

	game_music.setLoop (true);

	eog.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Sounds/End_Game.wav");
	end_music.setBuffer (eog);

	htsd.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Sounds/Heart.wav");
	heart_sound.setBuffer (htsd);

	winsd.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Sounds/Win.wav");
	win_sound.setBuffer (winsd);

	stsd.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Sounds/Star.wav");
	star_sound.setBuffer (stsd);
	
	mtbulsd.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Sounds/MeteorBullet.wav");
	metbul_sound.setBuffer (mtbulsd);

	font.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/BPdots.otf");
	player1Text.setFont (font);
	player2Text.setFont (font);
	player1ScoreText.setFont (font);
	player2ScoreText.setFont (font);
	demolText.setFont (font);

	endText.setFont (font);
	levelText.setFont (font);

	demolText.setString ("The Demolisher");
	player1Text.setString (map.player1Name);
	player2Text.setString (map.player2Name);
	player1ScoreText.setString (convertInt(map.player1Score));
	player2ScoreText.setString (convertInt(map.player2Score));

	demolText.setCharacterSize(24);
	player1Text.setCharacterSize(24);
	player2Text.setCharacterSize(24);
	player1ScoreText.setCharacterSize(30);
	player2ScoreText.setCharacterSize(30);

	demolText.setColor(sf::Color::White);
	player1Text.setColor(sf::Color::White);
	player2Text.setColor(sf::Color::White);
	player1ScoreText.setColor(sf::Color::White);
	player2ScoreText.setColor(sf::Color::White);

	demolText.setPosition (500, 650);
	player1Text.setPosition (10, 650);
	player2Text.setPosition (500, 650);
	player1ScoreText.setPosition (380, 643);
	player2ScoreText.setPosition (880, 643);

	//end_music.play();
	
//	set_score (0, 0);

	background_x = 0;
	start = false;
}

string Graphic::convertInt(int number){
    if (number == 0)
        return "0";
    string temp="";
    string returnvalue="";
    while (number>0)
    {
        temp+=number%10+48;
        number/=10;
    }
    for (int i=0;i<temp.length();i++)
        returnvalue+=temp[temp.length()-i-1];
    return returnvalue;
}


Graphic::~Graphic () {}

void Graphic::gameOver() {

	if (gameMode == 2) {
	
		map.player1Score += map.player1UP * 100;
		map.player2Score += map.player2UP * 100;
		if (map.player1Score > map.player2Score)
			gameOver (1);
		else if (map.player1Score < map.player2Score)
			gameOver (2);
		else
			gameOver (0);
		return;
	}

	game.close();
	
	game_music.stop ();


	sf::RenderWindow result (sf::VideoMode (game_x , game_y) , "Game Over!");

	result.clear (sf::Color::Black);

	if (map.player1UP) {
		endText.setString ("You Win!");
		win_sound.play();
	}
	else {
		endText.setString ("You Lose!");
		end_music.play();
	}
	endText.setColor(sf::Color::White);
	endText.setCharacterSize(100);
	endText.setPosition (250, 280);

	result.draw (end_pic);
	result.draw (endText);

	while (result.isOpen())
	{	

		sf::Event event;
		while (result.pollEvent (event))
		{
			if (event.type == sf::Event::Closed)
				result.close();
		}
		result.display ();
	}

}

void Graphic::gameOver(int w)
{

	game.close();
	
	game_music.stop ();

	end_music.play();

	sf::RenderWindow result (sf::VideoMode (game_x , game_y) , "Game Over!");

	if (w == 1)
		endText.setString (map.player1Name + " Wins!");
	if (w == 2)
		endText.setString (map.player2Name + " Wins!");
	if (!w)
		endText.setString ("Nobody Wins!");

	result.clear (sf::Color::Black);

	endText.setColor(sf::Color::White);
	endText.setCharacterSize(100);
	endText.setPosition (100, 280);

	result.draw (end_pic);
	result.draw (endText);	

	while (result.isOpen())
	{	
		sf::Event event;
		while (result.pollEvent (event))
		{
			if (event.type == sf::Event::Closed)
				result.close();
		}
		result.display ();
	}
}



void Graphic::showMenu ()
{

	menu_music.setLoop (true);
	menu_music.play();

	// make menu window
	sf::RenderWindow menu(sf::VideoMode(1024, 800), "Menu");
	
	// initialize the pictures

	sf::Texture backtex;
	backtex.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/menu.jpg");
	sf::Sprite ground (backtex);
	ground.setPosition (0 , 0);

	sf::Texture nametex;
	nametex.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Logo.png");
	sf::Sprite name (nametex);
	name.setPosition (65 , 0);
	
	sf::Texture playtex;
	playtex.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Buttons/button.bmp");
	sf::Sprite play (playtex);
	play.setPosition (325 , 270);
	
	sf::Texture howtex;
	howtex.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Buttons/button.bmp");
	sf::Sprite howplay (howtex);
	howplay.setPosition (325 , 400);
	
	sf::Texture themetex;
	themetex.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Buttons/button.bmp");
	sf::Sprite theme_pic (themetex);
	theme_pic.setPosition (325 , 530);
		

	sf::Texture quittex;
	quittex.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Buttons/button.bmp");
	sf::Sprite quit (quittex);
	quit.setPosition (325 , 660);

	menu.clear(sf::Color::Black);
	menu.draw (ground);	
	menu.draw (name);
	menu.draw (play);
	menu.draw (howplay);
	menu.draw (theme_pic);
	menu.draw (quit);
	menu.display();

	while (menu.isOpen ())
	{		

		if (isClicked (play, menuButtonLength, menuButtonHeight, menu)) {
			menu_music.stop ();
			menu.close ();
			usleep (10000);
			start_game ();
		}
	
		if (isClicked (quit, menuButtonLength, menuButtonHeight, menu)) {
			menu_music.stop ();
			menu.close ();
		}

	/*	if (contains(quit , sf::Mouse::getPosition(menu).x, sf::Mouse::getPosition(menu).y))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    			{
				menu.close();
    			}
		}

		if (contains(about , sf::Mouse::getPosition(menu).x, sf::Mouse::getPosition(menu).y))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    			{
				menu.setVisible (false);
				show_about ();
				menu.setVisible (true);
    			}
		}

		if (contains(play , sf::Mouse::getPosition(menu).x, sf::Mouse::getPosition(menu).y))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    			{
				menu.close();
				start_game ();
    			}
		}
	*/	
		// handle window close
		sf::Event event;
		while(menu.pollEvent(event))
    		{
			if (event.type == sf::Event::Closed)
			{
				menu.close ();
			}
		}
	}

}



bool Graphic::is_open()
{
	return (game.isOpen() && start);
}


/*void My_Graphic::show_about ()
{
	// make window
	sf::RenderWindow aboutwindow (sf::VideoMode (645 , 525) , "About");
	
	// initialize pics
	sf::Texture abouttex;
	abouttex.loadFromFile ("/home/mona/Desktop/project/Code/Pictures/about-con.png");
	sf::Sprite about (abouttex);
	about.setPosition (5 , 5);

	while (aboutwindow.isOpen ())
	{
		//add pics
		aboutwindow.clear (sf::Color::Black);
		aboutwindow.draw (about);
		aboutwindow.display ();

		// handle window close
		sf::Event event;
		while(aboutwindow.pollEvent(event))
    		{
			if (event.type == sf::Event::Closed)
			{
				aboutwindow.close ();
				break;
			}
		}
	}

}

*/



bool Graphic::isClicked (sf::Sprite& sprite, int length, int height, sf::RenderWindow &window) {	
	int x = sf::Mouse::getPosition(window).x;
	int y = sf::Mouse::getPosition(window).y;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (x >= sprite.getPosition().x && x <= sprite.getPosition().x+length) && (y >= sprite.getPosition().y && y <= sprite.getPosition().y+height))
		return true;
	return false;
		
}



void Graphic::repaint ()
{


	game.clear (sf::Color::Black);
	

	background_pic.setPosition (background_x, 0);
   	game.draw (background_pic);
       	background_pic.setPosition (background_x+game_x*2, 0);		
       	game.draw (background_pic);

	spaceship1_pic.setPosition (map.spaceship1.x*100 + 10, map.spaceship1.getY());
	game.draw (spaceship1_pic);

	if (gameMode == 2) {
		spaceship2_pic.setPosition (map.spaceship2.x*100 + 10, map.spaceship2.getY());
		game.draw (spaceship2_pic);
	}

	//Draw Meteors
	for (int i = 0; i < map.meteors.size(); i++) {
		meteor_pic.setPosition (map.meteors[i].x, map.meteors[i].line * 100 + 30);
		game.draw (meteor_pic);
	}


	//Draw Stars
	for (int i = 0; i < map.stars.size(); i++) {
		star_pic.setPosition (map.stars[i].x, map.stars[i].line * 100 + 30);
		game.draw (star_pic);
	}
	//Draw Hearts
	for (int i = 0; i < map.hearts.size(); i++) {
		heart_pic.setPosition (map.hearts[i].x, map.hearts[i].line * 100 + 30);
		game.draw (heart_pic);
	}


	//Draw Bullets
	for (int i = 0; i < map.bullets.size(); i++) {
//		if (map.bullets[i].type == 1) {
			bullet1_pic.setPosition (map.bullets[i].x, map.bullets[i].getY());
			game.draw (bullet1_pic);
//			cerr<<map.bullets[i].x<<"\t"<< map.bullets[i].getY()<<endl;
//		}
	}

	//Draw Aliens
	for (int i = 0; i < map.aliens.size(); i++) {
		if (map.aliens[i].type) {//Large Alien
			alien2_pic.setPosition (map.aliens[i].x, map.aliens[i].getY());
			game.draw (alien2_pic);
		}
		else { //Small Alien
			alien1_pic.setPosition (map.aliens[i].x, map.aliens[i].getY());
			game.draw (alien1_pic);
		}
	}

	if (level == 4) {
		monster_pic.setPosition (map.monster.x, map.monster.y);
		game.draw (monster_pic);
	}
	
	game.draw (player1Text);
	player1ScoreText.setString (convertInt(map.player1Score));
	game.draw (player1ScoreText);
	//Draw Lives
	int livesX = 140;
	for (int i = 0; i < map.player1UP; i++) {
		littleheart_pic.setPosition (livesX, 655);
		game.draw (littleheart_pic);
		livesX += 27;
	}


	if (gameMode == 2) {
		game.draw (player2Text);
		player2ScoreText.setString (convertInt(map.player2Score));
		game.draw (player2ScoreText);
		//Draw Lives
		livesX = 640;
		for (int i = 0; i < map.player2UP; i++) {
			littleheart_pic.setPosition (livesX, 655);
			game.draw (littleheart_pic);
			livesX += 27;
		}
	}
	
	if (level == 4) {
		game.draw (demolText);
		livesX = 720;
		for (int i = 0; i < map.monster.UP; i++) {
			littleheart_pic.setPosition (livesX,655);
			game.draw (littleheart_pic);
			livesX += 27;
		}	
	}

	game.display();
}

void Graphic::moveBackground () {

	if (level != 4) {
		background_x -= gameSpeed * 1;
		if (background_x <= -game_x*2)
			background_x = 0;			
	}
}

void Graphic::start_game () {
	game.create(sf::VideoMode(game_x, game_y), "Alien Shooter");
	game_music.play();
	level = 1;
	start = true;
	startLevel();
	repaint ();
}

void Graphic::startLevel () {
	background_x = 0;
	switch (level) {
		case 1:
			if (theme == 1) {
				backgroundtex.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Level1.jpg");
				background_pic.setTexture (backgroundtex);
			}
			else {
				backgroundtex.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/2_Level1.jpg");
				background_pic.setTexture (backgroundtex);
			}
			levelText.setString ("Level 1");
		break;
		case 2:
			if (theme == 1) {
				backgroundtex.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Level2.jpg");
				background_pic.setTexture (backgroundtex);
			}
			else {
				backgroundtex.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/2_Level2.jpg");
				background_pic.setTexture (backgroundtex);

			}
			levelText.setString ("Level 2");
		break;
		case 3:
			if (theme == 1) {
				backgroundtex.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Level3.jpg");
				background_pic.setTexture (backgroundtex);
			}
			else {
				backgroundtex.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/2_Level3.jpg");
				background_pic.setTexture (backgroundtex);

			}
			levelText.setString ("Level 3");
		
		break;
		case 4:
			if (theme == 1) {
				backgroundtex.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/Level4.png");
				background_pic.setTexture (backgroundtex);
			}
			else {
				backgroundtex.loadFromFile ("/home/eizhen/Documents/AP Project/Code/Pictures/2_Level4.png");
				background_pic.setTexture (backgroundtex);
				
			}
			levelText.setString ("Level 4");
		break;	
	}
	game.clear (sf::Color::Black);
	game.draw (background_pic);
	levelText.setColor(sf::Color::White);
	levelText.setCharacterSize(100);
	levelText.setPosition (250, 280);
	game.draw(levelText);
	game.display();
	usleep (levelTextDelay);	
}


class Movement {
	public:
		void move (Map &, Graphic &);
		void shoot (Spaceship &, Map &, int);
		void changeLevel (Map &);
		Movement();
	private:
		void checkCol (Map &, Graphic &);
		void createObject (Map &);
		void moveObject (Map &);
		sf::Time meteorTime, starTime, spaceshipColTime, heartTime, alien1Time, alien2Time;
		bool spaceshipCol, upStar, centerStar, downStar;
		static const int spaceshipColDelay = 150, shootDelay = 500; 
		
};


Movement::Movement () {
	meteorTime = starTime = gameClock.getElapsedTime();
	heartTime = gameClock.getElapsedTime() + sf::milliseconds(1000);
	spaceshipCol = false;
	upStar = downStar = centerStar = false;
}

void Movement::changeLevel (Map &map) {
	map.bullets.clear();
	map.stars.clear();
	map.aliens.clear();
	map.hearts.clear();
	map.meteors.clear();
	meteorTime = starTime = gameClock.getElapsedTime();
	heartTime = gameClock.getElapsedTime() + sf::milliseconds(1000);
	alien1Time = alien2Time = gameClock.getElapsedTime() - sf::milliseconds(gameDuration / 20 - 1000);
	spaceshipCol = false;
	upStar = downStar = centerStar = false;
	map.spaceship1.restart ();
	map.spaceship2.restart ();
}


void Movement::move (Map &map, Graphic &graphic) {
	moveObject (map);
	createObject (map);
	checkCol (map, graphic);	
}


void Movement::moveObject (Map &map) {

			//Move Meteors
			for (int i = 0; i < map.meteors.size(); i++) {
				map.meteors[i].moveMeteor();
				if (map.meteors[i].x < -map.meteors[i].length)
					map.meteors.erase (map.meteors.begin() + (i--));
			}
			//Move Stars
			for (int i = 0; i < map.stars.size(); i++) {
				map.stars[i].moveStar();
				if (map.stars[i].x < -map.stars[i].length)
					map.stars.erase (map.stars.begin() + (i--));
			}
			//Move Hearts			
			for (int i = 0; i < map.hearts.size(); i++) {
				map.hearts[i].moveHeart();
			}
			//Move Bullets
			for (int i = 0; i < map.bullets.size(); i++) {
				//cout << "ssdfg\n";
				map.bullets[i].moveBullet();
				if (map.bullets[i].x < -20 || map.bullets[i].x > game_x + 200) {
					//cout <<"Dddd\n";
					map.bullets.erase (map.bullets.begin() + (i--));
				}
			}
			//Move Aliens
			for (int i = 0; i < map.aliens.size(); i++) {
				if (!(map.aliens[i].type) && gameClock.getElapsedTime() - map.aliens[i].lastShoot >=  sf::milliseconds(map.aliens[i].shootDelay)) {
					map.aliens[i].lastShoot = gameClock.getElapsedTime();
					//cerr << map.aliens[i].y << endl;
					Bullet bul (map.aliens[i].x, map.aliens[i].y + (map.aliens[i].height / 2), 0);
					map.bullets.push_back (bul);
				}
				map.aliens[i].moveAlien();
				if (map.aliens[i].x < -map.aliens[i].length)
					map.aliens.erase (map.aliens.begin() + (i--));
			}
			if (level == 4) {
				if (gameClock.getElapsedTime() - map.monster.lastShoot >=  sf::milliseconds(map.monster.shootDelay)) {
					map.monster.lastShoot = gameClock.getElapsedTime();
					Bullet bul (map.monster.x, map.monster.y + (map.monster.height / 2), 0);
					map.bullets.push_back (bul);
				}
				map.monster.moveMonster ();
			}
}

void Movement::createObject (Map &map) {
	switch (level) {
		case 1:
			//Create Meteors
			if ((gameClock.getElapsedTime() - meteorTime) >= sf::milliseconds(gameDuration / 300) && (gameClock.getElapsedTime() - starTime) >= sf::milliseconds(500)) {
				meteorTime = gameClock.getElapsedTime();
				int h = (rand() % 6);
				Meteor mt (h);
				map.meteors.push_back (mt);
			}
			//Create Stars
			if ((gameClock.getElapsedTime() - starTime) >= sf::milliseconds(gameDuration / 30)) {
				starTime = gameClock.getElapsedTime();
				int h = (rand() % 6);
				Star st (h);
				map.stars.push_back (st);
			}
			//Create Hearts
			if ((gameClock.getElapsedTime() - heartTime) >= sf::milliseconds(gameDuration / 3)) {
				heartTime = gameClock.getElapsedTime();
				bool found = false;
				do {
					int whichStar = rand() % 3;
					switch (whichStar) {
						case 0: //upStar
							if (!upStar) {
								upStar = true;
								found = true;
								int l = rand() % 2;
								Heart ht (l);
								map.hearts.push_back (ht);
							}
						break;
						case 1: //centerStar
							if (!centerStar) {
								centerStar = true;
								found = true;
								int l = rand() % 2 + 2;
								Heart ht (l);
								map.hearts.push_back (ht);
							}
						break;
						case 2: //downStar
							if (!downStar) {
								downStar = true;
								found = true;
								int l = rand() % 2 + 4;
								Heart ht (l);
								map.hearts.push_back (ht);
							}
						break;
					}				
				} while (!found);
			}
		break;
		
		case 2:
			//Create Aliens
			if ((gameClock.getElapsedTime() - alien2Time) >= sf::milliseconds(gameDuration / 30)) {
				//cerr << gameClock.getElapsedTime().asMilliseconds() << "\t" << alien2Time.asMilliseconds() << endl;
				alien2Time = gameClock.getElapsedTime();
				int h = rand () % 600;
				int which = rand () % 3;
				if (!which) {
					Alien ali (h, 0);
					map.aliens.push_back (ali);
				}
				else {
					Alien ali (h, 1);
					map.aliens.push_back (ali);
				}
			}

			//Create Hearts
			if ((gameClock.getElapsedTime() - heartTime) >= sf::milliseconds(gameDuration / 3)) {
				heartTime = gameClock.getElapsedTime();
				bool found = false;
				do {
					int whichStar = rand() % 3;
					switch (whichStar) {
						case 0: //upStar
							if (!upStar) {
								upStar = true;
								found = true;
								int l = rand() % 2;
								Heart ht (l);
								map.hearts.push_back (ht);
							}
						break;
						case 1: //centerStar
							if (!centerStar) {
								centerStar = true;
								found = true;
								int l = rand() % 2 + 2;
								Heart ht (l);
								map.hearts.push_back (ht);
							}
						break;
						case 2: //downStar
							if (!downStar) {
								downStar = true;
								found = true;
								int l = rand() % 2 + 4;
								Heart ht (l);
								map.hearts.push_back (ht);
							}
						break;
					}				
					} while (!found);
			}
			//Create Stars
			if ((gameClock.getElapsedTime() - starTime) >= sf::milliseconds(gameDuration / 30)) {
				starTime = gameClock.getElapsedTime();
				int h = (rand() % 6);
				Star st (h);
				map.stars.push_back (st);
			}		
		break;
		case 3:
			//Create Meteors
			if ((gameClock.getElapsedTime() - meteorTime) >= sf::milliseconds(gameDuration / 200) && (gameClock.getElapsedTime() - starTime) >= sf::milliseconds(500)) {
				meteorTime = gameClock.getElapsedTime();
				int h = (rand() % 6);
				Meteor mt (h);
				map.meteors.push_back (mt);
			}
			//Create Stars
			if ((gameClock.getElapsedTime() - starTime) >= sf::milliseconds(gameDuration / 30)) {
				starTime = gameClock.getElapsedTime();
				int h = (rand() % 6);
				Star st (h);
				map.stars.push_back (st);
			}
			//Create Hearts
			if ((gameClock.getElapsedTime() - heartTime) >= sf::milliseconds(gameDuration / 3)) {
				heartTime = gameClock.getElapsedTime();
				bool found = false;
				do {
					int whichStar = rand() % 3;
					switch (whichStar) {
						case 0: //upStar
							if (!upStar) {
								upStar = true;
								found = true;
								int l = rand() % 2;
								Heart ht (l);
								map.hearts.push_back (ht);
							}
						break;
						case 1: //centerStar
							if (!centerStar) {
								centerStar = true;
								found = true;
								int l = rand() % 2 + 2;
								Heart ht (l);
								map.hearts.push_back (ht);
							}
						break;
						case 2: //downStar
							if (!downStar) {
								downStar = true;
								found = true;
								int l = rand() % 2 + 4;
								Heart ht (l);
								map.hearts.push_back (ht);
							}
						break;
					}				
					} while (!found);
			}
			//Create Aliens
			if ((gameClock.getElapsedTime() - alien2Time) >= sf::milliseconds(gameDuration / 30)) {
				//cerr << gameClock.getElapsedTime().asMilliseconds() << "\t" << alien2Time.asMilliseconds() << endl;
				alien2Time = gameClock.getElapsedTime();
				int h = rand () % 600;
				int which = rand () % 3;
				if (!which) {
					Alien ali (h, 0);
					map.aliens.push_back (ali);
				}
				else {
					Alien ali (h, 1);
					map.aliens.push_back (ali);
				}
			}
		break;
	}
}

void Movement::checkCol (Map &map, Graphic & graphic) {
	//Check Meteor
	for (int i = 0; i < map.meteors.size(); i++) {
		//For Bullets
		bool bulletCol = false;
		for (int j = 0; j < map.bullets.size(); j++) {
			if (map.meteors[i].line == map.bullets[j].line && map.meteors[i].x <= map.bullets[j].x && map.meteors[i].x + 10 >= map.bullets[j].x && map.bullets[j].type) {
				map.meteors.erase (map.meteors.begin() + (i--));
				bulletCol = true;
				map.bullets.erase (map.bullets.begin() + (j--));
				graphic.metbul_sound.play ();
			}
		}
		//For Spaceships
		if (!bulletCol) {
			bool is = false;
			if (map.meteors[i].line == map.spaceship1.line && map.meteors[i].x <= map.spaceship1.getX() && map.meteors[i].x >= map.spaceship1.getX() - map.spaceship1.length) {
				is = true;
				map.player1UP--;	
			}
			if (gameMode == 2) {
				if (map.meteors[i].line == map.spaceship2.line && map.meteors[i].x <= map.spaceship2.getX() && map.meteors[i].x >= map.spaceship2.getX() - map.spaceship2.length) {
					is = true;
					map.player2UP--;
				}
			}
			if (is) map.meteors.erase (map.meteors.begin() + (i--));
		}
	}

	//Check Alien
	for (int i = 0; i < map.aliens.size(); i++) {
		//For Bullets		
		bool bulletCol = false;
		for (int j = 0; j < map.bullets.size(); j++) {
			if (map.bullets[j].type){
				if ((map.bullets[j].getY() >= map.aliens[i].getY() && map.bullets[j].getY() <= map.aliens[i].getY() + map.aliens[i].height) && (map.bullets[j].x >= map.aliens[i].x && map.bullets[j].x <= map.aliens[i].x + map.aliens[i].length)) {
					map.aliens.erase (map.aliens.begin() + (i--));
					bulletCol = true;
					if (map.bullets[j].type == 1)
						map.player1Score += 5;
					else if (map.bullets[j].type == 2)
						map.player1Score += 5;
					map.bullets.erase (map.bullets.begin() + (j--));
				}
			}
		}
		//For Spaceships
	//	if (!bulletCol) {
////	//		bool is = false;
///	//		if (map.aliens[i].x <= map.spaceship1.getX() ) {
//	//			is = true;
//	//			map.player1UP--;	
///	//		}
////	//		if () {
//	////			is = true;
//	//			map.player2UP--;
//	//		}
//	//		if (is) map.meteors.erase (map.meteors.begin() + (i--));
//	//	}
	}

	//Check Bullet with Spaceship
	for (int i = 0; i < map.bullets.size(); i++) {
		bool col = false;
		if (!map.bullets[i].type) {
			if ((map.bullets[i].x <= map.spaceship1.getX()) && (map.bullets[i].x >= (map.spaceship1.getX() - map.spaceship1.length)) &&
			    (map.bullets[i].y >= map.spaceship1.getY()) && (map.bullets[i].y <= (map.spaceship1.getY() + map.spaceship1.height)) ) {
				map.player1UP--;
				map.bullets.erase (map.bullets.begin() + (i--));
			}
			if (!col && gameMode == 2) {
				if ((map.bullets[i].x <= map.spaceship1.getX()) && (map.bullets[i].x >= (map.spaceship1.getX() - map.spaceship1.length)) &&
			    (map.bullets[i].y >= map.spaceship1.getY()) && (map.bullets[i].y <= (map.spaceship1.getY() + map.spaceship1.height)) ) {
					map.player2UP--;
					map.bullets.erase (map.bullets.begin() + (i--));
				}
			}
		}
	}
	//Check Star
	for (int i = 0; i < map.stars.size(); i++) {
		bool is = false;
		if (map.stars[i].line == map.spaceship1.line && map.stars[i].x <= map.spaceship1.getX() && map.stars[i].x >= map.spaceship1.getX() - map.spaceship1.length) {
			is = true;
			map.player1Score += 10;
		}
		if (gameMode == 2)
			if (map.stars[i].line == map.spaceship2.line && map.stars[i].x <= map.spaceship2.getX() && map.stars[i].x >= map.spaceship2.getX() - map.spaceship2.length) {
				is = true;
				map.player2Score += 10;
			}
		if (is) {
			map.stars.erase (map.stars.begin() + (i--));
			graphic.star_sound.play();
		}
	}
	//Check Spaceship
	if (gameMode == 2)
		if (!spaceshipCol && map.spaceship1.line == map.spaceship2.line && map.spaceship1.x == map.spaceship2.x) {
			map.player1UP--;
			map.player2UP--;
			spaceshipColTime = gameClock.getElapsedTime();
			spaceshipCol = true;
		}
		if (spaceshipCol && (gameClock.getElapsedTime() - spaceshipColTime >= sf::milliseconds(spaceshipColDelay))) {
			spaceshipCol = false;
			map.resetSpaceships();
		}
	//Check Heart
	for (int i = 0; i < map.hearts.size(); i++) {
		bool is = false;
		if (map.hearts[i].line == map.spaceship1.line && map.hearts[i].x <= map.spaceship1.getX() && map.hearts[i].x >= map.spaceship1.getX() - map.spaceship1.length) {
			is = true;
			map.player1UP ++;
		}
		if (gameMode == 2)
			if (map.hearts[i].line == map.spaceship2.line && map.hearts[i].x <= map.spaceship2.getX() && map.hearts[i].x >= map.spaceship2.getX() - map.spaceship2.length) {
				is = true;
				map.player2UP ++;
			}
		if (is) {
			map.hearts.erase (map.hearts.begin() + (i--));
			graphic.heart_sound.play();
		}
	}

	//Check Monster
	if (level == 4) {
		for (int i = 0; i < map.bullets.size(); i++) {
			if (map.bullets[i].type) {
				if ((map.bullets[i].x >= map.monster.x) && (map.bullets[i].x <= (map.monster.x + map.monster.length)) && (map.bullets[i].getY() >= map.monster.y) && (map.bullets[i].getY() <= (map.monster.y + map.monster.height))) {
					map.monster.UP--;
					map.bullets.erase (map.bullets.begin() + (i--));
					if (!map.monster.UP)
						graphic.gameOver();
				}
			}
		}
	}


	//Check Spaceship Lives
	if (gameMode == 2) {
		if (!map.player1UP && !map.player2UP) {
			map.player1Score += map.player1UP * 100;
			map.player2Score += map.player2UP * 100;
			if (map.player1Score > map.player2Score)
				graphic.gameOver (1);
			else if (map.player1Score < map.player2Score)
				graphic.gameOver (2);
			else
				graphic.gameOver (0);
		}
		else if (!map.player1UP)
			graphic.gameOver (2);
		else if (!map.player2UP)
			graphic.gameOver (1);		
	}
	else if (!map.player1UP) graphic.gameOver ();
		
}


void Movement::shoot (Spaceship &ship, Map &map, int type) { //Bullet type
	if (gameClock.getElapsedTime() - ship.lastShoot >= sf::milliseconds(shootDelay) && !ship.moves.size()) {
		ship.lastShoot = gameClock.getElapsedTime();
		Bullet bul (ship.getX(), ship.line, type);
		map.bullets.push_back (bul);
	}
}



class Server {
	public:
		int nextMove (Map &, Spaceship&, Spaceship&);
};


int Server::nextMove (Map &map, Spaceship &ship, Spaceship &othership) {
	for (int i = 0; i < map.meteors.size(); i++) {

		if (map.meteors[i].line == ship.line && map.meteors[i].x - ship.getX() <= 30 && map.meteors[i].x > ship.getX())
			return right;


		if (map.meteors[i].line == ship.line && map.meteors[i].x - ship.getX() <= 80 && map.meteors[i].x > ship.getX()) {
			if (!ship.line) {
				for (int j = 0; j < map.meteors.size(); j++) {
					if (map.meteors[j].line == ship.line + 1 && map.meteors[j].x + map.meteors[j].length > ship.getX() - ship.length && map.meteors[j].x - ship.getX() <= 100)
						return 6;
				}
				if (othership.line == ship.line + 1)
					return left;
				return down;
			}
			if (ship.line == 5) {
				if (othership.line == ship.line - 1)
					return left;
				return up;
			}
			if (othership.line == ship.line + 1) {
				return left;
			}
			return up;
		}
	}
	return 6;
}
