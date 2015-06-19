<<<<<<< HEAD
Routing Project Framework
===============

## Project Overview

You will use this framework for the following projects:

- Grid Routing
- Advanced Grid Routing
- Soukup Global Grid Routing
- Line Routing

The project will involve reading in JSON test files (located in the test directory) and parsing these files into a Problem Object. This step has already been done for you and is written at the beginning of main.cc to allow for command line arguments. **You should not change this code**, as it will be used for testing. Please note that the ProblemObject gives you the width and height of the chip, which will be **indexed from 0 to n-1**.

You will then take this Problem Object, and pass it into your own custom problem map object. The custom problem map object is the object you use to reprsent the problem space. In the example we give that object is the Grid object, however **you should not use the grid object** as it is not adequate for this project, and has been provided only as an example. You may, however, want to take advantage of the other classes we give you, such as the Node and Edge classes. While it is not required that you use any classes we give you (except for the ProblemObject input and Path/Netlist output), we highly encourage you to use as much of the provided code as fits your requirements. You will also find that many of the classes we provide are too general for your use, in this case we advise you to extend the base class with a child class designed to fit your specific needs.

Once you have created your custom problem map object, you should make a call to a routing algorithm (the specific algorithm will depend on which project you are working on). You should strive to be able to use the same custom problem object for mulitple routers, however it is not required that all routes work with the same custom problem object. Your routing algorithm **must return either a Path/vector<Path> or Netlist/vector<Netlist>** depending on what your specific project requires.

- Path: A path is a set of straight line segments from one source to one sink
- Netlist: A netlist is a set of straight line segments from one souce to multiple sinks

Once you have a solution in the form of a Path/vector<Path> or Netlist/vector<Netlist>, you should print your solution to std out for review.

Currently there is only one test case given, and it is not a robust test case. It is given as an example so you can create your own test cases. **You will need to create multiple robust test cases** that are capable of testing malformed input files, edge cases, and problems that are impossible to solve (just to name a few). On input files that cause your program to fail, **your program should error gracefully**, meaning if there is a bad input file or some type of error the program should not simply crash. It should give an explination as to the possible error and exit. We have given you the claim system for printing errors and closing the program, please use it.

## Running the Framework

In order to run the framework, you must first go to Utilities/JSON_parser and run the "make" command. This will create a json_parse.so file that is need to parse the JSON test files that we give you. Next you will go back to the root directory and run the "make" command. This will generate an executable called "grid_router", which you can run by calling "./grid_router <test_case>", where <test_case> is the path of a test case. In the framework we give you there is only one provided test case, located at "Tests/test_sample.json".

**Please Note:** some of you may get a library error when trying to run grid_router. If you recieve this library error, please copy the file "json_parser.so" from Utilities/JSON_parser to the root directory where the file "grid_router" exists and this should fix the problem. Note that you must have already called "make" in Utilities/JSON_parser for "json_parse.so" to exist.

## Bugs

We expect there to be bugs in this framework. When you find a bug please [write a good bug report](http://www.chiark.greenend.org.uk/~sgtatham/bugs.html) and submit it to the GitHub issues page for this repository. Or better yet create a fix to the problem and [submit a pull request](https://help.github.com/articles/using-pull-requests/). You should also be **checking the issues tracking page** of the repository for known problems before emailing me or submitting a new bug report.

## Your Repository

For your submission, you must have a repository updated with the code you wish to submit by 8:00AM PST on the due date. Your repository must also adhere to the following rules.

- I must be [added as a collaborator on your repository](https://help.github.com/articles/adding-collaborators-to-a-personal-repository/) (username: brrcrites)
- Your README.md must at minimum have the name(s) of your group members (and hopefully more useful information)
- You must email me a link to your repository

In addition, the master branch of your repository **must** always be compilable. If you are working on a new feature that will cause your code to no longer compile, those updates should be put in a branch and merged to the master when completed, tested, and stable.

=======
# Grid-Routing

Implementation of Lee's Algorithm and Hadlock's Algorithm using the base project source code provided in: https://github.com/brrcrites/Routing_Framework 
>>>>>>> 0735dc1d37d7761e544a2e95b16fcc82072eb291
