#!/bin/bash

# Personal access token to access repositories (replace with your own personal access token)
personal_access_token=""

# Academic year and semester (replace with your academic year and semester)
academic_year_semester="23s2"

# Team numbers (replace with your team numbers)
team_numbers=21

# Base URL for the repositories (replace with your own repo links' common prefix)
base_url="https://${personal_access_token}@github.com/nus-cs3203"

# The destination directory where repositories will be cloned (replace this with your own repo path)
destination_dir="../sample/repo"

# Ensure the destination directory exists or create it
mkdir -p "$destination_dir"

# Determine the branch name based on team number
get_branch_name() {
  case $1 in
    1) echo "milestone1-2";;
    2) echo "milestone1";;
    3) echo "milestone1";;
    4) echo "milestone1-2";;
    5) echo "milestone1-late-1";;
    6) echo "milestone1-5";;
    7) echo "milestone1-1";;
    8) echo "milestone1-1";;
    9) echo "milestone1-2";;
    10) echo "milestone1";;
    11) echo "milestone1";;
    12) echo "milestone1";;
    13) echo "master";;
    14) echo "milestone1";;
    15) echo "milestone1-1";;
    16) echo "milestone1-1";;
    17) echo "milestone1";;
    18) echo "milestone1-1";;
    19) echo "milestone1";;
    20) echo "milestone1";;
    21) echo "milestone1-1";;
    *) echo "master";; # Default case if team number is outside expected range
  esac
}

# List of teams to clone
for team_number in $(seq 1 $team_numbers); do
  # Format team numbers with leading zeros
  formatted_team_number=$(printf "%02d" "$team_number")

  # Determine the branch name based on team number
  branch_name=$(get_branch_name "$team_number")

  # Team 14 has different suffix (adjust based on your requirements)
  if [ "$formatted_team_number" == "14" ]; then
    team_type="win"
  else
    team_type="cp"
  fi

  # Construct the repository URL
  repo_name="$academic_year_semester-$team_type-spa-team-$formatted_team_number"
  repo_url="$base_url/$repo_name.git"

  # Clone the repository
  echo "Cloning $repo_url into $destination_dir from branch $branch_name"
  git clone --depth 1 --branch "$branch_name" "$repo_url" "$destination_dir/$repo_name"
done
