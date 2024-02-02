#!/bin/bash

# Base URL for the repositories (replace with your own repo links' common prefix)
base_url="https://github.com/nus-cs3203/23s1-"

# The destination directory where repositories will be cloned (replace this with your own repo path)
destination_dir="../sample/repo"

# Ensure the destination directory exists or create it
mkdir -p "$destination_dir"

# List of teams to clone (replace with your team numbers)
for team_number in {1..37}; do
  # Format team numbers with leading zeros
    formatted_team_number=$(printf "%02d" $team_number)

  # Team numbers 08, 18, and 29 have different suffixes (replace with your team names)
  if [ "$formatted_team_number" == "08" ]; then
    team_type="java"
  elif [ "$formatted_team_number" == "18" ] || [ "$formatted_team_number" == "29" ]; then
    team_type="win"
  else
    team_type="cp"
  fi

  # Construct the repository URL
  repo_url="${base_url}${team_type}-spa-team-${formatted_team_number}.git"

  # Clone the repository (replace with the correct milestone number)
  echo "Cloning $repo_url into $destination_dir"
  git clone --depth 1 --branch milestone3 "$repo_url" "$destination_dir/${team_type}-spa-team-${formatted_team_number}"
done
