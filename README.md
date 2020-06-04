# evalvid-tool
Docker file for building evalvid toolset 
----
This is the repository for compiling Evalvid toolset source code. It uses the Docker environment, which is easy to deploy.
Although the original evalvid source code cannot download from the official website (https://www.tkn.tu-berlin.de/research/evalvid/), there are currently three available links on the github: https://github.com/soohyunc/evalvid-2.7, https://github.com/lsiddd/evalvid, and https://github.com/janieltec/Evalvid-2.7. I choose the source code from https://github.com/soohyunc/evalvid-2.7 and do some modification and compilation as suggested on this website https://janieltec.wordpress.com/2017/01/09/how-install-the-evalvid-2-7/. 

#To create the docker image:
- git clone https://github.com/aphirak/evalvid-tool evalvid-tool
- cd evalvid-tool
- docker build evalvid/ -t "evalvidtool:latest" --target evalvid
- docker build evalvid/ -f evalvid/Dockerfile.1204 -t "evalvidtool:12.04" --target evalvid
- docker build evalvid/ -f evalvid/Dockerfile.1204 -t "evalvidtool:12.04-ffmpeg" --target evalvidffmpeg
- docker build evalvid/ -f evalvid/Dockerfile.1404 -t "evalvidtool:14.04" --target evalvid
- docker build evalvid/ -f evalvid/Dockerfile.1404 -t "evalvidtool:14.04-ffmpeg" --target evalvidffmpeg
- docker build evalvid/ -f evalvid/Dockerfile.1604 -t "evalvidtool:16.04"

#To use the docker image (all compiled tools are in docker image):
- docker run -it evalvidtool:latest /bin/bash

#To use the docker image with host directory (ex. from host:/home/aphirak/data mapping to docker:/data)
- docker run -v /home/aphirak/data:/data -it evalvidtool:latest /bin/bash

Dockerhub
---
For anyone who would not like to build your own docker images, you can pull images from Docker hub. My personal Docker hub URL for evalvid toolset is located at https://hub.docker.com/r/aphirak/evalvidtool. Please feel free to use it.

June 5, 2020
