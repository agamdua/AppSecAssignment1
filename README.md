Assignment 1 - CS-GY 9163
=========================

[![Build Status](https://travis-ci.org/agamdua/AppSecAssignment1.svg?branch=agam%2Fworkit)](https://travis-ci.org/agamdua/AppSecAssignment1)

This is an assignment created for the Application Security class at
the NYU Tandon School of Engineering.


## Setup

 This assignment prefers an Ubuntu setup. For those who wish to run a virtual
 machine can follow the instructions below.

 To run this, install [Vagrant](https://www.vagrantup.com) on your development
 machine. You can use your OS package manager (e.g., homebrew for macOS) or get
 the binary straight from their [downloads section](https://www.vagrantup.com/downloads.html).

 Vagrant is an open-source tool that provides a declarative method for
 configuring a virtual machine. 

 With the Vagrantfile provided, it is very easy to get this up and running
 from the command-line:

 ```
 $ vagrant up
 ```

 This command will begin the bootup process of the virtual machine which will
 also execute the install script for the dependencies of the assignment.

 Once the command finishes successfully, you should be able to run:
 ```
 $ vagrant ssh
 ```

 to ssh on to the virtual machine just created.

 The directory you are working in from the host machine will be shared
 on the virtual machine you have ssh'd onto. To get here:

 ```
 # after running vagrant ssh to ssh on to the machine
 $ cd /vagrant
 $ ls -al
 ```

 And you should see a copy of this document and other files in the project
 root present here.

 For more `vagrant` commands, type `vagrant --help`.

 Good luck with the assignment!
