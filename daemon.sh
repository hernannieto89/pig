#!/bin/bash

# This script copies files from my homedirectory into the webserver directory.
# (use scp and SSH keys for a remote directory)
# A new directory is created every hour.

while true; do 
    http get localhost:5000/ping/;
    sleep 10;
done