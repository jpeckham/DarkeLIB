# DarkeLIB
The original DarkeLIB as used by Daybreak Ridge from 1999. i made some edits in 2005 wuth schroom and hellbringer.

thrace and i hosted darkelib as daybreak ridge in 1998 with traer. i got the darkelib from drizzt and zortek with permission.

# How to use

* Download and install docker
  * set docker to linux mode if you're on windows
* Clone or download this repo into a folder
* execute `docker-compose up`
* if you don't have docker-compose then:
  * `docker build -t darkelib:latest .`
  * windows: `docker run --rmi -p 7878:7878 -v %CD%:/home/parnell/mud darkelib`

Log in as parnell `password` to be admin

