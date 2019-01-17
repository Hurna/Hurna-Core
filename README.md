# H.urna Core - C++

H.urna Core is a relatively small **open source template project** that uses modern C++.
It is implemented as a **cross-platform, easy to build, full headed library** containing some of the
most known **Algorithms and Data Structures** (cf. [Implementations](#implementations)).

It aims to **help to learn programming in C++, practice job interview coding tests** and
allows anyone to **build its own best quality software and
work-flow with no proprietary dependencies nor costs**.

It uses **best practices software development**
(cross-building, unit testing, automated documentation...); giving you a **great caveat to start developing
with the promise to conserve great quality standards**.
Please note that **all the possible dependencies included are open-source and free to use**.

## Practices and Guidelines

- **Automated, Customized, Documentation Generation**: CMake, Doxygen, Bootstrap-Doxygen
- **Automated Building, Cross Building, Portability**: CMake
- **Bug Tracking System, Code Reviews**: Git
- **Coding Style, Coding Standard**: _CppLint_
- **Code Coverage, Code Quality, Dashboarding, Monitoring**: CDash, CTest, GTest
- **Statical analysis**: _Cppcheck_
- **Proper Folders Structure - Consistency**
- **Testing, Unit Testing (UT), Test-Driven Development (TDD)**: CMake, GTest
- **Version Control System**: Git

## Tools
*Required:*
- [**CMake**](http://www.cmake.org):
The building relies on CMake, an Open-Source system that
allows to easily build cross-platform applications by generating native makefiles.
- [**Git**](https://git-scm.com):
Git is a free and open source distributed version control system designed to
handle everything from small to very large projects with speed and efficiency.

*Optionnal:*
- [**CDash**](https://www.cdash.org): Is a web dashboard that aggregates, analyzes, and displays summary and
detailed views of build and test results on a historical basis.
It builds organized builds by submitter, platform, architecture, compiler, and compiler setting.
- [**CppLint**](https://github.com/google/styleguide):
Every major open-source project has its own style guide: a set of conventions (sometimes arbitrary)
about how to write code for that project. It is much easier to understand a large codebase when
all the code in it is in a consistent style.
CppLint a C++ style checker following Google's C++ style guide.
- [**Cppcheck**](https://github.com/danmar/cppcheck):
Cppcheck is a static analysis tool for C/C++ code. It provides unique code analysis to
detect bugs and focuses on detecting undefined behaviour and dangerous coding constructs.
The goal is to detect only real errors in the code (i.e. have very few false positives).
- [**CTest**](https://gitlab.kitware.com/cmake/community/wikis/doc/ctest/Testing-With-CTest):
CTest is a testing tool distributed as a part of CMake. It can be used
to automate updating (using CVS for example), configuring, building,
testing, performing memory checking, performing coverage, and submitting
results to a CDash orcDart dashboard system.
- [**Doxygen**](http://www.stack.nl/~dimitri/doxygen):
Doxygen is the de facto standard tool for generating documentation from annotated C++ sources,
but it also supports other popular programming languages such as C, Objective-C, C#, PHP, Java, Python,
 IDL (Corba, Microsoft, and UNO/OpenOffice flavors), Fortran, VHDL, Tcl, and to some extent D.
- [**Doxygen-Bootstrapped**](https://github.com/Velron/doxygen-bootstrapped):
Customize doxygen output to use the twitter bootstrap framework.
- [**GTest**](https://github.com/google/googletest):
Google Test (GTest) is a cross-platform unit testing library for the C++ programming language.
It allows unit-testing of C sources as well as C++ with minimal modification.
The tests themselves could be run one at a time, or even be called to run all at once,
making the debugging process very flexible.


This toolset lower costs and reduce risk by identifying problems earlier:
it **improves work-flow, increase productivity, gives you agility and project health monitoring**.

## Quick Access

- [**Documentation**](https://wiki.hurna.io/hurna_core/doc/index.html)
- [**Git Repository**](https://github.com/Hurna/Hurna-Core)

### Related

- [**H.urna Website**](https://hurna.io/): The main website of H.urna organization.
- [**H.urna Explorer**](https://demo.hurna.io/): A free, online, visualization platform.
- [**H.urna Wiki**](https://wiki.hurna.io/): Wiki containing free sciences courses and resources.
- [**H.urna Community**](https://community.hurna.io/): H.urna community to get help and news.

# Getting Started
## Build using CMake-GUI:

    Run CMake-GUI
    Select source/target directory
    Click on Configure the project
    Click on Generate the Solution files
    Open the solution file (if MSVC) and build or use make command.

## Build using Command-Line (UNIX):

    mkdir Hurna-Core-Build (Create Build directory)
    cd Hurna-Core-Build    (Open build director)
    ccmake ../Hurna-Core   (Configure the make given Source repository)
                           (Press 'c' to configure, then 'g' to generate the solution)
    make                   (Make command, use -j parameter to use multi-core)

## Build using QT Creator:

    Open CMakeLists.txt
    Configure build directories and CMake options
    Build

# Doxygen Documentation
Use the CMake **BUILD_DOC** (default to true) option to automatically setup Documentation Generation.

# GTest - Unit Testing
Use the CMake **BUILD_TESTING** (default to true) option to automatically setup Unit Testing.
It allows to very quickly get set up with google test:

- Clone GTest via GIT
- Setup the Unit Testings
- Compile GTest at build time

Use the CMake **WITH_COVERAGE** (default to true) option to automatically setup Coverage Generation.
The minimal required coverage for this project is **95%**.

# Running Unit Tests (UTs) and Update Dashboards
You can whether use **CTest** or **manually** run the unit tests.

To run the set of tests in multi-core:

    cd Hurna-Core-Build
    ctest -j4

To run the set and upload them to the dashboard:

    cd Hurna-Core-Build
    ctest -D Experimental

To manually run the 'TestBasicBinary' Unit Test (using GTest):

    Hurna-Core-Build/Modules/Search/Testing/Debug/TestBinary.exe  (Win)
    ./Hurna-Core-Build/Modules/Search/Testing/Debug/TestBinary    (UNIX)

# <a name="implementations"></a>Implementations
## Combinatory
- **Combinations:** Compute all possible combinations of elements containing within the sequence.
- **Intersection:** Compute the intersection of two sequences keeping duplicate keys distinct.
- **IsInterleaved:** Determine whether or not a sequence is the interleave of the two others.
- **Permutations:** Compute all possible permutations of elements containing within the sequence.

## Data Structures
- **Binary Search Tree:** Binary Search Tree, Ordered Tree or Sorted Binary Tree divides all its sub-trees into two segments: left sub-tree and right sub-tree.

## Maze Generators
- **Binary Tree:** Generate a maze using a binary tree strategy.
- **Depth First Search:** Generate a maze using a randomized version of the Depth First Search (DFS) algorithm.
- **Kruskal's:** Generate a maze using a randomized version of the Kruskal's algorithm.
- **Prim's:** Generate a maze using a randomized version of the Prim's algorithm.
- **Recursive Division:** Generate a maze by recursively building walls (fractal kind).
- **Sidewinder:** Generate a maze using a binary tree strategy.

## Search
- **Binary Search:** Iteratively proceed a dichotomous search, within a sorted sequence, on the first occurrence of the key.
- **K'th Order Statistics:** Find the k'th smallest/biggest element.
- **Maximal/Minimal Distance:** Identify the two elements of the sequence that give the maximal/minimal distance.
- **Maximal/Minimal M Elements:** Retrieve the m maximal/minimal values sorted in respectively decreasing increasing order.
- **Maximal/Minimal Sub-Sequence:** Identify the sub-sequence with the maximum/minimum sum. One of the problem resolved by this algorithm is:
"Given an array of gains/losses over time, find the period that represents the best/worst cumulative gain."

## Sort
- **Bubble Sort:** Sometimes referred to as sinking sort: proceed an in-place bubble-sort on the elements.
- **Cocktail Sort:** Variation of bubble sort. Optimize a bubble sort bubbling in both directions on each pass.
- **Comb Sort:** Variation of bubble sort. The inner loop of bubble sort, which does the actual swap,
is modified such that gap between swapped elements goes down (for each iteration of outer loop) in steps of a "shrink factor" k: [ n/k, n/k2, n/k3, ..., 1 ].
- **MergeInplace:** Functor that proceeds a in place merge of two sequences of elements.
- **MergeSort:** John von Neumann in 1945: Proceed merge-sort on the elements whether using an in-place strategy or using a buffer.
- **MergeWithBuffer:** Functor that proceeds a merge of two sequences of elements using a buffer to improve time computation.
- **Partition-Exchange:** Proceed an in-place partitioning on the elements.
- **Quick Sort - Partition-Exchange Sort:** Proceed an in-place quick-sort on the elements.
- **Raddix Sort - LSD:** Proceed the Least Significant Digit Raddix sort, a non-comparative integer sorting algorithm.
