#! /bin/bash

apt-get update -y && apt-get dist-upgrade -y
apt-get install -y gpiod libgpiod2 libgpiod-dev

gpioinfo > gpioinfo.txt
gpiodetect > gpiodetect.txt
