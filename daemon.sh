#!/bin/bash


while true; do 
    http get localhost:5000/ping/;
    sleep 10;
done