FROM ubuntu:16.04 as evalvid

RUN apt-get update -y \
     && apt-get install --no-install-recommends -y git make pkg-config g++ zlib1g-dev \
     libfreetype6-dev libjpeg62-dev libpng12-dev libopenjpeg-dev libmad0-dev libfaad-dev \
     libogg-dev libvorbis-dev libtheora-dev liba52-0.7.4-dev libavcodec-dev libavformat-dev \
     libavutil-dev libswscale-dev libavresample-dev libxv-dev x11proto-video-dev libgl1-mesa-dev \
     x11proto-gl-dev linux-sound-base libxvidcore-dev libssl-dev libjack-dev libasound2-dev \
     libpulse-dev libsdl1.2-dev dvb-apps libavcodec-extra libavdevice-dev libmozjs185-dev \
     libghc-glut-dev vim \
    && dpkg --add-architecture i386 && apt-get update -y \
    && apt-get install --no-install-recommends -y wine \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /home/evalvid
COPY src/ .
#RUN sh ./compile.sh
RUN sh compile-static.sh
RUN mkdir -p /data
CMD ["/bin/bash"]
