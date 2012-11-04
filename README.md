Rovers
======


Description
-----------

The Rovers project simulates a squad of robotic rovers exploring
a plateau on Mars.

Repository Contents
-------------------

  * src - Sources (C++ TR1 and Boost)
  * tests - Unit tests and test files
  * docs - Documentation

Documentation
------------

The docs folder contains additional documents:
  * problem_description.txt - the original problem statement
  * journal.md - a journal of development activity.
  * rovers_design.docx - The Rovers Design document.


Prequisities
------------

This version of _Rovers_ is currently limited to 'nix systems.  It's been
tested on Mac OS X 10.8 and Ubuntu 12.04.  However the program uses only 
standard C++ TR1, STL and Boost, thus combined with the use of cmake 
means it should also function on Windows with a suitable bootstrap  
(not provided at this time).

 - 'nix (Ubuntu, Mac OS X verified)
 - Boost (tested on 1.51)
 - Developer/Command line tools (on Mac OS X 10.8)
 - cmake 2.8+
 - git (not essential but recommended)


Installing Prequisites 
----------------------

 ### Ubuntu

 sudo apt-get install git cmake libboost-all-dev

 ### Mac OS X

 Using MacPorts (http://www.macports.org/install.php):

 sudo port install git-core cmake boost

 Install XCode developer tools from Apple's App store (wish I had a port for that!)
 Update XCode (via Preferences) to install Command Line Tools.


Installing
----------

Assuming you've received the repository as a tgz, first extract it:

  tar xzf rovers.tgz
 
### Build & Test Rovers

 1. Run bootstrap 

    cd rovers
    ./bootstrap.sh
 
    This will initialize the build system (dl gtest, run cmake).
 
 2. Build Rovers

    cd build
    make

 3. Run the unit-tests
    ./unittest-all


Usage
-----

From the build directory, run rovers with the help option.

  ./rovers --help

With no arguments specified, rovers reads simulation input from stdin.
  An example session of 2 rovers on a 6x6 plateau:

  ./rovers

>    5 5
>    1 2 N
>    LMLMLMLMM
>    3 3 E
>    MMRMMRMRRM
>    ^D

  Where ^D signals end-of-input.

Credits
-------

Bit of this, bit of that - Daniel C Grigg

