#!/bin/bash
# install_tab9.sh: Script to install necessary system packages and clone the TabNine self-hosted repository.

# Exit immediately if a command exits with a non-zero status, and treat unset variables as an error.
set -eu

REPO_URL="https://github.com/mixplorernano-debug/tab9-self-hosted"
PACKAGE_NAME="tab9selfhosted"
CLONE_DIR="tab9-self-hosted"

echo "--- Starting TabNine Self-Hosted Setup ---"

# Step 1: Update package lists
echo "1/4: Updating system package lists..."
if sudo apt update; then
    echo "Package lists updated."
else
    echo "Error updating package lists. Aborting." >&2
    exit 1
fi

# Step 2: Install the specified package non-interactively
echo "2/4: Installing package: '$PACKAGE_NAME'..."
# The -y flag assumes 'yes' to prompts, essential for scripted installs.
if sudo apt install -y "$PACKAGE_NAME"; then
    echo "Package '$PACKAGE_NAME' installed successfully."
else
    echo "Error installing '$PACKAGE_NAME'. Check if the package name is correct and repositories are configured." >&2
    # We continue to ensure git is present for the next step, but alert the user.
fi

# Step 3: Ensure Git is installed (required for the next step)
echo "3/4: Ensuring Git is installed..."
sudo apt install -y git

# Step 4: Clone the repository
echo "4/4: Cloning repository '$REPO_URL'..."

if [ -d "$CLONE_DIR" ]; then
    echo "Warning: Directory '$CLONE_DIR' already exists. Skipping clone operation."
else
    git clone "$REPO_URL"
    echo "Repository cloned successfully into './$CLONE_DIR'."
fi

echo "--- Setup Complete! ---"
echo "You can now navigate into the cloned directory: cd $CLONE_DIR"