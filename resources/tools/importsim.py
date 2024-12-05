#!/usr/bin/env python3
#
#   This file is part of OpenFLUID software
#   Copyright(c) 2007, INRA - Montpellier SupAgro
#
#
#  == GNU General Public License Usage ==
#
#   OpenFLUID is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   OpenFLUID is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.
#
#
#  == Other Usage ==
#
#   Other Usage means a use of OpenFLUID that is inconsistent with the GPL
#   license, and requires a written agreement between You and INRA.
#   Licensees for Other Usage of OpenFLUID may use this file in accordance
#   with the terms contained in the written agreement between You and INRA.
#
############################################################################
#
#  This scripts imports simulators in a destination folder by specifying the git url and branch.
#  It will then remove all git related files in simualors and potential fragments
#
#  Usage: importsim.py /path/to/sims_text_file /path/to/destination_folder
#
#  - sims_text_file: Text file where we specify the simulators to import on each lines of the file.
#                    A line must be written as follows : 
#                    GitURL#branch_name clone_folder_name
#  - /path/to/destination_folder: The destination folder path where all simulators are cloned
#
############################################################################


import subprocess
import sys
import os
import shutil


def delete_folder(folder_path):
  if(os.path.exists(folder_path) and os.path.isdir(folder_path)):
    shutil.rmtree(folder_path)
    return True
  
  return False

# ===========================================================
# ===========================================================


def delete_file(filename):
  if(os.path.exists(filename) and os.path.isfile(filename)):
    os.remove(filename)
    return True

  return False


# ===========================================================
# ===========================================================


def clone_and_clean_repo(file_line, destination_path):
  cleaned_file_line = file_line.strip()
  file_line_split = cleaned_file_line.split(" ")
  if(len(file_line_split) != 2):
    print("[ERROR] Wrong file format")
    return

  giturl_branch = file_line_split[0]
  repo_name = file_line_split[1]
  giturl_split = giturl_branch.split("#")

  if(len(giturl_split) != 2):
    print("[ERROR] Wrong file format")
    return

  git_url, git_branch = giturl_split

  # Delete folder if exists
  repo_path = os.path.join(destination_path, repo_name)

  if(delete_folder(repo_path)):
    print(repo_path + " deleted")


  # Clone repo
  clone_process = subprocess.run(
                  ["git", "clone", git_url, repo_name], 
                  cwd=destination_path, 
                  capture_output=True, 
                  text=True)

  if clone_process.returncode != 0:
    print("[ERROR] Could not clone git repo " + git_url)
    return

  print(repo_path + " successfullly cloned")


  # Checkout branch
  branch_process = subprocess.run(
                   ["git", "checkout", git_branch],
                   cwd=repo_path, 
                   capture_output=True, 
                   text=True)
  
  if branch_process.returncode != 0:
    print("[ERROR] Could not checkout branch " + git_branch)
    return

  print(git_branch + " successfullly checkout")

  # Update potential submodules
  subprocess.run(["git", "submodule", "update", "--init", "--recursive"],
                 cwd=repo_path,
                 capture_output=True,
                 text=True)

  # Delete versionning files
  git_folder = os.path.join(repo_path, ".git")
  if(delete_folder(git_folder)):
    print(git_folder + " deleted")

  gitignore_file = os.path.join(repo_path, ".gitignore")
  if(delete_file(gitignore_file)):
    print(gitignore_file + " deleted")

  # Check for submodules
  git_submodules_file = os.path.join(repo_path, ".gitmodules")
  if(delete_file(git_submodules_file)):
    print(git_submodules_file + " deleted")

    # Loop through fragments
    fragments_path = os.path.join(repo_path, "src", "fragments")
    if(os.path.exists(fragments_path) and os.path.isdir(fragments_path)):
      for fragment in os.listdir(fragments_path):
        fragment_git_file = os.path.join(fragments_path, fragment, ".git")
        if(delete_file(fragment_git_file)):
          print(fragment_git_file + " deleted")

        fragment_gitignore_file = os.path.join(fragments_path, fragment, ".gitignore")
        if(delete_file(fragment_gitignore_file)):
          print(fragment_gitignore_file + " deleted")


# ===========================================================
# ===========================================================


def main():

  if(len(sys.argv) != 3):
    print("[ERROR] wrong number of arguments")
    print("Usage: importsim.py sims_text_file_path destination_folder_path")
    return

  file_path = sys.argv[1]
  destination_path = sys.argv[2]

  if(not(os.path.exists(destination_path)) or not(os.path.isdir(destination_path))):
    print("[ERROR] Destination path " + destination_path + " does not exists or is not a folder")
    return

  try:
    file = open(file_path, "r")
    for line in file:
      if not line:
        continue
      clone_and_clean_repo(line, destination_path)

  except Exception as e:
    print(f"[ERROR] {e}")

main()