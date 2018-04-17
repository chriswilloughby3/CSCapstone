# Introduction

Completing the Computer Science Program at Southern New Hampshire University has provided me a strong foundation to beginning a career in the Technology Industry.  The following three programing projects will demonstrate my ability to write code using industry standard languages and apply those languages to create programs that implement modern computer program techniques. The first program is a simulator for a microprocessor that implements a five-stage pipeline and shows my ability implement and a modern microprocessor that exists only in software using the C programming language.  The second program displays that I have a solid understanding of recursive algorithms and the ability to implement those algorithms in Java.  The final program exhibits my competence to enact a REST API for a modern database like Mongo with the Python programming language.  The three programs demonstrate that I possess the skill in a wide range of Computer Science concepts using industry standard programming languages.

# Programing Projects

Forcing a line-break\s\s
Next line in the list

## Code Review Video

[Click here to view the video](https://github.com/chriswilloughby3/CSCapstone/blob/master/CodeReview.mp4)

## Microprocessor Simulator Discription

  The original single stage and parts of the five-stage pipeline microprocessor simulator was created during the winter semester of 2015 at the University of Michigan as part the of microprocessor class that I have completed there.  The purpose of the simulator is to create a microprocessor in software that can execute a provided instruction set architecture.
  
  I selected the microprocessor simulator for the engineering portion of my ePortfolio because it demonstrates my ability to create, design, and test hardware while abstracting the physical components into software.  The simulator also shows that I have the ability to program in C and that I understand modern microprocessor architecture and design.
  
  The initial five-stage pipeline was straightforward but it required NOOP instructions to be inserted into the pipeline for every type of data hazard.  The main challenge was implementing the look ahead functionally that eliminated the need for NOOP instructions to be inserting for the SW, ADD, NOR, and some of the LW instructions.  Even though not all of the NOOP instructions were eliminated, the look ahead ability reduce the number NOOP insertions and brought the microprocessor close to executing one instruction per-cycle.
  
  [Click here to view the source code](https://github.com/chriswilloughby3/CSCapstone/blob/master/fiveStageSimulator.c)

## Implementation of a Binary Search Tree

  The binary search tree (BST) program that I included uses a BST data structure to store and organize a special data type called Bid, which contains information on items sold through an auction.  The original program was written in C++ on October 11, 2017 for the CS-260 course.
  
  I selected the specialized BST implementation for the algorithms and data structures portion of my ePortfolio because it demonstrates my ability to program using the C++ programing language and that I have solid understanding of how to write and implement recursive algorithms that require the use of custom data types. 
  
  I ran into several issues when translating the binary search tree program from C++ to Java.  The first major issue that I ran into is that Java is more object oriented than C++, so I need to restructure the custom data types into their own classes and therefore resulted in me separating those classes into their own Java files as well for ease of reading the code.  The next major issue that I ran into was that my implementation of the BST in C++ was done primarily by the manipulation of pointers because C++ implements the concept of pass by reference, whereas Java only implements pass by value and therefore caused me to redesign the algorithms to properly function using pass by value.  The final issue that I had when translating the BST program from C++ to Java is that C++ requires the programmer to allocate and release dynamic memory, whereas Java manages the release of dynamic memory for the programmer and therefore required me to remove all of the implementations of the destructor functions.  I also learned how to override the toString function, which allowed me to customize how to print to screen the Bid object. 
  
  [Click here to view the source code](https://github.com/chriswilloughby3/CSCapstone/tree/master/BST%20Application)
  
## Implementation of a MongoDB RESTful API

  The MongoDB RESTful API was created to provide an interface between the Mongo Database native CRUD operations and an outside program or website.  The original program was written in PyMongo on January 15, 2018 for the CS-340 course.

  I selected the MongoDB RESTful API for the databases portion of my ePortfolio because it demonstrates my ability to implement modern programming techniques for web services, to use the Python scripting language, and to work with modern databases like Mongo.
  
  Besides gaining more experience with the Python scripting when creating the RESTful API, I learned techniques to effectively troubleshoot scripting languages, which differs a bit from complied languages like C++.  The primary purpose of the enhancement was to make the API CRUD operations move universal by allowing different kinds of data types or formats to be passed into the functions.  By making the CRUD operation functions more universal will allow for easier integration and reusability of my MongoDB RESTful API for future applications. 


## Welcome to GitHub Pages

You can use the [editor on GitHub](https://github.com/chriswilloughby3/CSCapstone/edit/master/index.md) to maintain and preview the content for your website in Markdown files.

Whenever you commit to this repository, GitHub Pages will run [Jekyll](https://jekyllrb.com/) to rebuild the pages in your site, from the content in your Markdown files.

### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/chriswilloughby3/CSCapstone/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://help.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.
