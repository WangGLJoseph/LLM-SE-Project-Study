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

# List of teams to clone
for team_number in $(seq 1 $team_numbers); do
  # Format team numbers with leading zeros
  formatted_team_number=$(printf "%02d" $team_number)

  # Team 14 has different suffix (replace with your team names)
  if [ "$formatted_team_number" == "14" ]; then
    team_type="win"
  else
    team_type="cp"
  fi

  # Construct the repository URL
  repo_name="$academic_year_semester-$team_type-spa-team-$formatted_team_number"
  repo_url="$base_url/$repo_name.git"

  # Clone the repository (replace with the correct milestone number)
  echo "Cloning $repo_url into $destination_dir"
  git clone --depth 1 --branch milestone3 "$repo_url" "$destination_dir/$repo_name"
done
