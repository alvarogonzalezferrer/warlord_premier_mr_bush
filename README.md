
# Mighty Warlord Premier Bush, SpeedHack 2007

A speedhack game of evil shooting action, winner most evil category 2007

By Kronoman
Under the MIT license - 2007-2020

"at a crossroads, I did a deal to make my game the most evil ever"

## Tools and Libraries Used

Tools GNU/GCC and KDE, Kate, bash, GIMP, Audacity

Platform: Linux, Windows, MS-DOS (maybe, not tested yet)

Language: C++

Libraries: Allegro 4.2 (will NOT work with Allegro 5!)

## How to Compile

### Windows

You need Allegro 4.2 library, I suggest precompiled one!

https://sourceforge.net/projects/alleg/files/allegro-bin/

And the Mingw32 + MSYS toolchain and compiler

http://www.mingw.org/

### Linux

You need Allegro 4 dev package and g++ compiler.

### MS-DOS / DOSBOX

You need the special source code massaged for MS-DOS.
Is in the release folder.

The tools you need:
- DJGPP C++ (gxx) compiler
- Allegro 4.2 (NOT ALLEGRO 5.X!)
- DOSBOX or real hardware i.e a 486 DX2

Have retro fun!

### Game data

The game data is compiled with the GRABBER tool that comes with Allegro 4

## SpeedHack Rules

This game was made in 24 hs to fullfil the SpeedHack 2007 rules, and won most evil game that year.

http://www.speedhack.allegro.cc/

### RULES of the contest

The rules, and how I implemented them

* You must be the evil bad guy, destroying all the good guys. Mercy and compassion have no place in the game.

Warlord Premier Bush drops bombs over innocent childs to drink their blood and stay forever young and rich.

* Some objects must have a property that will affect other objects within a given radius. The effect must be proportional to the distance between the objects.

Grenades and airstrike. Also air friction and gravity affects falling objects (like child limbs >:{)

* The game must feature characters or objects that are affected differently depending on the amount of friction of the surface with which they are in contact.

The surface of the floor affects speed of moving child. Ice, grass, rock, etc have different friction.
Also, there is gravity, the little fellows will jump and such, watch closely as they do sky jumps on snow land.

* You must support at least one widescreen and one fullscreen resolution.

The game can support any resolution that Allegro can, press F11 on any place in the game to change it. I could not test the widescreen ones, because I don't have such monitor, but if Allegro can detect the resolution in your widescreen monitor, the game will be able to run on it too. In fact, it will run in any true color resolution, but I recommend 640x480@24 bpp.

* Reverse Stereotypes

Well, the real guy(tm) is a good nice man, he does lots of good stuff, and kisses childs instead of shooting them, and everybody loves him (somehow he won more than once the biggest political chair in the world, right? a lot of people must love him :P )
On the other hand, the child usually don't run around trying to conquer the world, right?

* Dialogue

When you want to do a preemptive airstrike, your secretary will advice you on the course to take.
Just use Y/N to answer her questions and things will happen, good or bad...

* Act of Generosity

I really did not tested the wide screen support for lack of hardware. I hope it works, if not, I apply this rule :P

## REUSED CODE & STUFF

Well, I reused a Arcade Game Framework that I made back then in 2007, was great sucess!

All the rest was coded on a hurry in about 7 or 8 hs. :P

I'm not really proud of the hacked code, I used all the dirty tricks to code fast, and not elegant, so the code is a mess, there are parts of code that are never used, and others that look more like BASIC code. 

I code much better when I'm in a relaxed working environment. 

The whole game took like 8 hs in 3 days. Anyways, the job is done, and works(tm).

I used stock photos off the web, and I did some sprites with Gimp too. Sounds are free stock, modified and tuned by me, most grabbed from a CD with sounds of the magazine Computer Music:Samples del milenio vol 12. and from the web.

## About the Game

You are Mighty Warlord Premier Bush.
Your mission is to drink the blood of every single man, woman and child!
Destroy the world so that for the next thousand years not even a single lizard will survive in the land.

## How to Play

You must kill the childs fast enough to harvest blood to replenish your life after each level and stay alive and kicking.

To do so, use this controls:

Aim with the mouse.
Left mouse button to shoot.
Right mouse button to drop a WOMD
Middle mouse button or ENTER to call in preemptive airstrike (Use Y/N to dialog with secretary Yarr!)

F10 to quit the game
F11 to change video mode
SPACE to skip intro screen, etc

The command line takes some switches.
Try -w for windowed mode
Try -? for other commands.
