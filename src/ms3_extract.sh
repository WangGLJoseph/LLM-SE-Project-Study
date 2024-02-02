#!/bin/bash

# Compile the C++ program
g++ -std=c++17 ace.cpp -o ace

# Define the error log file path (replace with your error log file path)
error_log="../sample/data/errorlog.txt"

# Loop through all teams, modify team numbers as needed (only 0 is used here for sample)
for ((i=0; i<=1; i++)); do
  input="T$(printf "%02d" $i)"
  output="T$(printf "%02d" $i).txt"

  # Format team number with leading zeros (T01-T09)
  team_number=$(printf "%02d" $i)

  # Modify team names as needed due to course supporting different environments
  case "$i" in
    8)
      team_name="23s1-java-spa-team-08"
      ;;
    18)
      team_name="23s1-win-spa-team-18"
      ;;
    29)
      team_name="23s1-win-spa-team-29"
      ;;
    *)
      team_name="23s1-cp-spa-team-$team_number"
      ;;
  esac

  # Extract for each team, redirect output and error to error log
  ./ace "../sample/repo/$team_name" "../sample/data/$output" 2>&1 | tee -a "$error_log"
done
