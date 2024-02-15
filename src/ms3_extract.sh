#!/bin/bash

# Compile the C++ program
g++ -std=c++17 ace.cpp -o ace

# Academic year and semester (replace with your academic year and semester)
academic_year_semester="23s2"

# Team numbers (replace with your team numbers)
team_numbers=21

# The source directory where repositories are cloned (replace this with your own repo path)
source_dir="../sample/repo"

# The destination directory where data will be saved (replace this with your data path)
data_dir="../sample/data"

# The destination directory where AI code will be extracted (replace this with your code path)
code_dir="$data_dir/code"

# The destination directory where AI chat will be extracted (replace this with your chat path)
chat_dir="$data_dir/chat"

# The destination directory where AI test cases will be extracted (replace this with your test path)
test_dir="$data_dir/test"

# Define the dataset file path (replace with your dataset file path)
dataset="$data_dir/dataset.csv"

# Define the error log file path (replace with your error log file path)
error_log="$data_dir/error_log.txt"

# Clear the code directory
if [ -d "$code_dir" ]; then
    rm -rf "$code_dir"/*
fi

# Clear the chat directory
if [ -d "$chat_dir" ]; then
    rm -rf "$chat_dir"/*
fi

# Clear the test directory
if [ -d "$test_dir" ]; then
    rm -rf "$test_dir"/*
fi

# Remove the dataset file if exists
if [ -f "$dataset" ]; then
    rm "$dataset"
fi

# Remove the error log file if exists
if [ -f "$error_log" ]; then
    rm "$error_log"
fi

# Loop through all teams
for team_number in $(seq 1 $team_numbers); do
  # Format team numbers with leading zeros
  formatted_team_number=$(printf "%02d" $team_number)
  output="T$formatted_team_number.txt"

  # Ensure the test directory exists or create it
  mkdir -p "$test_dir/T$formatted_team_number"

  # Team 14 has different suffix (replace with your team names)
  if [ "$formatted_team_number" == "14" ]; then
    team_type="win"
  else
    team_type="cp"
  fi

  repo_name="$academic_year_semester-$team_type-spa-team-$formatted_team_number"

  # Extract for each team, redirect output and error to error log
  echo "Extracting $source_dir/$repo_name into $destination_dir/$output"
  ./ace "$source_dir/$repo_name" "$code_dir/$output" "$chat_dir" "$test_dir/T$formatted_team_number" "$dataset" 2>&1 | tee -a "$error_log"
done
