-------------------
Beats on the Barbie
-------------------


Operating System
----------------
The algorithm has been tested on Raspbian and Ubuntu 14.04.5 LTS and Ubuntu 16.1


Installation
------------
The software relies on several dependencies. To install these dependencies
and build the source, run install_ibt.sh. The
installation may take some time. And will require you give root privelages
towards the end. Do not run the whole script with root priveleges as it
will generate read-only files.

sh install_ibt.sh

Running the beat tracker
------------------------

The Installation script patches the ibt beat tracker
with a slightly modified version. The binary will be installed
to marsyas-release-0.5/build/bin/ibt

Marsyas does not install correctly on many operating systems. This
is the reason our install generates a binary that is statically linked
to Marsyas. If on your system Marsyas is installed correctly, ibt can
also be built using the make file in the ibt folder.

Usage: ./ibt [-i input_wav] [-o output_txt] [-m]

Parameters
----------
	-i				Input wav file for beat tracking
	-o				Output text file for saving beats
	-m				Use the microphone for input"
	--drums			Write beats to the hardware over wiringPi

Running on the Raspberry Pi
---------------------------

When using the software installed on a raspberry pi.
It is a requirement the the desired audio device be set
as hw:0,0.

If using an external soundcard (such as a USB interface), I
found the easiest solution was to simply disable the default
sound device on the raspberry pi, thus shunting the USB device
up to hw:0,0. See the below instructions.

http://www.instructables.com/id/Disable-the-Built-in-Sound-Card-of-Raspberry-Pi/

Notes
-----
The --drums flag will only work on the embedded system. It also
requires running the beat tracker with root permissions.

The main dependecies are:

cmake
libsndfile
jack
build-essentials
marsyas

If you have any issues installing the beat tracking algorithm
please contact me at: jeremyfbell@gmail.com


