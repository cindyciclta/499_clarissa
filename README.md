# 499_clarissa
Cindy Clarissa
Clarissa@usc.edu
CSCI 499


Compile with 'make'.
Run ./servicelayer
Run ./backend
Run ./clientcommandline

1. vagrantfile (included in project)
	Download vagrant first:
	https://app.vagrantup.com/generic/boxes/ubuntu1804
2. Set up steps
	1. Download vagrant.
	2. Replace vagrant file with my current vagrantfile
	3. Download vagrant: https://www.vagrantup.com/downloads.html
		$ vagrant init project
		a vagrantfile is placed, replace it with the one I have already provided.
		$ vagrant up
		$ vagrant reload
		$ vagrant ssh
	4. Place 499_clarissa folder inside 'project'
	5. Install:
		$ sudo apt install make
		$ sudo apt install gcc
		$ sudo apt install clang
		$ sudo apt install g++
		$ sudo apt install build-essential
	6. Install homebrew: https://docs.brew.sh/Linuxbrew
		$ sh -c "$(curl -fsSL https://raw.githubusercontent.com/Linuxbrew/install/master/install.sh)"
		$ test -d ~/.linuxbrew && eval $(~/.linuxbrew/bin/brew shellenv)
		$ test -d /home/linuxbrew/.linuxbrew && eval $(/home/linuxbrew/.linuxbrew/bin/brew shellenv)
		$ test -r ~/.bash_profile && echo "eval \$($(brew --prefix)/bin/brew shellenv)" >>~/.bash_profile
		$ echo "eval \$($(brew --prefix)/bin/brew shellenv)" >>~/.profile
	$ brew install hello
	$ sudo apt-get install build-essential curl file git
	7. Install protobuf: https://github.com/grpc/grpc/blob/master/BUILDING.md
		$ git clone https://github.com/protocolbuffers/protobuf.git
		$ sudo apt-get install build-essential autoconf libtool pkg-config
		$ sudo apt-get install libgflags-dev libgtest-dev
		$ sudo apt-get update
		$ sudo apt-get install clang libc++-dev


		$ sudo apt-get install pkg-config

		$ git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc

		inside /vagrant/grpc:
		$ git submodule update --init
		$ make
		$ sudo make install

		then:
		$ cd third_party/protobuf
		$ make && sudo make install

	install gflags and gtest
	$ git clone https://github.com/gflags/gflags.git
	$ git clone https://github.com/google/googletest.git
		$ brew install cmake
		$ sudo apt-get install cmake
		$ sudo apt-get install cmake # install cmake
		$ cd /usr/src/gtest
		$ sudo cmake CMakeLists.txt
		$ sudo make
		$ sudo cp *.a /usr/lib

		$ brew install protobuf

3. complication instructions
	In 'project', type 'make' to compile.
	Then, ./backend for key value store
	./servicelayer for the serveice layer
	./clientcommandline for client commandline


4. basic example usage
	Once ./backend and ./servicelayer is running, you can make commands:

	./clientcommandline --register cindy
		{Registered user: cindy}

	./clientcommandline --user cindy --chirp "My first chirp."
		{Succesfully chirped with id: <id>}

	./clientcommandline --register damian
		{Registered user: damian}
	./clientcommandline --register cat --chirp "is it going to work?"
		{Syntax Error. Please try again.}
