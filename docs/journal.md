Rover Journal
=============

Overview
--------

This journal traces the decisions, assumptions made and milestones of the Rover project.  Given the scope and purpose of the project the tone is fairly casual and lightweight.  
Keep in mind this is a living and changing document.  If you wish to observe the evolution, be sure to play around with the git logs and diffs.

Let's have some fun :)

1 Initial Thoughts
------------------

There's two sets of requirements at work here - the actual problem of implementing a working solution and secondly, providing evidence of a professional and effective development approach. After all, we're trying to prove we're more than just an ordinary hacker.

To that end let's break the project into three parts:
 * the development 'process'
 * break down the spec into requirements/stories
 * create a technical environment to support development


### The process

The requirements are known up-front, but regardless we'll take an iterative approach because:
  * We'll always have a 'kind-of' working solution nearby.
  * We can prioritise our requirements
  * The requirements may still demand bad assumptions and thus change.
  * We may exhaust our time-budget
  * Forces us to adopt good techniques - TDD, CI, easing any necessary refactoring.


### Implicit requirements and assumptions.

Most of the requirements given by the spec we can use as is, i.e., the actions for receiving L, M and R commands.  However, some need assumptions for a correct solution. Our assumptions lend conservatively towards rover safety and recovery.  That is, we'll completely ignore a series of commands that may damage or confuse the rover.

* The plateau is a finite, uniform 2d grid
* All grid coordinates, dimensions and rover (X, Y) positions are positive integers.
* A rover's facing is specified by N, E, S and W, matching North, East, South and West compass directions respectively.
* A rover will halt and ignore future commands if the next command is invalid.
* A rover command is invalid when it either, results in moving off the plateau edge, crashing into another rover or is not a character in the set {'L', 'M', 'R'}.
* A spin command, {'L', 'R'}, is relative to the rover's facing.
* A move forward, 'M' moves in the direction of the rover's facing.
* A rover always perfectly receives sent commands
* A rovers actions are independent of other rovers.
* A rover's position is always denoted by an X Y F tuple, matching x-coordinate, y-coordinate and facing respectively.
* A rover that is executing spin commands is classified as 'moving', implying the next rover must wait.
* A single (X, Y) position corresponds to a maximum of one rover.  For multiple rovers,
this implies they'll start and end in non-overlapping positions.
* Multiple rovers starting in the same (X, Y) position aborts the simulation (physically impossible universe!).
* Input and output commands are ASCII encoded


### The Technical Environment

Alright the fun stuff! Essentially we want a lightweight CI environment such that we 
can easily incorporate new features and tests and revise existing ones.  For this project, I think it's sufficient to rely on 'on-demand' compilation and test executions - build servers are too heavyweight and just overkill. We want to support multiple platforms too, since we don't know which is preferred (let's not interrupt people's weekends :) and it's just good practice. 

Implementation wise we apparently have total freedom in language…but since the position description expects strong C++ we'll stick to that…no lisps! Since C++11 has been released and is supported well on all platforms via GCC, clang and MSVC++ we'll utilise that over C++ 03.  Apart from Boost and testing libs, we won't use any 3rd party tools.

To create a platform independent build system we'll adopt CMakeCMake supports all platforms of interest, unlike autotools. Additionally, it's straightforward to pull in external dependencies and maintain a good project hierarchy.

For unit tests we'll adopt Google-Test.  It's a little simpler than CppUnit and has additional features like mocks.

For configuration management I'll use Git.  The only good CM is distributed, leaving mostly Git and Mercurial and I'm more familiar with Git.

For project tooling we'll use Bash scripts.  This eliminates Windows as a platform but it's safe to assume either Linux or Mac OS X is available.


## 2 - Let's start building!

First steps are to
* Incorporate some of our journal notes into some documents, like a design rationale.
* Create the skeleton project layout and import our docs
* Create a git repository

We don't want to go overboard with documentation, there's no handovers, contractual obligations or standards
we need to follow, so any documents we create are for a) improving our understanding and b) clarifying 
the decisions and assumptions made.  I believe a single design doc will suffice.

## 3 - Build a solid environment

We want to avoid dependencies on particular IDE's etc and they only make cross platform 
issues more prevalent, even Qt.  So we'll assemble a little recursive CMake hierarchy,
with a couple scripts to pull dependencies.  Even though we don't 'really' need it yet,
we'll split the Rover program into roverlib with an API for controlling rovers and a separate
rovers executable - saves messing with the CMakeLists later.


## 4 - Prioritize

Ultimately we want to be able to send command lists to multiple rovers 
and they'll execute them in order. We can break this down into smaller,
prioritized tasks:

1. A single rover can _act_ (move and rotate).
2. A rover can perform multiple successive actions.
3. A rover can be commmanded with ascii command strings.
4. A rover moves only to unoccupied squares.
5. A rover gracefully stops when an invalid command is received.
6. Multiple rovers can explore the plateau.
7. A user can input an environment (plateau, rovers) and rover command lists.
8. Successive rovers explore without collisions.
9. All user-input rovers output the final position of the exploration path.
9. A user receives the final positions of all user-input rovers 
10. A user can receive help using the program.

## 5 Accepting change...

It appears gtest 1.6 doesn't support c++11 - conflicts with its default use
of tr1 tuple.  Although we could explicitly tell gtest to build using
its own tuple, there's likely other issues on my platform or another.  So,
I regretfully wave goodbye to c++11 usage in the interest of portability.
I'm going to miss my lambdas and initialisers.

## 6 First task complete!

Hurrah and all that.  The core model logic is there, with unit tests to prove it.  The documentation is coming along nicely too.  So far there's no real need for design or refactoring, though I have considered is making the Rover immutable - less chance of bugs. Also let's adopt a more functional 
approach, as seen with the reduce/accumulate usage in the command() function.


